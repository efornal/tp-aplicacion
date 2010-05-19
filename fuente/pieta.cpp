#define cimg_use_fftw3 1

#ifdef cimg_use_fftw3
extern "C" {
#include "fftw3.h"
}
#endif

#include <CImg.h>
#include "../lib/commons.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char **argv) {
	const char *filename = cimg_option( "-f",
			"imagenes/la-piedad.jpg",
			"ruta archivo imagen" );
	int ancho = cimg_option ( "-ancho", 20, "ancho de recuadro");
	int alto = cimg_option ( "-alto", 20, "alto de recuadro");
	float orden = cimg_option ("-orden", 2.0, "orden del filtro butter");
	float frec_corte =
			cimg_option ("-frec_corte", 70.0, "frec. corte del filtro butter");

	CImgDisplay disp, disp2;
	CImg<double> img(filename);
	img.channel(0); //temporalmente
	img.display(disp);

	CImgList<double> lista = segmentar(img, ancho, alto);

	//promedio de una lista de imagenes:
	CImgDisplay disp_promedio(promedio(lista), "promedio de lista de imagenes");
	//deteccion de los bordes:
	CImgDisplay disp_bordes(detectar_bordes(img, orden, frec_corte),
			"bordes de imagen");

	while ((!disp.is_closed() && !disp.is_keyQ())) {
		disp.wait_all();
	}

	printf("tamanio de la lista: %d; error: %f", lista.size(), calcular_mse(
			lista.get_images(0, 0), lista.get_images(1, 1)));

	lista[0].display();
	lista[1].display();
	return 0;
}
