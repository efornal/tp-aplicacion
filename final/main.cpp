//#include "../lib/commons.h"
#include <CImg.h>
#include "def_clase_2.h"

using namespace cimg_library;

int main(int argc, char **argv) {
  const char *imagen1 = cimg_option( "-img1", "./imagenes/chaco/chaco00.jpg",
				     "primera imagen a comparar" );
  const char *imagen2 = cimg_option( "-img2", "./imagenes/trenfrente/tren_frente01.jpg",
				     "segunda imagen a comparar" );
  const char *base_dir = cimg_option( "-base", "./imagenes/chaco/",
				      "directorio con el que generar la base" );
  int tamanio = cimg_option ( "-tam", 20, "lado de recuadro");

  ComparadorImagenes<double> comp;

  int t = comp.cargar_imagenes( base_dir );
  printf( "Se han cargado %d imagenes en la base.\n", t );

  t = comp.calcular_caracteristicas ();
  printf( "Se han calculado %d caracteristicas de las imagenes de la base.\n", t );

  t = comp.encontrar_mas_parecida( CImg<double>( imagen1 ) );
  printf( "La imagen mas parecida es:\n%s\n", comp.nombre( t ).c_str() );

  return 0;
}
