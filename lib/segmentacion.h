/*
 * filtros.h
 *
 *  Created on: 16/05/2010`
 *      Author: christian
 */

#include <CImg.h>
#include <vector>
#include <list>

using namespace std;
using namespace cimg_library;

//horizontal
template<class T>
CImg<T> get_horizontal() {
	//genera una mascara que detecta bordes horizontalmente
	CImg<T> hx(3, 3, 1, 1, -1);
	hx(1, 0) = 2.0;
	hx(1, 1) = 2.0;
	hx(1, 2) = 2.0;
	return hx;
}

//diagonal 45 grados
template<class T>
CImg<T> get_diag_45() {
	//genera una mascara que detecta bordes a 45 grados
	CImg<T> hx(3, 3, 1, 1, -1);
	hx(0, 2) = 2.0;
	hx(1, 1) = 2.0;
	hx(2, 0) = 2.0;
	return hx;
}

//diagonal -45 grados
template<class T>
CImg<T> get_diag_135() {
	//genera una mascara que detecta bordes a -45=135 grados
	CImg<T> hx(3, 3, 1, 1, -1);
	hx(0, 0) = 2.0;
	hx(1, 1) = 2.0;
	hx(2, 2) = 2.0;
	return hx;
}
//vertical
template<class T>
CImg<T> get_vertical() {
	//genera una mascara que detecta bordes a -45=135 grados
	CImg<T> hx(3, 3, 1, 1, -1);
	hx(0, 1) = 2.0;
	hx(1, 1) = 2.0;
	hx(2, 1) = 2.0;
	return hx;
}

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

//definicion de mascaras:
//roberts:
template<class T>
CImg<T> get_roberts_x() {
	//genera una mascara de roberts en la direccion x
	CImg<T> hx(2, 2, 1, 1, 0);
	hx(0, 0) = -1.0;
	hx(1, 1) = 1.0;
	return hx;
}

template<class T>
CImg<T> get_roberts_y() {
	//genera una mascara de roberts en la direccion y
	CImg<T> hy(2, 2, 1, 1, 0);
	hy(0, 1) = -1.0;
	hy(1, 0) = 1.0;
	return hy;
}

//prewitt:
template<class T>
CImg<T> get_prewitt_x() {
	//genera una mascara de prewitt en la direccion x
	CImg<T> hx(3, 3, 1, 1, 0);
	hx(0, 0) = -1.0;
	hx(0, 1) = -1.0;
	hx(0, 2) = -1.0;

	hx(1, 0) = 0.0;
	hx(1, 1) = 0.0;
	hx(1, 2) = 0.0;

	hx(2, 0) = 1.0;
	hx(2, 1) = 1.0;
	hx(2, 2) = 1.0;
	return hx;
}

template<class T>
CImg<T> get_prewitt_y() {
	//genera una mascara de prewitt en la direccion y
	CImg<T> hy(3, 3, 1, 1, 0);
	hy(0, 0) = -1.0;
	hy(0, 1) = 0.0;
	hy(0, 2) = 1.0;

	hy(1, 0) = -1.0;
	hy(1, 1) = 0.0;
	hy(1, 2) = 1.0;

	hy(2, 0) = -1.0;
	hy(2, 1) = 0.0;
	hy(2, 2) = 1.0;
	return hy;
}

//prewitt diagonal:
template<class T>
CImg<T> get_prewitt_x_diagonal() {
	//genera una mascara de prewitt en la direccion x diagonal 45 grados
	CImg<T> hx(3, 3, 1, 1, 0);
	hx(0, 0) = 0.0;
	hx(0, 1) = 1.0;
	hx(0, 2) = 1.0;

	hx(1, 0) = -1.0;
	hx(1, 1) = 0.0;
	hx(1, 2) = 1.0;

	hx(2, 0) = -1.0;
	hx(2, 1) = -1.0;
	hx(2, 2) = 0.0;
	return hx;
}

