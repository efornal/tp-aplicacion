/*
 * comparar_estadisticas.cpp
 * objetivo: comparar dos imágenes con una base, la que se genera promediando las imágenes
 *           contenidas en el directorio pasado como parámetro -base, según distribuciones
 *           estadísticas como el histograma conjunto y la distribución sumada vertical y
 *           horizontal, por ejemplo.
 * @TODO manejar colores!!!!!!!!
 */

// #define cimg_use_fftw3 1

// #ifdef cimg_use_fftw3
// extern "C" {
// #include "fftw3.h"
// }
// #endif

#include <CImg.h>
#include "../lib/filtrado_espacial.h"
#include "../lib/temporales.h"
#include "../lib/commons.h"

using namespace cimg_library;

int main(int argc, char **argv) {
  const char *imagen1 = cimg_option( "-img1", "./imagenes/chaco/chaco00.jpg",
				     "primera imagen a comparar" );
  const char *imagen2 = cimg_option( "-img2", "./imagenes/trenfrente/tren_frente01.jpg",
				     "segunda imagen a comparar" );
  const char *base_dir = cimg_option( "-base", "./imagenes/chaco/",
				      "directorio con el que generar la base" );
  int tamanio = cimg_option ( "-tam", 20, "lado de recuadro");

  // cargo las imágenes
  CImg<double> img1( imagen1 ), img2( imagen2 );
  CImg<double> base = generar_base<double>( base_dir );

  // convierto las imágenes a HSV y me quedo con el canal de valor
  base.RGBtoHSI();
  img1.RGBtoHSI();
  img2.RGBtoHSI();
  base.channel(2);
  img1.channel(2);
  img2.channel(2);

  CImgList<double> l_base = segmentar ( base, tamanio, tamanio );
  CImgList<double> l_img1 = segmentar ( img1, tamanio, tamanio );
  CImgList<double> l_img2 = segmentar ( img2, tamanio, tamanio );
  
  printf("mse estadisticas img1-base: %f\n", error_estadisticas_imagen(img1,base) );
  printf("mse estadisticas img2-base: %f\n", error_estadisticas_imagen(img2,base) );

  printf("mse estadisticas img1-base segmentadas: %f \n", error_estadisticas_imagen<double>(l_img1,l_base) );
  printf("mse estadisticas img2-base segmentadas: %f \n", error_estadisticas_imagen<double>(l_img2,l_base) );

  return 0;
}
