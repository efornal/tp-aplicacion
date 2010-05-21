// objetivo:
// hacer el siguiente proceso:
// lista -> lista-promediar-bordes-absoluto-segmentar
// match -> img-bordes-absoluto-segmentar

#define cimg_use_fftw3 1

#ifdef cimg_use_fftw3
extern "C" {
#include "fftw3.h"
}
#endif

#include <CImg.h>
#include "../lib/commons.h"
#include "../lib/temporales.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char **argv) {
    const char *filename = cimg_option( "-f",
                                        "../imagenes/la-piedad.jpg",
                                        "ruta archivo imagen" );
    int wc = cimg_option ("-wc", 70, "frec corte");
    int orden = cimg_option ("-orden", 2, "orden");

    CImgList<double> lista = lista_con_img_chaco();
    CImg<double> tmp;
    CImg<double> match( filename );
    
    match = abs(detectar_bordes( match, orden, wc ));

    CImg<double> promediada = abs(detectar_bordes( promedio(lista), orden, wc ));
    CImgList<double> match_segmentada = segmentar( match, 40, 40 );
    CImgList<double> promediada_segmentada = segmentar( promediada, 40, 40 );

    match.print("match");
    promediada.print("promediada");

    printf("Error entre imagenes: %f\n", 
           promediada.MSE(match));
    printf("Error entre segmentadas: %f\n",  
           calcular_mse(match_segmentada,promediada_segmentada));

    CImgDisplay disp( promediada, "promediada"),
        disp2( match, "match");

    while (!disp.is_closed()) {
        disp.wait();
    }

    return 0;
}
