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
    const char *filename = cimg_option( "-f",
                                        "imagenes/la-piedad.jpg",
                                        "ruta archivo imagen" );
    int wc = cimg_option ("-wc", 70, "frec corte");
    int orden = cimg_option ("-orden", 2, "orden");
    // proceso:
    // lista -> lista-promediar-bordes-normalizar-segmentar
    // match -> img-bordes-normalizar-segmentar

    CImgList<double> lista;
    CImg<double> tmp;
    CImg<double> match( filename );

    match = detectar_bordes( match, orden, wc ).normalize(0,255);
    //match = match.normalize(0,255);


    // for (int i = 2; i < argc; i++) {
    //     tmp.assign( argv[i] );
    //     lista.push_back(tmp);
    // }
    CImg<double> chaco00("../imagenes/chaco/chaco00.jpg"),
        chaco01("../imagenes/chaco/chaco01.jpg"),
        chaco02("../imagenes/chaco/chaco02.jpg"),
        chaco03("../imagenes/chaco/chaco03.jpg"),
        chaco04("../imagenes/chaco/chaco04.jpg"),
        chaco05("../imagenes/chaco/chaco05.jpg"),
        chaco06("../imagenes/chaco/chaco06.jpg"),
        chaco07("../imagenes/chaco/chaco07.jpg"),
        chaco08("../imagenes/chaco/chaco08.jpg"),
        chaco09("../imagenes/chaco/chaco09.jpg");
    lista.push_back(chaco00);
    lista.push_back(chaco01);
    lista.push_back(chaco02);
    lista.push_back(chaco03);
    lista.push_back(chaco04);
    lista.push_back(chaco05);
    lista.push_back(chaco06);
    lista.push_back(chaco07);
    lista.push_back(chaco08);
    lista.push_back(chaco09);

    CImg<double> promediada = detectar_bordes( promedio(lista), orden, wc ).normalize(0,255);
    //CImg<double> promediada = promedio(lista).normalize(0,255);

    promediada.threshold(127);
    match.threshold(127);

    CImgList<double> match_segmentada = segmentar( match, 100, 100 );
    CImgList<double> promediada_segmentada = segmentar( promediada, 100, 100 );

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