template<class T>
CImg<T> get_prewitt_y_diagonal() {
	//genera una mascara de prewitt en la direccion y diagonal -45 grados
	CImg<T> hy(3, 3, 1, 1, 0);
	hy(0, 0) = -1.0;
	hy(0, 1) = -1.0;
	hy(0, 2) = 0.0;

	hy(1, 0) = -1.0;
	hy(1, 1) = 0.0;
	hy(1, 2) = 1.0;

	hy(2, 0) = 0.0;
	hy(2, 1) = 1.0;
	hy(2, 2) = 1.0;
	return hy;
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

//sobel diagonal:
template<class T>
CImg<T> get_sobel_x_diagonal() {
	//genera una mascara de sobel en la direccion x diagonal 45 grados
	CImg<T> hx(3, 3, 1, 1, 0);
	hx(0, 0) = 0.0;
	hx(0, 1) = 1.0;
	hx(0, 2) = 2.0;

	hx(1, 0) = -1.0;
	hx(1, 1) = 0.0;
	hx(1, 2) = 1.0;

	hx(2, 0) = -2.0;
	hx(2, 1) = -1.0;
	hx(2, 2) = 0.0;
	return hx;
}

template<class T>
CImg<T> get_sobel_y_diagonal() {
	//genera una mascara de sobel en la direccion y -45 grados
	CImg<T> hy(3, 3, 1, 1, 0);
	hy(0, 0) = -2.0;
	hy(0, 1) = -1.0;
	hy(0, 2) = 0.0;

	hy(1, 0) = -1.0;
	hy(1, 1) = 0.0;
	hy(1, 2) = 1.0;

	hy(2, 0) = 0.0;
	hy(2, 1) = 1.0;
	hy(2, 2) = 2.0;
	return hy;
}

template<class T>
CImg<T> get_LoG() {
	//genera una mascara de sobel en la direccion y -45 grados
	CImg<T> hy(5, 5, 1, 1, 0);
	hy(0, 0) = 0.0;
	hy(0, 1) = 0.0;
	hy(0, 2) = 1.0;
	hy(0, 3) = 0.0;
	hy(0, 4) = 0.0;

	hy(1, 0) = 0.0;
	hy(1, 1) = -1.0;
	hy(1, 2) = -2.0;
	hy(1, 3) = -1.0;
	hy(1, 4) = 0.0;

	hy(2, 0) = -1.0;
	hy(2, 1) = -2.0;
	hy(2, 2) = 16.0;
	hy(2, 3) = -2.0;
	hy(2, 4) = -1.0;

	hy(3, 0) = 0.0;
	hy(3, 1) = 0.0;
	hy(3, 2) = 1.0;
	hy(3, 3) = 0.0;
	hy(3, 4) = 0.0;

	hy(4, 0) = 0.0;
	hy(4, 1) = -1.0;
	hy(4, 2) = -2.0;
	hy(4, 3) = -1.0;
	hy(4, 4) = 0.0;
	return hy;
}

template<class T>
CImg<T> get_laplaciano1() {
	//genera una mascara de tipo laplaciano (no tien en cuenta las diagonales
	CImg<T> hy(3, 3, 1, 1, 0);
	hy(0, 0) = 0.0;
	hy(0, 1) = -1.0;
	hy(0, 2) = 0.0;

	hy(1, 0) = -1.0;
	hy(1, 1) = 4.0;
	hy(1, 2) = -1.0;

	hy(2, 0) = 0.0;
	hy(2, 1) = -1.0;
	hy(2, 2) = 0.0;
	return hy;
}

template<class T>
CImg<T> get_laplaciano2() {
	//genera una mascara de tipo laplaciano (tiene en cuenta las diagonales
	CImg<T> hy(3, 3, 1, 1, 0);
	hy(0, 0) = -1.0;
	hy(0, 1) = -1.0;
	hy(0, 2) = -1.0;

	hy(1, 0) = -1.0;
	hy(1, 1) = 8.0;
	hy(1, 2) = -1.0;

	hy(2, 0) = -1.0;
	hy(2, 1) = -1.0;
	hy(2, 2) = -1.0;
	return hy;
}

template<class T>
CImg<T> aplicar_roberts(CImg<T> imagen, float umbral = 40.0, bool binaria =
		true) {
	/*aplica una mascara de roberts a la imagen y devuelve el resultado
	 * */
	return segmentar(imagen, get_roberts_x<T> (), get_roberts_y<T> (), umbral,
			binaria);
}
template<class T>
CImg<T> aplicar_prewitt(CImg<T> imagen, float umbral = 40.0, bool binaria =
		true) {
	/*aplica una mascara de prewitt a la imagen y devuelve el resultado
	 * */
	return segmentar(imagen, get_prewitt_x<T> (), get_prewitt_y<T> (), umbral,
			binaria);
}
template<class T>
CImg<T> aplicar_sobel(CImg<T> imagen, float umbral = 40.0, bool binaria = true) {
	/*aplica una mascara de sobel a la imagen y devuelve el resultado
	 * */
	return segmentar(imagen, get_sobel_x<T> (), get_sobel_y<T> (), umbral,
			binaria);
}
template<class T>
CImg<T> aplicar_sobel_diagonal(CImg<T> imagen, float umbral = 40.0,
		bool binaria = true) {
	/*aplica una mascara de sobel para det de diag. a la imagen y devuelve el resultado
	 * */
	return segmentar(imagen, get_sobel_x_diagonal<T> (),
			get_sobel_y_diagonal<T> (), umbral, binaria);
}
template<class T>
CImg<T> aplicar_prewitt_diagonal(CImg<T> imagen, float umbral = 40.0,
		bool binaria = true) {
	/*aplica una mascara de prewitt para det de diag. a la imagen y devuelve el resultado
	 * */
	return segmentar(imagen, get_prewitt_x_diagonal<T> (),
			get_prewitt_y_diagonal<T> (), umbral, binaria);
}
template<class T>
CImg<T> aplicar_laplaciano(CImg<T> imagen, float umbral = 40.0, bool binaria =
		true) {
	/*aplica una mascara de laplaciano(sin tener en cuentas diagonales
	 *  a la imagen y devuelve el resultado
	 * */
	CImg<T> resul = imagen.channel(0).get_convolve(get_laplaciano1<T> ());
	resul.abs();
	if (binaria) {
		cimg_forXY(resul,x,y)
			{
				(resul(x, y) < umbral) ? resul(x, y) = 0 : resul(x, y) = 255;
			}
	}
	return resul;
}

template<class T>
CImg<T> aplicar_laplaciano_condiagonales(CImg<T> imagen, float umbral = 40.0,
		bool binaria = true) {
	/*aplica una mascara de laplaciano (TIENE en cuentas diagonales
	 *  a la imagen y devuelve el resultado
	 * */
	CImg<T> resul = imagen.channel(0).get_convolve(get_laplaciano2<T> ());
	resul.abs();
	if (binaria) {
		cimg_forXY(resul,x,y)
			{
				(resul(x, y) < umbral) ? resul(x, y) = 0 : resul(x, y) = 255;
			}
	}
	return resul;
}

template<class T>
CImg<T> aplicar_LoG(CImg<T> imagen, float umbral = 40.0, bool binaria = true) {
	/*aplica una mascara de laplaciano (TIENE en cuentas diagonales
	 *  a la imagen y devuelve el resultado
	 * */
	CImg<T> resul = imagen.channel(0).get_convolve(get_LoG<T> ());
	resul.abs();
	if (binaria) {
		cimg_forXY(resul,x,y)
			{
				(resul(x, y) > umbral) ? resul(x, y) = 0 : resul(x, y) = 255;
			}
	}
	return resul;
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
	max.push_back(valor);
	return max;
}

template<class T>
CImg<T> obtener_maximos_acumuladores(CImg<T> imagen, int cantidad = 1,
		int direccion = -99, int tolerancia = 0) {
	vector<T> maximo_actual;
	CImg<T> acum(cantidad, 1, 1, 1, 0);
	int x_con_tol_izq = 0;
	int x_con_tol_der = 0;
	if (direccion != -99) { //busca en direccion especifica
		int ancho = imagen.width() - 1;
		int alto = imagen.height() - 1;
		int medio = ancho / 2.0; // este va a ser el 0 grados
		//	cout << "medio" << medio << endl << "         ancho: " << ancho << endl;
		x_con_tol_izq = medio + ((direccion - tolerancia) * ((ancho - medio)
				/ 90.0)); //posicion real del plano rho theta donde se quiere

		x_con_tol_der = medio + ((direccion + tolerancia) * ((ancho - medio)
				/ 90.0)); //posicion real del plano rho theta donde se quiere

		if (x_con_tol_izq < 0 || x_con_tol_izq > ancho)
			x_con_tol_izq = 0; //para que no explote porque para -90 tira -1

		else if (x_con_tol_der < 0 || x_con_tol_der > ancho)
			x_con_tol_der = 0; //para que no explote porque para -90 tira -1

		// busar los maximos
		imagen.crop(x_con_tol_izq, 0, x_con_tol_der, alto); //ojo estoy  modificando la imagen!
		imagen.display();
	}

	for (int i = 0; i < cantidad; i++) { //hallo la posicion d elos maximos
		maximo_actual.clear();
		maximo_actual = get_pos_max_acumulador(imagen);
		imagen(maximo_actual[0], maximo_actual[1]) = 0; // lo pongo negro en el cacho de imagen para que detecte el proximo maximo
		acum[i]=(maximo_actual[2]);
	}
	return acum;
}

/* funciones para transformada de hough*/
template<class T>
vector<T> obtener_maximos(CImg<T> imagen, int cantidad = 1,
		int direccion = -99, int tolerancia = 0) {
	/* funcion que deuvelve en un arrelgo la cantidad de maximos especificados
	 * siendo la pos 0 del arreglo el maximo de la imagen, pos 1 el anterior al maximo, etc.
	 * @param imagen: es la imagen sobre la cual se hallara los maximos (debe ser la imagen del espacio
	 * de hough que se obtuvo a partir de una imagen de bordes... imagen=hough_directa(bordes(imagen_bordes))
	 * @param cantidad: cantidad de maximos que se desean extraer de la imagen (por defecto 1)
	 * @param direccion: obtiene solo los maximos en la direccion especificada por defecto -99 = todas las direcciones.
	 * 					 el valor de direccion debe estar entre -90 y 90.
	 * @param tolerancia: es la tolerancia de la direccion en que busca los maximos.... direcions+-tolerancia
	 * @param acumladores es deuvelto por referencia con los valores de los acumuladores.
	 * */
	vector<T> maximo_actual;
	vector<T> maximos;
	int x_con_tol_izq = 0;
	int x_con_tol_der = 0;
	if (direccion != -99) { //busca en direccion especifica
		int ancho = imagen.width() - 1;
		int alto = imagen.height() - 1;
		int medio = ancho / 2.0; // este va a ser el 0 grados
		cout << "medio" << medio << endl << "         ancho: " << ancho << endl;
		x_con_tol_izq = medio + ((direccion - tolerancia) * ((ancho - medio)
				/ 90.0)); //posicion real del plano rho theta donde se quiere

		x_con_tol_der = medio + ((direccion + tolerancia) * ((ancho - medio)
				/ 90.0)); //posicion real del plano rho theta donde se quiere

		if (x_con_tol_izq < 0 || x_con_tol_izq > ancho)
			x_con_tol_izq = 0; //para que no explote porque para -90 tira -1

		else if (x_con_tol_der < 0 || x_con_tol_der > ancho)
			x_con_tol_der = 0; //para que no explote porque para -90 tira -1

		// busar los maximos
		imagen.crop(x_con_tol_izq, 0, x_con_tol_der, alto); //ojo estoy  modificando la imagen!
		imagen.display();
	}

	for (int i = 0; i < cantidad; i++) { //hallo la posicion d elos maximos
		maximo_actual.clear();
		maximo_actual = get_pos_max(imagen); //tengo la posicion del maximo del pedazo de la imagen
		maximos.push_back(maximo_actual[0] + x_con_tol_izq); // posicion en x maximo actual es del pedacito! ojo!
		//ojo que aca imagen es un cachito de la  imagen solo el pedazo donde hay que encontrar maximos por
		//eso se suma el x_con_tol_izq.

		//cout << "maximo x : " << maximo_actual[0] + x_con_tol_izq << endl;
		maximos.push_back(maximo_actual[1]); //posicion en y del maximo sobre el pedazo de imagen...
		//como va desde 0 no se le suma nada...
		imagen(maximo_actual[0], maximo_actual[1]) = 0; // lo pongo negro en el cacho de imagen para que detecte el proximo maximo
	}
	return maximos;
}

template<class T>
CImg<T> colorea_imagen(CImg<T> imagen, CImg<T> lineas_colorear, T r = 255, T g =
		0, T b = 0) {
	/* colorea una imagen con color rgb a partir de lineas_corear
	 * @lineas colorear es una imagen binaria los pixels !=0 aparecen rojos ne la imagen devuelta
	 * @imagen: es la imagen sobre la cual se dibuja las lineas_a_colorear
	 * */
	//muestro la pista con las dos lineas de color arriba
	CImg<float> img_color(imagen.width(), imagen.height(), 1, 3); // rgb channels
	cimg_forXY(img_color, X, Y)
		{
			if (lineas_colorear(X, Y) != 0) {
				img_color(X, Y, 0, 0) = r; //rojo
				img_color(X, Y, 0, 1) = g; //verde
				img_color(X, Y, 0, 2) = b;
			} else {//dejo las cosas con el valor que estaban
				img_color(X, Y, 0, 0) = imagen(X, Y, 0, 0);
				img_color(X, Y, 0, 1) = imagen(X, Y, 0, 0);
				img_color(X, Y, 0, 2) = imagen(X, Y, 0, 0);
			}
		}
	return img_color;
}

template<class T>
void explorar_intensidad(int x_inicial, int y_inicial, T intensidad, int width,
		int height, float tolerancia, CImg<T> &imagen,
		CImg<T> &imagen_segmentada, int cant_vecinos = 4) {
	/* <NO USAR ESTA FUNCION USAR region_growing()...
	 * segmenta una imagen segun la semilla inicial x,y, con uan cierta tolerancia y usa 4 vecinos para la comparacion
	 * @param: x_inicial: posicion en x del pixel a segmentar
	 * @param: y_inicial: posicion en y del pixel a segmentar
	 * @param: intensidad: intensidad del pixel a segmentar
	 * @param: widht: ancho de la imagen pasada como parametro
	 * @param: height: alto de la imagen pasada como parametro
	 * @param: tolerancia: tolerancia con la que se segmenta intendidad+- tolerancia.
	 * @param: imagen: imagen sobre la cual se aplica la segmentacion
	 * @param: imagen_segmentada: imagen segmentada que es devuelta por referencia. (incialmente debe ser una del mismo tamanio
	 * que  imagen y rellenada con negro.
	 * @param: cant_vecinos(valores aceptados=4 o 8): es la cantidad de vecinos que se usan para comparar. 4 por defecto
	 * */

	if (x_inicial > width - 1 || x_inicial < 0 || y_inicial > height - 1
			|| y_inicial < 0) {
		return;
	}
	int valor = intensidad - tolerancia;
	if (valor < 0)
		valor = 0;
	if ((imagen(x_inicial, y_inicial) <= (intensidad + tolerancia) && imagen(
			x_inicial, y_inicial) >= valor) && imagen_segmentada(x_inicial,
			y_inicial) == 0) {
		imagen_segmentada(x_inicial, y_inicial) = 1;
		if (cant_vecinos == 8) { //hace los de la diagonal y cuando sale del if hace los que faltan...
			explorar_intensidad(x_inicial - 1, y_inicial - 1, intensidad,
					width, height, tolerancia, imagen, imagen_segmentada);//esquina superior izquierda
			explorar_intensidad(x_inicial - 1, y_inicial + 1, intensidad,
					width, height, tolerancia, imagen, imagen_segmentada);//esquina superior derecha
			explorar_intensidad(x_inicial + 1, y_inicial - 1, intensidad,
					width, height, tolerancia, imagen, imagen_segmentada); //esquina inferior izquierda
			explorar_intensidad(x_inicial + 1, y_inicial + 1, intensidad,
					width, height, tolerancia, imagen, imagen_segmentada); //esquina inferior derecha
		}// si no selecciono los 8 solo hago los 4 vecinos:
		explorar_intensidad(x_inicial + 1, y_inicial, intensidad, width,
				height, tolerancia, imagen, imagen_segmentada);//vecino inferior centro
		explorar_intensidad(x_inicial, y_inicial + 1, intensidad, width,
				height, tolerancia, imagen, imagen_segmentada);//vecino vecindo derecho centro
		explorar_intensidad(x_inicial - 1, y_inicial, intensidad, width,
				height, tolerancia, imagen, imagen_segmentada); //vecino superior centro
		explorar_intensidad(x_inicial, y_inicial - 1, intensidad, width,
				height, tolerancia, imagen, imagen_segmentada); //vecino izquierdo centro
	} else
		return;
}

template<class T>
CImg<T> region_growing(CImg<T> imagen_a_segmentar, int x_inicial,
		int y_inicial, float tolerancia = 50.0, int cantidad_vecinos = 4) {
	/* Funcion wrapper que hace el crecimiento de region sobre una imagen en base al parecido
	 * con sus vecinos intensidad del vecino+-tolerancia
	 * devuelve una imagen binaria pintada de blanco la parte que pertenece a la region.
	 * SOLO SIRVE PARA IMAGENES DE 1 solo canal!
	 * @param: imagen_a_segmentar: es la imagen sobre la cual se quiere realizar la segmentacion(regionalizacion)
	 * @param: x_inicial: posicion en x del pixel a segmentar
	 * @param: y_inicial: posicion en y del pixel a segmentar
	 * @param: tolerancia: los pixeles segemntados seran aquellos que cumpla con intensidad+-tolerancia..
	 * @param: cantidad_vecinos(Val. posibles: 4 u 8): cantidad de vecinos usados en la segmentacion 4->cruz, 8->_todo el borde
	 * */
	int width = imagen_a_segmentar.width();
	int height = imagen_a_segmentar.height();
	T intensidad = imagen_a_segmentar(x_inicial, y_inicial);
	CImg<T> imagen_segmentada(imagen_a_segmentar.width(),
			imagen_a_segmentar.height(), 1, 1, 0); //relleno con ceros
	explorar_intensidad(x_inicial, y_inicial, intensidad, width, height,
			tolerancia, imagen_a_segmentar, imagen_segmentada, cantidad_vecinos);
	return imagen_segmentada;
}

template<class T>
CImg<T> binaria_a_original(CImg<T> imagen_binaria, CImg<T> imagen_original) {
	/* Devuelve una imagen en base a la mascara imagen_binaria aplicada sobre la imagen_original...
	 * Lo que este blanco en imagen_binaria es remplazado por lo que tenga la imagen_original y es devuelto en una nueva imagen
	 * */
	CImg<T> imagen(imagen_original.width(), imagen_original.height(), 1, 1, 0); //imagen original rellena con cero de entrada
	cimg_forXY(imagen_binaria, x, y)
		{
			if (imagen_binaria(x, y) != 0) {
				imagen(x, y) = imagen_original(x, y);
			}
		}
	return imagen;
}

template<class T>
CImg<T> binarizar(CImg<T> imagen) {
	/* binariza una imagen haciendo 1 todos aquellos valores de intensidad de la imagen difrente de 0
	 * @param: imagen: es la imagen que se quiere binarizar
	 * */
	CImg<T> imagen_binaria(imagen.width(), imagen.height(), 1, 1, 0);
	cimg_forXYC(imagen, x, y, c)
			{
				if (imagen(x, y, c) != 0) {
					imagen_binaria(x, y) = 1.0;
				}
			}
	return imagen_binaria;
}

int contar_diferentes(CImg<int> imagen_etiquetada) {
	/* Funcion que cuenta la cantidad de etiquetas diferentes que existen en una imagen
	 * @param: imagen_etiquetada es la imagen etiquetada que se obtuvo con labelcc.
	 * Retorna un enetero con la cantidad de regiones diferentes (ignorando la region con valores de intensidad 0).
	 */
	list<int> etiquetas;
	cimg_forXY(imagen_etiquetada, x, y)
		{
			etiquetas.push_back(imagen_etiquetada(x, y)); //paso _todo a una lista
		}
	etiquetas.sort(); //ordeno lista
	etiquetas.unique(); //borro repetidos
	if (*etiquetas.begin() == 0) { //etiquetas.begin es un iterador con * referencio el contenido
		return (etiquetas.size() - 1); //si existe el cero en la lista tengo que restar 1
	}
	return etiquetas.size();
}
template<class T>
CImg<T> grafica_maximos(vector<T> posiciones_maximos, int width, int height) {
	/* Funcion que dado un Vector <T> de posiciones_maximo devueto por la funcion
	 * obtener_maximos retorna una imagen de tamanio Width x height con los maximos
	 * marcados en blanco sobre un fondo negro
	 * @param: posciones_maximos: vector de maximos obtenidos con obtener_maximos
	 * @param width: ancho de la imagen con la que se obtuvo el vector
	 * @param height: alto de la imagen con la que se obtuvo el vector
	 * */
	CImg<T> maxs(width, height, 1, 1);//imagen que voy a usar para dibujar maximos
	maxs.fill(0.0);
	maxs.normalize(0, 255);
	for (unsigned int i = 0; i < (posiciones_maximos.size() - 1); i += 2) {
		cout << "i: " << i << "   i+1: " << i + 1 << endl;
		maxs(posiciones_maximos[i], posiciones_maximos[i + 1]) = 255.0;
		cout << "maximos (x,y)= (" << posiciones_maximos[i] << ", "
				<< posiciones_maximos[i + 1] << ")" << endl;
	}
	return maxs;
}
template<class T>
double detectar_inclinacion(CImg<T> img, float umbral = 1.0) {
	/*devuelve la inclinacion en grados respecto al eje de las x
	 * @param img es la imagen sobre la cual se quiere determinar la inclinacion
	 * @param umbral es el umbral que se aplica a la hora de detectar bordes.
	 * todo: ver si anda para rotaciones en ambos sentidos...
	 * */
	CImg<T> IMG_real = img.get_FFT()[0];
	IMG_real = IMG_real.crop(0, 0, IMG_real.width() / 2, IMG_real.height()
			/ 2.0); // porque no esta centrda
	CImg<T> bordes = aplicar_sobel(IMG_real, umbral, true);
	CImg<T> IMG_HOUGH = hough_directa(bordes);
	vector<T> maxs = obtener_maximos(IMG_HOUGH, 1, -99, 0);
	CImg<T> maxs_img = grafica_maximos(maxs, IMG_HOUGH.width(),
			IMG_HOUGH.height());
	CImg<T> deteccion = hough_inversa(maxs_img);

	//mido la distancia:
	T medio = deteccion.width() / 2.0;
	T y1 = -1.0;
	cimg_forY(deteccion, y) {
		if (deteccion(medio, y) != 0) {
			y1 = y;
			break;
		}
	}
	return (180 * (atan(y1 / medio)) / M_PI); //(180*radiantes/pi)
}

//////////////////////////////////////////////////////////////////////////////////////////////
//funciones chaco
/////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
CImg<T> filtrar_hough_theta(CImg<T> hough, double u_theta, double tol_theta) {
	/**
	 * Filtra la matriz de hough para valores de theta=tita
	 * igual al especificado (mas/menos una tolerancia).
	 * es decir: Deja solo lineas con un angulo de inclinacion
	 * igual al especificado mas/menos una tolerancia
	 * El angulo debe estar -90 y 90
	 */
	u_theta = (u_theta * M_PI) / 180.0;
	int M = hough.width(), N = hough.height();
	double theta, step_theta;

	step_theta = M_PI / (M - 1); //paso en eje theta (M_PI=pi) (theta=[-90,90]) */

	cimg_forXY(hough,t,r)
		{
			theta = t * step_theta - M_PI / 2;
			if (sqrt(pow(theta - u_theta, 2)) <= tol_theta) {
				/* printf("if %f <= %f   => %f   tomo: %f \n", */
				/*        sqrt( pow(theta-u_theta,2) ) , */
				/*        tol_theta, */
				/*        hough(t,r), */
				/*        theta ); */
			} else {
				hough(t, r) = 0;
				/* printf("if %f <= %f   => %f   tomo: %f \n", */
				/*        sqrt( pow(theta-u_theta,2) ) , */
				/*        tol_theta, */
				/*        hough(t,r), */
				/*        theta ); */
			}
		}
	return hough;
}

template<class T>
CImg<T> filtrar_hough_rho(CImg<T> hough, double u_rho, double tol_rho) {
	/**
	 * Filtra la matriz de hough para valores de rho
	 * igual al especificado (mas/menos una tolerancia).
	 *
	 * Es decir: Permite definir la posicion de la recta
	 * en el plano x-y. mas/menos una tolerancia (no el largo!)
	 */
	int M = hough.width(), N = hough.height();
	double max_rho = sqrt(float(pow(N, 2) + pow(M, 2))), step_rho = 2.
			* max_rho / (N - 1), //paso en eje rho (rho=[-max_rho , max_rho])
			rho;

	cimg_forXY(hough,t,r)
		{
			rho = r * step_rho - max_rho; // mapea [0,N] en [-max_rho,max_rho]
			if (sqrt(pow(rho - u_rho, 2)) <= tol_rho) {
				/* printf("if %f <= %f   => %f   tomo: %f \n", */
				/*        sqrt( pow(rho-u_rho,2) ) , */
				/*        tol_rho, */
				/*        hough(t,r), */
				/*        rho ); */
			} else {
				hough(t, r) = 0;
				/* printf("if %f <= %f   => %f   tomo: %f \n", */
				/*        sqrt( pow(rho-u_rho,2) ) , */
				/*        tol_rho, */
				/*        hough(t,r), */
				/*        rho ); */
			}
		}
	return hough;
}

template<class T>
CImg<T> filtrar_hough(CImg<T> hough, double u_rho, double u_theta,
		double tol_rho, double tol_theta) {
	/**
	 * Filtra la matriz de hough para valores de rho y theta
	 * igual al especificado (mas/menos una tolerancia).
	 *
	 * Es decir: Permite definir la posicion (no el largo!) de la recta
	 * en el plano x-y y el angulo de la misma, mas/menos una tolerancia
	 *
	 * para combinaciones usar  funciones combinadas
	 * (coomo lo hace esta)
	 */
	return filtrar_hough_rho(filtrar_hough_theta(hough, u_theta, tol_theta),
			u_rho, tol_rho);
}

template<class T>
CImg<T> filtrar_hough_rho_min(CImg<T> hough, double u_rho) {
	/**
	 * Filtra la matriz de hough para valores de rho
	 * mayores o iguales al especificado.
	 * es decir: Toma solo las linas hubicadas de una determinada
	 * posicion(umbral) en la imagen, en adelante
	 */
	int M = hough.width(), N = hough.height();
	double max_rho = sqrt(float(pow(N, 2) + pow(M, 2))), step_rho = 2.
			* max_rho / (N - 1), //paso en eje rho (rho=[-max_rho , max_rho])
			rho;

	cimg_forXY(hough,t,r)
		{
			rho = r * step_rho - max_rho; // mapea [0,N] en [-max_rho,max_rho]
			if (abs(rho) >= u_rho) {
				hough(t, r) = 0;
			}
		}
	return hough;
}

template<class T>
CImg<T> filtrar_hough_rho_max(CImg<T> hough, double u_rho) {
	/**
	 * Filtra la matriz de hough para valores de rho
	 * menores o iguales al especificado.
	 *
	 * es decir: Toma solo las linas hubicadas de una determinada
	 * posicion(umbral) en la imagen, hacia atrás. (hasta el umbral)
	 */
	int M = hough.width(), N = hough.height();
	double max_rho = sqrt(float(pow(N, 2) + pow(M, 2))), step_rho = 2.
			* max_rho / (N - 1), //paso en eje rho (rho=[-max_rho , max_rho])
			rho;

	cimg_forXY(hough,t,r)
		{
			rho = r * step_rho - max_rho; // mapea [0,N] en [-max_rho,max_rho]
			if (abs(rho) <= u_rho) {
				hough(t, r) = 0;
			}
		}
	return hough;
}
