#include <CImg.h>

using namespace cimg_library;

template<class T>
CImgList<T> segmentar(CImg<T> img, int ancho = 20, int alto = 20) {
	int cont = 0;
	CImgList<T> cuadros;
	for (int y = 0; y < img.height() && y + alto < img.height(); y += alto) {
		for (int x = 0; x < img.width() && x + ancho < img.width(); x += ancho) {
			cuadros.insert(
					img.get_crop(x, y, x + ancho - 1, y + alto - 1, true),
					cont++);
		}
	}
	return cuadros;
}

template<class T>
CImg<T> multiplicar(CImg<T> im1, CImg<T> im2, bool normalizar = true) {
	//multiplica 2 imagenes...
	CImg<T> imagen(im1.width(), im1.height(), 1, 1);
	cimg_forXY(im1, x, y)
		{
			imagen(x, y) = im1(x, y) * im2(x, y);
		}
	if (normalizar)
		return imagen.normalize();
	else
		return imagen;
}

template<class T>
CImg<T> filtrar(CImg<T> imagen, CImg<T> H) {
	/**
	 * Retorna la imagen filtrada con el filtro pasado
	 * El filtro debe estar diseñado centrado
	 */
	CImgList<T> F = imagen.get_FFT();//obtengo la transformada
	CImg<T> F_real = F[0]; //parte real
	CImg<T> F_imag = F[1]; //parte imaginaria
	//realizo el filtrado en el dominio de las frecuecnias:
	F_real = multiplicar<T> (F_real, H, false);
	F_imag = multiplicar<T> (F_imag, H, false);
	F[0] = F_real;
	F[1] = F_imag;
	return F.get_FFT(true)[0]; //devuelvo la parte real
}
template<class T>
CImg<T> get_PA_Butter(int dimx, int dimy, float frec_corte = 10.0, float orden =
		1.0) {
	//genera un filtro pasa altos butterworth de dimx x dimy con frec corte =10 por defecto y orden 1.0 por defecto
	float distancia;
	float mediox = dimx / 2.0;
	float medioy = dimy / 2.0;
	CImg<T> H(dimx, dimy, 1, 1, 0.0);
	cimg_forXY(H,x,y)
		{
			distancia = sqrt(pow(x - mediox, 2.0) + pow(y - medioy, 2.0));
			H(x, y) = 1.0 / (1.0 + pow(abs(frec_corte) / distancia, 2.0 * abs(
					orden)));
		}
	return H;
}
template<class T>
CImg<T> aplicar_PA_Butter(CImg<T> imagen, float orden = 1.0, float frec_corte =
		10.0) {
	/* aplica un filtro pasa Altos Butterwortch con frecuencia de corte: frec_corte y orden = orden.
	 * Por defecto: frec_corte=10 y orden=1
	 * Devuelve la imagen filtrada para ser mostrada.
	 */
	CImg<T> H = get_PA_Butter<T> (imagen.width(), imagen.height(), frec_corte,
			orden);
	CImg<T> Hf = H.get_shift(H.width() / 2, H.height() / 2, 0, 0, 2);
	return filtrar<T> (imagen, Hf);
}

template<class T>
CImg<T> detectar_bordes(CImg<T> objetivo, float orden = 1.0, float frec_corte =
		10.0) {
	/* funcion que devuelve la imagen filtrada con los bordes de una imagen mediante un filtro
	 * Pasa altos del tipo Butterworth
	 * @objetivo: imagen sobre la cual se aplica el filtro
	 * @orden: orden del filtro Butterworth que se aplicara sobre @objetivo - por defecto 1.0
	 * @frec_corte: frecuencia de corte del filtro Butterworth - por defecto 10.0
	 * */
	return aplicar_PA_Butter<T> (objetivo, orden, frec_corte);
}
