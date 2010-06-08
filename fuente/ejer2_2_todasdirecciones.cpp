/*
 * APLICACION DE LA TRANSF. DE HOUGH. SE GUARDAN UNA CIERTA CANTIDAD DE ACUMULADORES
 * DE LA MATRIZ RHO TITA DE HOUGH (LOS VALORES REALES Y SON DEVUELTOS EN UN VECTOR)
 * PARA LAS IMAGENES A COMPARAR. AQUELLAS QUE PRESENTEN UN MENOR MSE DEBERIA SER LA MAS PARECIDA....
 * ES LO QUE SUPONGO...
 * */
#define cimg_use_fftw3 1

#ifdef cimg_use_fftw3
extern "C" {
#include "fftw3.h"
}
#endif

#include <iostream>
#include <CImg.h>
#include "../lib/CPDSI_segmentacion.h"
#include "../lib/segmentacion.h"

using namespace std;
using namespace cimg_library;

int main(int argc, char **argv) {
	const char
			*filename =
					cimg_option("-f", "/home/christian/Documentos/universidad/2010/captura/tp-aplicacion/imagenes/trenfrente/tren_frente04.jpg",
							"ruta archivo imagen");
	const char
			*filename1 =
					cimg_option("-f", "/home/christian/Documentos/universidad/2010/captura/tp-aplicacion/imagenes/trenfrente/tren_frente03.jpg",
							"ruta archivo imagen");
	const char
			*filenamedif =
					cimg_option("-f", "/home/christian/Documentos/universidad/2010/captura/tp-aplicacion/imagenes/mpg20/obj7__1.jpg",
							"ruta archivo imagen");
	const float umbral = atof(cimg_option("-umbral", "20.0", "umbral"));
	const int
			direccion =
					atoi(
							cimg_option ("-direccionborde", "-99", "direccion borde a buscar - -99 implica todas las direcciones"));
	const int
			cant_maximos =
					atoi(
							cimg_option("-cantmaximos", "10", "cantidad de maximos a detectar"));
	int tol_grados = atoi(
			cimg_option ("-tolgrados", "0", "tolerancia en grados"));

	//imagen original
	CImg<double> img(filename); //una realizacion de img
	CImg<double> img1(filename1); //otra realizacion de img
	CImg<double> imgd(filenamedif); //esto nada que ver...
	//img:
	CImg<double> img_bordes = aplicar_sobel<double> (img, umbral, true); //img_bordes es binaria y tiene valores entre 0 y 255...
	CImg<double> HOUGH_IMG_BORDES = hough_directa(img_bordes); // aplico la transformada
	vector<double> posiciones_maximos = obtener_maximos(HOUGH_IMG_BORDES,
			cant_maximos, direccion, tol_grados);
	CImg<double> acums = obtener_maximos_acumuladores(HOUGH_IMG_BORDES,
			cant_maximos, direccion, tol_grados);
	//cout<<endl<<endl<<"acums: "<<acums[0]<<"     "<<acums[1]<<"       "<<acums[2]<<"       "<<endl<<endl;

	//img1:
	CImg<double> img_bordes1 = aplicar_sobel<double> (img1, umbral, true); //img_bordes es binaria y tiene valores entre 0 y 255...
	CImg<double> HOUGH_IMG_BORDES1 = hough_directa(img_bordes1); // aplico la transformada
	vector<double> posiciones_maximos1 = obtener_maximos(HOUGH_IMG_BORDES1,
			cant_maximos, direccion, tol_grados);
	CImg<double> acums1 = obtener_maximos_acumuladores(HOUGH_IMG_BORDES1,
			cant_maximos, direccion, tol_grados);
	//imgd:
	CImg<double> img_bordesd = aplicar_sobel<double> (imgd, umbral, true); //img_bordes es binaria y tiene valores entre 0 y 255...
	CImg<double> HOUGH_IMG_BORDESd = hough_directa(img_bordesd); // aplico la transformada
	vector<double> posiciones_maximosd = obtener_maximos(HOUGH_IMG_BORDESd,
			cant_maximos, direccion, tol_grados);
	CImg<double> acumsd = obtener_maximos_acumuladores(HOUGH_IMG_BORDESd,
			cant_maximos, direccion, tol_grados);
	cout << "***************************************************************"
			<< endl;
	cout << "error img 1 e img2 (parecidas): " << acums.MSE(acums1) << endl;
	cout << "error img 1 e imgd (diferentes): " << acums.MSE(acumsd) << endl;
	cout << "error img 2 e imgd (diferentes): " << acums1.MSE(acumsd) << endl;
	cout << "error img 1 e img1 (iguales): " << acums.MSE(acums1) << endl;
	cout << "***************************************************************"
			<< endl;
	/*	CImg<double> maxs(img); //imagen que voy a usar para dibujar maximos
	 //luego se lehace la inversa de hough a maxs para ver las lineas
	 maxs.normalize(0, 255);
	 maxs.fill(0.0);

	 for (unsigned int i = 0; i < (posiciones_maximos.size() - 1); i += 2) {
	 cout << "i: " << i << "   i+1: " << i + 1 << endl;
	 maxs(posiciones_maximos[i], posiciones_maximos[i + 1]) = 255.0;
	 cout << "maximos (x,y)= (" << posiciones_maximos[i] << ", "
	 << posiciones_maximos[i + 1] << ")" << endl;
	 }*/
	//CImg<double> deteccion = hough_inversa(maxs);

	/*	//muestro
	 CImgDisplay d1(img, "imagen original");
	 CImgDisplay d2(img_bordes, "deteccion bordes");
	 CImgDisplay d3(HOUGH_IMG_BORDES, "Transf. Hough");
	 CImgDisplay d4(maxs, "maximos detectados");
	 CImgDisplay disp(deteccion, "inversaHough(maximos)");
	 //CImgDisplay d5(deteccion_final, "and entre imagen y bordes");

	 CImgDisplay disppp(img);*/

	/*while (!disp.is_closed()) {
	 disp.wait();
	 }*/
	return 0;
}
