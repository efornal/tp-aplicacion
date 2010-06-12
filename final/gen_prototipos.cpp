#include <CImg.h>
#include "def_clase_2.h"

using namespace cimg_library;

int main(int argc, char **argv) {
  const char *base_dir = cimg_option( "-base", "./imagenes/base/",
				      "directorio con el que generar la base" );
  const char *proto_dir = cimg_option( "-proto", "./imagenes/prototipos/",
				      "directorio donde se guardaran los prototipos" );

  int tamanio = cimg_option ( "-tam", 20, "lado de recuadro");

  ComparadorImagenes<double> comp;

  int t = comp.cargar_imagenes( base_dir );
  printf( "Se han cargado %d imagenes en la base.\n", t );

  t = comp.etiquetar_imagenes();
  printf( "Se han etiquetado %d imagenes.\n", t );

  t = comp.calcular_caracteristicas ();
  printf( "Se han calculado %d caracteristicas de las imagenes de la base.\n", t );

  t = comp.generar_prototipos();
  if ( t==0 )
    printf( "Se han generado los prototipos correctamente.\n" );

  t = comp.guardar_prototipos( proto_dir );
  if ( t==0 )
    printf( "Se han guardado los prototipos correctamente.\n" );

  return 0;
}
