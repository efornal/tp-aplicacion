#include <CImg.h>
#include <commons.h>

using namespace cimg_library;
using namespace std;

int main( int argc, char **argv ) {
    const char *filename = cimg_option( "-f",
                                        "imagenes/la-piedad.jpg",
                                        "ruta archivo imagen" );
    int ancho = cimg_option ( "-ancho", 20, "ancho de recuadro");
    int alto  = cimg_option ( "-alto", 20, "alto de recuadro");

    CImgDisplay disp,disp2;
    CImg<double> img (filename);
    img.channel(0); //temporalmente
    img.display(disp);

    CImgList<double> lista = segmentar( img, ancho, alto );

    lista.display(disp2);
    
    while ( (!disp.is_closed() &&  !disp.is_keyQ()) ) {
        disp.wait();
    }
    return 0;
}
