#include <CImg.h>
#include <string>
#include <glob.h>
#include <regex.h>

using namespace cimg_library;

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
CImgList<T> segmentar(CImg<T> img, int ancho, int alto) ;

template<class T>
CImg<T> multiplicar(CImg<T> im1, CImg<T> im2, bool normalizar );

template<class T>
CImg<T> filtrar(CImg<T> imagen, CImg<T> H);

template<class T>
CImg<T> get_PA_Butter(int dimx, int dimy, float frec_corte, float orden );

template<class T>
CImg<T> aplicar_PA_Butter(CImg<T> imagen, float orden, float frec_corte );

template<class T>
CImg<T> detectar_bordes(CImg<T> objetivo, float orden , float frec_corte );

template<class T>
CImg<T> promedio(CImgList<T> lista_imagenes);

template<class T>
double calcular_mse(CImgList<T> imagenes, CImgList<T> patrones);

/**
 * Dado un conjunto de imagenes en la lista, genera una base
 * (imagen) que es la que luego se usarï¿½ para comparar contra una imagen dada.
 * @param CImgList<T> Lista con imagenes a procesar
 * @return CImg<T> Imagen base usada para la comparacion
 *
 * La idea es que este procedimiento sea el encargado de realizar
 * las acciones necesarias (pre-procesos) para que la imagen quede lista
 * para usar en la comparacion con cualquier otra dada. Es la funcion
 * de -abstracciï¿½n- de los procesos aplicados a la misma.
 * TODO: hacer parametros configurables
 */
template<class T>
CImg<T> generar_base(CImgList<T> lista);

/**
 * generar_base
 * genera una imagen base con todas las imï¿½genes que encuentre en el directorio
 * pasado como parï¿½metro.
 * @param directorio directorio donde buscar las imï¿½genes para generar la base
 * @return base generada (sin segmentar, por ahora)
 */
template<class T>
CImg<T> generar_base(const char* directorio);

/**
 * generar_base_lista
 * genera una lista de imagenes a partir del directorio pasado como param.
 * @param directorio directorio donde buscar las imágenes
 * @return lista de imágenes cargada del directorio 
 */
template<class T>
CImgList<T> generar_base( const char* directorio );

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
CImg<T> sumar_dim(const CImg<T> imagen, char dimension );

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
CImg<double> estadisticas_imagen(const CImg<T> imagen);

/**
 * error_estadisticas_imagen
 * calcula el MSE entre los vectores de estadï¿½sticas de dos imï¿½genes.
 * @param img1, img2 las imï¿½genes a comparar
 * @return MSE (double) entre los vectores de estadï¿½sticas de las imï¿½genes.
 */
template<class T>
double error_estadisticas_imagen(const CImg<T> img1, const CImg<T> img2,
				 double normalize_min, double normalize_max );

template<class T>
double error_estadisticas_imagen(const CImgList<T> img1,
				 const CImgList<T> img2, double normalize_min, double normalize_max );

/**
 * Cuenta los blancos por cada cuadrito de la lista lo que genera un
 * perfil, retorna el mismo como imagen
 * 
 * @param CImgList<T> imagenes Lista con Imagenes de los cuadritos
 * @return CImg<unsigned int> Imagen con Perfil de blancos
 *
 * Perfil de blancos: Sea la lista con imagenes de los cuadritos,
 * el perfil de blancos es la imagen formada por la cantidad de pixels
 * blancos que tenga cada cuadrito.
 *
 * Se supone las imagenes de la lista son binarias, por lo que hacer
 * una suma retorna la cantidad de unos (blancos).
 */
template<class T>
CImg<unsigned int> perfil_blancos(CImgList<T> imagenes);

/**
 * Transformada Hough directa
 *
 * tomada de funciones de la catedra
 *
 *                        alto                  alto
 * escala rho = ---------------------------  = ------
 *              2 * sqrt{ alto^2 + ancho^2 }   2sqrt{D}
 *
 * NOTA: D = sqrt{ alto^2 + ancho^2 }, se multiplica por 2
 * para armar el acumulador con parte positiva y negativa
 * 
 * escala theta = ancho / pi

 * M_PI = valor de pi, puede definirse si no esta como:
 * ifndef...define M_PI           3.14159265358979323846
 *
 */
template<class T>
CImg<T> hough_directa(CImg<T> img);

/**
 * Transformada Hough inversa
 *
 * tomada de funciones de la catedra
 */
template<class T>
CImg<T> hough_inversa(CImg<T> img);

/**
 * solo maximos de la t hough
 * FIXME: para valores menores que uno no va detectarlos
 * o va detecta el mismo q esta usando para marcar..
 * se podria usar valores negativos???
 */
template<class T>
CImg<T> get_solo_maximos(CImg<T> img, int cantidad );
