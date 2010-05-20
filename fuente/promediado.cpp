#define cimg_use_fftw3 1

#ifdef cimg_use_fftw3
extern "C" {
#include "fftw3.h"
}
#endif

#include <CImg.h>
#include "../lib/commons.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char **argv) {

    // proceso:
    // lista -> lista-promediar-bordes-normalizar-segmentar
    // match -> img-bordes-normalizar-segmentar

    CImgList<double> lista;
    CImg<double> tmp;
    CImg<double> match( argv[1] );

    match = detectar_bordes( match, 2.0, 70.0 ).normalize( 0, 1 );

    for (int i = 2; i < argc; i++) {
        tmp.assign( argv[i] );
        lista.push_back(tmp);
    }

    CImg<double> promediada = detectar_bordes( promedio(lista), 2.0, 70.0 ).
        normalize( 0, 1 );
    CImgList<double> match_segmentada = segmentar( match, 100, 100 );
    CImgList<double> promediada_segmentada = segmentar( promediada, 100, 100 );

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
