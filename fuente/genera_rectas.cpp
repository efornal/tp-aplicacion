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


// CImg<double> acura( CImg<double> img, int size=10 ) {

//   // promediado canales RGB
//   cimg_forXY(img,x,y) {
//     img(x, y, 0, 0) += img(x, y, 0, 1) + img(x, y, 0, 2);
//     img(x, y, 0, 0) /= 3.0;
//   }
//   img.channel(0);
  
//   filtrado_sobel( img );

//   img.resize( size, size );
// }

int main(int argc, char **argv) {
    const char *filename1 = cimg_option( "-f1",
        "./imagenes/base/iglesiasanfrancisco05.jpg",
        "ruta archivo imagen" );
    const char *filename2 = cimg_option( "-f2",
        "./imagenes/base/catedral04.jpg",
        "ruta archivo imagen" );
    const char *filename3 = cimg_option( "-base",
        "./imagenes/base/iglesiasanfrancisco01.jpg",
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
    CImg<double> img1 ( filename1 ), img2( filename2 ), img3( filename3 );

  cimg_forXY(img1,x,y) {
    img1(x, y, 0, 0) += img1(x, y, 0, 1) + img1(x, y, 0, 2);
    img1(x, y, 0, 0) /= 3.0;
    img2(x, y, 0, 0) += img2(x, y, 0, 1) + img2(x, y, 0, 2);
    img2(x, y, 0, 0) /= 3.0;
    img3(x, y, 0, 0) += img3(x, y, 0, 1) + img3(x, y, 0, 2);
    img3(x, y, 0, 0) /= 3.0;

  }
  img1.channel(0);
  img2.channel(0);
  img3.channel(0);

    CImg<double> promediada = img3;

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
    double var_prom=0, var_pare=0, var_dist=0;
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


    for (unsigned i = 0; i < prom_seg_sob.size(); i++) {
      var_prom += prom_seg_sob(i).variance();
      var_pare += pare_seg_sob(i).variance();
      var_dist += dist_seg_sob(i).variance();
      printf("varianza hist: \t\t prom %f  pare %f  dist %f \n",
             prom_seg_sob(i).get_histogram(256,0,255).variance(),
             pare_seg_sob(i).get_histogram(256,0,255).variance(),
             dist_seg_sob(i).get_histogram(256,0,255).variance());
      printf("varianza: \t\t prom %f  pare %f  dist %f \n",
             prom_seg_sob(i).variance(),
             pare_seg_sob(i).variance(),
             dist_seg_sob(i).variance());

        prom_seg_sob(i) = 
          hough_directa( get_solo_maximos(prom_seg_sob(i), puntos));
        pare_seg_sob(i) = 
          hough_directa( get_solo_maximos(pare_seg_sob(i), puntos));
        dist_seg_sob(i) = 
          hough_directa( get_solo_maximos(dist_seg_sob(i), puntos));

        // prom_seg_sob(i) = 
        //   hough_inversa(get_solo_maximos( hough_directa( get_solo_maximos(prom_seg_sob(i), puntos)), puntos));
        // pare_seg_sob(i) = 
        //   hough_inversa( get_solo_maximos( hough_directa( get_solo_maximos(pare_seg_sob(i), puntos)), puntos));
        // dist_seg_sob(i) = 
        //   hough_inversa( get_solo_maximos( hough_directa( get_solo_maximos(dist_seg_sob(i), puntos)), puntos));

        // prom_seg_sob(i) = 
        //   get_solo_maximos( hough_directa( get_solo_maximos(prom_seg_sob(i), puntos)), puntos);
        // pare_seg_sob(i) = 
        //   get_solo_maximos( hough_directa( get_solo_maximos(pare_seg_sob(i), puntos)), puntos);
        // dist_seg_sob(i) = 
        //   get_solo_maximos( hough_directa( get_solo_maximos(dist_seg_sob(i), puntos)), puntos);

        
    }
    var_prom /= prom_seg_sob.size();
    var_pare /= prom_seg_sob.size();
    var_dist /= prom_seg_sob.size();
    printf("varianza media: prom %f  <-> pare %f (%f)  dist %f (%f) \n",
           var_prom,  var_pare, var_prom-var_pare, var_dist, var_prom-var_dist);

    printf( "MSE promedio con parecida (con sobel) segmentada - hough: %f \n",
            calcular_mse( prom_seg_sob, pare_seg_sob ) );
    printf( "MSE promedio con distinta (con sobel) segmentada - hough: %f \n", 
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
