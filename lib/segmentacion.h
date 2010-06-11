#include <CImg.h>
#include <vector>
#include <list>
#include <iostream>
#include "CPDSI_segmentacion.h"

using namespace std;
using namespace cimg_library;

template<class T>
CImg<T> segmentar(CImg<T> imagen, CImg<T> mascara_x, CImg<T> mascara_y,
    float umbral = 40.0, bool binaria = true) {
  /* LA IMAGEN DEVUELTA ES NORMALIZADA ENTRE 0 Y 255
   * fucnion generica que segmenta una imagen con las mascaras pasadas por parametro
   * por defecto binaria=true indica que devuelve una imagen binaria como resultado
   * @umbral= valor de umbral para binarizado para valores menosres asigna 0
   * */
  CImg<T> img_filx = imagen.get_convolve(mascara_x).get_abs();
  CImg<T> img_fily = imagen.get_convolve(mascara_y).get_abs();
  // armamos el grandiente

  CImg<T> suma(img_filx.width(), img_filx.height(), 1, 1, 0);

  cimg_forXY(imagen,x,y)
	{
	  suma(x, y) = img_filx(x, y) + img_fily(x, y);
	}
  suma.normalize(0, 255); //normalizo porque los valores estan por encima de 255

  if (binaria) {
	cimg_forXY(imagen, x, y)
	  {
		(suma(x, y) < umbral) ? suma(x, y) = 0 : suma(x, y) = 255;
	  }
  }
  return suma;
}

//sobel:
template<class T>
CImg<T> get_sobel_x() {
  //genera una mascara de sobel en la direccion x
  CImg<T> hx(3, 3, 1, 1, 0);
  hx(0, 0) = -1.0;
  hx(0, 1) = -2.0;
  hx(0, 2) = -1.0;

  hx(1, 0) = 0.0;
  hx(1, 1) = 0.0;
  hx(1, 2) = 0.0;

  hx(2, 0) = 1.0;
  hx(2, 1) = 2.0;
  hx(2, 2) = 1.0;
  return hx;
}

template<class T>
CImg<T> get_sobel_y() {
  //genera una mascara de sobel en la direccion y
  CImg<T> hy(3, 3, 1, 1, 0);
  hy(0, 0) = -1.0;
  hy(0, 1) = 0.0;
  hy(0, 2) = 1.0;

  hy(1, 0) = -2.0;
  hy(1, 1) = 0.0;
  hy(1, 2) = 2.0;

  hy(2, 0) = -1.0;
  hy(2, 1) = 0.0;
  hy(2, 2) = 1.0;
  return hy;
}

template<class T>
CImg<T> aplicar_sobel(CImg<T> imagen, float umbral = 40.0, bool binaria = true) {
  /*aplica una mascara de sobel a la imagen y devuelve el resultado
   * */
  return segmentar(imagen, get_sobel_x<T> (), get_sobel_y<T> (), umbral,
	  binaria);
}

template<class T>
vector<T> get_pos_max(CImg<T> imagen) {
  /* Retorna en un vector la posicion del maximo de una imagen;
   * */
  T posx = -1, posy = -1;
  T valor = -9999;
  cimg_forXY(imagen, x, y)
	{
	  if (imagen(x, y) > valor) {
		posx = x;
		posy = y;
		valor = imagen(x, y);
	  }
	}
  vector<T> max;
  max.push_back(posx);
  max.push_back(posy);
  return max;
}

template<class T>
vector<T> get_pos_max_acumulador(CImg<T> imagen) {
  /* Retorna en un vector la posicion del maximo de una imagen;
   * */
  T posx = -1, posy = -1;
  T valor = -9999;
  cimg_forXY(imagen, x, y)
	{
	  if (imagen(x, y) > valor) {
		posx = x;
		posy = y;
		valor = imagen(x, y);
	  }
	}
  vector<T> max;
  max.push_back(posx);
  max.push_back(posy);
  return max;
}

/**
 * Normaliza la imagen dividiendo cada valor por el max en absoluto
 * Obs: El absoluto deberia estar contemplado en 
 * el calculo de MSE, no aca, por eso es get_abs()
 */
template<class T>
void normalizar(CImg<T> &imagen) {
  T maximo = imagen.get_abs().max();

  cimg_forXY( imagen,x,y )
	{
	  if (maximo > 0.0000001) { //fixme: si es 0 que hacemos?
		imagen(x, y) /= (double) maximo;
	  }
	}
}

