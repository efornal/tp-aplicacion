//#include "../lib/commons.h"
#include <CImg.h>
#include "def_clase_2.h"

using namespace cimg_library;

int main(int argc, char **argv) {
  const char *imagen1 = cimg_option( "-img1", "./imagenes/prueba/fich10.jpg",
				     "primera imagen a comparar" );
  const char *base_dir = cimg_option( "-base", "./imagenes/base/",
				      "directorio con el que generar la base" );
  const char *prueba_dir = cimg_option( "-prueba", "./imagenes/prueba/",
				      "directorio con imgs de prueba" );
  const char *proto_dir = cimg_option( "-proto", "./imagenes/prototipos/",
				      "directorio con imagenes de prototipos" );

  int tamanio = cimg_option ( "-tam", 20, "lado de recuadro");

  ComparadorImagenes<double> comp;
  int t;
  //  int t = comp.cargar_imagenes( proto_dir );
  //  printf( "Se han cargado %d imagenes de la base.\n", t );

  //    t = comp.etiquetar_imagenes();
  //  printf( "Se han etiquetado %d imagenes.\n", t );

  // t = comp.calcular_caracteristicas ();
  // printf( "Se han calculado %d caracteristicas de las imagenes de la base.\n", t );

  // t = comp.generar_prototipos();
  // if ( t==0 )
  //   printf( "Se han generado los prototipos correctamente.\n" );

  //// t = comp.encontrar_mas_parecida( CImg<double>( imagen1 ) );
  //// printf( "La imagen mas parecida es:\n%s\n", comp.nombre( t ).c_str() );

  // comp.cargar_prototipos( );

  t = comp.clasificar_la_imagen( CImg<double>(imagen1), proto_dir );
  //  printf( "La imagen es de la clase:\n%s\n", comp.etiqueta( t ).c_str() );

  // vector<string> nombres; vector<int> clases;
  // t = comp.clasificar_directorio( prueba_dir, nombres, clases );
  // for (unsigned i=0; i<nombres.size(); i++ )
  //   printf( "La imagen %s es de la clase:\n%s\n", nombres[i].c_str(), comp.etiqueta( clases[i] ).c_str() );


  return 0;
}
