#ifndef DEF_CLASE_2_H
#define DEF_CLASE_2_H

#include "comparar_estadisticas_2.h"
#include "../lib/segmentacion.h"
#include "../lib/filtrado_espacial.h"

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

  public:

  // este tiene tamaño n_caracteristicas
  vector<double> ponderaciones;

  // este tiene tamaño [n_imagenes][n_caracteristicas]
  vector<vector<CImg<double> > > caracteristicas;

  //este tiene tamaño [n_imagenes]
  vector<string> nombres_imagenes;

  //este tiene tamaño [n_imagenes]
  vector<int> clases_imagenes;

  // este tiene tamaño [n_clases][n_caracteristicas]
  vector<vector<CImg<double> > > prototipos;

  // este tiene tamaño [n_clases]
  vector<string> etiquetas;

  // genera una lista vector<string> con todas las imágenes
  // que encuentra en directorio.
  vector<string> listar_imagenes(const char* directorio);

  // compara las caracteristicas de img versus la base de datos en la pos idx
  // devuelve el mse entre los dos conjs de caracteristicas
  double comparar_caracteristicas( vector<CImg<double> > caracs, unsigned idx,
				   int primera, int ultima);

  // compara las caracteristicas de img versus el prototipo idx
  // devuelve el mse entre los conjs de caracteristicas de la img y proto
  double comparar_caracteristicas_proto( vector<CImg<double> > caracs,
					 unsigned idx, int primera,
					 int ultima);

 public:
  unsigned n_caracteristicas;
  unsigned n_imagenes;
  unsigned n_clases;

  // etiqueta las imagenes segun su nombre de archivo
  int etiquetar_imagenes();

  // calcula los prototipos
  int generar_prototipos();

  // constructor
  ComparadorImagenes();

  // destructor
  ~ComparadorImagenes();

  // cargar_imagenes guarda en la lista de archivos de la base
  // las imagenes que encuentra en el directorio directorio
  int cargar_imagenes(const char* directorio);

  // calcular_caracteristicas inicializa la base de datos calculando las
  // caracteristicas para todas las imagenes que estan en la lista de
  // la base.
  int calcular_caracteristicas();

  // dada una imagen, encuentra el indice de la que es mas parecida
  int encontrar_mas_parecida(CImg<T> imagen, int primera, int ultima);

  // dada una imagen, la clasifica según MSE contra los prototipos
  int clasificar_imagen(CImg<T> imagen, int primera, int ultima);

  // dado un directorio, clasifica todas las imágenes
  int clasificar_directorio( const char* directorio, vector<string> &nombres,
			     vector<int> &clases, int primera, int ultima );

  // guarda los prototipos generados en archivos tipo imagenes
  int guardar_prototipos( const char* directorio );

  // dado un indice de la base, devuelve el nombre
  string nombre(int indice) {
    return nombres_imagenes[indice];
  }

  // dado un indice de la base, devuelve el nombre
  string etiqueta(int indice) {
    return etiquetas[indice];
  }

  // devuelve una ref al iesimo del vector de ponderaciones
  double & ponderacion( unsigned i );

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * ComparadorImagenes()
 * Constructor por defecto.
 */
template<class T>
ComparadorImagenes<T>::ComparadorImagenes() {
  n_caracteristicas = 2;
  n_imagenes = 0;
  ponderaciones.push_back(1.0);
  ponderaciones.push_back(1.0);
}

/**
 * ~ComparadorImagenes()
 * Destructor.
 */
template<class T>
ComparadorImagenes<T>::~ComparadorImagenes() {
}

/**
 * etiquetar_imagenes()
 * detecta las etiquetas (categoría, clasificación, tipo, ...) de las imágenes
 * según su nombre de archivo, que tendrá que tener formato <etiqueta>XXXXX,
 * donde <etiqueta> tiene sólo letras, y a partir del primer caracter que no sea
 * una letra, se ignora.
 * Las etiquetas detectadas se guardan en el vector<sting> etiquetas, y en el
 * vector clases_imagenes<int> se guarda en la posición Q el índice I, indicando
 * que la Q-ésima imagen tiene la I-ésima etiqueta.
 * @return: el número de imágenes al que se ha asignado etiqueta.
 */
