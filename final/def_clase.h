#ifndef DEF_CLASE_H
#define DEF_CLASE_H

#include <vector>
#include <string>
#include <algorithm>
#include <CImg.h>
#include <glob.h>
#include <regex.h>

using namespace std;
using namespace cimg_library;
      
/**
 * ExtractorCaracteristicas
 * Clase virtual de la que heredan todas las funciones de comparacion.
 * Lo creo asi en vez de un puntero a funcion cosa que se le puedan agregar
 * parámetros personalizados y en todo caso también funciones para setearlos.
 */
template <class T>
struct ExtractorCaracts {
  string nombre;
  virtual CImg<double> extraer_caracteristicas( CImg<T> imagen );
};

/**
 * ComparadorImagenes
 * Clase virtual que modela la "base de datos" de imágenes.
 * provee funciones para establecer criterios de comparación,
 * cargar imágenes del disco a la base, y tembién comparar una imagen
 * determinada con la base.
 */
template<class T>
class ComparadorImagenes {

 public:
  struct ExtractorCaracteristicas {
    string nombre;
    virtual CImg<double> extraer_caracteristicas( CImg<T> imagen );
  };

 private:

  unsigned n_caracteristicas;
  unsigned n_imagenes;

  // este tiene tamaño n_caracteristicas
  vector<ExtractorCaracteristicas > extractores;

  // este tiene tamaño n_caracteristicas
  vector<double> ponderaciones;

  // este tiene tamaño [n_imagenes][n_caracteristicas]
  vector< vector<CImg<double> > > caracteristicas;

  //este tiene tamaño [n_imagenes]
  vector<string> nombres_imagenes;

  // genera una lista vector<string> con todas las imágenes
  // que encuentra en directorio.
  vector<string> listar_imagenes( const char* directorio );

  // compara las caracteristicas de img versus la base de datos en la pos idx
  // devuelve el mse entre los dos conjs de caracteristicas
  double comparar_caracteristicas ( vector<CImg<T> > img, unsigned idx );

 public:

  ComparadorImagenes();
  ~ComparadorImagenes();

  // agrega el extractor de caracteristicas pasado de parametro
  // para comparacion
  int agregar_extractor( ExtractorCaracteristicas extractor,
			 double ponderacion = 1.0 );

  // cargar_imagenes guarda en la lista de archivos de la base
  // las imagenes que encuentra en el directori
  // @TODO ver cual es la necesidad de esta vs listar_imagenes
  //       (no será que puedo hacer de las 2 una y chau?)
  int cargar_imagenes( const char* directorio );

  // calcular_caracteristicas inicializa la base de datos calculando las
  // caracteristicas para todas las imagenes que estan en la lista de
  // la base.
  int calcular_caracteristicas ( );

  // dada una imagen, encuentra el indice de la que es mas parecida
  int encontrar_mas_parecida ( CImg<T> imagen );


  // dado un indice de la base, devuelve el nombre
  string nombre ( int indice ) {
    return nombres_imagenes[indice];
  }

};


// genera una lista vector<string> con todas las imágenes
// que encuentra en directorio.
template<class T>
vector<string> ComparadorImagenes<T>::listar_imagenes( const char* directorio ) {
  vector<string> lista_imagenes;
  glob_t globbuf;
  regex_t match;
  regcomp( &match, ".*\\.\\(jpe\\?g\\|bmp\\tif{1,2}\\|png\\|gif\\)$", REG_ICASE );
  glob( string( string(directorio)+string("/*")).c_str(),
	GLOB_MARK, NULL, &globbuf );
  for (unsigned i=0; i<globbuf.gl_pathc; i++ ) {
      if( regexec( &match, globbuf.gl_pathv[i], 0, NULL, 0) == 0 ) {
	lista_imagenes.push_back( string( globbuf.gl_pathv[i] ) );
      }
  }
  regfree(&match);
    globfree(&globbuf);
    // devuelvo la lista con las imagenes cargadas
    return  lista_imagenes;
}

// compara las caracteristicas de img versus la base de datos en la pos idx
// devuelve el mse entre los dos conjs de caracteristicas
template<class T>
double ComparadorImagenes<T>::comparar_caracteristicas ( vector<CImg<T> > img,
					    unsigned idx ) {
  double resultado = 0;
  for ( unsigned i=0; i<extractores.size(); i++ ) {
    resultado += ( img[i].MSE( caracteristicas[idx][i] ) * ponderaciones[i] );
  }
  return resultado/(double)extractores.size();
}

template<class T>
ComparadorImagenes<T>::ComparadorImagenes() { n_caracteristicas=0; n_imagenes=0; }

template<class T>
ComparadorImagenes<T>::~ComparadorImagenes() { }

  // agrega el extractor de caracteristicas pasado de parametro
  // para comparacion
template<class T>
int ComparadorImagenes<T>::agregar_extractor( ExtractorCaracteristicas extractor,
					      double ponderacion ) {
  extractores.push_back( extractor );
  ponderaciones.push_back( ponderacion );
  return extractores.size() - 1;
}

// cargar_imagenes guarda en la lista de archivos de la base
// las imagenes que encuentra en el directori
// @TODO ver cual es la necesidad de esta vs listar_imagenes
//       (no será que puedo hacer de las 2 una y chau?)
template<class T>
int ComparadorImagenes<T>::cargar_imagenes( const char* directorio ) {
  int i=0;
  vector<string> lista_temp = listar_imagenes( directorio );
  for ( i; i<lista_temp.size(); i++ ) {
    nombres_imagenes.push_back( lista_temp[i] );
  }
  return i;
}

// calcular_caracteristicas inicializa la base de datos calculando las
// caracteristicas para todas las imagenes que estan en la lista de
// la base.
template<class T>
int ComparadorImagenes<T>::calcular_caracteristicas ( ) {
  int cont=0, k;
  CImg<T> imagen_temp;
  vector<CImg<T> > vector_caracts_temp;
  caracteristicas.clear();
  for ( cont; cont<nombres_imagenes.size(); cont++ ) {
    imagen_temp = CImg<T>( nombres_imagenes[cont].c_str() );
    vector_caracts_temp.clear();
    for ( k=0; k<extractores.size(); k++ ) {
      vector_caracts_temp.push_back( extractores[k].extraer_caracteristicas( imagen_temp ) );
    }
    caracteristicas.push_back( vector_caracts_temp );
  }
  return cont;    
}

// dada una imagen, encuentra el indice de la que es mas parecida
template<class T>
int ComparadorImagenes<T>::encontrar_mas_parecida ( CImg<T> imagen ) {
  vector<double> errores( caracteristicas.size() );
  
  // calculo las caracteristicas para esta imagen
  vector<CImg<T> > vector_caracts_temp;
    for ( unsigned k=0; k<extractores.size(); k++ ) {
      vector_caracts_temp.push_back( extractores[k].extraer_caracteristicas( imagen ) );
    }
    
    // calculo de la base cual es la mas parecida
    for ( unsigned k=0; k<caracteristicas.size(); k++ ) {
      errores[k] = comparar_caracteristicas( vector_caracts_temp, k );
    }
    
    // devuelvo el índice de la más parecida
    return distance( errores.begin(),
		     min_element( errores.begin(), errores.end() ));
}

template class ComparadorImagenes<double>;

#endif
