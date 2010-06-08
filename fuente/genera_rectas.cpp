/**
 * objetivo:
 * dado que detectores_mayores.cpp tomaba valores mayores sin sentido
 * la idea seria tomar mayores de la matriz de hough directa, 
 * seleccionar max y las inversas generadas deberian ser rectas
 * que solapandose minimizan MSE.
 * 
 * pruebas:
 * ../genera_rectas -size 100 -seg 100 -ver 1 -puntos 5
 * conclusion:
*/
#ifdef cimg_use_fftw3
extern "C" {
#include "fftw3.h"
}
#endif

#include <CImg.h>
#include <filtrado_espacial.h>
#include <temporales.h>
#include <commons.h>

using namespace cimg_library;
using namespace std;


int main(int argc, char **argv) {
    const char *filename1 = cimg_option( "-f1",
        "../imagenes/chaco/prueba/chaco12.jpg",
        "ruta archivo imagen" );
    const char *filename2 = cimg_option( "-f2",
        "../imagenes/trenfrente/prueba/tren_frente10.jpg",
        "ruta archivo imagen" );
    int umbral = cimg_option("-umbral" , 127, "umbral threshold");
    int size   = cimg_option("-size" , 100, "tama�o imagen a segmentar");
    int seg    = cimg_option("-seg" , 10, "tama�o cuadritos segmentacion");
    int ver    = cimg_option("-ver" , 0, "ver imagenes - plot");
    int puntos    = cimg_option("-puntos" , 5, "puntos para la inversa de hough");

    CImgDisplay disp1, disp2, disp3, disp4, 
        disp5, disp6, disp7,
        disp10, disp11, disp12,
        disp13, disp14, disp15,
        disp16, disp17, disp18;
    CImg<double> img1 ( filename1 ), img2( filename2 );
    CImgList<double> lista = lista_con_img_chaco_y_un_canal();
    img1.channel(0);
    img2.channel(0);
    CImg<double> promediada = promedio(lista);

    CImg<double> promediada_sobel = filtrado_sobel(promediada);
    CImg<double> parecida_sobel   = filtrado_sobel(img1);
    CImg<double> distinta_sobel   = filtrado_sobel(img2);

    //====================== sobel =======================

    CImgList<double> 
        lista5 ( promediada_sobel.get_resize(300,300), 
                 parecida_sobel.get_resize(300,300), 
                 distinta_sobel.get_resize(300,300) );
    if (ver) lista5.display(disp5);
    disp5.set_title("promediada - parecida - distinta");

    printf( "MSE promedio con parecida (con sobel): %f \n", 
            promediada_sobel.MSE( parecida_sobel ) );
    printf( "MSE promedio con distinta (con sobel): %f \n", 
            promediada_sobel.MSE( distinta_sobel ) );

    promediada_sobel.resize(size,size);
    CImgList<double> prom_seg_sob = segmentar(promediada_sobel , seg, seg);
    parecida_sobel.resize(size,size);
    CImgList<double> pare_seg_sob = segmentar(parecida_sobel , seg, seg);
    distinta_sobel.resize(size,size);
    CImgList<double> dist_seg_sob = segmentar(distinta_sobel , seg, seg);

    printf( "MSE promedio con parecida (con sobel) segmentada + hough: %f \n",
            calcular_mse( prom_seg_sob, pare_seg_sob ) );
    printf( "MSE promedio con distinta (con sobel) segmentada + hough: %f \n", 
            calcular_mse( prom_seg_sob, dist_seg_sob ) );


    for (unsigned i = 0; i < prom_seg_sob.size(); i++) {

        prom_seg_sob(i) = 
            //            get_solo_maximos(
            hough_directa( get_solo_maximos(prom_seg_sob(i), puntos));
        pare_seg_sob(i) = 
            //            get_solo_maximos(
            hough_directa( get_solo_maximos(pare_seg_sob(i), puntos));
        dist_seg_sob(i) = 
            //            get_solo_maximos(
            hough_directa( get_solo_maximos(dist_seg_sob(i), puntos));
        
    }

    printf( "MSE promedio con parecida (con sobel) segmentada: %f \n",
            calcular_mse( prom_seg_sob, pare_seg_sob ) );
    printf( "MSE promedio con distinta (con sobel) segmentada: %f \n", 
            calcular_mse( prom_seg_sob, dist_seg_sob ) );
    printf( "## diferencia: %f \n", 
            calcular_mse( prom_seg_sob, dist_seg_sob ) -
            calcular_mse( prom_seg_sob, pare_seg_sob ) );
 for (unsigned i = 0; i < prom_seg_sob.size(); i++) {

        prom_seg_sob(i).resize(300,300);
        pare_seg_sob(i).resize(300,300);
        dist_seg_sob(i).resize(300,300);
 }
    if(ver){
        prom_seg_sob.display(disp13); disp13.set_title("prom_seg_sob");
        pare_seg_sob.display(disp14); disp14.set_title("pare_seg_sob");
        dist_seg_sob.display(disp15); disp15.set_title("dist_seg_sob");
    }


    while ((!disp5.is_closed() && !disp5.is_keyQ())) {
        disp5.wait_all();
    }

}
