/**
 * objetivo:
 * armar un perfil con la cantidad de pixels blancos que
 *  tiene cada cuadrito
 * luego aplicar alguna medida estadistica a los mismos
 *
 * pruebas:
 *../contar_cuadros_blancos -size 50 -seg 10 -verfil 1 -umbral 5
 *
 * conclusines
 * con el MSE los resultados varian segun la imagen, no funciona 
 * bien 
 * 
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

    int umbral = cimg_option("-umbral" , 50, "umbral threshold");
    int size   = cimg_option("-size" , 100, "tamaño imagen a segmentar");
    int seg    = cimg_option("-seg" , 10, "tamaño cuadritos segmentacion");
    int verfil     = cimg_option("-verfil" , 0, "ver imagenes filtradas");
    int vercuadros = cimg_option("-vercuadros" , 0, "ver imagenes cuadritos");
    int umbralar   = cimg_option("-umbralar" , 1, "aplicar umbral a las imagenes");
    int normalizar = cimg_option("-normalizar" , 1, "aplicar normalize a las imagenes");

    CImgDisplay disp1, disp2, disp3, disp4, 
        disp5, disp6, disp7,
        disp10, disp11, disp12,
        disp13, disp14, disp15,
        disp16, disp17, disp18;
    CImg<double> img1 ( filename1 ), img2( filename2 );

    //CImgList<double> lista = lista_con_img_chaco_y_un_canal();
    CImgList<double> lista = lista_con_img_tren_frente_y_un_canal();
    //CImgList<double> lista = lista_con_img_tren_perspectiva_y_un_canal();

    img1.channel(0); // ver aplicar en todos los canales !
    img2.channel(0); // ver aplicar en todos los canales !
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

    // normalizado y threshold, el abs ya esta en el filtrado

    if (normalizar) {
        promediada_roberts.normalize(0,255);
        parecida_roberts.normalize(0,255);
        distinta_roberts.normalize(0,255);

        promediada_sobel.normalize(0,255);
        parecida_sobel.normalize(0,255);
        distinta_sobel.normalize(0,255);

        promediada_laplaciano_g.normalize(0,255);
        parecida_laplaciano_g.normalize(0,255);
        distinta_laplaciano_g.normalize(0,255);
    }
    if (umbralar) {
        promediada_roberts.threshold(umbral);
        parecida_roberts.threshold(umbral);
        distinta_roberts.threshold(umbral);

        promediada_sobel.threshold(umbral);
        parecida_sobel.threshold(umbral);
        distinta_sobel.threshold(umbral);

        promediada_laplaciano_g.threshold(umbral);
        parecida_laplaciano_g.threshold(umbral);
        distinta_laplaciano_g.threshold(umbral);
    }

    //====================== roberts =======================
    CImgList<double> 
        lista1 ( promediada_roberts.get_resize(300,300), 
                 parecida_roberts.get_resize(300,300), 
                 distinta_roberts.get_resize(300,300) );
    if (verfil) lista1.display(disp1);
    disp1.set_title("roberts: promediada - parecida - distinta");

    promediada_roberts.resize(size,size);
    CImgList<double> prom_seg_rob = segmentar(promediada_roberts , seg, seg);
    parecida_roberts.resize(size,size);
    CImgList<double> pare_seg_rob = segmentar(parecida_roberts , seg, seg);
    distinta_roberts.resize(size,size);
    CImgList<double> dist_seg_rob = segmentar(distinta_roberts , seg, seg);

    CImg<unsigned int> perfil_prom_seg_rob = perfil_blancos( prom_seg_rob );
    CImg<unsigned int> perfil_pare_seg_rob = perfil_blancos( pare_seg_rob );
    CImg<unsigned int> perfil_dist_seg_rob = perfil_blancos( dist_seg_rob );

    printf( "MSE perfil promedio/parecida (con roberts): %f \n",
            perfil_prom_seg_rob.MSE(perfil_pare_seg_rob));
    printf( "MSE perfil promedio/distinta (con roberts): %f \n\n", 
            perfil_prom_seg_rob.MSE(perfil_dist_seg_rob));

    for (unsigned i = 0; i < prom_seg_rob.size(); i++) {
        prom_seg_rob(i).resize(300,300);
        pare_seg_rob(i).resize(300,300);
        dist_seg_rob(i).resize(300,300);
    }
    if (vercuadros) {
        prom_seg_rob.display(disp10); disp10.set_title("prom_seg_rob");
        pare_seg_rob.display(disp11); disp11.set_title("pare_seg_rob");
        dist_seg_rob.display(disp12); disp12.set_title("dist_seg_rob");
    }

    //====================== sobel =======================
    CImgList<double> 
        lista5 ( promediada_sobel.get_resize(300,300), 
                 parecida_sobel.get_resize(300,300), 
                 distinta_sobel.get_resize(300,300) );
    if (verfil) lista5.display(disp5);
    disp5.set_title("sobel: promediada - parecida - distinta");

    promediada_sobel.resize(size,size);
    CImgList<double> prom_seg_sob = segmentar(promediada_sobel , seg, seg);
    parecida_sobel.resize(size,size);
    CImgList<double> pare_seg_sob = segmentar(parecida_sobel , seg, seg);
    distinta_sobel.resize(size,size);
    CImgList<double> dist_seg_sob = segmentar(distinta_sobel , seg, seg);

    CImg<unsigned int> perfil_prom_seg_sob = perfil_blancos( prom_seg_sob );
    CImg<unsigned int> perfil_pare_seg_sob = perfil_blancos( pare_seg_sob );
    CImg<unsigned int> perfil_dist_seg_sob = perfil_blancos( dist_seg_sob );

    printf( "MSE perfil promedio/parecida (con sobel): %f \n",
            perfil_prom_seg_sob.MSE(perfil_pare_seg_sob));
    printf( "MSE perfil promedio/distinta (con sobel): %f \n\n", 
            perfil_prom_seg_sob.MSE(perfil_dist_seg_sob));

    for (unsigned i = 0; i < prom_seg_sob.size(); i++) {
        prom_seg_sob(i).resize(300,300);
        pare_seg_sob(i).resize(300,300);
        dist_seg_sob(i).resize(300,300);
    }
    if(vercuadros){
        prom_seg_sob.display(disp13); disp13.set_title("prom_seg_sob");
        pare_seg_sob.display(disp14); disp14.set_title("pare_seg_sob");
        dist_seg_sob.display(disp15); disp15.set_title("dist_seg_sob");
    }

    //====================== laplaciano_g =======================
    CImgList<double> 
        lista6 ( promediada_laplaciano_g.get_resize(300,300), 
                 parecida_laplaciano_g.get_resize(300,300), 
                 distinta_laplaciano_g.get_resize(300,300) );
    if (verfil) lista6.display(disp6);
    disp6.set_title("LoG: promediada - parecida - distinta");

    promediada_laplaciano_g.resize(size,size);
    CImgList<double> prom_seg_lap = segmentar(promediada_laplaciano_g , seg, seg);
    parecida_laplaciano_g.resize(size,size);
    CImgList<double> pare_seg_lap = segmentar(parecida_laplaciano_g , seg, seg);
    distinta_laplaciano_g.resize(size,size);
    CImgList<double> dist_seg_lap = segmentar(distinta_laplaciano_g , seg, seg);

    CImg<unsigned int> perfil_prom_seg_lap = perfil_blancos( prom_seg_lap );
    CImg<unsigned int> perfil_pare_seg_lap = perfil_blancos( pare_seg_lap );
    CImg<unsigned int> perfil_dist_seg_lap = perfil_blancos( dist_seg_lap );

    printf( "MSE perfil promedio/parecida (con LoG): %f \n",
            perfil_prom_seg_lap.MSE(perfil_pare_seg_lap));
    printf( "MSE perfil promedio/distinta (con LoG): %f \n\n", 
            perfil_prom_seg_lap.MSE(perfil_dist_seg_lap));

    for (unsigned i = 0; i < prom_seg_lap.size(); i++) {
        prom_seg_lap(i).resize(300,300);
        pare_seg_lap(i).resize(300,300);
        dist_seg_lap(i).resize(300,300);
    }
    if (vercuadros){
        prom_seg_lap.display(disp16); disp16.set_title("prom_seg_lap");
        pare_seg_lap.display(disp17); disp17.set_title("pare_seg_lap");
        dist_seg_lap.display(disp18); disp18.set_title("dist_seg_lap");
    }


    while ((!disp1.is_closed() && !disp1.is_keyQ())) {
        disp1.wait_all();
    }

}
