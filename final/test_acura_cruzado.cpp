/**
 * se da por echo que 
 * estan todas las img de pruebas correspondientes ojo!
 * mismo nombre por lo que se listan ordenadas igual por separado
*/
//#include "../lib/commons.h"
#include <CImg.h>
#include "def_clase_2.h"

using namespace cimg_library;

// lista solo las *01.match
vector<string> listar_imagenes_pruebas(const char* directorio) {
  vector<string> lista_imagenes;
  glob_t globbuf;
  regex_t match;
  regcomp( &match, ".*11\\.\\(jpe\\?g\\|bmp\\tif{1,2}\\|png\\|gif\\)$",
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


int main(int argc, char **argv) {
  const char *filename = cimg_option( "-f", "./imagenes/pruebas/",
				     "primera imagen a comparar" );
  const char *base_dir = cimg_option( "-base", "./imagenes/base/",
				      "directorio con el que generar la base" );
  const char *pruebas_dir = cimg_option( "-pruebas", "./imagenes/pruebas/",
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

  vector<string> pruebas = listar_imagenes_pruebas( pruebas_dir );
  vector<CImg<double> > caract_pruebas(  comp.n_imagenes );
  
  for (int j=0; j < comp.n_clases; j++ ) {
    printf("etiqueta prueba %d: %s => %s \n", j,
           string( pruebas[j]).c_str(),
           string(comp.etiqueta(j)).c_str() );

    caract_pruebas[j] = 
      acura( CImg<double> ( string( pruebas[j]).c_str()) );
  }

  for (int i=0; i <  comp.n_clases; i++ ) {
    for (int j=0; j < comp.n_clases; j++ ) {
      printf("MSE %s -> %s \t\t\t %f \n",  
             string(comp.etiqueta(i)).c_str(),
             string(comp.etiqueta(j)).c_str(),
             caract_pruebas[i].MSE( comp.caracteristicas[j][2] ) );
      //comp.caracteristicas[i][1].display();
    }
  }


}
