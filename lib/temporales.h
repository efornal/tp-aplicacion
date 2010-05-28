
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

CImgList<double> lista_con_img_tren_frente_y_un_canal(){
    CImgList<double> lista;
    CImg<double>trenfrente01("../imagenes/trenfrente/tren_frente01.jpg"),
        trenfrente02("../imagenes/trenfrente/tren_frente02.jpg"),
        trenfrente03("../imagenes/trenfrente/tren_frente03.jpg"),
        trenfrente04("../imagenes/trenfrente/tren_frente04.jpg"),
        trenfrente05("../imagenes/trenfrente/tren_frente05.jpg"),
        trenfrente06("../imagenes/trenfrente/tren_frente06.jpg"),
        trenfrente07("../imagenes/trenfrente/tren_frente07.jpg"),
        trenfrente08("../imagenes/trenfrente/tren_frente08.jpg"),
        trenfrente09("../imagenes/trenfrente/tren_frente09.jpg");
    lista.push_back(trenfrente01.channel(0));
    lista.push_back(trenfrente02.channel(0));
    lista.push_back(trenfrente03.channel(0));
    lista.push_back(trenfrente04.channel(0));
    lista.push_back(trenfrente05.channel(0));
    lista.push_back(trenfrente06.channel(0));
    lista.push_back(trenfrente07.channel(0));
    lista.push_back(trenfrente08.channel(0));
    lista.push_back(trenfrente09.channel(0));
    return lista;
}


CImgList<double> lista_con_img_tren_perspectiva_y_un_canal(){
    CImgList<double> lista;
    CImg<double>tren_pers01("../imagenes/tren_perspe/tren_pers01.jpg"),
        tren_pers02("../imagenes/tren_perspe/tren_pers02.jpg"),
        tren_pers03("../imagenes/tren_perspe/tren_pers03.jpg"),
        tren_pers04("../imagenes/tren_perspe/tren_pers04.jpg"),
        tren_pers05("../imagenes/tren_perspe/tren_pers05.jpg"),
        tren_pers06("../imagenes/tren_perspe/tren_pers06.jpg"),
        tren_pers07("../imagenes/tren_perspe/tren_pers07.jpg"),
        tren_pers08("../imagenes/tren_perspe/tren_pers08.jpg"),
        tren_pers09("../imagenes/tren_perspe/tren_pers09.jpg");
    lista.push_back(tren_pers01.channel(0));
    lista.push_back(tren_pers02.channel(0));
    lista.push_back(tren_pers03.channel(0));
    lista.push_back(tren_pers04.channel(0));
    lista.push_back(tren_pers05.channel(0));
    lista.push_back(tren_pers06.channel(0));
    lista.push_back(tren_pers07.channel(0));
    lista.push_back(tren_pers08.channel(0));
    lista.push_back(tren_pers09.channel(0));
    return lista;
}