template<class T>
int ComparadorImagenes<T>::etiquetar_imagenes() {
  regex_t match;
  regcomp(&match, ".*/\\([abcdefghijklmnopqrstuvwxyz]\\+\\)", REG_ICASE );

  n_clases = 0;
  string etiq_actual = "";

  // regmatch_t es un struct que contiene los offsets donde matchearon las
  // substrings.
  // el primer elemento contiene en rm_so el lugar a partir de donde matcheó la
  // regexp completa, el 2do tiene el comienzo y el fin de donde matcheó el
  // primer grupo (lo que esta entre parentesis). para devolver el string
  // correspondiente saco un substring ( rm_so, rm_eo-rm_so ).
  regmatch_t sub[2];

  // para cada imagen en la lista
  for (unsigned i = 0; i < nombres_imagenes.size(); i++) {
    // me fijo la "etiqueta" (primera parte del nombre de arch
    if (regexec(&match, nombres_imagenes[i].c_str(), 2, &sub[0], 0) == 0) {
      etiq_actual = nombres_imagenes[i].substr(sub[1].rm_so, sub[1].rm_eo
					       - sub[1].rm_so);
      // si nclases == 0 todavia no guarde ninguna clase
      if (n_clases == 0 || etiq_actual != etiquetas[n_clases - 1]) {
	etiquetas.push_back(etiq_actual);
	n_clases++;
      }
      // @TODO corregir lo siguiente para que en vez del último índice
      // asigne la posición devuelta por un find( etiqueta 9 en el vector
      clases_imagenes.push_back(n_clases - 1);
    }
  }

  // @TODO comentar esto. salida al pedo.
  for (unsigned w = 0; w < etiquetas.size(); w++) {
    cout << "etiqueta " << w << ": " << etiquetas[w] << endl;
  }

  regfree(&match);
  return clases_imagenes.size();
}

/**
 * generar_prototipos()
 * genera los prototipos correspondientes a cada etiqueta, tomando para cada
 * etiqueta, las características de las imágenes etiquetadas con ésta, y
 * promediando sus valores para luego guardarlos en el vector de prototipos.
 * @return: -1 si hubo error (no hay características calculadas para las
 *          imágenes), 0 si se han generado los p8s sin problemas.
 */
template<class T>
int ComparadorImagenes<T>::generar_prototipos() {

  // chequeo que las características estén ya calculadas
  // @TODO verificar que el criterio usado es realmente válido
  if (caracteristicas.size() < nombres_imagenes.size())
    return -1;

  // inicializo copiandome la primer caracteristica, total despues
  // reseteo llenando todo con ceros.
  vector<CImg<double> > proto_temp = caracteristicas[0];

  // cuenta las imagenes encontradas para la clase
  unsigned contador_clase;

  // para cada clase...
  for (unsigned clase = 0; clase < n_clases; clase++) {
    // seteo acumuladores en cero
    proto_temp[0].fill(0.0);
    proto_temp[1].fill(0.0);
    contador_clase = 0;

    // busco en todas las imagenes...
    for (unsigned i = 0; i < caracteristicas.size(); i++) {
      // ...las que son de la clase actual ...
      if (clases_imagenes[i] == clase) {
	// ... y acumulo los valores.
	proto_temp[0] += caracteristicas[i][0];
	proto_temp[1] += caracteristicas[i][1];
	contador_clase++;
      }
    }

    // si he encontrado imágenes de la clase acual...
    if (contador_clase > 0) {
      // ...divido por la cantidad de imágenes ya que esto debe ser un promedio
      proto_temp[0] /= (double) contador_clase;
      proto_temp[1] /= (double) contador_clase;
    }

    // finalmente, guardo el p7o generado en el vector de p8s
    prototipos.push_back(proto_temp);
  }

  // descomentar lo siguiente para visualizar los p8s generados
  /*
  for (unsigned p=0; p<prototipos.size(); p++ ){
    prototipos[p][0].display();
    prototipos[p][1].display();
  }
  */

  return 0;
}

/**
 * listar_imagenes()
 * lee un directorio y devuelve un vector<string> con las imágenes encontradas
 * en el mismo. no altera el estado de la clase, podría usarse como función
 * externa auxiliar tranquilamente.
 * Para entender como funciona ver una versión comentada de ésta en `commons.h`.
 * @param directorio: const char* con la ruta del directorio a leer
 * @return las imágenes encontradas en un vector<string>
 */
