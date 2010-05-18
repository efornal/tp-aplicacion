#include <CImg.h>

using namespace cimg_library;
using namespace std;

int main( int argc, char **argv ) {
    const char *filename = cimg_option( "-f",
                                        "imagenes/la-piedad.jpg",
                                        "ruta archivo imagen" );
    CImgDisplay disp;
    CImg<double> img (filename);

    img.display(disp);

    while ( (!disp.is_closed() &&  !disp.is_keyQ()) ) {
        disp.wait();
    }
    return 0;
}
