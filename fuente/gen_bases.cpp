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
    if ( argc < 2 ) {
        printf ( "uso:\n %s base img1 img2 ...imgn\n", argv[0]);
        return 0;
    }
    // proceso:
    // lista -> lista-promediar-bordes-normalizar-segmentar

    CImgList<double> lista;
    CImg<double> tmp;

    for ( int i = 2; i < argc; i++ ) {
        printf( "Agregada: %s \n", argv[i] );
        tmp.assign(argv[i]);
        lista.push_back(tmp);
    }

    CImg<double> base = generar_base( lista );
    base.save_png( argv[1] );
    printf( "Base generada: %s \n", argv[1] );

    return 0;
}
