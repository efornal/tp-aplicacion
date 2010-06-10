#ifndef DEF_CLASE_2_H
#define DEF_CLASE_2_H

#include "comparar_estadisticas_2.h"
#include "../lib/segmentacion.h"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <CImg.h>
#include <glob.h>
#include <regex.h>

using namespace std;
using namespace cimg_library;
      
template<class T>
class ComparadorImagenes {
 private:
  unsigned n_caracteristicas;
  unsigned n_imagenes;
  unsigned n_clases;

  // este tiene tamaño n_caracteristicas
  vector<double> ponderaciones;

  // este tiene tamaño [n_imagenes][n_caracteristicas]
  vector< vector<CImg<double> > > caracteristicas;

  //este tiene tamaño [n_imagenes]
  vector<string> nombres_imagenes;

  //este tiene tamaño [n_imagenes]
  vector<int> clases_imagenes;

  // este tiene tamaño [n_clases][n_caracteristicas]
  vector< vector<CImg<double> > > prototipos;

  // este tiene tamaño [n_clases]
  vector<string> etiquetas;

  // genera una lista vector<string> con todas las imágenes
  // que encuentra en directorio.
  vector<string> listar_imagenes( const char* directorio );

  // compara las caracteristicas de img versus la base de datos en la pos idx
  // devuelve el mse entre los dos conjs de caracteristicas
  double comparar_caracteristicas ( vector<CImg<double> > img, unsigned idx );


 public:

  // etiqueta las imagenes segun su nombre de archivo
  int etiquetar_imagenes ( );


  ComparadorImagenes();
  ~ComparadorImagenes();

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

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

// Coonstructor por defecto
template<class T>
ComparadorImagenes<T>::ComparadorImagenes() {
  n_caracteristicas = 2;
  n_imagenes = 0;
  ponderaciones.push_back(1.0);
  ponderaciones.push_back(1.0);
}

// Destructor
template<class T>
ComparadorImagenes<T>::~ComparadorImagenes() {
}

// etiqueta las imagenes segun su nombre
template<class T>
int ComparadorImagenes<T>::etiquetar_imagenes() {
  regex_t match;
  regcomp( &match, ".*/\\([abcdefghijklmnopqrstuvwxyz]\\+\\)", REG_ICASE );

  n_clases = 0;
  string etiq_actual = "";

  // regmatch_t es un struct que contiene los offsets donde matchearon las substrings.
  // el primer elemento contiene en so el lugar a partir de donde matcheó la
  // regexp completa, el 2do tiene el comienzo y el fin de donde matcheó el
  // primer grupo (lo que esta entre parentesis). para devolver el string
  // correspondiente saco un substring ( so, eo-so ).
  regmatch_t sub[2];

  // para cada imagen en la lista
  for (unsigned i=0; i<nombres_imagenes.size(); i++ ) { 
    // me fijo la "etiqueta" (primera parte del nombre de arch
    if( regexec( &match, nombres_imagenes[i].c_str(), 2, &sub[0], 0) == 0 ) {
      etiq_actual = nombres_imagenes[i].substr(sub[1].rm_so,sub[1].rm_eo-sub[1].rm_so);
      // si nclases == 0 todavia no guarde ninguna clase
      if ( n_clases == 0 || etiq_actual != etiquetas[n_clases-1] ){
	etiquetas.push_back( etiq_actual );
	n_clases++;
      }
      clases_imagenes.push_back( n_clases-1 );
      //cout<<"etiq: "<<nombres_imagenes[i].substr(sub[1].rm_so,sub[1].rm_eo-sub[1].rm_so)<<endl;
    }
  }

  regfree(&match);
  return  clases_imagenes.size();
}

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
double ComparadorImagenes<T>::comparar_caracteristicas ( vector<CImg<double> > img,
					    unsigned idx ) {
  double resultado = 0;
  for ( unsigned i=0; i<n_caracteristicas; i++ ) {
    resultado += ( img[i].MSE( caracteristicas[idx][i] ) * ponderaciones[i] );
  }
  return resultado/(double)n_caracteristicas;
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
    vector_caracts_temp.push_back( estadisticas_imagen<T>( imagen_temp ) );
    vector_caracts_temp.push_back( extraer_valores_caracteristicos<T>( imagen_temp ) );
    caracteristicas.push_back( vector_caracts_temp );
  }
  return caracteristicas.size();//cont;    
}

// dada una imagen, encuentra el indice de la que es mas parecida
template<class T>
int ComparadorImagenes<T>::encontrar_mas_parecida ( CImg<T> imagen ) {
  vector<double> errores( caracteristicas.size() );
  
  // calculo las caracteristicas para esta imagen
  vector<CImg<T> > vector_caracts_temp;
  vector_caracts_temp.push_back( estadisticas_imagen<T>( imagen ) );
  vector_caracts_temp.push_back( extraer_valores_caracteristicos<T>( imagen ) );
    
  // calculo de la base cual es la mas parecida
  for ( unsigned k=0; k<caracteristicas.size(); k++ ) {
    errores[k] = comparar_caracteristicas( vector_caracts_temp, k );
  }
    
  // devuelvo el índice de la más parecida
  return distance( errores.begin(),
		   min_element( errores.begin(), errores.end() ));
}

#endif
