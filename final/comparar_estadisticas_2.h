/*
 * extract_estadisticas_histo.cpp
 * define el extractor de características ExtractorCaracteristicasHistogramas
 */

#ifndef COMPARAR_ESTADISTICAS_2_H
#define COMPARAR_ESTADISTICAS_2_H

#include <CImg.h>
#include <string>
#include <glob.h>
#include <regex.h>

using namespace cimg_library;
using namespace std;

template<class T>
CImg<T> promedio(CImgList<T> lista_imagenes) {
	/* funcion que dada una lista de imagenes retorna el promedio de la mismas
	 * */
	CImg<T> promediada = lista_imagenes(0);
	unsigned int tam_lista = lista_imagenes.size();

	unsigned w = lista_imagenes(0).width(), h = lista_imagenes(0).height(), d =
			lista_imagenes(0).spectrum();

	for (unsigned int i = 1; i < tam_lista; i++) {
		// valido que las imÃ¡genes a comparar tengan las mismas dimensiones ( x, y, c )
		if (lista_imagenes(i).width() != w || lista_imagenes(i).height() != h
				|| lista_imagenes(i).spectrum() != d)
			return promediada; // caracoles, no explotarÃ¡ la cosa acÃ¡?
		//->fixme: comento esto porque me patea si retornas null
		/*	promediada += lista_imagenes(i);*/
		cimg_forXYC(promediada,x,y,c)
				{
					promediada(x, y, c) += lista_imagenes[i](x, y, c);
				}
	}
	cimg_forXYC(promediada,x,y,c)
			{
				promediada(x, y, c) /= (double) tam_lista;
			}

	return promediada;
}

/**
 * generar_base
 * genera una imagen base con todas las imï¿½genes que encuentre en el directorio
 * pasado como parï¿½metro.
 * @param directorio directorio donde buscar las imï¿½genes para generar la base
 * @return base generada (sin segmentar, por ahora)
 */
template<class T>
CImg<T> generar_base(const char* directorio) {
	CImgList<T> lista_imagenes;
	CImg<T> temp;

	// el "globbing" me permite expandir los asteriscos como cuando hago `ls ./*`
	// acï¿½ lo que hago es expandir la expresiï¿½n (directorio)/*, y para cada una
	// de las expansiones matcheo la expresiï¿½n regular "match", que matchea para
	// los archivos tipo imagen: .jpg, .jpeg, .JPG, .bmp, et cetera.

	// globbuf es la estructura donde se guardan los resultados del glob.
	// me interesan 2 de sus miembros: gl_pathc, que contiene el nï¿½mero
	// de expansiones, y gl_pathv que es un char** con las expansiones mismas
	glob_t globbuf;

	// match contendrï¿½ la expresiï¿½n regular "compilada" en la llamada a regcomp
	regex_t match;

	// compilo la expresiï¿½n regular en el 2do parï¿½metro en match,
	// el 3er argumento le dice que ignore mayusculas/minusculas
	// por alguna putï¿½sima razï¿½n tengo que comentar con \\ los caracteres ()?|
	regcomp( &match, ".*\\.\\(jpe\\?g\\|bmp\\tif{1,2}\\|png\\|gif\\)$",
			REG_ICASE );

	// expando directorio/*
	glob(std::string(std::string(directorio) + std::string("/*")).c_str(),
			GLOB_MARK, NULL, &globbuf);

	// recorro todas las expansiones
	for (unsigned i = 0; i < globbuf.gl_pathc; i++) {
		// matcheo la expresion regular
		if (regexec(&match, globbuf.gl_pathv[i], 0, NULL, 0) == 0) {
			// si matchea cargo la imagen en temp y la meto al final de la lista
			lista_imagenes.push_back(CImg<T> (globbuf.gl_pathv[i]));
		}
	}

	// libero los cosos auxiliares de glob y regex
	regfree(&match);
	globfree(&globbuf);

	// devuelvo la base generada a partir del promedio de las imagenes
	return promedio(lista_imagenes);
}

/**
 * Segmenta una imagen en cuadritos de ancho por alto especificado
 * @param CImg<T> img Imagen a segmentar
 * @param int ancho Ancho de cada cuadrito
 * @param int alto Alto de cada cuadrito
 * @return CImgList<T> Lista de cuadritos de la imagen segmentada
 *
 * La particion de la imagen se realiza de izq a derecha 
 * y de arriba hacia abajo en cuadros de tamaï¿½o ancho x alto
 * Si el tamaï¿½o de la imagen no es divisible de forma exacta 
 * (sin decimales) con el tamaï¿½o especificado para los cuadritos,
 * la segmentacion omite la parte sobrante (no divisible), tanto en alto
 * como en ancho.
 */
template<class T>
CImgList<T> segmentar(CImg<T> img, int ancho = 20, int alto = 20) {
	CImgList<T> cuadros;
	for (int y = 0; y <= img.height() && y + alto <= img.height(); y += alto) {
		for (int x = 0; x <= img.width() && x + ancho <= img.width(); x
				+= ancho) {
			cuadros.push_back(img.get_crop(x, y, x + ancho - 1, y + alto - 1));
		}
	}
	return cuadros;
}

