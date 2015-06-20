#include <QtXml/QDomDocument> //classes necessarias para criar a estrutura XML
#include <QtXml/QDomElement> //classes necessarias para criar a estrutura XML
#include "objeto.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
#include <sstream>
#include <QFileInfo>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <limits>
#include <GL/gl.h>
#include <QFileInfo>
*/

#include <camera.h>
//------------------------------------------------------------------------------------------------------------





FILE *arquivo;

Objeto::Objeto()
{
}

float Objeto::bola(vect equacao){

    float delta;
    delta = (equacao.y*equacao.y)+((-4.0)*equacao.x*equacao.z);
    return delta;
}


vect Objeto::normal_esfera(vect ponto,vect origem){

    vect norma;

    norma.x = (ponto.x-origem.x);
    norma.y = (ponto.y-origem.y);
    norma.z = (ponto.z-origem.z);
    norma.v = 0.0;

    return norma;
}

vect Objeto::equacao_esfera(vect vetor_dm,vect eye,objeto esfera){

    float x[3];
    float y[3];
    float z[3];

    float a = esfera.x;
    float b = esfera.y;
    float c = esfera.z;
    float raio = esfera.raio;

    vect equacao;

    x[0] = vetor_dm.x*vetor_dm.x;
    x[1] = 2.0*(vetor_dm.x*eye.x);
    x[2] = eye.x*eye.x;

    y[0] = vetor_dm.y*vetor_dm.y;
    y[1] = 2.0*(vetor_dm.y*eye.y);
    y[2] = eye.y*eye.y;

    z[0] = vetor_dm.z*vetor_dm.z;
    z[1] = 2.0*(vetor_dm.z*eye.z);
    z[2] = eye.z*eye.z;

    equacao.x = x[0]+y[0]+z[0];
    equacao.y = x[1]+y[1]+z[1];
    equacao.z = x[2]+y[2]+z[2];

    equacao.y = equacao.y+((-2)*vetor_dm.x*a)+((-2)*vetor_dm.y*b)+((-2)*vetor_dm.z*c);
    equacao.z = equacao.z+((-2)*eye.x*a)+((-2)*eye.y*b)+((-2)*eye.z*c)+((a*a)+(b*b)+(c*c))-raio;

    return equacao;
}

vect Objeto::ponto_esfera(vect vetor_dm,vect eye,vect equacao,objeto esfera){

    float x1;
    float x2;
    float delta;
    vect ponto;

    delta = (equacao.y*equacao.y)+((-4.0)*equacao.x*equacao.z);

    x1 = (((-1.0)*equacao.y)+sqrt(delta))/(2.0*equacao.x);
    x2 = (((-1.0)*equacao.y)-sqrt(delta))/(2.0*equacao.x);

    if(x1 <= x2){
        ponto.x = eye.x+(vetor_dm.x*x1);
        ponto.y = eye.y+(vetor_dm.y*x1);
        ponto.z = eye.z+(vetor_dm.z*x1);
    }
    else if(x2 < x1){
        ponto.x = eye.x+(vetor_dm.x*x2);
        ponto.y = eye.y+(vetor_dm.y*x2);
        ponto.z = eye.z+(vetor_dm.z*x2);
    }

    ponto.v = 1.0;

    return ponto;
}










float Objeto::plano(vect vetor_dm,vect eye,vect normal,vect ponto){

    float t;
    float npont = _algebra.vet_prtescalar(normal,ponto);
    float neye = _algebra.vet_prtescalar(normal,eye);
    float nvet = _algebra.vet_prtescalar(normal,vetor_dm);

    if(nvet != 0){
        t = (npont-neye)/nvet;
    }
    else{
        t = -1;
    }

    return t;
}

float Objeto::distancia_plano(float t,vect normal,vect eye,vect vetor_dm,vect ponto){

    vect veye;
    vect vet;
    float result;

    veye.x = eye.x+(vetor_dm.x*t);
    veye.y = eye.y+(vetor_dm.y*t);
    veye.z = eye.z+(vetor_dm.z*t);

    vect vet_dist = _algebra.vet_dist(veye,ponto);
    vet = _algebra.vet_unit(vet_dist);

    result = _algebra.vet_prtescalar(normal,vet);

    return result;
}


