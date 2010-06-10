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
 * y de arriba hacia abajo en cuadros de tamaño ancho x alto
 * Si el tamaño de la imagen no es divisible de forma exacta 
 * (sin decimales) con el tamaño especificado para los cuadritos,
 * la segmentacion omite la parte sobrante (no divisible), tanto en alto
 * como en ancho.
*/
template<class T>
CImgList<T> segmentar(CImg<T> img, int ancho = 20, int alto = 20) {
    CImgList<T> cuadros;
    for (int y = 0; y <= img.height() && y + alto <= img.height(); y += alto) {
        for (int x = 0; x <= img.width() && x + ancho <= img.width(); x += ancho) {
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
     * El filtro debe estar diseÃ±ado centrado
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
        // valido que las imÃ¡genes a comparar tengan las mismas dimensiones ( x, y, c )
        if (lista_imagenes(i).width() != w || lista_imagenes(i).height() != h
            || lista_imagenes(i).spectrum() != d)
            return promediada; // caracoles, no explotarÃ¡ la cosa acÃ¡?
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

    /* funcion que devuelve el error cuadrÃ¡tico medio entre dos listas de
     * imÃ¡genes chiquitas
     * @imagenes: imÃ¡genes a comparar con los patrones
     * @patrones: patrones con los cuales se compararÃ¡ el MSE
     * */

    // chequeo que haya el mismo numero de imÃ¡genes
    if (imagenes.size() != patrones.size())
        return -1.0;

    double mse = 0;

    for (unsigned i = 0; i < patrones.size(); i++) {
        // valido que las imÃ¡genes a comparar tengan las mismas dimensiones ( x, y, c )
        if (imagenes(i).width() != patrones(i).width() || imagenes(i).height()
            != patrones(i).height() || imagenes(i).spectrum()
            != patrones(i).spectrum())
            return -1.0;

        // el MSE total es simplemente la suma de los mses de cada imagencita
        mse += imagenes(i).MSE(patrones(i));
    }
    return mse / (double)patrones.size();
}


/**
 * Dado un conjunto de imagenes en la lista, genera una base
 * (imagen) que es la que luego se usará para comparar contra una imagen dada.
 * @param CImgList<T> Lista con imagenes a procesar
 * @return CImg<T> Imagen base usada para la comparacion
 *
 * La idea es que este procedimiento sea el encargado de realizar
 * las acciones necesarias (pre-procesos) para que la imagen quede lista
 * para usar en la comparacion con cualquier otra dada. Es la funcion
 * de -abstracción- de los procesos aplicados a la misma.
 * TODO: hacer parametros configurables
 */
template<class T>
CImg<T> generar_base ( CImgList<T> lista ) {
    return segmentar( detectar_bordes( promedio( lista ), 2.0, 70.0 ).
                      normalize( 0, 255 ),
                      100, 100 ).get_append('x');
}

/**
 * generar_base
 * genera una imagen base con todas las imágenes que encuentre en el directorio
 * pasado como parámetro.
 * @param directorio directorio donde buscar las imágenes para generar la base
 * @return base generada (sin segmentar, por ahora)
 */
template<class T>
CImg<T> generar_base( const char* directorio ) {
  CImgList<T> lista_imagenes;
  CImg<T> temp;

  // el "globbing" me permite expandir los asteriscos como cuando hago `ls ./*`
  // acá lo que hago es expandir la expresión (directorio)/*, y para cada una
  // de las expansiones matcheo la expresión regular "match", que matchea para
  // los archivos tipo imagen: .jpg, .jpeg, .JPG, .bmp, et cetera.

  // globbuf es la estructura donde se guardan los resultados del glob.
  // me interesan 2 de sus miembros: gl_pathc, que contiene el número
  // de expansiones, y gl_pathv que es un char** con las expansiones mismas
  glob_t globbuf;

  // match contendrá la expresión regular "compilada" en la llamada a regcomp
  regex_t match;

  // compilo la expresión regular en el 2do parámetro en match,
  // el 3er argumento le dice que ignore mayusculas/minusculas
  // por alguna putísima razón tengo que comentar con \\ los caracteres ()?|
  regcomp( &match, ".*\\.\\(jpe\\?g\\|bmp\\tif{1,2}\\|png\\|gif\\)$", REG_ICASE );

  // expando directorio/*
  glob( std::string( std::string(directorio)+std::string("/*")).c_str(),
	GLOB_MARK, NULL, &globbuf );

  // recorro todas las expansiones
  for (unsigned i=0; i<globbuf.gl_pathc; i++ ) {
    // matcheo la expresion regular
    if( regexec( &match, globbuf.gl_pathv[i], 0, NULL, 0) == 0 ) {
      // si matchea cargo la imagen en temp y la meto al final de la lista
      lista_imagenes.push_back( CImg<T>( globbuf.gl_pathv[i] ) );
    }
  }

  // libero los cosos auxiliares de glob y regex
  regfree(&match);
  globfree(&globbuf);

  // devuelvo la base generada a partir del promedio de las imagenes
  return promedio( lista_imagenes );
}

/**
 * generar_base_lista
 * genera una lista de imagenes a partir del directorio pasado como param.
 * @param directorio directorio donde buscar las imágenes
 * @return lista de imágenes cargada del directorio 
 */
template<class T>
CImgList<T> generar_base( const char* directorio ) {
  CImgList<T> lista_imagenes;
  // para entender esto del globbing y las regexps ver la otra generar_base
  glob_t globbuf;

  regex_t match;

  regcomp( &match, ".*\\.\\(jpe\\?g\\|bmp\\tif{1,2}\\|png\\|gif\\)$", REG_ICASE );

  // expando directorio/*
  glob( std::string( std::string(directorio)+std::string("/*")).c_str(),
	GLOB_MARK, NULL, &globbuf );

  for (unsigned i=0; i<globbuf.gl_pathc; i++ ) {
    if( regexec( &match, globbuf.gl_pathv[i], 0, NULL, 0) == 0 ) {
      lista_imagenes.push_back( CImg<T>( globbuf.gl_pathv[i] ) );
    }
  }

  regfree(&match);
  globfree(&globbuf);

  // devuelvo la lista con las imagenes cargadas
  return  lista_imagenes;
}

/**
 * sumar_dim
 * agarra una imagen y la suma a lo largo de una dimensión, i.e. si sumo
 * a lo largo de x entonces obtendré un vector en y del mismo alto que la imagen
 * original, donde cada punto en y contendrá el valor de sum(0:ancho,y)
 * @param imagen la imagen sobre la que calcular la suma
 * @param dimension la dimensión a sumar, por defecto x, valores posibles: 'xXyY'
 * @return vector de la suma, con una dimensión menos que la imagen original.
 * @TODO generalizar para sumar a lo largo de las dimensiones z y c.
 */
template<class T>
CImg<T> sumar_dim ( const CImg<T> imagen, char dimension='x' ) {
  CImg<T> vector_salida;
  // si dimension es x, recorro para cada Y y sumo todo X en cada Y
  if ( dimension== 'x' | dimension=='X' ) {
    vector_salida.assign( 1, imagen.height(), 1, 1, 0 );
    cimg_forY( imagen, y ) {
      vector_salida(0,y) = imagen.get_crop(0,y,imagen.width()-1,y).sum();
    }
  }
  // viceversa si dimensión es X
  if ( dimension== 'y' | dimension=='Y' ) {
    vector_salida.assign( imagen.width(), 1, 1, 1, 0 );
    cimg_forX( imagen, x ) {
      vector_salida(x,0) = imagen.get_crop(x,0,x,imagen.height()-1).sum();
    }
  }
  return vector_salida;
}

/**
 * estadisticas_imagen
 * calcula una serie de estadísticas sobre la imagen pasada como parámetro, a saber
 * media y stddev sobre la imagen entera, media y stddev sobre las sumas horizontales
 * y verticales, media y stddev sobre el histograma de la imagen entera, y sobre las
 * sumas horiz y vert.
 * @param imagen la imagen sobre la que se calcularán estas medidas
 * @return un vector horizontal CImg<double> con los valores en el orden que se
 *         mencionan ariba.
 */
template<class T>
CImg<double> estadisticas_imagen ( const CImg<T> imagen ) {
  CImg<double> resultados ( 12,1,1,1,0);

  // calculo las sumas h y v
  CImg<T> sum_h = sumar_dim<T> ( imagen, 'y' ), sum_v = sumar_dim<T> ( imagen, 'x' );

  // calculo los histogramas de la imagen entera y las sumas
  CImg<double> histo = imagen.get_normalize(0,255).get_histogram(256,0,255),
    histo_h = sum_h.normalize(0,255).get_histogram(256,0,255),
    histo_v = sum_v.normalize(0,255).get_histogram(256,0,255);

  // relleno el vector con resultados. uso sqrt(var) = stddev porque es más
  // parecido en magnitud a las medias, y mostrando el vector se "ve mejor"
  resultados(0) = imagen.get_normalize(0,255).mean();
  resultados(1) = sqrt( imagen.get_normalize(0,255).variance() );
  resultados(2) = sum_h.mean();
  resultados(3) = sqrt( sum_h.variance() );
  resultados(4) = sum_v.mean();
  resultados(5) = sqrt( sum_v.variance() );
  resultados(6) = histo.mean();
  resultados(7) = sqrt( histo.variance() );
  resultados(8) = histo_h.mean();
  resultados(9) = sqrt( histo_h.variance() );
  resultados(10) = histo_v.mean();
  resultados(11) = sqrt( histo_v.variance() );

  return resultados;
}

/**
 * error_estadisticas_imagen
 * calcula el MSE entre los vectores de estadísticas de dos imágenes.
 * @param img1, img2 las imágenes a comparar
 * @return MSE (double) entre los vectores de estadísticas de las imágenes.
 */
template<class T>
double error_estadisticas_imagen ( const CImg<T> img1, const CImg<T> img2,
				   double normalize_min=0, double normalize_max=255 ) {
  CImg<double>eimg1 = estadisticas_imagen<double>(img1);
  CImg<double>eimg2 = estadisticas_imagen<double>(img2);
  return eimg1.normalize(normalize_min,normalize_max).MSE(
	       eimg2.normalize(normalize_min,normalize_max) );
}

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
CImg<unsigned int> perfil_blancos( CImgList<T> imagenes ) {
    CImg<int> perfil(imagenes.size(),1,1,1,0);

    for (int i = 0; i < imagenes.size(); i++ ) {
        perfil(i) = imagenes(i).sum();
    }
    return perfil;
}


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
CImg<T> hough_directa( CImg<T> img ) {

    CImg<double> iHough(img);

    // genera espacio parametrico mediante valores equiespaciados 
    // de p y rho inicializandolo en cero(acumulador)
    iHough.fill(0.0); 
    const unsigned M = img.width(), N = img.height();
    int r; // radio mapeado en los N pixeles

    // maximo valor posible de radio se da en la diagonal pcipal
    double max_rho = sqrt(float(pow(N, 2) + pow(M, 2))), 
        step_rho = 2. * max_rho / (N - 1), //paso en eje rho (rho=[-max_rho , max_rho])
        step_theta = M_PI / (M - 1), //paso en eje theta (M_PI=pi) (theta=[-90,90])
        rho, theta;
    
    cimg_forXY(img,x,y) {
        if ( img(x, y) > 0.5 )
            //calculo rho variando theta en _todo el eje, con x e y fijo
            for (int t = 0; t < M; t++) { 
                theta = t * step_theta - M_PI / 2; // mapea [0,M] en [-90,90]
                rho = x * cos(theta) + y * sin(theta); // calcula rho
                r = int((rho + max_rho) / step_rho); // mapea [-max_rho , max_rho] en [0,N]
                iHough(t, r) += 1; // suma el acumulador
            }
    }
    return iHough;
}

/**
 * Transformada Hough inversa
 *
 * tomada de funciones de la catedra
 */
template<class T>
CImg<T> hough_inversa( CImg<T> img ) {

    const double blanco[1] = { 255.f };
    CImg<double> iHoughI(img);
    iHoughI.fill(0.0);
    int M = img.width(), 
        N = img.height(), 
        y0, y1;

    //maximo valor posible de radio se da en la diagonal pcipal
    double max_rho = sqrt(float(pow(N, 2) + pow(M, 2))),
        step_rho = 2. * max_rho / (N - 1), //paso en eje rho (rho=[-max_rho , max_rho])
        step_theta = M_PI / (M - 1), //paso en eje theta (M_PI=pi) (theta=[-90,90])
        rho, theta;

    cimg_forXY(img,t,r) {
        if ( img(t, r) > 0.5 ) {
            theta = t * step_theta - M_PI / 2; // mapea [0,M] en [-90,90]
            rho = r * step_rho - max_rho; // mapea [0,N] en [-max_rho,max_rho]
            y0 = int(rho / sin(theta));
            y1 = int(rho / sin(theta) - M / tan(theta));
            iHoughI.draw_line(0, (int) y0, (int) M, (int) y1, blanco);
        }
    }
    return iHoughI;
}

/**
 * solo maximos de la t hough
 * FIXME: para valores menores que uno no va detectarlos
 * o va detecta el mismo q esta usando para marcar..
 * se podria usar valores negativos???
 */
template <class T>
CImg<T> get_solo_maximos( CImg<T> img, int cantidad=1 ) {

    CImg<T> maximos(cantidad,1,1,1,0);
    CImg<T> aux(img);
    int cont = 0;
    for ( int i = 0; i < cantidad; i++ ) {
        maximos(cont++) = img.max();
        img.max() = 1;
    }
    cimg_forXY(img,x,y){
        if ( img(x,y) == aux(x,y) ) img(x,y) = 0;
    }
    return img;
}
