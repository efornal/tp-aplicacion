#ifndef COMPARAR_ESTADISTICAS_2_H
#define COMPARAR_ESTADISTICAS_2_H

#include <iostream>
#include <CImg.h>
#include <string>
#include <glob.h>
#include <regex.h>

using namespace cimg_library;
using namespace std;

/**
 * promedio()
 * devuelve una imagen promedio de todas las contenidas en la CImgList pasada
 * como parámetro lista_imagenes. Si las imágenes en la lista tienen tamaños
 * diferentes, falla (devuelve la primer imagen en la lista, en realidad).
 * @param lista_imagenes: la lista con las imágenes a promediar.
 * @return laa CImg<T> promedio
 */
template<class T>
CImg<T> promedio(CImgList<T> lista_imagenes) {

  CImg<T> promediada = lista_imagenes(0);
  unsigned int tam_lista = lista_imagenes.size();

  unsigned w = lista_imagenes(0).width(),
    h = lista_imagenes(0).height(),
    d = lista_imagenes(0).spectrum();

  for (unsigned int i = 1; i < tam_lista; i++) {
    // valido que las imágenes tengan las mismas dimensiones (x,y,c)
    if ( lista_imagenes(i).width() != w
	 || lista_imagenes(i).height() != h
	 || lista_imagenes(i).spectrum() != d )
      return promediada;

    cimg_forXYC(promediada,x,y,c) {
	promediada(x, y, c) += lista_imagenes[i](x, y, c);
    }
  }
  cimg_forXYC(promediada,x,y,c) {
      promediada(x, y, c) /= (double) tam_lista;
  }
  
  return promediada;
}

/**
 * generar_base()
 * genera una imagen base (promedio), a partir de todas las imágenes que
 * encuentre en el directorio pasado como parámetro.
 * @param directorio directorio donde buscar las imágenes para generar la base
 * @return base generada (sin segmentar, por ahora)
 */
template<class T>
CImg<T> generar_base(const char* directorio) {
  CImgList<T> lista_imagenes;
  CImg<T> temp;

  // el "globbing" me permite expandir los asteriscos como cuando hago `ls ./*`
  // acá lo que hago es expandir la expresión (directorio)/*, y para cada una
  // de las expansiones matcheo la expresión regular "match", que matchea para
  // los archivos tipo imagen: .jpg, .jpeg, .JPG, .bmp, et cetera.

  // globbuf es la estructura donde se guardan los resultados del glob.
  // me interesan 2 de sus miembros: gl_pathc, que contiene el número
  // de expansiones, y gl_pathv que es un char** con las expansiones mismas
  glob_t globbuf;

  // match contendrá la expresón regular "compilada" en la llamada a regcomp
  regex_t match;

  // compilo la expresión regular en el 2do parámetro en match,
  // el 3er argumento le dice que ignore mayusculas/minusculas
  // por alguna putísima razón tengo que comentar con \\ los caracteres ()?|
  regcomp( &match, ".*\\.\\(jpe\\?g\\|bmp\\tif{1,2}\\|png\\|gif\\)$",
	   REG_ICASE );

  // expando directorio/*
  glob(std::string(std::string(directorio) + std::string("/*")).c_str(),
       GLOB_MARK, NULL, &globbuf);

  // recorro todas las expansiones
  for (unsigned i = 0; i < globbuf.gl_pathc; i++) {
    // matcheo la expresion regular
    if (regexec(&match, globbuf.gl_pathv[i], 0, NULL, 0) == 0) {
      // si matchea cargo la imagen en temp y la meto al final de la lista
      lista_imagenes.push_back(CImg<T> (globbuf.gl_pathv[i]));
    }
  }

  // libero los cosos auxiliares de glob y regex
  regfree(&match);
  globfree(&globbuf);

  // devuelvo la base generada a partir del promedio de las imagenes
  return promedio(lista_imagenes);
}

/**
 * segentar()
 * Segmenta una imagen en cuadritos de ancho por alto especificado
 * La particion de la imagen se realiza de izq a derecha y de arriba hacia abajo
 * en cuadros de tamaño ancho x alto. Si el tamaño de la imagen no es divisible
 * de forma exacta (sin decimales) con el tamañoo especificado, la segmentacion
 * omite la parte sobrante (no divisible), tanto en alto como en ancho.
 * @param CImg<T> img Imagen a segmentar
 * @param int ancho Ancho de cada cuadrito
 * @param int alto Alto de cada cuadrito
 * @return CImgList<T> Lista de cuadritos de la imagen segmentada
 */
template<class T>
CImgList<T> segmentar(CImg<T> img, int ancho = 20, int alto = 20) {
  CImgList<T> cuadros;
  for (int y = 0; y <= img.height() && y + alto <= img.height(); y += alto) {
    for (int x = 0; x <= img.width() && x + ancho <= img.width(); x
	   += ancho) {
      cuadros.push_back(img.get_crop(x, y, x + ancho - 1, y + alto - 1));
    }
  }
  return cuadros;
}