template<class T>
vector<string> ComparadorImagenes<T>::listar_imagenes(const char* directorio) {
  vector<string> lista_imagenes;
  glob_t globbuf;
  regex_t match;
  regcomp( &match, ".*\\.\\(jpe\\?g\\|bmp\\tif{1,2}\\|png\\|gif\\)$",
	   REG_ICASE );
  glob( string(string(directorio)+string("/*")).c_str(), GLOB_MARK, NULL,
       &globbuf);
  for (unsigned i = 0; i < globbuf.gl_pathc; i++) {
    if (regexec(&match, globbuf.gl_pathv[i], 0, NULL, 0) == 0) {
      lista_imagenes.push_back(string(globbuf.gl_pathv[i]));
    }
  }
  regfree(&match);
  globfree(&globbuf);

  // devuelvo la lista con las imagenes cargadas
  return lista_imagenes;
}

/**
 * comparar_caracteristicas()
 * dado un vector de caracteristicas caracs, calcula el MSE entre éstas y las de
 * la idx-ésima imagen en la base, ponderando según el vector de ponderaciones.
 * Los últimos dos parámetros, opcionales, me dicen qué caracteristicas tendrá
 * en cuenta para el calculo, en forma de un rango [primera,última]. Si se
 * omiten entonces se realizan los cálculos con todas las características.
 * Notar que antes se debe llamar a listar imagenes y a calcular caracteristicas
 * para que esto funcione.
 * Notar además que compara contra las característics de una imagen directamente
 * y NO contra un prototipo.
 * @param caracs: vector de CImg<double> con las características a comparar.
 * @param idx: índice en el array de características contra el que se comparará.
 * @return un double con el MSE entre los 2 conjuntos de caracs, o bien un valor
 *         negativo en caso que el vector de caracs no esté inicializado.
 */
template<class T>
double ComparadorImagenes<T>::comparar_caracteristicas (
                    vector<CImg<double> > caracs, unsigned idx,
		    int primera = 0, int ultima = -1 ) {
  if ( ultima < 0 )
    ultima = n_caracteristicas;

  // valido que el vector de caracteristicas tenga el elemento idx
  if ( prototipos.size() - 1 < idx || n_caracteristicas < 1 )
    return -1.0;

  double resultado = 0;
  for (unsigned i = 0; i < n_caracteristicas; i++) {
    resultado += (caracs[i].MSE(caracteristicas[idx][i]) * ponderaciones[i]);
  }
  return resultado / (double) n_caracteristicas;
}

/**
 * comparar_caracteristicas_proto()
 * dado un vector de caracteristicas caracs, calcula el MSE entre éstas y las
 * del idx-ésimo *prototipo*, ponderando según el vector de ponderaciones.
 * los últimos dos parámetros, opcionales, me dicen qué caracteristicas tendrá
 * en cuenta para el calculo, en forma de un rango [primera,última]. Si se
 * omiten entonces se realizan los cálculos con todas las características.
 * Notar que antes se debe llamar a generar_prototipos, o cargarlos de disco,
 * para que esto funcione.
 * @param caracs: vector de CImg<double> con las características a comparar.
 * @param idx: índice en el array de prototipos contra el que se comparará.
 * @param primera: primera característica en el rango a tener en cuenta (o sea,
 *                 primera posicion en el vector<CImg> a comparar)
 * @param ultima: última característica en el rango a tener en cuenta.
 * @return un double con el MSE entre los 2 conjuntos de caracs, o bien un valor
 *         negativo en caso que el vector de prototipos no esté inicializado.
 */
template<class T>
double ComparadorImagenes<T>::comparar_caracteristicas_proto (
                    vector<CImg<double> > caracs, unsigned idx,
		    int primera = 0, int ultima = -1 ) {
  if ( ultima < 0 )
    ultima = n_caracteristicas;

  // valido que exista el prototipo idx y que n_caracteristicas > 0
  if ( prototipos.size() - 1 < idx || n_caracteristicas < 1 )
    return -1.0;

  double resultado = 0;
  for (unsigned i = primera; i < ultima; i++) {
    resultado += ( caracs[i].MSE(prototipos[idx][i]) * ponderaciones[i] );
  }
  return resultado / (double) n_caracteristicas;
}

