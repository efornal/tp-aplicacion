#include <CImg.h>

using namespace cimg_library;

/**
 * Segmenta una imagen en cuadritos de ancho por alto especificado
 * @param CImg<T> img Imagen a segmentar
 * @param int ancho Ancho de cada cuadrito
 * @param int alto Alto de cada cuadrito
 * @return CImgList<T> Lista de cuadritos de la imagen segmentada
 *
 * La particion de la imagen se realiza de izq a derecha 
 * y de arriba hacia abajo en cuadros de tama�o ancho x alto
 * Si el tama�o de la imagen no es divisible de forma exacta 
 * (sin decimales) con el tama�o especificado para los cuadritos,
 * la segmentacion omite la parte sobrante (no divisible), tanto en alto
 * como en ancho.
*/
template<class T>
CImgList<T> segmentar(CImg<T> img, int ancho = 20, int alto = 20) {
    CImgList<T> cuadros;
    for (int y = 0; y < img.height() && y + alto < img.height(); y += alto) {
        for (int x = 0; x < img.width() && x + ancho < img.width(); x += ancho) {
            cuadros.push_back( img.get_crop(x, y, x + ancho - 1, y + alto - 1) );
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
    /*genera un filtro pasa altos butterworth de dimx x dimy con frec corte =10
     *  por defecto y orden 1.0 por defecto*/
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
    /* aplica un filtro pasa Altos Butterwortch con frecuencia de corte:
     *  frec_corte y orden = orden.
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
    /* funcion wrapper que devuelve la imagen filtrada con los bordes de una
     * imagen mediante un filtro Pasa altos del tipo Butterworth
     *
     * @objetivo: imagen sobre la cual se aplica el filtro
     *
     * @orden: orden del filtro Butterworth que se aplicara sobre @objetivo
     *  - por defecto 1.0
     *
     * @frec_corte: frecuencia de corte del filtro Butterworth - por defecto
     *  10.0
     * */
    return aplicar_PA_Butter<T> (objetivo, orden, frec_corte);
}

template<class T>
CImg<T> promedio(CImgList<T> lista_imagenes) {
    /* funcion que dada una lista de imagenes retorna el promedio de la mismas
     * */
    CImg<T> promediada = lista_imagenes(0);
    unsigned int tam_lista = lista_imagenes.size();

    unsigned w = lista_imagenes(0).width(), h = lista_imagenes(0).height(), d =
        lista_imagenes(0).spectrum();

    for (unsigned int i = 1; i < tam_lista; i++) {
        // valido que las imágenes a comparar tengan las mismas dimensiones ( x, y, c )
        if (lista_imagenes(i).width() != w || lista_imagenes(i).height() != h
            || lista_imagenes(i).spectrum() != d)
            return promediada; // caracoles, no explotará la cosa acá?
        //->fixme: comento esto porque me patea si retornas null
        /*	promediada += lista_imagenes(i);*/
        cimg_forXYC(promediada,x,y,c){
            promediada(x,y,c)+=lista_imagenes[i](x,y,c);
        }
    }
    cimg_forXYC(promediada,x,y,c){
        promediada(x,y,c)/=(double) tam_lista;
    }

    return promediada;
}

template<class T>
double calcular_mse(CImgList<T> imagenes, CImgList<T> patrones) {

    /* funcion que devuelve el error cuadrático medio entre dos listas de
     * imágenes chiquitas
     * @imagenes: imágenes a comparar con los patrones
     * @patrones: patrones con los cuales se comparará el MSE
     * */

    // chequeo que haya el mismo numero de imágenes
    if (imagenes.size() != patrones.size())
        return -1.0;

    double mse = 0;

    for (unsigned i = 0; i < patrones.size(); i++) {
        // valido que las imágenes a comparar tengan las mismas dimensiones ( x, y, c )
        if (imagenes(i).width() != patrones(i).width() || imagenes(i).height()
            != patrones(i).height() || imagenes(i).spectrum()
            != patrones(i).spectrum())
            return -1.0;

        // el MSE total es simplemente la suma de los mses de cada imagencita
        mse += imagenes(i).MSE(patrones(i));
    }
    return mse;
}
