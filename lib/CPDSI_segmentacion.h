// Implementacion de métodos de crecimiento de regiones
// y transformación Hough directa e inversa.
// CPDSI - 2009

#include <CImg.h>
#include <vector>
using namespace std;
using namespace cimg_library;
///****************************************
/// Crecimiento de regiones
///****************************************
/*region_growing(nodo, propiedad, etiqueta) {
	if (nodo NO verifica propiedad) return;
	if (nodo etiquetado) return;
	etiquetar nodo;
	region_growing(nodo derecho, propiedad, etiqueta);
	region_growing(nodo izquierdo, propiedad, etiqueta);
	region_growing(nodo superior, propiedad, etiqueta);
	region_growing(nodo inferior, propiedad, etiqueta);
}*/

///****************************************
/// Crecimiento de regiones con cola
///****************************************


/*region_growing_alternativo(nodo, propiedad, etiqueta) {
	inicializar una cola vacia
	etiquetar nodo (semilla)
	agregar nodo al final de la cola
	while (cola no este vacia) {
		tomar el primer nodo de la cola de Q (como pivote) y borrarlo de la cola (ya esta etiquetado)
		if (nodo derecho verifica propiedad) etiquetar nodo derecho y agregarlo a la cola
		if (nodo izquierdo verifica propiedad) etiquetar nodo izquierdo y agregarlo a la cola
		if (nodo superior verifica propiedad) etiquetar nodo superior y agregarlo a la cola
		if (nodo inferior verifica propiedad) etiquetar nodo inferior y agregarlo a la cola
	}
}*/

///****************************************
/// Etiquetado de componentes conectadas
///****************************************
CImg<int> label_cc(CImg<int> img, int blanco = 1, int nueva_etiqueta = 2) {

	vector<int> equiv(nueva_etiqueta + 1, 0); //vector de equivalencias
	vector<int> vecinos; //vector de etiquetas vecinos superiores e izquierda
	int pos, etiqueta, aux;

	cimg_forXY(img,x,y)
		{ // recorro la imagen
			if (img(x, y) == blanco) { // si es blanco
				vecinos.clear(); // inicializo
				if (x && y) // si x no es borde izq e y no es borde superior miro el vecino sup izq
					if (img(x - 1, y - 1) != 0)
						vecinos.push_back(img(x - 1, y - 1)); // si tiene etiqueta la guardo
				if (y) // si y no es borde superior miro vecino superior
					if (img(x, y - 1) != 0)
						vecinos.push_back(img(x, y - 1)); // si tiene etiqueta la guardo
				if (y && x != img.width() - 1) // si x no es borde derecho e y borde superior miro vecino sup der
					if (img(x + 1, y - 1) != 0)
						vecinos.push_back(img(x + 1, y - 1)); // si tiene etiqueta la guardo
				if (x) // si x no es borde izquierdo miro vecino izq
					if (img(x - 1, y) != 0)
						vecinos.push_back(img(x - 1, y)); // si tiene etiqueta la guardo
				if (vecinos.empty()) { // si no tengo vecinos etiquetados debo generar nueva etiqueta
					vecinos.push_back(nueva_etiqueta); // de vecinos voy a sacar la etiqueta que corresponde al pixel
					equiv[nueva_etiqueta] = nueva_etiqueta; // guardo en la posicion nva etiqueta el valor nva etiqueta
					nueva_etiqueta++; // genero una nueva etiqueta para cdo necesite
					equiv.push_back(0); // agrego una posicion en equivalencias con 0
				} else {
					for (int i = 0; i < vecinos.size() - 1; i++) // controlo la lista de etiquetas vecinas
						if (vecinos[i] != vecinos[i + 1]) { // si hay diferentes etiquetas en el vecindario anoto
							etiqueta = vecinos[i];
							pos = vecinos[i + 1];
							if (pos < etiqueta) { // en la pos de la mayor etiqueta anoto el valor de la menor
								aux = etiqueta;
								etiqueta = pos;
								pos = aux;
							}
							if (equiv[pos] != etiqueta) { // si tengo una entrada en esa pos reviso la cadena
								if (equiv[pos] != pos) {
									aux = etiqueta;
									etiqueta = equiv[pos];
									pos = aux;
									while (equiv[pos] != pos)
										pos = equiv[pos];
									if (equiv[pos] < etiqueta)
										etiqueta = equiv[pos];
								}
								equiv[pos] = etiqueta;
							}
						}
				}
				img(x, y) = vecinos.front(); // asigno etiqueta
			}
		}
	//img.display("Primera Pasada");

	// Muestro como quedo la tabla
	cout << "Tabla de equivalencias" << endl << endl;
	for (int j = 0; j < equiv.size(); j++)
		cout << j << " " << equiv[j] << endl;
	cout << endl;

	// reasigno la etiqueta
	cimg_forXY(img,x,y)
			if (img(x, y) != 0)
				if (equiv[img(x, y)] != img(x, y)) {
					etiqueta = equiv[img(x, y)];
					while (equiv[etiqueta] != etiqueta)
						etiqueta = equiv[etiqueta];
					img(x, y) = etiqueta;
				}

	return img;
}

