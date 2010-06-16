#include <CImg.h>
#include "def_clase_2.h"

#include <unistd.h>
#include "gnuplot_i.hpp"

using namespace cimg_library;

int main(int argc, char **argv) {
  double pon1 =
    atof( cimg_option("-pe",
                      "0.0",
                      "Ponderacion Met. estadisticas (debe ser = 0)"));
  double pon2 =
    atof( cimg_option("-ph",
                      "2.0",
                      "Ponderacion Met. Hough"));
  string tipo_grafico = cimg_option( "-tg",
                                          "lines",
                                          "tipo se linea en el grafico" );

  int umbral = cimg_option ( "-umbral", 90, "umbral en sobel");
  int delta  = cimg_option ( "-delta", 10, "paso o incremento de umbral");
  int minimo = cimg_option ( "-min", 10, "minimo valor de umbral (inicio)");
  int maximo = cimg_option ( "-max", 200, "maximo valor de umbral");

  vector<string> base_dir, prueba_dir;

  base_dir.push_back("./imagenes/base1/");
  prueba_dir.push_back("./imagenes/prueba1/");

  vector<double> error;
  vector<double> eje_x;
  double err = 0;

// ============== calculo para hough ================

  for( int cant_maximos=minimo; cant_maximos <= maximo; cant_maximos+=delta ) {

    printf("cantidad maximos: %d \n", cant_maximos );

    for (int i = 0; i < base_dir.size(); i++) {

      ComparadorImagenes<double> comp;

      comp.cantidad_maximos_hough = cant_maximos ;
      comp.umbral_hough = umbral;

      int t = comp.cargar_imagenes(string(base_dir[i]).c_str());
      printf("Se han cargado %d imagenes en la base.\n", t);

      t = comp.etiquetar_imagenes();
      printf("Se han etiquetado %d imagenes.\n", t);

      t = comp.calcular_caracteristicas();
      printf("Se han calculado %d caracteristicas de las imagenes de la base.\n",
             t);

      comp.ponderacion(0) = pon1; // anulamos estadisticas
      comp.ponderacion(1) = pon2; // ponderamos por 2 por la division
      printf("ponderaciones: %f %f\n", comp.ponderacion(0), comp.ponderacion(1));

      t = comp.generar_prototipos();
      if (t == 0)
        printf("Se han generado los prototipos correctamente.\n");

      vector<string> nombres;
      vector<int> clases;
      t = comp.clasificar_directorio(string(prueba_dir[i]).c_str(), nombres,
                                     clases);
      for (unsigned k = 0; k < nombres.size(); k++)
        printf("%s: %s\n", nombres[k].c_str(), comp.etiqueta(clases[k]).c_str());

      err = comp.error_clasificacion(string(prueba_dir[i]).c_str());
      printf("#### Error Clasificación: %f ## \n", err);

    } //end for

    error.push_back( err );
    eje_x.push_back( cant_maximos );

  }

  // estilos: lines - points - linespoints -
  //          impulses - dots - steps - errorbars - boxes - boxerrorbars
  Gnuplot g = Gnuplot( "variación de maximos",
                       tipo_grafico,
                       "err.",
                       "cantidad de maximos", eje_x, error );

  return 0;
}