/**
 * cargar_imagenes()
 * agrega la lista de imágenes encontradas en directorio a la lista de
 * imágenes de base en nombres_imagenes
 * @param directorio directorio a leer.
 * @return el número de imágenes cargadas.
 */
template<class T>
int ComparadorImagenes<T>::cargar_imagenes( const char* directorio ) {
  int i = 0;
  vector<string> lista_temp = listar_imagenes(directorio);
  for (i; i < lista_temp.size(); i++)
    nombres_imagenes.push_back(lista_temp[i]);

  // seteo el valor del contador de imagenes global
  n_imagenes += i;

  return i;
}

/**
 * calcular_caracteristicas()
 * calcula las características de todas las imágenes en la lista
 * nombres_imagenes. NOTAR que las funciones de extraer características están
 * HARD-CODED en esta función, siendo algo totalmente no-general; lo terminé
 * dejando de esta forma dado el tremendo quilombo que se armó cuando intenté
 * usar una clase abstracta para modelar la función de extracción.
 * @return el número de características calculadas.
 * @TODO generalizar esto de alguna forma para que funcione con N extractores
 *       de características, sea usando clases abstractas o señales de humo...
 */
template<class T>
int ComparadorImagenes<T>::calcular_caracteristicas ( ) {
  int cont = 0, k;
  CImg<T> imagen_temp;
  vector<CImg<T> > vector_caracts_temp;

  // borro las características ya calculadas
  caracteristicas.clear();

  // para cada imagen en la lista de archivos...
  for (cont; cont < nombres_imagenes.size(); cont++) {
    // ..la leo del disco...
    imagen_temp = CImg<T> (nombres_imagenes[cont].c_str());

    // .. y genero el vector de características,
    vector_caracts_temp.clear();
    vector_caracts_temp.push_back( estadisticas_imagen<T>(imagen_temp) );
    vector_caracts_temp.push_back(
			  extraer_valores_caracteristicos<T>(imagen_temp) );
    vector_caracts_temp.push_back( acura<T>(imagen_temp) );

    // finalmente inserto el vector de características generado
    // en el vector de (vectores de) caractarísticas global.
    caracteristicas.push_back(vector_caracts_temp);
  }
  return cont;
}

/**
 * encontrar_mas_parecida()
 * Dada una imagen pasada como parámetro en imagen, busca la más parecida en la
 * base (lista de nombres de imágenes), y devuelve la posición.
 * El razonamiento es que la imagen mas parecida debe minimizar el MSE contra
 * las características de la pasada como parámetro.
 * Los parámetros primera y ultima indican las características que serán tenidas
 * en cuenta (igual que en comparar_imagenes).
 * @param imagen la imagen dato.
 * @param primera la priemra caracteristica a tener en cuenta, por defecto 0
 * @param ultima la ultima c13a tener en cuenta, por defecto n_caracteristicas
 * @return el índice de la imagen que se encontró más parecida.
 */
template<class T>
int ComparadorImagenes<T>::encontrar_mas_parecida(CImg<T> imagen,
						  int primera=0,
						  int ultima=-1 ) {
  if ( ultima < 0 )
    ultima = n_caracteristicas;

  vector<double> errores(caracteristicas.size());

  // calculo las caracteristicas para esta imagen
  vector<CImg<T> > vector_caracts_temp;
  vector_caracts_temp.push_back(estadisticas_imagen<T> (imagen));
  vector_caracts_temp.push_back(extraer_valores_caracteristicos<T> (imagen));

  // calculo el MSE contra todas las imagenes de la base
  for (unsigned k = 0; k < caracteristicas.size(); k++) {
    errores[k] = comparar_caracteristicas(vector_caracts_temp,k,primera,ultima);
  }

  // devuelvo el índice del MSE mínimo encontrado
  return distance( errores.begin(), min_element(errores.begin(),errores.end()));
}