/**
 * sumar_dim()
 * agarra una imagen y la suma a lo largo de una dimensión, i.e. si sumo
 * a lo largo de x entonces obtendré un vector en y del mismo alto que la imagen
 * original, donde cada punto en y contendrá el valor de sum(0:ancho,y)
 * @param imagen la imagen sobre la que calcular la suma
 * @param dimension la dimensión a sumar, por defecto x,
 *                  valores posibles: 'xXyY'
 * @return vector de la suma, con una dimensión menos que la imagen original.
 * @TODO generalizar para sumar a lo largo de las dimensiones z y c.
 */
template<class T>
CImg<T> sumar_dim( const CImg<T> imagen, char dimension = 'x',
		   bool promediar = false) {
  CImg<T> vector_salida;
  // si dimension es x, recorro para cada Y y sumo todo X en cada Y
  if (dimension == 'x' | dimension == 'X') {
    vector_salida.assign(1, imagen.height(), 1, 1, 0);
    cimg_forY( imagen, y ) {
      vector_salida(0, y) = imagen.get_crop(0, y, imagen.width()-1, y).sum();
    }
    if ( promediar )
      vector_salida /= imagen.width();
  }
  // viceversa si dimensión es X
  if (dimension == 'y' | dimension == 'Y') {
    vector_salida.assign(imagen.width(), 1, 1, 1, 0);
    cimg_forX( imagen, x ) {
      vector_salida(x, 0) = imagen.get_crop(x, 0, x, imagen.height()-1).sum();
    }
    if ( promediar )
      vector_salida /= imagen.height();
  }
  return vector_salida;
}

/* double media ( const CImg<double> &histog ) { */
/*   unsigned x,y; */
/*   double media = 0; */
/*   cimg_forXY(histog, x, y) { */
/*     media += histog(x,y)*(double)(x+y); */
/*   } */
/*   return media/(double)(histog.width()*histog.height()); */
/* } */

/* double stddev ( const CImg<double> &histog ) { */
/*   unsigned x,y; */
/*   double stddev=0, 
        mu = (double)media(histog)*(double)(histog.width()*histog.height()); */
/*   cimg_forXY(histog, x, y) { */
/*     stddev += (double)histog(x,y)*abs((double)(x+y)-mu); */
/*   } */
/*   return stddev/(double)(histog.width()*histog.height()); */
/* } */

/**
 * mediana()
 * devuelve la mediana (coordenada-x donde se da el percentil 50) en el
 * histograma pasado como parámetro histo.
 * @param histo el histograma dato.
 * @return el valor calculado de mediana
 */
template<class T>
T mediana ( const CImg<T> & histo ) {
  T sum = histo.sum(), acum = (T)0;
  unsigned x = 0;
  while ( acum < (sum/2.0) ) {
    acum += histo(x);
    x++;
  }
  return (T)x;
}

/**
 * absdev()
 * devuelve la desviación absoluta del histograma respecto de la media
 * M (parámetro). La desviación absoluta se define como: sum(abs(x-M))/n
 * @param histo el histograma dato
 * @param M la media respecto de la cual calcular el histograma.
 * @return el valor calculado de desviación absoluta.
 */
template<class T>
T absdev ( const CImg<T> & histo, T M ) {
  unsigned x = 0;
  T absdev = 0;
  for ( x; x< histo.width(); x++ ) {
    absdev += abs( histo(x) - M );
  }
  return absdev/(T)histo.width();
}

/**
 * obtener_cuadrante()
 * dada la imagen imagen, devuelve el cuadrante 1, 2, 3, o 4, si el
 * parámetro cuadrante es alguno de esos valores; en otro caso devuelve una
 * copia de la imagen original.
 * @param imagen la imagen de donde se saca el cuadrante
 * @param cuadrante el numero de cuadrante a devolver
 * @return el cuadrante calculado de la imagen.
 */
template<class T>
CImg<T> obtener_cuadrante( const CImg<T> &imagen, int cuadrante ) {
  switch(cuadrante) {
  case 1: return imagen.get_crop( imagen.width()/2, imagen.height()/2,
				  imagen.width(), imagen.height() );
  case 2: return imagen.get_crop( 0, imagen.height()/2,
				  imagen.width()/2, imagen.height() );
  case 3: return imagen.get_crop( 0, 0,
				  imagen.width()/2, imagen.height()/2 );
  case 4: return imagen.get_crop( imagen.width()/2, 0,
				  imagen.width(), imagen.height()/2 );
  }
  return imagen;
}

