/*
 * Pasos:
 * Por cada imagen se hace:
 *  0- Obtengo canal 0, quantizo en 16 niveles y hago un resize
 * 	1- Deteccion de bordes con Gradiente (mascaras de Sobel)
 *  2- Binarizacion de la imagen con los bordes detectados
 *  3- Aplicacion de la transf de Hough y obtencion de los N maximos 
 *     valores de los acumuladores
 *     (sin limitar tita y rho). N es un parametro a seleccionar - 
 *     cantmaximos en el codigo
 *     (La idea es que en la base de datos solo se guarden estos 
 *     valores en vez de toda la imagen.
 *
 *  4- Aplicar el mismo proceso (1 a 3) a la imagen a comparar y 
 *     comparar con el vector de imagenes base
 *  5- El de menor MSE en 4 es el mas parecido y por tanto la imagen que "Machea"
 *
 * TODO: probar con diferetnes tolearncias de umbral y de cantidad de maximos
 *       Arreglar las funciones de obtener_maximos y demas que podrian ser optimizadas.
 *
 * CONCLUSIONES:
 * todo: habria que correrlo para diferentes cantidades de maximos y hacer
 * una grafica cant. maximos - MSE tanto con una imagen parecida y con una imagen
 * nada que ver para ver hasta donde tiene sentido seguir aumentandolo..
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
	const char *filename =
			cimg_option("-f1", "./imagenes/trenfrente/tren_frente04.jpg",
					"ruta archivo imagen de la base de datos a comparar");
	const char *filename1 =
			cimg_option("-f2", "./imagenes/trenfrente/tren_frente03.jpg",
					"ruta archivo imagen");
	const char *filenamedif =
			cimg_option("-f3", "./imagenes/mpg20/obj7__3.jpg",
					"ruta archivo imagen");

	const float umbral = atof(cimg_option("-umbral", "20.0", "umbral"));
	const int
			direccion =
					atoi(
							cimg_option ("-direccionborde", "-99", "direccion borde a buscar - -99 implica todas las direcciones"));
	const int
			cant_maximos =
					atoi(
							cimg_option("-cantmaximos", "50", "cantidad de maximos a detectar"));
	int tol_grados = atoi(
			cimg_option ("-tolgrados", "0", "tolerancia en grados"));

	//imagen original
	CImg<double> img(filename); //una realizacion de img
	CImg<double> img1(filename1); //otra realizacion de img
	CImg<double> imgd(filenamedif); //esto nada que ver...


	CImgList<double> lista(img, img1, imgd);
	imprimir_lista<double>(comparar_imagenes<double>(img, lista));

	//img:
	CImg<double> acums = extraer_valores_caracteristicos(img, cant_maximos,
			umbral, direccion, tol_grados);
	//img1:
	CImg<double> acums1 = extraer_valores_caracteristicos(img1, cant_maximos,
			umbral, direccion, tol_grados);
	//imgd:
	CImg<double> acumsd = extraer_valores_caracteristicos(imgd, cant_maximos,
			umbral, direccion, tol_grados);

	//Muestro errores en consola:
	cout << "***************************************************************"
			<< endl;
	cout << "error f1 y f1 (iguales - trivial para verificacion): "
			<< acums.MSE(acums) << endl;
	cout << "error f1 y f2 (parecidas): " << acums.MSE(acums1) << endl;
	cout << "error f1 y f3 (diferentes): " << acums.MSE(acumsd) << endl;
	cout << "error f2 y f3 (diferentes): " << acums1.MSE(acumsd) << endl;
	cout << "***************************************************************"
			<< endl;
	return 0;
}
