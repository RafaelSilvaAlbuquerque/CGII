#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <QLabel>
#include <vect.h>


#include <sstream>
#include <QFileInfo>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <limits>
#include <GL/gl.h>
#include <QFileInfo>

#define FRUSTUM_SIZE 50

Display::Display(QWidget *parent) :
    QGLWidget(parent)
{
}

QImage Display::draw(QImage image,int li,int co){

    QRgb value;

    float resolucao_tela_linhas = li;
    float resolucao_tela_colunas = co;

    float resolucao_imagen_linhas = li;
    float resolucao_imagen_colunas = co;

    int var = resolucao_imagen_linhas*resolucao_imagen_colunas;

    cam.ampixel(resolucao_tela_linhas,resolucao_tela_colunas,resolucao_imagen_linhas,resolucao_imagen_colunas);
    cam.map(resolucao_tela_linhas,resolucao_tela_colunas,resolucao_imagen_linhas,resolucao_imagen_colunas);

    //cam.coordcamera(0.0,0.0,0.0);//0.0,0.0,1.0

    /*
    vect at,up,eye;
    at.set(0.0,0.0,0.0,1.0);
    eye.set(0.0,4.0,5.0,1.0);
    up.set(0.0,2.0,5.0,1.0);
    */


    //vect at,up,eye;
    //at.set(0.0,0.0,0.0,1.0);
    //eye.set(0.0,5.0,-5.0,1.0);
    //up.set(0.0,6.0,-5.0,1.0);



    vect at,up,eye;
    at.set(0.0,0.0,0.0,1.0);
    eye.set(0.0,0.0,7.0,1.0);
    up.set(0.0,0.0,1.0,1.0);
    //cam.sistemaCamera(at,up,eye);
    //cam.hortoProjection(false);
    //cam.perspectiva(5.0,5.0,0.01,120000.0);
    //cam.frustum(-50,50,-50,50,-50,50);

//------------------------------------------------------------------------------------------------------------------------------------
    //define iluminação

    light fonte;
    fonte.state(false);
    fonte.coord(-2.0,2.0,3.0);                          //-0.2,0.2,1.0
    fonte.comp_ambiente(1.0,1.0,1.0);
    fonte.comp_difusa(1.0,1.0,1.0);
    fonte.comp_specular(1.0,1.0,1.0);

    light fonte2;
    fonte2.state(false);
    fonte2.coord(2.0,5.0,0.0);                         //0.2,0.5,0.0
    fonte2.comp_ambiente(1.0,1.0,1.0);
    fonte2.comp_difusa(1.0,1.0,1.0);
    fonte2.comp_specular(1.0,1.0,1.0);

    cam.addlight(fonte);
    cam.addlight(fonte2);


    cam.atenuacao(false);
    cam.enableshadow(true);
//-----------------------------------------------------------------------------------------------------------------------------------
//define objetos

    _matrix.resetmatrix();
    _matrix.translate(0.2,-0.2,-0.5);
    objeto esfera;
    esfera.tipo("esfera");
    esfera.setraio(0.02);                               //esfera.raio = 0.02;
    esfera.refracao(true);
    esfera.reflexao(false);
    esfera.indrefracao(20.0);                           //20.8//esfera.centro(0.2,-0.2,-0.5);//0.2,-0.2,-0.5
    esfera.comp_ambiente(0.01,0.01,0.01);               //0.05,0.06,0.01   0.04,0.04,0.04
    esfera.comp_difusa(0.03,0.03,0.03);                 //0.5,0.6,0.1    0.08,0.08,0.08
    esfera.comp_specular(1.0,1.0,1.0,1000.0);           //1.0,1.0,1.0,100.0    1.0,1.0,1.0,1000
    esfera.setmatrix(_matrix.getmatrix());
    esfera.setinvmatrix(_matrix.getinvmatrix());
                                                        //_matrix.translate(0.2,0.0,0.0);//_matrix.rotate_xaxis(80.0);//_matrix.escala(1.0,1.0,1.0);
    _matrix.resetmatrix();
    //_matrix.escala(1.0,2.0,1.0);
    //_matrix.rotate_xaxis(45.0);
    _matrix.translate(0.1,0.3,-0.5);//0.1,0.3,-0.5
    objeto esfera2;                                     //esfera2.centro(0.0,0.0,-1.0);
    esfera2.tipo("esfera");
    //esfera2.centro(0.1,0.3,-0.5);
    esfera2.setraio(0.02);                              //esfera2.raio = 0.02;//esfera2.centro(0.2,0.0,0.0);//0.1,0.3,-0.5
    esfera2.comp_ambiente(0.05,0.06,0.01);              //0.03,0.02,0.01  0.05,0.06,0.01
    esfera2.comp_difusa(0.5,0.6,0.1);                   //0.3,0.2,0.1
    esfera2.comp_specular(1.0,1.0,1.0,100.0);           //1.0,1.0,1.0,100
    esfera2.setmatrix(_matrix.getmatrix());
    esfera2.setinvmatrix(_matrix.getinvmatrix());


    //_matrix.resetmatrix();

    /*
    objeto esfera2;
    esfera2.tipo("esfera");
    //esfera2.setraio(0.02);
    esfera2.comp_ambiente(0.05,0.06,0.01);
    esfera2.comp_difusa(0.5,0.6,0.1);
    esfera2.comp_specular(1.0,1.0,1.0,100.0);
    //esfera2.translate(0.5,0.0,0.0);
    //esfera2.scale(1.0,2.0,1.0);
    //esfera2.rotate_Z(90.0);
    //esfera2.rotate_X(90.0);
    //esfera2.rotate_Y(90.0);
    */


    objeto plano;
    plano.tipo("plano");
    plano.reflexao(true);
    plano.vet_normal(0.0,0.0,1.0);                      //0.0,0.0,1.0
    plano.pontop(0.0,0.0,-1.0);
    plano.comp_ambiente(0.04,0.04,0.04);                //0.05,0.05,0.05    0.04,0.04,0.04
    plano.comp_difusa(0.0,0.0,0.0);                  //0.7,0.7,0.7         0.08,0.08,0.08
    plano.comp_specular(1.0,1.0,1.0,1000.0);            //0.0,0.0,0.0
    plano.ponto_se(-0.5,0.5,-1.0);
    plano.ponto_id(0.5,-0.5,-1.0);

    objeto plano2;
    plano2.tipo("plano");
    plano2.vet_normal(-1.0,0.0,0.0);
    plano2.pontop(0.5,0.0,0.0);
    plano2.comp_ambiente(0.05,0.01,0.01);               //0.1,0.02,0.02
    plano2.comp_difusa(0.3,0.1,0.1);                    //0.3,0.1,0.1
    plano2.comp_specular(0.0,0.0,0.0,5.0);              //1.0,1.0,1.0,50
    plano2.ponto_se(0.5,0.5,-1.0);
    plano2.ponto_id(0.5,-0.5,1.0);

    objeto plano3;
    plano3.tipo("plano");
    plano3.vet_normal(0.0,1.0,0.0);
    plano3.pontop(0.0,-0.5,0.0);
    plano3.comp_ambiente(0.01,0.01,0.05);               //0.02,0.02,0.1
    plano3.comp_difusa(0.1,0.1,0.3);                    //0.1,0.1,0.3
    plano3.comp_specular(0.0,0.0,0.0,5.0);              //0.1,0.1,0.3
    plano3.ponto_se(-0.5,-0.5,-1.0);
    plano3.ponto_id(0.5,-0.5,1.0);

    objeto plano4;
    plano4.tipo("plano");
    plano4.vet_normal(1.0,0.0,0.0);                     //1.0,0.0,0.0
    plano4.pontop(-0.5,0.0,0.0);                        //-0.5,0.0,0.0
    plano4.comp_ambiente(0.01,0.05,0.01);               //0.02,0.1,0.02
    plano4.comp_difusa(0.1,0.3,0.1);                    //0.1,0.3,0.1
    plano4.comp_specular(0.0,0.0,0.0,5.0);              //0.1,0.3,0.1
    plano4.ponto_se(-0.5,0.5,-1.0);                     //-0.5,0.5,-1.0
    plano4.ponto_id(-0.5,-0.5,1.0);                     //-0.5,-0.5,1.0


    _matrix.resetmatrix();
    _matrix.translate(-0.2,-0.5,-0.5);                   //_matrix.escala(0.01,0.01,0.01);
    objeto cilindro;
    cilindro.tipo("cilindro");
    cilindro.raio = 0.01;
    cilindro.altura = 0.7;
    cilindro.indrefracao(20.0);
    cilindro.refracao(false);
    cilindro.reflexao(false);                           //cilindro.centro(-0.2,0.0,-0.5);//-0.2,0.0,-0.5
    cilindro.comp_ambiente(0.1,0.08,0.08);              //0.1,0.08,0.08
    cilindro.comp_difusa(0.5,0.4,0.4);
    cilindro.comp_specular(1.0,1.0,1.0,1000.0);
    cilindro.setmatrix(_matrix.getmatrix());
    cilindro.setinvmatrix(_matrix.getinvmatrix());

/*
    _matrix.resetmatrix();
    //_matrix.escala(1.0,2.0,1.0);
    _matrix.translate(0.0,-0.4,-0.2);                   //_matrix.rotate_xaxis(90.0);//_matrix.translate(0.0,-0.4,-0.2);//_matrix.escala(1.0,0.1,1.0);
    objeto cubo;
    cubo.tipo("cubo");                                  //cubo.centro(0.0,-0.4,-0.2);
    cubo.reflexao(false);
    cubo.dimencao = 0.2;                                //cubo.centro(0.0,-0.4,-0.2);   //0.0,-0.4,-0.2
    cubo.comp_ambiente(0.01,0.0027,0.0);                //0.01,0.0027,0.0               //0.1,0.0027,0.0    //0.01,0.0027,0.0
    cubo.comp_difusa(0.7,0.27,0.0);                     //0.7,0.27,0.0                  //1.0,0.27,0.0
    cubo.comp_specular(0.0,0.0,0.0,100.0);              //1.0,1.0,1.0,100.0
    cubo.setmatrix(_matrix.getmatrix());
    cubo.setinvmatrix(_matrix.getinvmatrix());
*/

    objeto cubo;
    cubo.tipo("cubo");
    //cubo.dimencao = 0.5;
    cubo.comp_ambiente(0.1,0.0027,0.0);
    cubo.comp_difusa(0.7,0.27,0.0);
    cubo.comp_specular(0.0,0.0,0.0,100.0);
    //cubo.translate(0.5,0.0,0.0);
    //cubo.scale(1.0,0.2,1.0);
    //cubo.rotate_X(45.0);
    //cubo.rotate_Y(45.0);
    //cubo.rotate_Z(45.0);

    //cubo._translate.printMatrix();
    //cubo.rotateX.printMatrix();
    //cubo.rotateY.printMatrix();
    //cubo.rotateZ.printMatrix();
    //cubo._escale.printMatrix();



    //cam.addObj(plano2);
    //cam.addObj(plano3);
    //cam.addObj(plano4);
    //cam.addObj(cilindro);
    //cam.addObj(esfera);
    //cam.addObj(esfera2);
    //cam.addObj(plano);
    //cam.addObj(cubo);

    QString nome;
    nome.append("teste");

    cam.loadSceneXML(nome);//--------------------------------

/*
    _matrix.resetmatrix();
    _matrix.translate(0.0,-0.2,0.5);
    _matrix.rotate_xaxis(90.0);
    _matrix.escala(0.7,0.01,0.7);
    objeto parede0;
    parede0.tipo("cubo");
    parede0.reflexao(false);
    parede0.dimencao = 1.0;
    parede0.comp_ambiente(0.04,0.04,0.04);
    parede0.comp_difusa(0.07,0.07,0.07);
    parede0.comp_specular(0.0,0.0,0.0,5.0);
    parede0.setmatrix(_matrix.getmatrix());
    parede0.setinvmatrix(_matrix.getinvmatrix());



    _matrix.resetmatrix();
    _matrix.translate(0.0,-0.5,0.0);
    _matrix.escala(0.7,0.01,0.7);
    objeto parede1;
    parede1.tipo("cubo");
    parede1.reflexao(false);
    parede1.dimencao = 1.0;
    parede1.comp_ambiente(0.01,0.01,0.05);
    parede1.comp_difusa(0.1,0.1,0.3);
    parede1.comp_specular(0.0,0.0,0.0,5.0);
    parede1.setmatrix(_matrix.getmatrix());
    parede1.setinvmatrix(_matrix.getinvmatrix());
*/

    //cam.addObj(parede0);
    //cam.addObj(parede1);


//----------------------------------------------------------------------------------------------------------------------------------



    cam.intersect();


    int r,g,b;
    for(int i=1;i<=var;i++){

        r = (int)(cam.film[i-1].cor[0]*255);
        g = (int)(cam.film[i-1].cor[1]*255);
        b = (int)(cam.film[i-1].cor[2]*255);

        if(r > 255){
            r = 255;
        }
        if(g > 255){
            g = 255;
        }
        if(b > 255){
            b = 255;
        }

        value = qRgb(r,g,b);
        image.setPixel(cam.film[i-1].x_tela,cam.film[i-1].y_tela,value);
    }

    free(cam.film);

    image.save("teste.png",0,100);
    return image;
}