/**
 * clasificar_imagen()
 * Dada una imagen pasada como parámetro en imagen, busca el prototipo que más
 * se parece a la misma y devuelve el índice de tal prototipo.
 * El razonamiento es que la imagen minimizará el el MSE contra el prototipo que
 * modela (generaliza) su clase/etiqueta.
 * Los parámetros primera y ultima indican las características que serán tenidas
 * en cuenta (igual que en comparar_imagenes).
 * @param imagen la imagen dato.
 * @param primera la priemra caracteristica a tener en cuenta, por defecto 0
 * @param ultima la ultima c13a tener en cuenta, por defecto n_caracteristicas
 * @return el índice del prototipoque mejor representa la imagen (clase).
 */
template<class T>
int ComparadorImagenes<T>::clasificar_imagen( CImg<T> imagen,
					      int primera=0, int ultima=-1 ) {
  if ( ultima < 0 )
    ultima = n_caracteristicas;

  vector<double> errores(n_clases);
  CImg<double> errores_img(n_clases,1,1,1,0);

  // calculo las caracteristicas para esta imagen
  vector<CImg<T> > vector_caracts_temp;
  vector_caracts_temp.push_back(estadisticas_imagen<T> (imagen));
  vector_caracts_temp.push_back(extraer_valores_caracteristicos<T> (imagen));

  // calculo el MSE contra todos los prototipos que tengo
  for (unsigned k = 0; k < n_clases; k++) {
    errores[k] = comparar_caracteristicas_proto( vector_caracts_temp, k,
						 primera, ultima );
    errores_img(k)=errores[k];
  }
  //  errores_img.display();
  // devuelvo el índice del MSE mínimo encontrado
  return distance( errores.begin(), min_element(errores.begin(),errores.end()));
}

/**
 * clasificar_directorio()
 * Clasifica todas las imágenes encontradas en directorio llamando a
 * clasificar_imagen con los parametros extra primera, ultima. Los nombres de
 * las imágenes encontradas se guardan en el vector nombres, y su clasificacion
 * respectiva en clases, ambos vectores pasados por referencia.
 * @param directorio el directorio donde buscar imágenes.
 * @param primera la priemra caracteristica a tener en cuenta, por defecto 0
 * @param ultima la ultima c13a tener en cuenta, por defecto n_caracteristicas
 * @return el número de imágenes clasificadas.
 */
template<class T>
int ComparadorImagenes<T>::clasificar_directorio( const char* directorio,
			   vector<string> &nombres, vector<int> &clases,
			   int primera = 0, int ultima = -1 ) {
  if ( ultima < 0 )
    ultima = n_caracteristicas;

  // reseteo vectores pasados por referencia
  clases.clear();
  nombres = listar_imagenes( directorio );

  CImg<T> img_temp;

  for ( unsigned i=0; i<nombres.size(); i++ ) {
    img_temp = CImg<T>( nombres[i].c_str() );
    clases.push_back( clasificar_imagen( img_temp, primera, ultima ));
  }

  return clases.size();
}

/**
 * guardar_prototipos()
 * guarda los prototipos generados a disco
 * @FIXME no anda!!!!!!!
 */
template<class T>
int ComparadorImagenes<T>::guardar_prototipos( const char* directorio ) {

  for (unsigned p=0; p<prototipos.size(); p++ ){

    prototipos[p][0].save_jpeg( string( string( directorio ) + 
                                       string( "estadistica_" ) + 
                                       string( etiqueta(p) ) +
                                       string( ".jpg" )
                                       ).c_str() );
    prototipos[p][1].save_jpeg( string( string( directorio ) + 
                                       string( "hough_" ) + 
                                       string( etiqueta(p) ) +
                                       string( ".jpg" )
                                       ).c_str() );
    printf("gen prototipo: %s \n", 
           string( string( directorio ) + 
                   string( "estadistica_" ) + 
                   string( etiqueta(p) ) +
                   string( ".jpg" )
                   ).c_str() );
    printf("gen prototipo: %s \n", 
           string( string( directorio ) + 
                   string( "hough_" ) + 
                   string( etiqueta(p) ) +
                   string( ".jpg" )
                   ).c_str() );

  }

  return 0;
}

/**
 * ponderaciones()
 * devuelve una referencia al elem i del vector de ponderaciones
 */
template<class T>
double & ComparadorImagenes<T>::ponderacion( unsigned i ) {
    return ponderaciones[i];
}

#endif // definicion de DEF_CLASE_2_H