/**
 * estadisticas_imagen()
 * calcula una serie de estadísticas sobre la imagen pasada como parámetro:
 * valor medio (media aritmética), mediana y desviación absoluta del histograma;
 * y los mismos tres valores para las sumas (perfiles) horizontal y vertical.
 * Además, estos nueve valores se calculan para los cuatro cuadrantes de la
 * imagen si el parámetro cuadrante vale 0. Si vale 1-4, los nueve valores se
 * calculan sobre el cuadrante dado por el valor de cuadrante; en cualquier otro
 * caso, los nueve valores se calculan sobre la imagen entera solamente.
 * @param imagen la imagen sobre la que se calcularán las medidas de histograma
 * @param cuadrante el cuadrante sobre el cual calcular, si 0 se calcula sobre
 *                  imagen entera + 4 cdrants, si otro valor sólo imagen entera
 * @return un vector horizontal CImg<double> con los valores calculados, de
 *         dimensión 45 si cuadrante=0, 9 en otro caso.
 */
template<class T>
CImg<double> estadisticas_imagen(const CImg<T> &imagen, int cuadrante = 0) {
  CImg<double> resultados;
  if ( cuadrante == 0)
    resultados.assign(45, 1, 1, 1, 0);
  else
    resultados.assign(9, 1, 1, 1, 0);

  CImg<T> img2 = obtener_cuadrante(imagen, cuadrante);

  // achico la imagen a un tamaño normalizado, para que sus sumas horizontal y
  // vertical tengan el mismo tamaño. El histograma de la imagen general también
  // deberé calcularlo sobre una versión escalada de la imagen.
  unsigned dim_menor, dim_norm, dim_norm2;
  if ( img2.width() > img2.height() )
    dim_menor = img2.height();
  else
    dim_menor = img2.width();
  dim_norm = floor(sqrt(dim_menor));
  dim_norm2 = pow(dim_norm,2);

  // acomodo la img para que quede cuadrada
  img2.resize( dim_norm2, dim_norm2, -100, -100, 3);

  // calculo los perfiles horiz y vert.
  CImg<T> sum_h = sumar_dim<T> (img2, 'y', true),
    sum_v = sumar_dim<T> (img2, 'x', true);

  // achico ahora a norm*norm
  img2.resize( dim_norm, dim_norm, -100, -100, 3);

  // ahora las 3 imágenes sum_h, sum_v y cuadrada tiene todas el mismo número
  // de pixeles, lo que hace a sus respectivos histogramas equiparables

  // calculo los histogramas de la imagen entera y las sumas
  CImg<double> histo = img2.get_histogram(256), // / (double)dim_norm,
    histo_h = sum_h.get_histogram(256), // / (double)dim_norm,
    histo_v = sum_v.get_histogram(256); // / (double)dim_norm;

  resultados(0) = img2.mean();
  resultados(1) = mediana(histo);
  resultados(2) = absdev(histo, resultados(1)); 
  resultados(3) = sum_h.mean();
  resultados(4) = mediana(sum_h);
  resultados(5) = absdev(sum_h, resultados(4));
  resultados(6) = sum_v.mean();
  resultados(7) = mediana(histo_v);
  resultados(8) = absdev(histo_v,resultados(7));

  resultados /= (double) dim_norm2;

  // si cuadrante=0, agrego al vector de resultados los datos de cada cuadrante
  if ( cuadrante == 0 ) {
    unsigned idx = 9, x = 0;
    CImg<double> temp;
    for ( unsigned i=1; i<5; i++ ) {
      temp = estadisticas_imagen( imagen, i );
      cimg_forX( temp, x ) {
	resultados(x+idx) = temp(x);
      }
      idx += 9;
    }
  }

  return resultados;
}

/**
 * error_estadisticas_imagen()
 * calcula el MSE entre los vectores de estadísticas de dos imágenes.
 * @param img1, img2 las imágenes a comparar
 * @return MSE (double) entre los vectores de estadísticas de las imágenes.
 */
template<class T>
double error_estadisticas_imagen(const CImg<T> &img1,
				 const CImg<T> &img2,
				 double normalize_min = 0,
				 double normalize_max = 255) {
  CImg<double> eimg1 = estadisticas_imagen<double> (img1);
  CImg<double> eimg2 = estadisticas_imagen<double> (img2);
  return eimg1.normalize(normalize_min,
			 normalize_max).MSE(eimg2.normalize( normalize_min,
							     normalize_max ));
}


/**
 * error_estadisticas_imagen()
 * versión que para CImglists de error_estadisticas_imagen
 * @return el MSE promedio entre las imágenes de las listas
 */
template<class T>
double error_estadisticas_imagen( const CImgList<T> &img1,
				  const CImgList<T> &img2,
				  double normalize_min = 0,
				  double normalize_max = 255 ) {
  double error = 0;
  unsigned int i = 0;
  for (i; i < img1.size(); i++) {
    error += error_estadisticas_imagen( img1[i], img2[i],
					normalize_min, normalize_max );
  }
  return error / (float) i;
}

#endif