vect Objeto::ponto_plano(float t,vect eye,vect vetor_dm){

    vect ponto;

    ponto.x = eye.x+(vetor_dm.x*t);
    ponto.y = eye.y+(vetor_dm.y*t);
    ponto.z = eye.z+(vetor_dm.z*t);
    ponto.v = 1.0;

    return ponto;
}

bool Objeto::teste_plano(vect coordpl,objeto obj){

    float var = 0.001;

    if((coordpl.z < obj.canto_se.z)&&(coordpl.z < obj.canto_id.z)&&((_algebra.modulo((coordpl.z-obj.canto_se.z)) > var)||(_algebra.modulo((coordpl.z-obj.canto_id.z)) > var))){
        return false;
    }
    if((coordpl.z > obj.canto_se.z)&&(coordpl.z > obj.canto_id.z)&&((_algebra.modulo((coordpl.z-obj.canto_se.z)) > var)||(_algebra.modulo((coordpl.z-obj.canto_id.z)) > var))){
        return false;
    }


    if((coordpl.y < obj.canto_se.y)&&(coordpl.y < obj.canto_id.y)&&((_algebra.modulo((coordpl.y-obj.canto_se.y)) > var)||(_algebra.modulo((coordpl.y-obj.canto_id.y)) > var))){
        return false;
    }
    if((coordpl.y > obj.canto_se.y)&&(coordpl.y > obj.canto_id.y)&&((_algebra.modulo((coordpl.y-obj.canto_se.y)) > var)||(_algebra.modulo((coordpl.y-obj.canto_id.y)) > var))){
        return false;
    }


    if((coordpl.x < obj.canto_se.x)&&(coordpl.x < obj.canto_id.x)&&((_algebra.modulo((coordpl.x-obj.canto_se.x)) > var)||(_algebra.modulo((coordpl.x-obj.canto_id.x)) > var))){
        return false;
    }
    if((coordpl.x > obj.canto_se.x)&&(coordpl.x > obj.canto_id.x)&&((_algebra.modulo((coordpl.x-obj.canto_se.x)) > var)||(_algebra.modulo((coordpl.x-obj.canto_id.x)) > var))){
        return false;
    }

    return true;
}

vect Objeto::equacao_cilindro(vect eye,vect raio,objeto obj){

    vect equacao;

    float x[3];
    float z[3];

    float a = obj.x;
    float b = obj.z;

    x[0] = raio.x*raio.x;
    x[1] = 2*(raio.x*eye.x);
    x[2] = eye.x*eye.x;

    z[0] = raio.z*raio.z;
    z[1] = 2*(raio.z*eye.z);
    z[2] = eye.z*eye.z;

    /*
    y[0] = raio[1]*raio[1];
    y[1] = 2*(raio[1]*eye[1]);
    y[2] = eye[1]*eye[1];
    */

    equacao.x = x[0]+z[0];
    equacao.y = x[1]+z[1];
    equacao.z = x[2]+z[2];

    equacao.y = equacao.y+((-2)*raio.x*a)+((-2)*raio.z*b);
    equacao.z = equacao.z+((-2)*eye.x*a)+((-2)*eye.z*b)+(a*a)+(b*b)-obj.raio;

    return equacao;
}

float Objeto::cilindro_delta(vect equacao){
    float delta;
    delta = (equacao.y*equacao.y)+((-4.0)*equacao.x*equacao.z);
    return delta;
}

vect Objeto::ponto_cilindro(vect eye,vect raio,vect equacao){

    float delta;
    float x1;
    float x2;
    vect ponto;;

    delta = this->cilindro_delta(equacao);

    x1 = (((-1.0)*equacao.y)+sqrt(delta))/(2.0*equacao.x);
    x2 = (((-1.0)*equacao.y)-sqrt(delta))/(2.0*equacao.x);

    if(x1 <= x2){
        ponto.x = eye.x+(raio.x*x1);
        ponto.y = eye.y+(raio.y*x1);
        ponto.z = eye.z+(raio.z*x1);
    }
    else if(x2 < x1){
        ponto.x = eye.x+(raio.x*x2);
        ponto.y = eye.y+(raio.y*x2);
        ponto.z = eye.z+(raio.z*x2);
    }

    ponto.v = 1.0;

    return ponto;

}

