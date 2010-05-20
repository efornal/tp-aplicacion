// objetivo:
// hacer el siguente proceso:
// lista -> lista-promediar-bordes-normalizar-segmentar
// match -> img-bordes-normalizar-segmentar
//
// ../frec_corte -f ../imagenes/trenfrente/prueba/tren_frente10.jpg -wc 127
//
//    Aplicar un umbral luego de pasar la imagen por un pasaaltos en frecuencia
//    genera un contraste mayor en la imagen, caracterizado mayormente por bordes.
//    esto tiende mas al objetivo del trabajo, el cual consiste en independizar
//    los bordes del resto de la info de la imagen y usar estos para cotejar.
//    Se podria probar con alta pontecia o alguno que asent�e los bordes y
//    elimine el resto de manera mas adecuada a como se hace con esta forma.
 
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
                                        "imagenes/la-piedad.jpg",
                                        "ruta archivo imagen" );
    int wc = cimg_option ("-wc", 70, "frec corte");
    int orden = cimg_option ("-orden", 2, "orden");

    CImgList<double> lista = lista_con_img_chaco();;
    CImg<double> tmp;
    CImg<double> match( filename );

    match = detectar_bordes( match, orden, wc ).normalize(0,255);;

    CImg<double> promediada = detectar_bordes( promedio(lista), orden, wc ).
        normalize(0,255);
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
