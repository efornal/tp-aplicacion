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
	CImgList<double> lista;
	CImg<double> tmp;
	for (int i = 1; i < argc; i++) {
		tmp.assign(argv[i]);
		tmp.display();
		lista.push_back(tmp);

	}
	//promedio(lista).display();


	/*while ((!disp.is_closed() && !disp.is_keyQ())) {
	 //		disp.wait_all();
	 }*/
	return 0;
}