/**
 * sumar_dim
 * agarra una imagen y la suma a lo largo de una dimensiï¿½n, i.e. si sumo
 * a lo largo de x entonces obtendrï¿½ un vector en y del mismo alto que la imagen
 * original, donde cada punto en y contendrï¿½ el valor de sum(0:ancho,y)
 * @param imagen la imagen sobre la que calcular la suma
 * @param dimension la dimensiï¿½n a sumar, por defecto x, valores posibles: 'xXyY'
 * @return vector de la suma, con una dimensiï¿½n menos que la imagen original.
 * @TODO generalizar para sumar a lo largo de las dimensiones z y c.
 */
template<class T>
CImg<T> sumar_dim(const CImg<T> imagen, char dimension = 'x') {
	CImg<T> vector_salida;
	// si dimension es x, recorro para cada Y y sumo todo X en cada Y
	if (dimension == 'x' | dimension == 'X') {
		vector_salida.assign(1, imagen.height(), 1, 1, 0);
		cimg_forY( imagen, y ) {
			vector_salida(0, y)
					= imagen.get_crop(0, y, imagen.width() - 1, y).sum();
		}
	}
	// viceversa si dimensiï¿½n es X
	if (dimension == 'y' | dimension == 'Y') {
		vector_salida.assign(imagen.width(), 1, 1, 1, 0);
		cimg_forX( imagen, x ) {
			vector_salida(x, 0)
					= imagen.get_crop(x, 0, x, imagen.height() - 1).sum();
		}
	}
	return vector_salida;
}

double media ( const CImg<double> &histog ) {
  unsigned x,y;
  double media = 0;
  cimg_forXY(histog, x, y) {
    media += histog(x,y)*(double)(x+y);
  }
  return media/(double)(histog.width()*histog.height());
}

double stddev ( const CImg<double> &histog ) {
  unsigned x,y;
  double stddev=0, mu = (double)media(histog)*(double)(histog.width()*histog.height());
  cimg_forXY(histog, x, y) {
    stddev += (double)histog(x,y)*abs((double)(x+y)-mu);
  }
  return stddev/(double)(histog.width()*histog.height());
}

/**
 * estadisticas_imagen
 * calcula una serie de estadï¿½sticas sobre la imagen pasada como parï¿½metro, a saber
 * media y stddev sobre la imagen entera, media y stddev sobre las sumas horizontales
 * y verticales, media y stddev sobre el histograma de la imagen entera, y sobre las
 * sumas horiz y vert.
 * @param imagen la imagen sobre la que se calcularï¿½n estas medidas
 * @return un vector horizontal CImg<double> con los valores en el orden que se
 *         mencionan ariba.
 */
template<class T>
CImg<double> estadisticas_imagen(const CImg<T> imagen) {
	CImg<double> resultados(12, 1, 1, 1, 0);

	// calculo las sumas h y v
	CImg<T> sum_h = sumar_dim<T> (imagen, 'y'), sum_v = sumar_dim<T> (imagen,
			'x');

	// calculo los histogramas de la imagen entera y las sumas
	CImg<double> histo = imagen.get_normalize(0,255).get_histogram(256),
	  histo_h = sum_h.normalize(0,255).get_histogram(256),
	  histo_v = sum_v.normalize(0,255).get_histogram(256);

	histo /= (double)(imagen.width()*imagen.height());
	histo_h /= (double)(sum_h.width()*sum_h.height());
	histo_v /= (double)(sum_v.width()*sum_v.height());

	// relleno el vector con resultados. uso sqrt(var) = stddev porque es mï¿½s
	// parecido en magnitud a las medias, y mostrando el vector se "ve mejor"
	resultados(0) = imagen.get_normalize(0,1).mean();
	resultados(1) = sqrt(imagen.get_normalize(0,1).variance());
	resultados(2) = sum_h.get_normalize(0,1).mean();
	resultados(3) = sqrt(sum_h.get_normalize(0,1).variance());
	resultados(4) = sum_v.get_normalize(0,1).mean();
	resultados(5) = sqrt(sum_v.get_normalize(0,1).variance());
	resultados(6) = media(histo);
	resultados(7) = stddev(histo);
	resultados(8) = media(histo_h);
	resultados(9) = stddev(histo_h);
	resultados(10) = media(histo_v);
	resultados(11) = stddev(histo_v);

	return resultados;
}

/**
 * error_estadisticas_imagen
 * calcula el MSE entre los vectores de estadï¿½sticas de dos imï¿½genes.
 * @param img1, img2 las imï¿½genes a comparar
 * @return MSE (double) entre los vectores de estadï¿½sticas de las imï¿½genes.
 */
template<class T>
double error_estadisticas_imagen(const CImg<T> img1, const CImg<T> img2,
		double normalize_min = 0, double normalize_max = 255) {
	CImg<double> eimg1 = estadisticas_imagen<double> (img1);
	CImg<double> eimg2 = estadisticas_imagen<double> (img2);
	return eimg1.normalize(normalize_min, normalize_max).MSE(eimg2.normalize(
			normalize_min, normalize_max));
}

template<class T>
double error_estadisticas_imagen(const CImgList<T> img1,
		const CImgList<T> img2, double normalize_min = 0, double normalize_max =
				255) {
	double error = 0;
	unsigned int i = 0;
	for (i; i < img1.size(); i++) {
		error += error_estadisticas_imagen(img1[i], img2[i], normalize_min,
				normalize_max);
	}
	return error / (float) i;
}

#endif
