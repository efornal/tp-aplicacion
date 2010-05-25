#include <CImg.h>
#include <masks.h>

using namespace std;
using namespace cimg_library;

template<class T>
CImg<T> filtrado_roberts( CImg<T> &img ) {
    return img.convolve( masks::roberts_gx() ) +
        img.convolve( masks::roberts_gy() );
}

template<class T>
CImg<T> filtrado_prewitt( CImg<T> &img ) {
    return img.convolve( masks::prewitt_gx() ) +
        img.convolve( masks::prewitt_gy() ) +
        img.convolve( masks::prewitt_gxy() ) +
        img.convolve( masks::prewitt_gyx() );
}

template<class T>
CImg<T> filtrado_sobel( CImg<T> &img ) {
    return img.convolve( masks::sobel_gx() ) +
        img.convolve( masks::sobel_gy() ) +
        img.convolve( masks::sobel_gxy() ) +
        img.convolve( masks::sobel_gyx() );
}

template<class T>
CImg<T> filtrado_laplaciano_g( CImg<T> &img ) {
    return img.convolve( masks::laplaciano_g() );
}
