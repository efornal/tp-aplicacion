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
	for (int i = 1; i < argc; i++) {
		tmp.assign(argv[i]);
		lista.push_back(detectar_bordes(tmp, 2.0, 70.0));
	}

	CImgDisplay disp(promedio(lista), "promediado");

	while (!disp.is_closed()){
		disp.wait();
	}

	return 0;
}
