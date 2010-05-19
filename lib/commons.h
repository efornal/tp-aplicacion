#include <CImg.h>

using namespace cimg_library;

template<class T>
CImgList<T> segmentar( CImg<T> img, int ancho=20, int alto=20 ) {

    int nro_cuadros = (img.width()/ancho) * (img.height()/alto);
    int cont = 0;
    CImgList<T> cuadros;

    for (int y=0; y < img.height() && y+alto < img.height(); y+=alto ) {
        for (int x=0; x < img.width() && x+ancho < img.width(); x+=ancho ) {
            cuadros.insert( img.get_crop( x, y, x+ancho-1, y+alto-1, true ), cont++ );
        }
    }

    return cuadros;
}
