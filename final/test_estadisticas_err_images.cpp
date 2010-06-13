//#include "../lib/commons.h"
#include <CImg.h>
#include "def_clase_2.h"

using namespace cimg_library;

int main(int argc, char **argv) {
  const char *filename = cimg_option( "-f", "./imagenes/prueba/fich10.jpg",
				     "primera imagen a comparar" );
  const char *base_dir = cimg_option( "-base", "./imagenes/base/",
				      "directorio con el que generar la base" );
  const char *prueba_dir = cimg_option( "-prueba", "./imagenes/prueba/",
				      "directorio con imgs de prueba" );

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



  CImg<double> img(filename);
  vector<CImg<double> > caract(  comp.n_imagenes );
  CImg<double> caract_img = estadisticas_imagen( img );

  for (int i=0; i <  comp.n_clases; i++ ) {
    
    printf("MSE %s \t\t %f \n",  
           string(comp.etiqueta(i)).c_str(),
           caract_img.MSE( comp.caracteristicas[i][0] ) );
    //comp.caracteristicas[i][1].display();
  }

}