template<class T>
CImg<T> obtener_maximos_acumuladores(CImg<T> imagen, int cantidad = 50,
    int direccion = -99, int tolerancia = 0) {
  vector<T> maximo_actual;
  CImg<T> acum(cantidad * 2, 1, 1, 1, 0);
  CImg<T> acum_rho(cantidad, 1, 1, 1, 0);
  CImg<T> acum_theta(cantidad, 1, 1, 1, 0);
  int x_con_tol_izq = 0;
  int x_con_tol_der = 0;

  int M = imagen.width(), N = imagen.height();

  double max_rho = sqrt(float(pow(N, 2) + pow(M, 2))), step_rho = 2. * max_rho
	  / (N - 1), // paso en eje rho (rho=[-max_rho , max_rho])
	  step_theta = M_PI / (M - 1), // paso en eje theta (M_PI=pi) (theta=[-90,90])
	  rho, theta;

  if (direccion != -99) { //busca en direccion especifica
	int ancho = imagen.width() - 1;
	int alto = imagen.height() - 1;
	int medio = ancho / 2.0; // este va a ser el 0 grados

	//posicion real del plano rho theta donde se quiere
	x_con_tol_izq = medio + ((direccion - tolerancia)
	    * ((ancho - medio) / 90.0));

	//posicion real del plano rho theta donde se quiere
	x_con_tol_der = medio + ((direccion + tolerancia)
	    * ((ancho - medio) / 90.0));

	if (x_con_tol_izq < 0 || x_con_tol_izq > ancho)
	  x_con_tol_izq = 0; //para que no explote porque para -90 tira -1

	else if (x_con_tol_der < 0 || x_con_tol_der > ancho)
	  x_con_tol_der = 0; //para que no explote porque para -90 tira -1

	// busar los maximos
	imagen.crop(x_con_tol_izq, 0, x_con_tol_der, alto); //ojo estoy  modificando la imagen!

  }

  for (int i = 0; i < cantidad; i++) { //hallo la posicion d elos maximos
	maximo_actual.clear();
	maximo_actual = get_pos_max_acumulador(imagen);

	// lo pongo negro en el cacho de imagen para que detecte el proximo maximo
	imagen(maximo_actual[0], maximo_actual[1]) = 0;
	theta = maximo_actual[1] * step_theta - M_PI / 2;
	rho = maximo_actual[0] * step_rho - max_rho; // mapea [0,N] en [-max_rho,max_rho]
	// FIXME deben estar rho y theta normalizados a una misma escala
	acum_rho[i] = rho / max_rho;
	acum_theta[i] = theta / (M_PI / 2);
  }
  //normalizar(acum_rho);
  //normalizar(acum_theta);
  int contador = 0;
  for (int x = 0; x < acum.width() - 1; x++) {
	acum[x] = acum_rho[contador]; // guardo valor de rho y theta
	acum[x + 1] = acum_theta[contador]; // guardo valor de rho y theta
	contador++;
  }
  return acum;
}

/* Devuelve una imagen de cant_maximos x 1 con los valores caracteristicos
 * de la imagen pasada como parametro para ser almacenados para
 * futuras comparaciones
 * Esto iria a una BD
 * @param: imagen es la imagen a la cual se le extraen las caracteristicas
 * @param: cant_maximos es la cantidad de maximos a extraer por defecto 50 maximos
 * @param: umbral es el valor de umbral para la extraccion de bordes con sobel 
 * - por defecto en 20.0
 * @param: direccion =-99 implica extraccion de los maximos en cualquier direccion.
 * @param: tol_grados es la tolerancia en grados resepecto a direccion. 
 * solo se aplica cuando direccion!-=-99
 * */
template<class T>
CImg<T> extraer_valores_caracteristicos(CImg<T> imagen, int cant_maximos = 50,
    float umbral = 20.0, int direccion = -99, int tol_grados = 0,
    bool channel0 = true, int q_levels = 16, int r_size = 200) {

  cimg_forXY(imagen,x,y)
	{
	  imagen(x, y, 0, 0) += imagen(x, y, 0, 1) + imagen(x, y, 0, 2);
	  imagen(x, y, 0, 0) /= 3.0;
	}

  if (channel0) {
	imagen.channel(0);
  }

  imagen.resize(r_size, r_size);
  //img_bordes es binaria y tiene valores entre 0 y 255...
  //imagen.display();
  CImg<T> img_bordes = aplicar_sobel<T> (imagen, umbral, true);
  CImg<T> HOUGH_IMG_BORDES = hough_directa(img_bordes); // aplico la transformada

  //HOUGH_IMG_BORDES.display();
  // deve devolver cant_maximos de rho y tita
  HOUGH_IMG_BORDES.resize(40, 40);

  CImg<T> acums = obtener_maximos_acumuladores(HOUGH_IMG_BORDES, cant_maximos,
	  direccion, tol_grados);

  //acums.display();
  return acums;
}

template<class T>
list<T> comparar_imagenes(CImg<T> imagen, CImgList<T> lista_imagenes,
    int cant_maximos = 50, float umbral = 20.0, int direccion = -99,
    int tol_grados = 0, bool channel0 = true, int q_levels = 16, int r_size =
        100) {
  /* Devuelve en una lista con el MSE entre una imagen y un conjunto de imagenes
   * aplicacndo el metodo de extraer caracteristicas mediante la obtencion de maximos
   * de la transformada de Hough
   *
   * El vector devuelto en el indice 0 contiene el MSE entre la iamgen y la primera imagen de la lista
   * El vector devuelto en el indice 1 contiene el MSE entre la iamgen y la segunda imagen de la lista
   * etc.
   *
   * @param: imagen es la imagen a comparar
   * @param: lista_imagenes es una lista con las imagenes que se quiere comparar
   * @param: cant_maximos es la cantidad de maximos a extraer por defecto 50 maximos
   * @param: umbral es el valor de umbral para la extraccion de bordes con sobel - por defecto en 20.0
   * @param: direccion =-99 implica extraccion de los maximos en cualquier direccion.
   * @param: tol_grados es la tolerancia en grados resepecto a direccion. solo se aplica cuando direccion!-=-99
   * */
  list<T> errores;
  cimglist_for(lista_imagenes, l) {
	errores.push_back(extraer_valores_caracteristicos(imagen, cant_maximos,
	    umbral, direccion, tol_grados, channel0, q_levels, r_size).MSE(
	    extraer_valores_caracteristicos(lista_imagenes[l], cant_maximos,
	        umbral, direccion, tol_grados, channel0, q_levels, r_size)));
  }
  return errores;
}

template<class T>
void imprimir_lista(list<T> lista) {
  //imprime la lista en consola...
  int pos = 0;
  cout << endl << "******************************************************"
	  << endl;
  while (!lista.empty()) {
	T value = lista.front();
	cout << "lista[" << pos++ << "]: " << (T) value << endl;
	lista.pop_front();
  }
  cout << endl << "******************************************************"
	  << endl;

}
