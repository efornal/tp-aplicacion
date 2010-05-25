/**
 * objetivo:
 * probar el mse con los diferentes detectores de bordes
 * y segmentadas, pero con la mascara binaria, no con niveles de grises
 *
 * pruebas:
 * ../detectores_bordes_binario  -size 100 -seg 10 -ver 1
 * 
 * conclusines
 * Con poca diferencia, pero da menos error con la imagen mas parecida
 * que con la diferente.
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
    int size   = cimg_option("-size" , 100, "tamaño imagen a segmentar");
    int seg    = cimg_option("-seg" , 10, "tamaño cuadritos segmentacion");
    int ver    = cimg_option("-ver" , 0, "ver imagenes - plot");

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

    CImg<double> promediada_roberts = filtrado_roberts(promediada);
    CImg<double> parecida_roberts   = filtrado_roberts(img1);
    CImg<double> distinta_roberts   = filtrado_roberts(img2);

    CImg<double> promediada_sobel = filtrado_sobel(promediada);
    CImg<double> parecida_sobel   = filtrado_sobel(img1);
    CImg<double> distinta_sobel   = filtrado_sobel(img2);

    CImg<double> promediada_laplaciano_g = filtrado_laplaciano_g(promediada);
    CImg<double> parecida_laplaciano_g   = filtrado_laplaciano_g(img1);
    CImg<double> distinta_laplaciano_g   = filtrado_laplaciano_g(img2);

    // solo tomamos la parte positiva ( ~= threshold)
    cimg_forXY(promediada_roberts,x,y){
        promediada_roberts(x,y) = ( promediada_roberts(x,y) < 0 ) ? 0:1;
        parecida_roberts(x,y) =  ( parecida_roberts(x,y) < 0 ) ? 0 : 1;
        distinta_roberts(x,y) = ( distinta_roberts(x,y) < 0 ) ? 0 : 1;

        promediada_sobel(x,y) = ( promediada_sobel(x,y) < 0 )? 0 : 1;
        parecida_sobel(x,y) = ( parecida_sobel(x,y) < 0 ) ? 0 : 1;
        distinta_sobel(x,y) = ( distinta_sobel(x,y) < 0 ) ? 0 : 1;

        promediada_laplaciano_g(x,y) = ( promediada_laplaciano_g(x,y) < 0 ) ? 0 : 1;
        parecida_laplaciano_g(x,y) = ( parecida_laplaciano_g(x,y) < 0 ) ? 0 : 1;
        distinta_laplaciano_g(x,y) =  ( distinta_laplaciano_g(x,y) < 0 ) ? 0 : 1;

    }
    promediada_roberts.normalize(0,1);
    parecida_roberts.normalize(0,1); 
    distinta_roberts.normalize(0,1);

    promediada_sobel.normalize(0,1);
    parecida_sobel.normalize(0,1); 
    distinta_sobel.normalize(0,1);

    promediada_laplaciano_g.normalize(0,1);
    parecida_laplaciano_g.normalize(0,1); 
    distinta_laplaciano_g.normalize(0,1);

    //====================== roberts =======================
    CImgList<double> 
        lista1 ( promediada_roberts.get_resize(300,300), 
                 parecida_roberts.get_resize(300,300), 
                 distinta_roberts.get_resize(300,300) );
    if (ver) lista1.display(disp1);
    disp1.set_title("promediada - parecida - distinta");

    printf( "MSE promedio con parecida (con roberts): %f \n", 
            promediada_roberts.MSE( parecida_roberts ) );
    printf( "MSE promedio con distinta (con roberts): %f \n", 
            promediada_roberts.MSE( distinta_roberts ) );
    printf( "### diferencia:  %f \n", 
            promediada_roberts.MSE( parecida_roberts ) - 
            promediada_roberts.MSE( distinta_roberts ) );

    promediada_roberts.resize(size,size);
    CImgList<double> prom_seg_rob = segmentar(promediada_roberts , seg, seg);
    parecida_roberts.resize(size,size);
    CImgList<double> pare_seg_rob = segmentar(parecida_roberts , seg, seg);
    distinta_roberts.resize(size,size);
    CImgList<double> dist_seg_rob = segmentar(distinta_roberts , seg, seg);

    printf( "MSE promedio con parecida (con roberts) segmentada: %f \n",
            calcular_mse( prom_seg_rob, pare_seg_rob ) );
    printf( "MSE promedio con distinta (con roberts) segmentada: %f \n", 
            calcular_mse( prom_seg_rob, dist_seg_rob ) );
  printf( "### diferencia:  %f \n", 
            calcular_mse( prom_seg_rob, pare_seg_rob )-
            calcular_mse( prom_seg_rob, dist_seg_rob ) );

    for (unsigned i = 0; i < prom_seg_rob.size(); i++) {
        prom_seg_rob(i).resize(300,300);
        pare_seg_rob(i).resize(300,300);
        dist_seg_rob(i).resize(300,300);
    }
    if (ver) {
        prom_seg_rob.display(disp10); disp10.set_title("prom_seg_rob");
        pare_seg_rob.display(disp11); disp11.set_title("pare_seg_rob");
        dist_seg_rob.display(disp12); disp12.set_title("dist_seg_rob");
    }

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
    printf( "### diferencia:  %f \n", 
            promediada_sobel.MSE( parecida_sobel ) - 
            promediada_sobel.MSE( distinta_sobel ) );

    promediada_sobel.resize(size,size);
    CImgList<double> prom_seg_sob = segmentar(promediada_sobel , seg, seg);
    parecida_sobel.resize(size,size);
    CImgList<double> pare_seg_sob = segmentar(parecida_sobel , seg, seg);
    distinta_sobel.resize(size,size);
    CImgList<double> dist_seg_sob = segmentar(distinta_sobel , seg, seg);

    printf( "MSE promedio con parecida (con sobel) segmentada: %f \n",
            calcular_mse( prom_seg_sob, pare_seg_sob ) );
    printf( "MSE promedio con distinta (con sobel) segmentada: %f \n", 
            calcular_mse( prom_seg_sob, dist_seg_sob ) );
    printf( "### diferencia:  %f \n", 
            calcular_mse( prom_seg_sob, pare_seg_sob )-
            calcular_mse( prom_seg_sob, dist_seg_sob ) );

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

    //====================== laplaciano_g =======================
    CImgList<double> 
        lista6 ( promediada_laplaciano_g.get_resize(300,300), 
                 parecida_laplaciano_g.get_resize(300,300), 
                 distinta_laplaciano_g.get_resize(300,300) );
    if (ver) lista6.display(disp6);
    disp6.set_title("promediada - parecida - distinta");

    printf( "MSE promedio con parecida (con laplaciano_g): %f \n", 
            promediada_laplaciano_g.MSE( parecida_laplaciano_g ) );
    printf( "MSE promedio con distinta (con laplaciano_g): %f \n", 
            promediada_laplaciano_g.MSE( distinta_laplaciano_g ) );
    printf( "### diferencia:  %f \n", 
            promediada_laplaciano_g.MSE( parecida_laplaciano_g ) - 
            promediada_laplaciano_g.MSE( distinta_laplaciano_g ) );

    promediada_laplaciano_g.resize(size,size);
    CImgList<double> prom_seg_lap = segmentar(promediada_laplaciano_g , seg, seg);
    parecida_laplaciano_g.resize(size,size);
    CImgList<double> pare_seg_lap = segmentar(parecida_laplaciano_g , seg, seg);
    distinta_laplaciano_g.resize(size,size);
    CImgList<double> dist_seg_lap = segmentar(distinta_laplaciano_g , seg, seg);

    printf( "MSE promedio con parecida (con laplaciano_g) segmentada: %f \n",
            calcular_mse( prom_seg_lap, pare_seg_lap ) );
    printf( "MSE promedio con distinta (con laplaciano_g) segmentada: %f \n", 
            calcular_mse( prom_seg_lap, dist_seg_lap ) );
    printf( "### diferencia:  %f \n", 
            calcular_mse( prom_seg_lap, pare_seg_lap )-
            calcular_mse( prom_seg_lap, dist_seg_lap ) );

    for (unsigned i = 0; i < prom_seg_lap.size(); i++) {
        prom_seg_lap(i).resize(300,300);
        pare_seg_lap(i).resize(300,300);
        dist_seg_lap(i).resize(300,300);
    }
    if (ver){
        prom_seg_lap.display(disp16); disp16.set_title("prom_seg_lap");
        pare_seg_lap.display(disp17); disp17.set_title("pare_seg_lap");
        dist_seg_lap.display(disp18); disp18.set_title("dist_seg_lap");
    }


    while ((!disp1.is_closed() && !disp1.is_keyQ())) {
        disp1.wait_all();
    }

}
