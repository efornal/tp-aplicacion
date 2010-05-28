#include <CImg.h>
#include <masks.h>

using namespace std;
using namespace cimg_library;

template<class T>
CImg<T> filtrado_roberts( CImg<T> &img ) {
    return img.get_convolve( masks::roberts_gx() ).abs() +
        img.get_convolve( masks::roberts_gy() ).abs();
}

template<class T>
CImg<T> filtrado_prewitt( CImg<T> &img ) {
    return img.get_convolve( masks::prewitt_gx() ).abs() +
        img.get_convolve( masks::prewitt_gy() ).abs() +
        img.get_convolve( masks::prewitt_gxy() ).abs() +
        img.get_convolve( masks::prewitt_gyx() ).abs();
}

template<class T>
CImg<T> filtrado_sobel( CImg<T> &img ) {
    return img.get_convolve( masks::sobel_gx() ).abs() +
        img.get_convolve( masks::sobel_gy() ).abs() +
        img.get_convolve( masks::sobel_gxy() ).abs() +
        img.get_convolve( masks::sobel_gyx() ).abs();
}

template<class T>
CImg<T> filtrado_laplaciano_g( CImg<T> &img ) {
    return img.get_convolve( masks::laplaciano_g() ).abs();
}
