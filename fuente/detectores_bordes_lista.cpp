/**
 * objetivo:
 * ver que da el mse con la lista segmentada luego de detectar bordes
 * y modificar el tamaño.  con un detector de bordes cualquiera: roberts, p.e.
 *
 * viendo las listas de las tres, vemos que la distribucion de la promediada
 * con la mas parecida, se asemejan. 
 * la promediada con la distinta tiene una distibucion diferente
 * por esto es de esperar que MSE de menor para el primer caso
 * pero a pesar de que las distribuciones se asemejan mas,
 * el MSE da mas alto con la mas parecida!?
 * a que se debe esto?
 * deberiamos tener en cuenta una variable que mida la dispersión
 * mas que el MSE?
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

    CImgDisplay disp1, disp2, disp3, disp4;
    CImg<double> img1 ( filename1 ), img2( filename2 );
    CImgList<double> lista = lista_con_img_chaco_y_un_canal();
    img1.channel(0);
    img2.channel(0);
    CImg<double> promediada = promedio(lista);

    CImg<double> promediada_roberts = filtrado_roberts(promediada);
    CImg<double> parecida_roberts   = filtrado_roberts(img1);
    CImg<double> distinta_roberts   = filtrado_roberts(img2);

    // solo tomamos la parte positiva ( ~= threshold)
    cimg_forXY(promediada_roberts,x,y){
        if ( promediada_roberts(x,y) < 0 ) promediada_roberts(x,y)=0;
        if ( parecida_roberts(x,y) < 0 ) parecida_roberts(x,y)=0;
        if ( distinta_roberts(x,y) < 0 ) distinta_roberts(x,y)=0;
    }
    promediada_roberts.normalize(0,255);
    parecida_roberts.normalize(0,255); 
    distinta_roberts.normalize(0,255);

    // mostramos como queda cada una
    CImgList<double> 
        lista1 ( promediada_roberts.get_resize(300,300), 
                 parecida_roberts.get_resize(300,300), 
                 distinta_roberts.get_resize(300,300) );
    lista1.display(disp1);
    disp1.set_title("promediada - parecida - distinta");

    // compara promediada contra la parecida y la distinta
    printf( "MSE promedio con parecida (con roberts): %f \n", 
            promediada_roberts.MSE( parecida_roberts ) );
    printf( "MSE promedio con distinta (con roberts): %f \n", 
            promediada_roberts.MSE( distinta_roberts ) );

    //ahora probamos reduciendo el tamaño y  segmentando
    promediada_roberts.resize(100,100);
    CImgList<double> prom_seg_rob = segmentar(promediada_roberts , 20, 20);
    parecida_roberts.resize(100,100);
    CImgList<double> pare_seg_rob = segmentar(parecida_roberts , 20, 20);
    distinta_roberts.resize(100,100);
    CImgList<double> dist_seg_rob = segmentar(distinta_roberts , 20, 20);

    printf( "MSE promedio con parecida (con roberts) segmentada: %f \n",
            calcular_mse( prom_seg_rob, pare_seg_rob ) );
    printf( "MSE promedio con distinta (con roberts) segmentada: %f \n", 
            calcular_mse( prom_seg_rob, dist_seg_rob ) );

    //simplemente para mostrar como queda la lista y la distribucion:
    for (unsigned i = 0; i < prom_seg_rob.size(); i++) {
        prom_seg_rob(i).resize(300,300);
        pare_seg_rob(i).resize(300,300);
        dist_seg_rob(i).resize(300,300);
    }
    prom_seg_rob.display(disp4);
    pare_seg_rob.display(disp2);
    dist_seg_rob.display(disp3);




    while ((!disp1.is_closed() && !disp1.is_keyQ())) {
        disp1.wait_all();
    }

}