///****************************************
/// Transformada Hough directa
///****************************************
CImg<double> hough_directa(CImg<double> img, double dtita = 1, double dro = 1,
		bool getHist = 0) {

	CImg<double> iHough(img);
	iHough.fill(0.0); //genera espacio parametrico mediante valores equiespaciados de p y rho inicializandolo en cero(acumulador)
	const unsigned M = img.width(), N = img.height();
	int r; // radio mapeado en los N pixeles

	double max_rho = sqrt(float(pow(N, 2) + pow(M, 2))), //maximo valor posible de radio se da en la diagonal pcipal
			step_rho = 2. * max_rho / (N - 1), //paso en eje rho (rho=[-max_rho , max_rho])
			step_theta = M_PI / (M - 1), //paso en eje theta (M_PI=pi) (theta=[-90,90])
			rho, theta;

	cimg_forXY(img,x,y)
		{
			if (img(x, y) > 0.5)
				for (int t = 0; t < M; t++) { //calculo rho variando theta en _todo el eje, con x e y fijo
					theta = t * step_theta - M_PI / 2; // mapea [0,M] en [-90,90]
					rho = x * cos(theta) + y * sin(theta); // calcula rho
					r = int((rho + max_rho) / step_rho); // mapea [-max_rho , max_rho] en [0,N]
					iHough(t, r) += 1; // suma el acumulador
				}
		}
	return iHough;
}

///****************************************
/// Transformada Hough inversa
///****************************************
CImg<double> hough_inversa(CImg<double> img) {
	const double blanco[1] = { 255.f };
	CImg<double> iHoughI(img);
	iHoughI.fill(0.0);
	int M = img.width(), N = img.height(), y0, y1;

	double max_rho = sqrt(float(pow(N, 2) + pow(M, 2))), //maximo valor posible de radio se da en la diagonal pcipal
			step_rho = 2. * max_rho / (N - 1), //paso en eje rho (rho=[-max_rho , max_rho])
			step_theta = M_PI / (M - 1), //paso en eje theta (M_PI=pi) (theta=[-90,90])
			rho, theta;

	cimg_forXY(img,t,r)
		{
			if (img(t, r) > 0.5) {
				theta = t * step_theta - M_PI / 2; // mapea [0,M] en [-90,90]
				rho = r * step_rho - max_rho; // mapea [0,N] en [-max_rho,max_rho]
				y0 = int(rho / sin(theta));
				y1 = int(rho / sin(theta) - M / tan(theta));
				iHoughI.draw_line(0, (int) y0, (int) M, (int) y1, blanco);
			}
		}
	return iHoughI;
}
