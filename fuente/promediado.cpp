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
	/*float orden = cimg_option ("-orden", 2.0, "orden del filtro butter");
	 float frec_corte =
	 cimg_option ("-frec_corte", 70.0, "frec. corte del filtro butter");*/
	CImgList<double> lista;
	CImg<double> tmp;
	CImg<double> match(argv[1]);
	match = detectar_bordes(match, 2.0, 70.0).normalize(0,1.0);
	for (int i = 2; i < argc; i++) {
		tmp.assign(argv[i]);
		lista.push_back(detectar_bordes(tmp, 2.0, 70.0).normalize(0,1.0));
	}
	CImg<double> prom = promedio(lista);
	CImgList<double> lista_segmentada = segmentar(match, 40, 40);

	CImgList<double> prom_segmentada = segmentar(prom, 40, 40);
	/*	CImgList<double> p_l(prom);
	 CImgList<double> m_l(match);*/
	prom.print();
	match.print();
	printf("Error = %f\n", prom.MSE(match));

	CImgDisplay disp(prom, "promediado");
	CImgDisplay di(match, "match");
	match.display();
	while (!disp.is_closed()) {
		disp.wait();
	}

	return 0;
}