vect Objeto::normal_cilindro(vect ponto,vect origem){

    vect vet;

    vet.x = ponto.x-origem.x;
    vet.y = ponto.y-origem.y;
    vet.z = ponto.z-origem.z;

    vet.v = 0.0;

    return vet;
}







dvet Objeto::cubo(vect raio,vect eye,objeto obj){

    float t;
    float tf;
    float distancia;
    float dist;
    float norma_vet;
    bool teste = false;

    vect normal;
    vect pontop;

    float a = obj.x;
    float b = obj.y;
    float c = obj.z;

    int nface=0;

    vect coordpl;
    vect vetor;
    vect ponto;



    //face_1 frente
    objeto face_1;
    face_1.vet_normal(0.0,0.0,1.0);
    face_1.pontop(0.0+a,0.0+b,(obj.dimencao/2.0)+c);
    face_1.ponto_se(((-1.0)*(obj.dimencao/2.0))+a,(obj.dimencao/2.0)+b,(obj.dimencao/2.0)+c);
    face_1.ponto_id((obj.dimencao/2.0)+a,((-1.0)*(obj.dimencao/2.0))+b,(obj.dimencao/2.0+c));

    //face_2 cima
    objeto face_2;
    face_2.vet_normal(0.0,1.0,0.0);
    face_2.pontop(0.0+a,(obj.dimencao/2.0)+b,0.0+c);
    face_2.ponto_se(((-1.0)*(obj.dimencao/2.0))+a,(obj.dimencao/2.0)+b,((-1.0)*(obj.dimencao/2.0))+c);
    face_2.ponto_id((obj.dimencao/2.0)+a,(obj.dimencao/2.0)+b,(obj.dimencao/2.0)+c);

    //face_3 direita
    objeto face_3;
    face_3.vet_normal(1.0,0.0,0.0);
    face_3.pontop((obj.dimencao/2.0)+a,0.0+b,0.0+c);
    face_3.ponto_se((obj.dimencao/2.0)+a,(obj.dimencao/2.0)+b,((-1.0)*(obj.dimencao/2.0))+c);
    face_3.ponto_id((obj.dimencao/2.0)+a,((-1.0)*(obj.dimencao/2.0))+b,(obj.dimencao/2.0)+c);

    //face_4 esquerda
    objeto face_4;
    face_4.vet_normal(-1.0,0.0,0.0);
    face_4.pontop(((-1.0)*(obj.dimencao/2.0))+a,0.0+b,0.0+c);
    face_4.ponto_se((((-1.0)*(obj.dimencao/2.0))+a),((obj.dimencao/2.0)+b),(obj.dimencao/2.0)+c);
    face_4.ponto_id(((-1.0)*(obj.dimencao/2.0))+a,((-1.0)*(obj.dimencao/2.0))+b,((-1.0)*(obj.dimencao/2.0))+c);

    //face_5 atras
    objeto face_5;
    face_5.vet_normal(0.0,0.0,-1.0);
    face_5.pontop(0.0+a,0.0+b,((-1.0)*(obj.dimencao/2.0))+c);
    face_5.ponto_se(((-1.0)*(obj.dimencao/2.0))+a,(obj.dimencao/2.0)+b,((-1.0)*(obj.dimencao/2.0))+c);
    face_5.ponto_id((obj.dimencao/2.0)+a,((-1.0)*(obj.dimencao/2.0))+b,((-1.0)*(obj.dimencao/2.0))+c);

    //face_6 baixo
    objeto face_6;
    face_6.vet_normal(0.0,-1.0,0.0);
    face_6.pontop(0.0+a,((-1.0)*(obj.dimencao/2.0))+b,0.0+c);
    face_6.ponto_se(((-1.0)*(obj.dimencao/2.0))+a,((-1.0)*(obj.dimencao/2.0))+b,((-1.0)*(obj.dimencao/2.0))+c);
    face_6.ponto_id((obj.dimencao/2.0)+a,((-1.0)*(obj.dimencao/2.0))+b,(obj.dimencao/2.0)+c);


    //if(nface < 3){
        //teste face_1
        t = this->plano(raio,eye,face_1.normal,face_1.ponto);
        distancia = this->distancia_plano(t,face_1.normal,eye,raio,face_1.ponto);

        if(distancia < 0.000000){
            distancia = 0.0;
        }

        if(distancia >= 0.0){
            coordpl = this->ponto_plano(t,eye,raio);
            if(this->teste_plano(coordpl,face_1)){
                nface++;
                vetor = _algebra.vet_dist(coordpl,eye);
                norma_vet = _algebra.vet_norma(vetor);
                if(teste == false){
                    teste = true;
                    ponto = coordpl;
                    dist = norma_vet;

                    pontop = face_1.ponto;
                    pontop.v = 1.0;

                    normal.x = 0.0;
                    normal.y = 0.0;
                    normal.z = 1.0;
                    normal.v = 0.0;

                    tf = t;
                }
                else{
                    if(norma_vet < dist){
                        ponto = coordpl;
                        dist = norma_vet;

                        pontop = face_1.ponto;
                        pontop.v = 1.0;

                        normal.x = 0.0;
                        normal.y = 0.0;
                        normal.z = 1.0;
                        normal.v = 0.0;

                        tf = t;
                    }
                }
            }
        }
    //}


    //if(nface < 3){
        //teste face_2
        t = this->plano(raio,eye,face_2.normal,face_2.ponto);
        distancia = this->distancia_plano(t,face_2.normal,eye,raio,face_2.ponto);
        if(distancia < 0.000000){
            distancia = 0.0;
        }
        if(distancia >= 0){
            coordpl = this->ponto_plano(t,eye,raio);
            if(this->teste_plano(coordpl,face_2)){
                nface++;
                vetor = _algebra.vet_dist(coordpl,eye);
                norma_vet = _algebra.vet_norma(vetor);
                if(teste == false){
                    teste = true;
                    ponto = coordpl;
                    dist = norma_vet;

                    pontop = face_2.ponto;
                    pontop.v = 1.0;

                    normal.x = 0.0;
                    normal.y = 1.0;
                    normal.z = 0.0;
                    normal.v = 0.0;

                    tf = t;
                }
                else{
                    if(norma_vet < dist){
                        ponto = coordpl;
                        dist = norma_vet;

                        pontop = face_2.ponto;
                        pontop.v = 1.0;

                        normal.x = 0.0;
                        normal.y = 1.0;
                        normal.z = 0.0;
                        normal.v = 0.0;

                        tf = t;
                    }
                }
            }
        }
    //}


    //if(nface < 3){
        //teste face_3
        t = this->plano(raio,eye,face_3.normal,face_3.ponto);
        distancia = this->distancia_plano(t,face_3.normal,eye,raio,face_3.ponto);
        if(distancia < 0.000000){
            distancia = 0.0;
        }
        if(distancia >= 0){
            coordpl = this->ponto_plano(t,eye,raio);
            if(this->teste_plano(coordpl,face_3)){
                nface++;
                vetor = _algebra.vet_dist(coordpl,eye);
                norma_vet = _algebra.vet_norma(vetor);
                if(teste == false){
                    teste = true;
                    ponto = coordpl;
                    dist = norma_vet;

                    pontop = face_3.ponto;
                    pontop.v = 1.0;

                    normal.x = 1.0;
                    normal.y = 0.0;
                    normal.z = 0.0;
                    normal.v = 0.0;

                    tf = t;
                }
                else{
                    if(norma_vet < dist){
                        ponto = coordpl;
                        dist = norma_vet;

                        pontop = face_3.ponto;
                        pontop.v = 1.0;

                        normal.x = 1.0;
                        normal.y = 0.0;
                        normal.z = 0.0;
                        normal.v = 0.0;

                        tf = t;
                    }
                }
            }
        }
    //}


    //if(nface < 3){
        //teste face_4
        t = this->plano(raio,eye,face_4.normal,face_4.ponto);
        distancia = this->distancia_plano(t,face_4.normal,eye,raio,face_4.ponto);
        if(distancia < 0.000000){
            distancia = 0.0;
        }
        if(distancia >= 0){
            coordpl = this->ponto_plano(t,eye,raio);
            if(this->teste_plano(coordpl,face_4)){
                nface++;
                vetor = _algebra.vet_dist(coordpl,eye);
                norma_vet = _algebra.vet_norma(vetor);
                if(teste == false){
                    teste = true;
                    ponto = coordpl;
                    dist = norma_vet;

                    pontop = face_4.ponto;
                    pontop.v = 1.0;

                    normal.x = -1.0;
                    normal.y = 0.0;
                    normal.z = 0.0;
                    normal.v = 0.0;

                    tf = t;
                }
                else{
                    if(norma_vet < dist){
                        ponto = coordpl;
                        dist = norma_vet;

                        pontop = face_4.ponto;
                        pontop.v = 1.0;

                        normal.x = -1.0;
                        normal.y = 0.0;
                        normal.z = 0.0;
                        normal.v = 0.0;

                        tf = t;
                    }
                }
            }
        }
    //}


    //if(nface < 3){
        //teste face_5
        t = this->plano(raio,eye,face_5.normal,face_5.ponto);
        distancia = this->distancia_plano(t,face_5.normal,eye,raio,face_5.ponto);
        if(distancia < 0.000000){
            distancia = 0.0;
        }
        if(distancia >= 0){
            coordpl = this->ponto_plano(t,eye,raio);
            if(this->teste_plano(coordpl,face_5)){
                nface++;
                vetor = _algebra.vet_dist(coordpl,eye);
                norma_vet = _algebra.vet_norma(vetor);
                if(teste == false){
                    teste = true;
                    ponto = coordpl;
                    dist = norma_vet;

                    pontop = face_5.ponto;
                    pontop.v = 1.0;

                    normal.x = 0.0;
                    normal.y = 0.0;
                    normal.z = -1.0;
                    normal.v = 0.0;

                    tf = t;
                }
                else{
                    if(norma_vet < dist){
                        ponto = coordpl;
                        dist = norma_vet;

                        pontop = face_5.ponto;
                        pontop.v = 1.0;

                        normal.x = 0.0;
                        normal.y = 0.0;
                        normal.z = -1.0;
                        normal.v = 0.0;

                        tf = t;
                    }
                }
            }
        }
    //}


    //if(nface < 3){
        //teste face_6
        t = this->plano(raio,eye,face_6.normal,face_6.ponto);
        distancia = this->distancia_plano(t,face_6.normal,eye,raio,face_6.ponto);
        if(distancia < 0.000000){
            distancia = 0.0;
        }
        if(distancia >= 0){
            coordpl = this->ponto_plano(t,eye,raio);
            if(this->teste_plano(coordpl,face_6)){
                nface++;
                vetor = _algebra.vet_dist(coordpl,eye);
                norma_vet = _algebra.vet_norma(vetor);
                if(teste == false){
                    teste = true;
                    ponto = coordpl;
                    dist = norma_vet;

                    pontop = face_6.ponto;
                    pontop.v = 1.0;


                    normal.x = 0.0;
                    normal.y = -1.0;
                    normal.z = 0.0;
                    normal.v = 0.0;

                    tf = t;
                }
                else{
                    if(norma_vet < dist){
                        ponto = coordpl;
                        dist = norma_vet;

                        pontop = face_6.ponto;
                        pontop.v = 1.0;


                        normal.x = 0.0;
                        normal.y = -1.0;
                        normal.z = 0.0;
                        normal.v = 0.0;

                        tf = t;
                    }
                }
            }
        }
    //}


    if(teste == true){
        dvet convet;
        convet.val = true;
        convet.ponto = ponto;
        convet.normal = normal;
        convet.pontop = pontop;
        convet.t = tf;

        return convet;
    }
    else{
        dvet convet;
        convet.val = false;

        return convet;
    }
}

