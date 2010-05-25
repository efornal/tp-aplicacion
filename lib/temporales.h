
CImgList<double> lista_con_img_chaco(){
    CImgList<double> lista;
    CImg<double> chaco00("../imagenes/chaco/chaco00.jpg"),
        chaco01("../imagenes/chaco/chaco01.jpg"),
        chaco02("../imagenes/chaco/chaco02.jpg"),
        chaco03("../imagenes/chaco/chaco03.jpg"),
        chaco04("../imagenes/chaco/chaco04.jpg"),
        chaco05("../imagenes/chaco/chaco05.jpg"),
        chaco06("../imagenes/chaco/chaco06.jpg"),
        chaco07("../imagenes/chaco/chaco07.jpg"),
        chaco08("../imagenes/chaco/chaco08.jpg"),
        chaco09("../imagenes/chaco/chaco09.jpg");
    lista.push_back(chaco00);
    lista.push_back(chaco01);
    lista.push_back(chaco02);
    lista.push_back(chaco03);
    lista.push_back(chaco04);
    lista.push_back(chaco05);
    lista.push_back(chaco06);
    lista.push_back(chaco07);
    lista.push_back(chaco08);
    lista.push_back(chaco09);
    return lista;
}

CImgList<double> lista_con_img_chaco_y_un_canal(){
    CImgList<double> lista;
    CImg<double> chaco00("../imagenes/chaco/chaco00.jpg"),
        chaco01("../imagenes/chaco/chaco01.jpg"),
        chaco02("../imagenes/chaco/chaco02.jpg"),
        chaco03("../imagenes/chaco/chaco03.jpg"),
        chaco04("../imagenes/chaco/chaco04.jpg"),
        chaco05("../imagenes/chaco/chaco05.jpg"),
        chaco06("../imagenes/chaco/chaco06.jpg"),
        chaco07("../imagenes/chaco/chaco07.jpg"),
        chaco08("../imagenes/chaco/chaco08.jpg"),
        chaco09("../imagenes/chaco/chaco09.jpg");
    lista.push_back(chaco00.channel(0));
    lista.push_back(chaco01.channel(0));
    lista.push_back(chaco02.channel(0));
    lista.push_back(chaco03.channel(0));
    lista.push_back(chaco04.channel(0));
    lista.push_back(chaco05.channel(0));
    lista.push_back(chaco06.channel(0));
    lista.push_back(chaco07.channel(0));
    lista.push_back(chaco08.channel(0));
    lista.push_back(chaco09.channel(0));
    return lista;
}
