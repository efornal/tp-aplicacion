#ifndef pdi_masks
#define pdi_masks 1

#include <CImg.h>
#include <math.h>

using namespace cimg_library;

namespace masks {

    //==================================================
    //                 SEGMENTACION
    //==================================================
    /**
     * mascara generica:
     *                  | z1  z2  z3 |
     *                  | z4  z5  z6 |
     *                  | z7  z8  z9 |
     */

    /**
     * retorna una mascara de Roberts Gx
     *  Gx = z9 - z5
     *   -1  0   
     *    0  1   
     */
    CImg<double> roberts_gx() {
        CImg<double> mask( 2, 2, 1, 1, 0);
        mask(0,0) = -1;
        mask(1,1) =  1;
        return mask;
    }

    /**
     * retorna una mascara de Roberts Gy
     *  Gx = z8 - z6
     *   0  -1   
     *   1   0    
     */
    CImg<double> roberts_gy() {
        CImg<double> mask( 2, 2, 1, 1, 0);
        mask(1,0) = -1;
        mask(0,1) =  1;
        return mask;
    }

    /**
     * retorna una mascara de Prewitt Horizontal
     *  Gx = (z7 + z8 + z9) - (z1 + z2 + z3)
     *   -1  -1  -1   
     *    0   0   0   bordes ---
     *    1   1   1
     */
    CImg<double> prewitt_gx() {
        CImg<double> mask( 3, 3, 1, 1, 0);
        cimg_forX(mask,x){
            mask(x,0) = -1;
            mask(x,2) =  1;
        }
        return mask;
    }

    /**
     * retorna una mascara de Prewitt Vertical
     *  Gx = (z3 + z6 + z9) - (z1 + z4 + z7)
     *   -1   0   1   
     *   -1   0   1   bordes |
     *   -1   0   1
     */
    CImg<double> prewitt_gy() {
        CImg<double> mask( 3, 3, 1, 1, 0);
        cimg_forY(mask,y){
            mask(0,y) = -1;
            mask(2,y) =  1;
        }
        return mask;
    }

    /**
     * retorna una mascara de Prewitt Diagonal x->y
     *   0   1   1   
     *  -1   0   1   bordes                     \
     *  -1  -1   0
     */
    CImg<double> prewitt_gxy() {
        CImg<double> mask( 3, 3, 1, 1, 0);
        mask(0,1) = -1;
        mask(0,2) = -1;
        mask(1,2) = -1;
        mask(1,0) =  1;
        mask(2,0) =  1;
        mask(2,1) =  1;
        return mask;
    }

    /**
     * retorna una mascara de Prewitt Diagonal y->x
     *   -1  -1   0   
     *   -1   0   1   bordes /
     *    0   1   1
     */
    CImg<double> prewitt_gyx() {
        CImg<double> mask( 3, 3, 1, 1, 0);
        mask(0,0) = -1;
        mask(0,1) = -1;
        mask(1,0) = -1;
        mask(2,1) =  1;
        mask(2,2) =  1;
        mask(1,2) =  1;
        return mask;
    }

    /**
     * retorna una mascara de Sobel Horizontal
     * Gx = (z7 + 2z8 + z9) - (z1 + 2z2 + z3)
     *  -1  -2  -1
     *   0   0   0   bordes --
     *   1   2   1
     */
    CImg<double> sobel_gx() {
        CImg<double> mask( 3, 3, 1, 1, 0);
        mask(0,0) = -1;
        mask(1,0) = -2;
        mask(2,0) = -1;
        mask(0,2) =  1;
        mask(1,2) =  2;
        mask(2,2) =  1;
        return mask;
    }

    /**
     * retorna una mascara de Sobel Vertical
     * Gy = (z3 + 2z6 + z9) - (z1 + 2z4 + z7)
     *  -1   0   1
     *  -2   0   2   bordes |
     *  -1   0   1
     */
    CImg<double> sobel_gy() {
        CImg<double> mask( 3, 3, 1, 1, 0);
        mask(0,0) = -1;
        mask(0,1) = -2;
        mask(0,2) = -1;
        mask(2,0) =  1;
        mask(2,1) =  2;
        mask(2,2) =  1;
        return mask;
    }

    /**
     * retorna una mascara de Sobel Diagonal x->y
     *   0   1   2
     *  -1   0   1   bordes \
     *  -2  -1   0
     */
    CImg<double> sobel_gxy() {
        CImg<double> mask( 3, 3, 1, 1, 0);
        mask(0,1) = -1;
        mask(0,2) = -2;
        mask(1,2) = -1;
        mask(1,0) =  1;
        mask(2,0) =  2;
        mask(2,1) =  1;
        return mask;
    }

    /**
     * retorna una mascara de Sobel Diagonal y->x
     *  -2  -1   0
     *  -1   0   1   bordes /
     *   0   1   2
     */
    CImg<double> sobel_gyx() {
        CImg<double> mask( 3, 3, 1, 1, 0);
        mask(0,0) = -2;
        mask(0,1) = -1;
        mask(1,0) = -1;
        mask(2,1) =  1;
        mask(2,2) =  2;
        mask(1,2) =  1;
        return mask;
    }

    /**
     * retorna una mascara Laplaciano N4
     * Lap{f} = gradiente{f}^2 = 4z5 - (z2 + z4 + z6 + z8)
     *   0  -1   0
     *  -1   4  -1    N=4
     *   0  -1   0
     */
    CImg<double> laplaciano_n4() {
        CImg<double> mask( 3, 3, 1, 1, 0);
        mask(0,1) = -1;
        mask(1,0) = -1;
        mask(1,2) = -1;
        mask(2,1) = -1;
        mask(1,1) =  4;
        return mask;
    }

    /**
     * retorna una mascara Laplaciano N8
     * Lap{f} = gradiente{f}^2 = 8z5 - (z1 + z2 + + z3 + z4 + z6 + z7 + z8)
     *  -1  -1  -1
     *  -1   8  -1    N=8
     *  -1  -1  -1
     */
    CImg<double> laplaciano_n8() {
        CImg<double> mask( 3, 3, 1, 1, -1);
        mask(1,1) =  8;
        return mask;
    }

    /**
     * retorna una mascara Laplaciano del gaussiano - LoG
     *
     *              - r^2 / 2sigma^2
     *  h(r)   = -e
     *
     *              (r^2) - sigma^2     - r^2 / 2sigma^2
     *  Lap{h} = -  --------------- . e
     *                  sigma^4     
     *
     *    con  r^2 = (x^2 + y^2)
     *
     *   0   0   -1   0   0
     *   0  -1   -2  -1   0
     *  -1  -2   16  -2  -1
     *   0  -1   -2  -1   0
     *   0   0   -1   0   0
     */
    CImg<double> laplaciano_g() {
        CImg<double> mask( 5, 5, 1, 1, 0);
        mask(0,2) =  -1;
        mask(1,1) =  -1;
        mask(1,3) =  -1;
        mask(2,0) =  -1;
        mask(2,4) =  -1;
        mask(3,1) =  -1;
        mask(3,3) =  -1;
        mask(4,2) =  -1;
        mask(1,2) =  -2;
        mask(2,1) =  -2;
        mask(2,3) =  -2;
        mask(3,2) =  -2;
        mask(2,2) =  16;
        return mask;
    }

}
#endif