/*
bool Objeto::loadSceneXML(QString fileName){

    QDomDocument doc( "SceneRayTracing" );
    //QFile file( fileName.toStdString().data() );

    QFile file("teste.xml");

    if( !file.open(QIODevice::ReadOnly) ){
        return false;
    }

    if( !doc.setContent( &file ) ){
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();

    if( root.tagName() != "Scene" ){
        return false;
    }

    qDebug() << "Begin!";
    QDomNode n = root.firstChild();



    while( !n.isNull() ){

        vect vec;
        QDomElement e = n.toElement();

        if( !e.isNull() ){

            if(e.tagName()=="Objects"){

                QDomNode  objs = e.firstChildElement("Object");

                while (!objs.isNull()){

                    QDomNode elemento = objs;
                    QDomElement obj = elemento.toElement();
                    QString type = obj.attribute("Type","");
                    QString name = obj.attribute("Name","");
                    bool enabled  = obj.attribute("Enabled","").toInt();
                    qDebug() << name;

                    objeto object;
                    if (type=="OBJSPHERE"){
                        object.tipo("esfera");
                    }
                    else{
                        break;
                    }

                    //posição
                    QDomElement pos = obj.firstChildElement("Position").toElement();
                    vec.x = pos.attribute("x","").toFloat();
                    vec.y = pos.attribute("y","").toFloat();
                    vec.z = pos.attribute("z","").toFloat();
                    vect position = vec;

                    //rotação
                    QDomElement rot = obj.firstChildElement("Rotation").toElement();
                    vec.x = rot.attribute("x","").toFloat();
                    vec.y = rot.attribute("y","").toFloat();
                    vec.z = rot.attribute("z","").toFloat();
                    vect rotation = vec;

                    //escala
                    QDomElement scale = obj.firstChildElement("Scale").toElement();
                    vec.x = scale.attribute("x","").toFloat();
                    vec.y = scale.attribute("y","").toFloat();
                    vec.z = scale.attribute("z","").toFloat();
                    vect sca = vec;

                    //define matriz de transformacao
                    Matrix4x4 transform;
                    transform.resetmatrix();
                    transform.escala(sca.x,sca.y,sca.z);
                    //transform.rotate_zaxis(rotation.z);
                    //transform.rotate_yaxis(rotation.y);
                    transform.rotate_xaxis(rotation.x);
                    transform.translate(position.x,position.y,position.z);
                    object.setmatrix(_matrix.getmatrix());
                    object.setinvmatrix(_matrix.getinvmatrix());

                    //material
                    QDomElement material = obj.firstChildElement("Material").toElement();
                    float refle,refra,grefra,grefle,shine;
                    refle = material.attribute("Reflection","").toFloat(); //reflexão
                    refra = material.attribute("Refraction","").toFloat(); //refração
                    grefle = material.attribute("Reflection","").toFloat(); //reflexão glossy
                    grefra = material.attribute("GLossyRefraction","").toFloat(); //refração glossy
                    shine = material.attribute("Shininess","").toFloat(); //shininess do material

                    //contribuição ambiente
                    QDomElement amb = material.firstChildElement("Ambient").toElement();
                    vec.x = amb.attribute("r","").toFloat();
                    vec.y = amb.attribute("g","").toFloat();
                    vec.z = amb.attribute("b","").toFloat();
                    vect ambient = vec;

                    //contribuição diffusa
                    QDomElement diff = material.firstChildElement("Diffuse").toElement();
                    vec.x = diff.attribute("r","").toFloat();
                    vec.y = diff.attribute("g","").toFloat();
                    vec.z = diff.attribute("b","").toFloat();
                    vect diffuse = vec;

                    //contribuição diffusa
                    QDomElement spe = material.firstChildElement("Specular").toElement();
                    vec.x = spe.attribute("r","").toFloat();
                    vec.y = spe.attribute("g","").toFloat();
                    vec.z = spe.attribute("b","").toFloat();
                    vect specular = vec;

                    //define componentes
                    object.comp_ambiente(ambient.x,ambient.y,ambient.z);
                    object.comp_difusa(diffuse.x,diffuse.y,diffuse.z);
                    object.comp_specular(specular.x,specular.y,specular.z,shine);

                    //efeitos
                    QDomElement effect = obj.firstChildElement("Effects").toElement();
                    bool has = effect.attribute("Motion","").toInt();
                    if(has){ //se exite algum efeito
                        QDomElement trans = effect.firstChildElement("Translate").toElement();
                        vec.x = trans.attribute("x","").toFloat();
                        vec.y = trans.attribute("y","").toFloat();
                        vec.z = trans.attribute("z","").toFloat();
                        //object->setMotion(vec);
                        QDomElement rot = effect.firstChildElement("Rotate").toElement();
                        vec.x = rot.attribute("x","").toFloat();
                        vec.y = rot.attribute("y","").toFloat();
                        vec.z = rot.attribute("z","").toFloat();
                        //se tiver alguma efeito de rotação para motion blur colocar aqui!!! Depois farei um.
                    }

                    //salva objeto

                    objs = objs.nextSibling();
                }
            }
        }
        n=n.nextSibling();
    }
    return true;
}
*/
