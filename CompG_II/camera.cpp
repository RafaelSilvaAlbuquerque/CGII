#include <QtXml/QDomDocument> //classes necessarias para criar a estrutura XML
#include <QtXml/QDomElement> //classes necessarias para criar a estrutura XML
#include "camera.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Camera::Camera()
{
}

//amostra os pixels da tela
//parametros (int número de linhas em pixels, int número de colunas em pixels, int número de linhas do film plane, int número de colunas do film plane)
void Camera::ampixel(float l,float c,float linhas,float colunas){//ok

    float plinha = l/linhas;
    float pcoluna = c/colunas;

    float clinha = plinha/2.0;
    float ccoluna = pcoluna/2.0;

    int var = linhas*colunas;
    int var_linha=1,var_coluna=1;

    this->film = new pixel[var];
    this->var = var;

    for(int i=1;i<=var;i++){

        this->film[i-1].x_tela = ((var_linha*plinha)-clinha);
        this->film[i-1].y_tela = ((var_coluna*pcoluna)-ccoluna);

        var_coluna++;

        if(var_coluna > colunas){
            var_coluna = 1;
            var_linha++;
        }
    }
}


//mapea coordenadas de tela para o film plane
//parametros (int número de linhas em pixels, int número de colunas em pixels, int número de linhas do film plane, int número de colunas do film plane)
void Camera::map(float l,float c,float linhas,float colunas){//ok

    float maplinha = 2.0/l;
    float mapcoluna = 2.0/c;

    int var = linhas*colunas;

    for(int i=1;i<=var;i++){

        this->film[i-1].x_plane = (-1.0) + (this->film[i-1].x_tela * maplinha);
        this->film[i-1].y_plane = (1.0) - (this->film[i-1].y_tela * mapcoluna);
        this->film[i-1].z_plane = -1.0;

        this->film[i-1].cor[0] = 0.0;
        this->film[i-1].cor[1] = 0.0;
        this->film[i-1].cor[2] = 0.0;
    }
}


void Camera::sistemaCamera(vect at,vect up,vect eye){//ok

    up.set(0,1,0);
    vect eixoz;
    vect eixoy;
    vect eixox;

    //vetor unitário look_at
    eixoz = _algebra.vet_unit(_algebra.vet_dist(eye,at));//(at,this->coordenadas)

    //vetor unitario vet_up
    eixox = _algebra.vet_unit(_algebra.vet_prtvetorial(_algebra.vet_unit(up),eixoz));//eixoz,up

    //vetor unitario componente k
    eixoy = _algebra.vet_unit(_algebra.vet_prtvetorial(eixoz,eixox));//eixoz,eixox


    //linha 1
    this->matriz_camera.matriz[0] = eixox.x;//coluna 1
        this->matriz_camera.matriz[1] = eixox.y;//coluna 2
            this->matriz_camera.matriz[2] = eixox.z;//coluna 3
                 this->matriz_camera.matriz[3] = 0;//coluna 4

    //linha 2
    this->matriz_camera.matriz[4] = eixoy.x;//coluna 1
        this->matriz_camera.matriz[5] = eixoy.y;//coluna 2
            this->matriz_camera.matriz[6] = eixoy.z;//coluna 3
                this->matriz_camera.matriz[7] = 0;//coluna 4

    //linha 3
    this->matriz_camera.matriz[8] = eixoz.x;//coluna 1
        this->matriz_camera.matriz[9] = eixoz.y;//coluna 2
            this->matriz_camera.matriz[10] = eixoz.z;//coluna 3
                this->matriz_camera.matriz[11] = 0;//coluna 4

    this->matriz_camera = _matrix.transposta(this->matriz_camera);
    //linha 4
//    this->matriz_camera.matriz[3] = -_algebra.vet_prtescalar(eixox,eye);//colina 1
//        this->matriz_camera.matriz[7] = -_algebra.vet_prtescalar(eixoy,eye);//coluna 2
//            this->matriz_camera.matriz[11] = -_algebra.vet_prtescalar(eixoz,eye);//coluna 3
//                this->matriz_camera.matriz[15] = 1.0;//coluna 4
    this->matriz_camera.matriz[3] = eye.x;//colina 1
        this->matriz_camera.matriz[7] = eye.y;//coluna 2
            this->matriz_camera.matriz[11] = eye.z;//coluna 3
                this->matriz_camera.matriz[15] = 1.0;//coluna 4

    //this->matriz_camera = _matrix.transposta(this->matriz_camera);

}

void Camera::perspectiva(float fov,float aspect,float znear,float zfar){

    float f = atan(fov/2.0);

    //linha 1
    this->matriz_perspective.matriz[0] = 1.0;//coluna 1
        this->matriz_perspective.matriz[1] = 0.0;//coluna 2
            this->matriz_perspective.matriz[2] = 0.0;//coluna 3
                 this->matriz_perspective.matriz[3] = 0.0;//coluna 4

    //linha 2
    this->matriz_perspective.matriz[4] = 0.0;//coluna 1
        this->matriz_perspective.matriz[5] = f;//coluna 2
            this->matriz_perspective.matriz[6] = 0.0;//coluna 3
                this->matriz_perspective.matriz[7] = 0.0;//coluna 4

    //linha 3
    this->matriz_perspective.matriz[8] = 0.0;//coluna 1
        this->matriz_perspective.matriz[9] = 0.0;//coluna 2
            this->matriz_perspective.matriz[10] = (-1*(zfar+znear))/(znear-zfar);//coluna 3
                this->matriz_perspective.matriz[11] = (2.0*zfar*znear)/(znear-zfar);//coluna 4

    //linha 4
    this->matriz_perspective.matriz[12] = 0.0;//colina 1
        this->matriz_perspective.matriz[13] = 0.0;//coluna 2
            this->matriz_perspective.matriz[14] = -1.0;//coluna 3
                this->matriz_perspective.matriz[15] = 0.0;//coluna 4
}

void Camera::frustum(float left,float right,float bottom,float top,float near,float far){

    float a = (right+left)/(right-left);
    float b = (top+bottom)/(top-bottom);
    float c = (far+near)/(far-near);
    float d = -1.0*((2*far*near)/(far-near));

    //linha 1
    this->matriz_frustum.matriz[0] = (2*near)/(right-left);//coluna 1
        this->matriz_frustum.matriz[1] = 0.0;//coluna 2
            this->matriz_frustum.matriz[2] = a;//coluna 3
                 this->matriz_frustum.matriz[3] = 0.0;//coluna 4

    //linha 2
    this->matriz_frustum.matriz[4] = 0.0;//coluna 1
        this->matriz_frustum.matriz[5] = (2*near)/(top-bottom);//coluna 2
            this->matriz_frustum.matriz[6] = b;//coluna 3
                this->matriz_frustum.matriz[7] = 0.0;//coluna 4

    //linha 3
    this->matriz_frustum.matriz[8] = 0.0;//coluna 1
        this->matriz_frustum.matriz[9] = 0.0;//coluna 2
            this->matriz_frustum.matriz[10] = c;//coluna 3
                this->matriz_frustum.matriz[11] = d;//coluna 4

    //linha 4
    this->matriz_frustum.matriz[12] = 0.0;//colina 1
        this->matriz_frustum.matriz[13] = 0.0;//coluna 2
            this->matriz_frustum.matriz[14] = -1.0;//coluna 3
                this->matriz_frustum.matriz[15] = 0.0;//coluna 4

}

void Camera::intersect(){

    vect ponto;
    int prof=0;
    int idobj = -1;

    bool t = true;


    for(int i=1;i<=this->var;i++){

        ponto.x = this->film[i-1].x_plane;
        ponto.y = this->film[i-1].y_plane;
        ponto.z = this->film[i-1].z_plane;
        ponto.v = 1.0;


        //calcula o raio
        vect projecao;
        vect eye;
        vect raio;



        if(this->hort){

            projecao.set(ponto.x,ponto.y,1.0,ponto.v);

            raio = _algebra.vet_unit(_algebra.vet_dist(ponto,projecao));
            raio.v = 0.0;

            eye = projecao;
            eye.v = 1.0;
        }
        else{

            projecao.set(0.0,0.0,2.0,1.0);

            raio = _algebra.vet_unit(_algebra.vet_dist(ponto,projecao));
            raio.v = 0.0;

            eye = projecao;
            eye.v = 1.0;

            raio = _algebra.vet_unit(_matrix.multvetmatrix(this->matriz_camera,raio));
            eye = _matrix.multvetmatrix(this->matriz_camera,eye);

            if(t){
                qDebug() << eye.x <<" "<<eye.y<<" "<<eye.z;
            }
            t = false;
        }



       //eye.x = eye.x / 1.5;
       //eye.y = eye.y / 1.5;
       //eye.z = eye.z / 1.5;

        //eye.x = eye.x * 20.0;
        //eye.y = eye.y * 20.0;
        //eye.z = eye.z * 20.0;

        prof=1;

        this->recursao(raio,eye,i,prof,idobj);
    }
}


void Camera::recursao(vect raio,vect eye,int i,int prof,int idobj){

    if(prof <= 4){

        objeto obj = this->maxprox(raio,eye,idobj,i);

        if(strcmp(obj.nome,"esfera") == 0){
            this->intersect_esfera(obj,raio,eye,obj.indice,i,prof);
        }

        if(strcmp(obj.nome,"plano") == 0){
            this->intersect_plano(obj,raio,eye,obj.indice,i,prof);
        }

        if(strcmp(obj.nome,"cilindro") == 0){
            this->intersect_cilindro(obj,raio,eye,obj.indice,i,prof);
        }

        if(strcmp(obj.nome,"cubo") == 0){
            this->intersect_cubo(obj,raio,eye,obj.indice,i,prof);
        }
    }
}


bool Camera::shadow(int id,vect ponto,vect coord_light){

    objetos *aux = this->teste;

    float vardist = -1;

    vect vet_dist;

    vet_dist = _algebra.vet_dist(coord_light,ponto);
    float distponto = _algebra.vet_norma(vet_dist);

    //vet_dist = _algebra.vet_dist(coord_light,ponto);
    vect raio = _algebra.vet_unit(vet_dist);

    vect vet1 = _algebra.vet_dist(coord_light,ponto);

    //comum
    vect dist_obj_ponto;
    vect equacao;
    vect coordobj;
    vect vet2;

    objeto obj;

    while(aux != NULL){

        //if(aux->indice != id){

            obj = aux->obj;

            if((strcmp(obj.nome,"esfera") == 0)&&(aux->indice != id)){

                //aplica a transformação inversa na origem do raio
                vect auxponto = ponto;
                vect auxraio = _algebra.vet_unit(raio);

                //translacao
                if(aux->obj._translate.enable){
                    auxponto = _matrix.multvetmatrix(obj._translate.getinvmatrix(),auxponto);
                }

                //rotacao x
                if(obj.rotateX.enable){
                    auxponto = _matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxraio));
                }

                //rotacao y
                if(obj.rotateY.enable){
                    auxponto = _matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxraio));
                }

                //rotacao z
                if(obj.rotateZ.enable){
                    auxponto = _matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxraio));
                }

                //escala
                if(obj._escale.enable){
                    auxponto = _matrix.multvetmatrix(obj._escale.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj._escale.getinvmatrix(),auxraio));
                }

                float delta;
                float auxdist;

                equacao = _obj.equacao_esfera(auxraio,auxponto,aux->obj);
                delta = _obj.bola(equacao);

                if(delta >= 0){

                    coordobj = _obj.ponto_esfera(auxraio,auxponto,equacao,aux->obj);

                    vet2 = _algebra.vet_dist(coordobj,auxponto);

                    if(_algebra.vet_prtescalar(vet1,vet2) > 0){

                        dist_obj_ponto = _algebra.vet_dist(coordobj,auxponto);
                        auxdist = _algebra.vet_norma(dist_obj_ponto);

                        if(auxdist < distponto){

                            return true;
                        }

                        if(vardist < 0){
                            vardist = auxdist;
                        }
                        else if(vardist > 0){
                            if(vardist > auxdist){
                                vardist = auxdist;
                            }
                        }
                    }
                }
            }

            if((strcmp(aux->obj.nome,"plano") == 0)&&(aux->indice != id)){

                float t;
                float distancia;
                float auxdist;

                t = _obj.plano(raio,ponto,aux->obj.normal,aux->obj.ponto);
                distancia = _obj.distancia_plano(t,aux->obj.normal,ponto,raio,aux->obj.ponto);

                if(distancia < 0.000000){
                    distancia = 0.0;
                }

                if(distancia >= 0){

                    coordobj = _obj.ponto_plano(t,ponto,raio);

                    vet2 = _algebra.vet_dist(coordobj,ponto);

                    if(_algebra.vet_prtescalar(vet1,vet2) > 0){

                        dist_obj_ponto = _algebra.vet_dist(coordobj,ponto);
                        auxdist = _algebra.vet_norma(dist_obj_ponto);

                        if(auxdist < distponto){

                            return true;
                        }

                        if(vardist < 0){
                            vardist = auxdist;
                        }
                        else if(vardist > 0){
                            if(vardist > auxdist){
                                vardist = auxdist;
                            }
                        }
                    }
                }
            }

            if((strcmp(aux->obj.nome,"cilindro") == 0)&&(aux->indice != id)){

                float delta;
                float auxdist;

                //aplica a transformação inversa na origem do raio
                vect auxponto = ponto;
                vect auxraio = _algebra.vet_unit(raio);

                //translacao
                if(obj._translate.enable){
                    auxponto = _matrix.multvetmatrix(obj._translate.getinvmatrix(),auxponto);
                }

                //rotacao x
                if(obj.rotateX.enable){
                    auxponto = _matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxraio));
                }

                //rotacao y
                if(obj.rotateY.enable){
                    auxponto = _matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxraio));
                }

                //rotacao z
                if(obj.rotateZ.enable){
                    auxponto = _matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxraio));
                }

                //escala
                if(obj._escale.enable){
                    auxponto = _matrix.multvetmatrix(obj._escale.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj._escale.getinvmatrix(),auxraio));
                }

                equacao = _obj.equacao_cilindro(auxponto,auxraio,aux->obj);
                delta = _obj.cilindro_delta(equacao);

                if(delta >= 0){

                    coordobj = _obj.ponto_cilindro(auxponto,auxraio,equacao);

                    if((coordobj.y <= 1.0)&&(coordobj.y >= 0.0)){

                        vet2 = _algebra.vet_dist(coordobj,auxponto);

                        if(_algebra.vet_prtescalar(vet1,vet2) > 0){

                            dist_obj_ponto = _algebra.vet_dist(coordobj,auxponto);
                            auxdist = _algebra.vet_norma(dist_obj_ponto);

                            if(auxdist < distponto){

                                return true;
                            }

                            if(vardist < 0){
                                vardist = auxdist;
                            }
                            else if(vardist > 0){
                                if(vardist > auxdist){
                                    vardist = auxdist;
                                }
                            }
                        }
                    }
                }
            }

            if((strcmp(aux->obj.nome,"cubo") == 0)&&(aux->indice != id)){

                //aplica a transformação inversa na origem do raio
                vect auxponto = ponto;
                vect auxraio = _algebra.vet_unit(raio);

                //translacao
                if(obj._translate.enable){
                    auxponto = _matrix.multvetmatrix(obj._translate.getinvmatrix(),auxponto);
                }

                //rotacao x
                if(obj.rotateX.enable){
                    auxponto = _matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxraio));
                }

                //rotacao y
                if(obj.rotateY.enable){
                    auxponto = _matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxraio));
                }

                //rotacao z
                if(obj.rotateZ.enable){
                    auxponto = _matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxraio));
                }

                //escala
                if(obj._escale.enable){
                    auxponto = _matrix.multvetmatrix(obj._escale.getinvmatrix(),auxponto);
                    auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj._escale.getinvmatrix(),auxraio));
                }

                float auxdist;

                dvet convet = _obj.cubo(auxraio,auxponto,aux->obj);

                if(convet.val != false){

                    vet2 = _algebra.vet_dist(convet.ponto,auxponto);//vet2 = _algebra.vet_dist(convet[0].vetor,ponto);


                    if(_algebra.vet_prtescalar(vet1,vet2) > 0){

                        dist_obj_ponto = _algebra.vet_dist(convet.ponto,auxponto);//dist_obj_ponto = _algebra.vet_dist(convet[0].vetor,ponto);
                        auxdist = _algebra.vet_norma(dist_obj_ponto);

                        if(auxdist < distponto){

                            return true;
                        }

                        if(vardist < 0){
                            vardist = auxdist;
                        }
                        else if(vardist > 0){
                            if(vardist > auxdist){
                                vardist = auxdist;
                            }
                        }
                    }
                }
            }
        //}

        aux = aux->prox;
    }

    if((vardist < distponto)&&(vardist >= 0)){
        return true;
    }
    else if(vardist > distponto){
        return false;
    }
}


void Camera::enableshadow(bool var){
    this->_shadow = var;
}


void Camera::intersect_esfera(objeto obj,vect raio,vect eye,int indice,int i,int prof){

    vect auxeye = eye;
    vect auxraio = _algebra.vet_unit(raio);

    //translacao
    if(obj._translate.enable){
        auxeye = _matrix.multvetmatrix(obj._translate.getinvmatrix(),auxeye);
    }

    //rotacao x
    if(obj.rotateX.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxraio));
    }

    //rotacao y
    if(obj.rotateY.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxraio));
    }

    //rotacao z
    if(obj.rotateZ.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxraio));
    }

    //escala
    if(obj._escale.enable){
        auxeye = _matrix.multvetmatrix(obj._escale.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj._escale.getinvmatrix(),auxraio));
    }


    vect equacao,normal,componentes,coordesf,vet_dist,auxcomponentes;
    vect ponto_camera,ponto_luz,vetor_normal,raio_refletido;

    float delta;
    float norma_vet;
    bool intershadow;

    vect ce;
    ce.set(obj.x,obj.y,obj.z,1.0);

    //verifica se raio intersepta a esfera
    equacao = _obj.equacao_esfera(auxraio,auxeye,obj);
    delta = _obj.bola(equacao);

    if(delta >= 0){

        //calcula ponto na superficie da esfera
        coordesf = _obj.ponto_esfera(auxraio,auxeye,equacao,obj);
        coordesf.v = 1.0;

        //calcula o vetor normal a superficie do objeto
        normal = _algebra.vet_unit(_obj.normal_esfera(coordesf,ce));

        //escala
        if(obj._escale.enable){
            coordesf = _matrix.multvetmatrix(obj._escale.getmatrix(),coordesf);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj._escale.getinvmatrix()),normal));
        }

        //rotacao x
        if(obj.rotateX.enable){
            coordesf = _matrix.multvetmatrix(obj.rotateX.getmatrix(),coordesf);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj.rotateX.getinvmatrix()),normal));
        }

        //rotacao y
        if(obj.rotateY.enable){
            coordesf = _matrix.multvetmatrix(obj.rotateY.getmatrix(),coordesf);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj.rotateY.getinvmatrix()),normal));
        }

        //rotacao z
        if(obj.rotateZ.enable){
            coordesf = _matrix.multvetmatrix(obj.rotateZ.getmatrix(),coordesf);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj.rotateZ.getinvmatrix()),normal));
        }

        //translacao
        if(obj._translate.enable){
            coordesf = _matrix.multvetmatrix(obj._translate.getmatrix(),coordesf);
        }


        //distancia do observardor para o ponto
        norma_vet = _algebra.vet_norma(_algebra.vet_dist(coordesf,auxeye));

        setlight *auxlight = this->fontes;
        light fonte;
        while(auxlight != NULL){

            fonte = auxlight->lig;

            if(this->_shadow){
                intershadow = this->shadow(indice,coordesf,fonte.coordenadas);
            }

            //vetor do ponto iluminado para a camera
            vet_dist = _algebra.vet_dist(eye,coordesf);
            ponto_camera = _algebra.vet_unit(vet_dist);

            //vetor do ponto iluminado para a fonte de luz
            vet_dist = _algebra.vet_dist(fonte.coordenadas,coordesf);
            ponto_luz = _algebra.vet_unit(vet_dist);

            //vetor normal ao ponto iluminado
            vetor_normal = normal;

            //calcula o raio refletido para um objeto não reflexivo e não refractivo
            raio_refletido = this->raiorefletido(ponto_luz,vetor_normal);

            //iluminação de phong retorna as três componentes rgb
            componentes = _light.phong(ponto_camera,ponto_luz,vetor_normal,raio_refletido,obj.R,fonte.L,obj.exp,this->film[i-1].dist,intershadow,this->atenuation);

            auxcomponentes.x = auxcomponentes.x + componentes.x;
            auxcomponentes.y = auxcomponentes.y + componentes.y;
            auxcomponentes.z = auxcomponentes.z + componentes.z;

            auxlight = auxlight->prox;
        }

        //se o raio que passa pelo pixel intercepta o objeto pela primeira vez então pinta o pixel da cor do objeto
        if(prof == 1){
            this->film[i-1].cor[0] = auxcomponentes.x;
            this->film[i-1].cor[1] = auxcomponentes.y;
            this->film[i-1].cor[2] = auxcomponentes.z;
        }
        else if(prof > 1){

            this->film[i-1].cor[0] = this->film[i-1].cor[0]+(auxcomponentes.x);
            this->film[i-1].cor[1] = this->film[i-1].cor[1]+(auxcomponentes.y);
            this->film[i-1].cor[2] = this->film[i-1].cor[2]+(auxcomponentes.z);
        }

        if(obj.reflexivo){
            prof++;
            vect rrefletido = this->raiorefletido(ponto_camera,vetor_normal);
            this->recursao(rrefletido,coordesf,i,prof,indice);
        }

        if(obj.refractvo){
            prof++;
            vect raiorefrat = this->raiorefratado(ponto_camera,vetor_normal,1.0003,obj.indicerefracao);//1.0003,20.0
            this->recursao(raiorefrat,coordesf,i,prof,indice);
        }
    }
}



void Camera::intersect_plano(objeto obj,vect raio,vect eye,int indice,int i,int prof){

    //aplica tranformação inversa no raio de interceção
    //eye = _matrix.multvetmatrix(obj.invmatrix,eye);
    //raio = _matrix.multvetmatrix(obj.invmatrix,raio);

    vect componentes,coordpl,vet_dist,ponto_camera,ponto_luz,vetor_normal,raio_refletido;

    vect auxcomponentes;

    float t;
    float distancia;
    float norma_vet;
    bool intershadow;

    vect ce;
    ce.x = obj.x;
    ce.y = obj.y;
    ce.z = obj.z;
    ce.v = 1.0;

    //verifica se raio intersepta a esfera
    t = _obj.plano(raio,eye,obj.normal,obj.ponto);
    distancia = _obj.distancia_plano(t,obj.normal,eye,raio,obj.ponto);

    if(distancia < 0.000000){
        distancia = 0.0;
    }

    if(distancia >= 0){

        //aplica transformação ao centro do plano
        //ce = _matrix.multvetmatrix(obj.matrix,ce);

        //calcula ponto na superficie do plano
        coordpl = _obj.ponto_plano(t,eye,raio);

        //aplica transformação ao ponto que junto ao vetor normal determina o plano
        //coordpl = _matrix.multvetmatrix(obj.matrix,coordpl);

        //distsncia do observador para o ponto
        norma_vet = _algebra.vet_norma(_algebra.vet_dist(coordpl,eye));

        setlight *auxlight = this->fontes;
        light fonte;
        while(auxlight != NULL){

            fonte = auxlight->lig;

            if(this->_shadow){
                intershadow = this->shadow(indice,coordpl,fonte.coordenadas);
            }

            //vetor do ponto iluminado para a camera
            vet_dist = _algebra.vet_dist(eye,coordpl);
            ponto_camera = _algebra.vet_unit(vet_dist);

            //vetor do ponto iluminado para a fonte de luz
            vet_dist = _algebra.vet_dist(fonte.coordenadas,coordpl);
            ponto_luz = _algebra.vet_unit(vet_dist);

            //vetor normal ao ponto iluminado
            vetor_normal = obj.normal;

            raio_refletido = this->raiorefletido(ponto_luz,vetor_normal);

            //iluminação de phong retorna as três componentes rgb
            componentes = _light.phong(ponto_camera,ponto_luz,vetor_normal,raio_refletido,obj.R,fonte.L,obj.exp,this->film[i-1].dist,intershadow,this->atenuation);

            auxcomponentes.x = auxcomponentes.x + componentes.x;
            auxcomponentes.y = auxcomponentes.y + componentes.y;
            auxcomponentes.z = auxcomponentes.z + componentes.z;

            auxlight = auxlight->prox;
        }

        if(prof == 1){

            this->film[i-1].cor[0] = auxcomponentes.x;
            this->film[i-1].cor[1] = auxcomponentes.y;
            this->film[i-1].cor[2] = auxcomponentes.z;
        }
        else if((prof > 1)&&(t >= 0.0)){

            this->film[i-1].cor[0] = this->film[i-1].cor[0]+auxcomponentes.x;
            this->film[i-1].cor[1] = this->film[i-1].cor[1]+auxcomponentes.y;
            this->film[i-1].cor[2] = this->film[i-1].cor[2]+auxcomponentes.z;
        }

        if(obj.reflexivo){
            prof++;
            vect rrefletido = this->raiorefletido(ponto_camera,vetor_normal);
            this->recursao(rrefletido,coordpl,i,prof,indice);
        }

        if(obj.refractvo){
            prof++;
            vect raiorefrat = this->raiorefratado(ponto_camera,vetor_normal,1.0003,obj.indicerefracao);
            this->recursao(raiorefrat,coordpl,i,prof,indice);
        }
    }
}


void Camera::intersect_cilindro(objeto obj,vect raio,vect eye,int indice,int i,int prof){

    vect auxeye = eye;
    vect auxraio = _algebra.vet_unit(raio);

    //translacao
    if(obj._translate.enable){
        auxeye = _matrix.multvetmatrix(obj._translate.getinvmatrix(),auxeye);
    }

    //rotacao x
    if(obj.rotateX.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxraio));
    }

    //rotacao y
    if(obj.rotateY.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxraio));
    }

    //rotacao z
    if(obj.rotateZ.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxraio));
    }

    //escala
    if(obj._escale.enable){
        auxeye = _matrix.multvetmatrix(obj._escale.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj._escale.getinvmatrix(),auxraio));
    }

    vect equacao,normal,componentes,coordobj,vet_dist,ponto_camera;
    vect ponto_luz,vetor_normal,raio_refletido;

    vect auxcomponentes;

    float delta;
    float norma_vet;
    bool intershadow;

    vect ce;
    ce.set(obj.x,obj.y,obj.z,1.0);

    //verifica se raio intersepta a esfera
    equacao = _obj.equacao_cilindro(auxeye,auxraio,obj);
    delta = _obj.cilindro_delta(equacao);

    if(delta >= 0){

        //calcula ponto na superficie da esfera
        coordobj = _obj.ponto_cilindro(auxeye,auxraio,equacao);
        coordobj.v = 1.0;

        //calcula o vetor norma em um ponto da esfera
        normal = _algebra.vet_unit(_obj.normal_cilindro(coordobj,ce));


        //escala
        if(obj._escale.enable){
            coordobj = _matrix.multvetmatrix(obj._escale.getmatrix(),coordobj);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj._escale.getinvmatrix()),normal));
        }

        //rotacao x
        if(obj.rotateX.enable){
            coordobj = _matrix.multvetmatrix(obj.rotateX.getmatrix(),coordobj);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj.rotateX.getinvmatrix()),normal));
        }

        //rotacao y
        if(obj.rotateY.enable){
            coordobj = _matrix.multvetmatrix(obj.rotateY.getmatrix(),coordobj);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj.rotateY.getinvmatrix()),normal));
        }

        //rotacao z
        if(obj.rotateZ.enable){
            coordobj = _matrix.multvetmatrix(obj.rotateZ.getmatrix(),coordobj);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj.rotateZ.getinvmatrix()),normal));
        }

        //translacao
        if(obj._translate.enable){
            coordobj = _matrix.multvetmatrix(obj._translate.getmatrix(),coordobj);
        }


        //distancia do observardor para o ponto
        norma_vet = _algebra.vet_norma(_algebra.vet_dist(coordobj,auxeye));

        setlight *auxlight = this->fontes;
        light fonte;
        while(auxlight != NULL){

            fonte = auxlight->lig;

            if(this->_shadow){
                intershadow = this->shadow(indice,coordobj,fonte.coordenadas);
            }

            //vetor do ponto iluminado para a camera
            vet_dist = _algebra.vet_dist(eye,coordobj);
            ponto_camera = _algebra.vet_unit(vet_dist);

            //vetor do ponto iluminado para a fonte de luz
            vet_dist = _algebra.vet_dist(fonte.coordenadas,coordobj);
            ponto_luz = _algebra.vet_unit(vet_dist);

            //vetor normal ao ponto iluminado
            vetor_normal = normal;

            //calcula o raio refletido para um objeto não reflexivo e não refractivo
            raio_refletido = this->raiorefletido(ponto_luz,vetor_normal);

            //iluminação de phong retorna as três componentes rgb
            componentes = _light.phong(ponto_camera,ponto_luz,vetor_normal,raio_refletido,obj.R,fonte.L,obj.exp,this->film[i-1].dist,intershadow,this->atenuation);

            auxcomponentes.x = auxcomponentes.x + componentes.x;
            auxcomponentes.y = auxcomponentes.y + componentes.y;
            auxcomponentes.z = auxcomponentes.z + componentes.z;

            auxlight = auxlight->prox;
        }

        if(prof == 1){

            this->film[i-1].cor[0] = auxcomponentes.x;
            this->film[i-1].cor[1] = auxcomponentes.y;
            this->film[i-1].cor[2] = auxcomponentes.z;
        }
        else if(prof > 1){

            this->film[i-1].cor[0] = this->film[i-1].cor[0]+auxcomponentes.x;
            this->film[i-1].cor[1] = this->film[i-1].cor[1]+auxcomponentes.y;
            this->film[i-1].cor[2] = this->film[i-1].cor[2]+auxcomponentes.z;
        }

        if(obj.reflexivo){
            prof++;
            vect rrefletido = this->raiorefletido(ponto_camera,vetor_normal);
            this->recursao(rrefletido,coordobj,i,prof,indice);
        }        

        if(obj.refractvo){
            prof++;
            vect raiorefrat = this->raiorefratado(ponto_camera,vetor_normal,1.0003,obj.indicerefracao);//
            this->recursao(raiorefrat,coordobj,i,prof,indice);
        }
    }
}




void Camera::intersect_cubo(objeto obj,vect raio,vect eye,int indice,int i,int prof){

    vect auxeye = eye;
    vect auxraio = _algebra.vet_unit(raio);


    //translacao
    if(obj._translate.enable){
        auxeye = _matrix.multvetmatrix(obj._translate.getinvmatrix(),auxeye);
    }

    //rotacao x
    if(obj.rotateX.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxraio));
    }

    //rotacao y
    if(obj.rotateY.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxraio));
    }

    //rotacao z
    if(obj.rotateZ.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxraio));
    }

    //escala
    if(obj._escale.enable){
        auxeye = _matrix.multvetmatrix(obj._escale.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj._escale.getinvmatrix(),auxraio));
    }




    vect componentes,vet_dist,ponto_camera,ponto_luz,vetor_normal,raio_refletido,normal;

    vect auxcomponentes;

    float norma_vet;
    bool intershadow;

    dvet convet;

    vect ce;
    ce.set(obj.x,obj.y,obj.z,1.0);

    convet = _obj.cubo(auxraio,auxeye,obj);

    if(convet.val != false){


        vect nponto = convet.ponto;
        normal = convet.normal;

        //escala
        if(obj._escale.enable){
            nponto = _matrix.multvetmatrix(obj._escale.getmatrix(),nponto);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj._escale.getinvmatrix()),normal));
        }

        //rotacao x
        if(obj.rotateX.enable){
            nponto = _matrix.multvetmatrix(obj.rotateX.getmatrix(),nponto);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj.rotateX.getinvmatrix()),normal));
        }

        //rotacao y
        if(obj.rotateY.enable){
            nponto = _matrix.multvetmatrix(obj.rotateY.getmatrix(),nponto);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj.rotateY.getinvmatrix()),normal));
        }

        //rotacao z
        if(obj.rotateZ.enable){
            nponto = _matrix.multvetmatrix(obj.rotateZ.getmatrix(),nponto);
            normal = _algebra.vet_unit(_matrix.multvetmatrix(_matrix.transposta(obj.rotateZ.getinvmatrix()),normal));
        }

        //translacao
        if(obj._translate.enable){
            nponto = _matrix.multvetmatrix(obj._translate.getmatrix(),nponto);
        }


        //distancia do observardor para o ponto
        norma_vet = _algebra.vet_norma(_algebra.vet_dist(nponto,auxeye));

        setlight *auxlight = this->fontes;
        light fonte;
        while(auxlight != NULL){

            fonte = auxlight->lig;

            if(this->_shadow){
                intershadow = this->shadow(indice,nponto,fonte.coordenadas);
            }

            //vetor do ponto iluminado para a camera
            vet_dist = _algebra.vet_dist(eye,nponto);
            ponto_camera = _algebra.vet_unit(vet_dist);                                                                                                                            //aplica a transformação inversa ao vetor do ponto observeado para o observador
                                                                                                                            //ponto_camera = _matrix.multvetmatrix(obj.invmatrix,ponto_camera);//----
            //vetor do ponto iluminado para a fonte de luz
            vet_dist = _algebra.vet_dist(fonte.coordenadas,nponto);
            ponto_luz = _algebra.vet_unit(vet_dist);
                                                                                                                            //aplica transformação no vetor do ponto iluminado para a fonte de luz                                                                                                                            //ponto_luz = _matrix.multvetmatrix(obj.invmatrix,ponto_luz);//----
            //aplica transformação ao vetor normal a superficie do objeto
            vetor_normal = normal;                                                                                      //_matrix.multvetmatrix(obj.invmatrix,vet_normal);//vet_normal;//_matrix.multvetmatrix(obj.invmatrix,vet_normal);

            //calcula o raio refletido para um objeto não reflexivo e não refractivo
            raio_refletido = this->raiorefletido(ponto_luz,vetor_normal);


            //iluminação de phong retorna as três componentes rgb
            componentes = _light.phong(ponto_camera,ponto_luz,vetor_normal,raio_refletido,obj.R,fonte.L,obj.exp,this->film[i-1].dist,intershadow,this->atenuation);

            auxcomponentes.x = auxcomponentes.x + componentes.x;
            auxcomponentes.y = auxcomponentes.y + componentes.y;
            auxcomponentes.z = auxcomponentes.z + componentes.z;

            auxlight = auxlight->prox;
        }

        if(prof == 1){

            this->film[i-1].cor[0] = auxcomponentes.x;
            this->film[i-1].cor[1] = auxcomponentes.y;
            this->film[i-1].cor[2] = auxcomponentes.z;
        }
        else if((prof > 1)&&(convet.t > 0)){

            this->film[i-1].cor[0] = this->film[i-1].cor[0]+auxcomponentes.x;
            this->film[i-1].cor[1] = this->film[i-1].cor[1]+auxcomponentes.y;
            this->film[i-1].cor[2] = this->film[i-1].cor[2]+auxcomponentes.z;
        }

        if(obj.reflexivo){
            prof++;
            vect rrefletido = this->raiorefletido(ponto_camera,vetor_normal);
            this->recursao(rrefletido,convet.ponto,i,prof,indice);
        }

        if(obj.refractvo){
            prof++;
            vect raiorefrat = this->raiorefratado(ponto_camera,vetor_normal,1.0003,obj.indicerefracao);
            this->recursao(raiorefrat,convet.ponto,i,prof,indice);
        }
    }
}



void Camera::coordcamera(float x,float y,float z){
    this->coordenadas.x = x;
    this->coordenadas.y = y;
    this->coordenadas.z = z;
    this->coordenadas.v = 1.0;
}


void Camera::addlight(light ligh){

    if(ligh.estado){
        setlight *lights = (setlight*)malloc(sizeof(setlight));

        lights->id = this->indice_light;
        this->indice_light++;
        lights->lig = ligh;
        lights->prox = NULL;

        if(this->fontes == NULL){
            this->fontes = lights;
        }
        else{
            lights->prox = this->fontes;
            this->fontes = lights;
        }
    }
}


void Camera::addObj(objeto obj){

    objetos *objts = (objetos*)malloc(sizeof(objetos));

    obj.indice = this->indiceobj;

    objts->indice = this->indiceobj;
    this->indiceobj++;
    objts->obj = obj;
    objts->prox = NULL;

    if(this->teste == NULL){
        this->teste = objts;
    }
    else{
        objts->prox = this->teste;
        this->teste = objts;

    }
}

void Camera::atenuacao(bool var){
    this->atenuation = var;
}


vect Camera::raiorefletido(vect ponto_luz,vect vetor_normal){

    float var = (2.0*_algebra.vet_prtescalar(ponto_luz,vetor_normal));

    vect var1;
    var1.set((vetor_normal.x*var),(vetor_normal.y*var),(vetor_normal.z*var),0.0);

    vect vet_dist = _algebra.vet_dist(var1,ponto_luz);
    vect raio_refletido = _algebra.vet_unit(vet_dist);

    return raio_refletido;
}


vect Camera::raiorefratado(vect ponto_luz,vect vetor_normal,float nl,float nt){



    float normal_ptl = _algebra.vet_norma(ponto_luz);
    float norma_ptn = _algebra.vet_norma(vetor_normal);

    float prodpn = _algebra.vet_prtescalar(ponto_luz,vetor_normal);

    float cospn = prodpn/(norma_ptn*normal_ptl);


    float angulo = acos(cospn);

//---------------------------------------------
    float sinangi = sin(angulo);
    float sinangt = (nl*sinangi)/nt;

    float cosangt = cos(asin(sinangt));
//--------------------------------------------

    vect invnormal;
    invnormal.set(vetor_normal.x*-1.0,vetor_normal.y*-1.0,vetor_normal.z*-1.0,0.0);

    float prtnl = _algebra.vet_prtescalar(vetor_normal,ponto_luz);

    vect auxn = vetor_normal;
    auxn.x = auxn.x * prtnl;
    auxn.y = auxn.y * prtnl;
    auxn.z = auxn.z * prtnl;

    vect aux = _algebra.vet_dist(ponto_luz,auxn);
    aux.x = aux.x*(sinangt/sinangi);
    aux.y = aux.y*(sinangt/sinangi);
    aux.z = aux.z*(sinangt/sinangi);

    vect ncos;
    ncos.x = invnormal.x * cosangt;
    ncos.y = invnormal.y * cosangt;
    ncos.z = invnormal.z * cosangt;

    vect result = _algebra.vet_dist(ncos,aux);

    return result;
}



float Camera::testeEsfera(vect raio,vect eye,objeto obj){

    //aplica tranformação inversa no raio de interceção

    vect auxeye = eye;
    vect auxraio = raio;


    //translacao
    if(obj._translate.enable){
        auxeye = _matrix.multvetmatrix(obj._translate.getinvmatrix(),auxeye);
    }

    //rotacao x
    if(obj.rotateX.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxraio));
    }

    //rotacao y
    if(obj.rotateY.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxraio));
    }

    //rotacao z
    if(obj.rotateZ.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxraio));
    }

    //escala
    if(obj._escale.enable){
        auxeye = _matrix.multvetmatrix(obj._escale.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj._escale.getinvmatrix(),auxraio));
    }

    vect equacao,coordesf;

    float delta;
    float distancia = -1.0;

    //verifica se raio intersepta a esfera
    equacao = _obj.equacao_esfera(auxraio,auxeye,obj);
    delta = _obj.bola(equacao);

    if(delta >= 0){

        //calcula ponto na superficie da esfera
        coordesf = _obj.ponto_esfera(auxraio,auxeye,equacao,obj);
        coordesf.v = 1.0;

        //escala
        if(obj._escale.enable){
            coordesf = _matrix.multvetmatrix(obj._escale.getmatrix(),coordesf);
        }

        //rotacao x
        if(obj.rotateX.enable){
            coordesf = _matrix.multvetmatrix(obj.rotateX.getmatrix(),coordesf);
        }

        //rotacao y
        if(obj.rotateY.enable){
            coordesf = _matrix.multvetmatrix(obj.rotateY.getmatrix(),coordesf);
        }

        //rotacao z
        if(obj.rotateZ.enable){
            coordesf = _matrix.multvetmatrix(obj.rotateZ.getmatrix(),coordesf);
        }

        //translacao
        if(obj._translate.enable){
            coordesf = _matrix.multvetmatrix(obj._translate.getmatrix(),coordesf);
        }

        //distancia do observardor para o ponto
        distancia = _algebra.vet_norma(_algebra.vet_dist(coordesf,eye));//distancia = _algebra.vet_norma(_algebra.vet_dist(coordesf,auxeye));
    }
    return distancia;
}

float Camera::testePlano(vect raio,vect eye,objeto obj){

    //aplica tranformação inversa no raio de interceção
    //vect auxeye = _matrix.multvetmatrix(obj.invmatrix,eye);
    //vect auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.invmatrix,raio));

    vect auxraio = raio;
    vect auxeye = eye;

    vect coordpl;

    float t;
    float distancia;
    float norma = -1.0;

    //verifica se raio intersepta o plano
    t = _obj.plano(auxraio,auxeye,obj.normal,obj.ponto);
    distancia = _obj.distancia_plano(t,obj.normal,auxeye,auxraio,obj.ponto);

    if(distancia < 0.000000){
        distancia = 0.0;
    }

    if(distancia >= 0){

        //calcula ponto na superficie da esfera
        coordpl = _obj.ponto_plano(t,auxeye,auxraio);

        if((_obj.teste_plano(coordpl,obj))&&(t >= 0)){
            //distsncia do observador para o ponto
            norma = _algebra.vet_norma(_algebra.vet_dist(coordpl,auxeye));
        }
    }
    return norma;
}

float Camera::testeCilindro(vect raio,vect eye,objeto obj){

    //aplica tranformação inversa no raio de interceção

    vect auxeye = eye;
    vect auxraio = raio;

    //translacao
    if(obj._translate.enable){
        auxeye = _matrix.multvetmatrix(obj._translate.getinvmatrix(),auxeye);
    }

    //rotacao x
    if(obj.rotateX.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxraio));
    }

    //rotacao y
    if(obj.rotateY.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxraio));
    }

    //rotacao z
    if(obj.rotateZ.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxraio));
    }

    //escala
    if(obj._escale.enable){
        auxeye = _matrix.multvetmatrix(obj._escale.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj._escale.getinvmatrix(),auxraio));
    }

    vect equacao,coordobj;

    float delta;
    float norma = -1.0;

    //verifica se raio intersepta o cilindro
    equacao = _obj.equacao_cilindro(auxeye,auxraio,obj);
    delta = _obj.cilindro_delta(equacao);

    if(delta >= 0){

        //calcula ponto na superficie da esfera
        coordobj = _obj.ponto_cilindro(auxeye,auxraio,equacao);

        if((coordobj.y <= 1.0)&&(coordobj.y >= 0.0)){

            //escala
            if(obj._escale.enable){
                coordobj = _matrix.multvetmatrix(obj._escale.getmatrix(),coordobj);
            }

            //rotacao x
            if(obj.rotateX.enable){
                coordobj = _matrix.multvetmatrix(obj.rotateX.getmatrix(),coordobj);
            }

            //rotacao y
            if(obj.rotateY.enable){
                coordobj = _matrix.multvetmatrix(obj.rotateY.getmatrix(),coordobj);
            }

            //rotacao z
            if(obj.rotateZ.enable){
                coordobj = _matrix.multvetmatrix(obj.rotateZ.getmatrix(),coordobj);
            }

            //translacao
            if(obj._translate.enable){
                coordobj = _matrix.multvetmatrix(obj._translate.getmatrix(),coordobj);
            }

            //distancia do observardor para o ponto
            norma = _algebra.vet_norma(_algebra.vet_dist(coordobj,eye));
        }
    }
    return norma;
}


float Camera::testeCubo(vect raio,vect eye,objeto obj){

    //aplica tranformação inversa no raio de interceção

    vect auxeye = eye;
    vect auxraio = raio;


    //translacao
    if(obj._translate.enable){
        auxeye = _matrix.multvetmatrix(obj._translate.getinvmatrix(),auxeye);
    }

    //rotacao x
    if(obj.rotateX.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateX.getinvmatrix(),auxraio));
    }

    //rotacao y
    if(obj.rotateY.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateY.getinvmatrix(),auxraio));
    }

    //rotacao z
    if(obj.rotateZ.enable){
        auxeye = _matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj.rotateZ.getinvmatrix(),auxraio));
    }

    //escala
    if(obj._escale.enable){
        auxeye = _matrix.multvetmatrix(obj._escale.getinvmatrix(),auxeye);
        auxraio = _algebra.vet_unit(_matrix.multvetmatrix(obj._escale.getinvmatrix(),auxraio));
    }


    float norma = -1.0;
    dvet convet;

    convet = _obj.cubo(auxraio,auxeye,obj);

    if((convet.val != false)&&(convet.t >= 0.0)){

        vect nponto = convet.ponto;
        nponto.v = 1.0;

        //escala
        if(obj._escale.enable){
            nponto = _matrix.multvetmatrix(obj._escale.getmatrix(),nponto);
        }

        //rotacao x
        if(obj.rotateX.enable){
            nponto = _matrix.multvetmatrix(obj.rotateX.getmatrix(),nponto);
        }

        //rotacao y
        if(obj.rotateY.enable){
            nponto = _matrix.multvetmatrix(obj.rotateY.getmatrix(),nponto);
        }

        //rotacao z
        if(obj.rotateZ.enable){
            nponto = _matrix.multvetmatrix(obj.rotateZ.getmatrix(),nponto);
        }

        //translacao
        if(obj._translate.enable){
            nponto = _matrix.multvetmatrix(obj._translate.getmatrix(),nponto);
        }

        //distancia do observardor para o ponto
        norma = _algebra.vet_norma(_algebra.vet_dist(nponto,eye));//norma = _algebra.vet_norma(_algebra.vet_dist(nponto,auxeye));
    }
    return norma;
}



objeto Camera::maxprox(vect raio,vect eye,int idobj,int i){

    objeto robj;
    robj.tipo("void");

    float auxdist;
    float rdist = 10000.0;

    objetos *aux = this->teste;
    while(aux != NULL){

        if((strcmp(aux->obj.nome,"esfera") == 0)&&(aux->indice != idobj)){
            auxdist = this->testeEsfera(raio,eye,aux->obj);
        }

        if((strcmp(aux->obj.nome,"plano") == 0)&&(aux->indice != idobj)){
            auxdist = this->testePlano(raio,eye,aux->obj);
        }

        if((strcmp(aux->obj.nome,"cilindro") == 0)&&(aux->indice != idobj)){
            auxdist = this->testeCilindro(raio,eye,aux->obj);
        }

        if((strcmp(aux->obj.nome,"cubo") == 0)&&(aux->indice != idobj)){
            auxdist = this->testeCubo(raio,eye,aux->obj);
        }



        if((auxdist > 0.0)&&(auxdist < rdist)){



            rdist = auxdist;
            robj = aux->obj;
        }

        aux = aux->prox;
    }

    //atualiza a distancia do ponto intersectado para a fonte de luz
    this->film[i-1].dist = this->film[i-1].dist+rdist;

    return robj;
}

//--------------------------------------------------------------------------------------------------------------------



bool Camera::loadSceneXML(QString fileName){

    QDomDocument doc( "SceneRayTracing" );
    //QFile file( fileName.toStdString().data() );

    QFile file("teste13.xml");

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

            if(e.tagName()=="Camera"){

                QDomElement eye = e.firstChildElement("Eye").toElement();
                vec.x = eye.attribute("x","").toFloat();
                vec.y = eye.attribute("y","").toFloat();
                vec.z = eye.attribute("z","").toFloat();
                vect veye = vec;
                veye.v = 1.0;

                QDomElement up = e.firstChildElement("Up").toElement();
                vec.x = up.attribute("x","").toFloat();
                vec.y = up.attribute("y","").toFloat();
                vec.z = up.attribute("z","").toFloat();
                vect vup = vec;
                vup.v = 1.0;

                QDomElement at = e.firstChildElement("At").toElement();
                vec.x = at.attribute("x","").toFloat();
                vec.y = at.attribute("y","").toFloat();
                vec.z = at.attribute("z","").toFloat();
                vect vat = vec;
                vat.v = 1.0;

                this->sistemaCamera(vat,vup,veye);
            }

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
                    else if (type=="OBJCUBE"){
                        object.tipo("cubo");
                    }
                    else if (type=="OBJCYLINDER"){
                        object.tipo("cilindro");
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

                    //define transformações do objeto
                    object.scale(sca.x,sca.y,sca.z);
                    object.rotate_Z(rotation.z);
                    object.rotate_Y(rotation.y);
                    object.rotate_X(rotation.x);
                    object.translate(position.x,position.y,position.z);


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

                    //contribuição specular
                    QDomElement spe = material.firstChildElement("Specular").toElement();
                    vec.x = spe.attribute("r","").toFloat();
                    vec.y = spe.attribute("g","").toFloat();
                    vec.z = spe.attribute("b","").toFloat();
                    vect specular = vec;

                    //define componentes
                    object.comp_ambiente(ambient.x,ambient.y,ambient.z);
                    object.comp_difusa(diffuse.x,diffuse.y,diffuse.z);
                    object.comp_specular(specular.x,specular.y,specular.z,shine);
                    if(refra != 0.0){
                        object.indrefracao(refra);
                    }
                    if(refle != 0.0){
                        object.indreflexao(refle);
                    }

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
                    this->addObj(object);
                    objs = objs.nextSibling();
                }
            }

            if(e.tagName()=="Lights"){

                QDomNode  ligs = e.firstChildElement("Light");

                while (!ligs.isNull()){

                    QDomNode elemento = ligs;
                    QDomElement lig = elemento.toElement();
                    QString type = lig.attribute("Type","");
                    QString name = lig.attribute("Name","");
                    bool enable = lig.attribute("Enable","").toInt();

                    light fonte;

                    if (type=="LIGHT_AMBIENT"){

                        QDomElement material = lig.firstChildElement("Material").toElement();
                        QDomElement amb = material.firstChildElement("Ambient").toElement();
                        vec.x = amb.attribute("r","").toFloat();
                        vec.y = amb.attribute("g","").toFloat();
                        vec.z = amb.attribute("b","").toFloat();

                        fonte.comp_ambiente(vec.x,vec.y,vec.z);
                    }
                    else if(type=="LIGHT_PONTUAL"){

                        QDomElement pos = lig.firstChildElement("Position").toElement();
                        vec.x = pos.attribute("x","").toFloat();
                        vec.y = pos.attribute("y","").toFloat();
                        vec.z = pos.attribute("z","").toFloat();

                        fonte.coord(vec.x,vec.y,vec.z);
                    }
                    else{
                        break;
                    }

                    if(!(type=="LIGHT_AMBIENT")){

                        QDomElement material = lig.firstChildElement("Material").toElement();
                        QDomElement amb = material.firstChildElement("Ambient").toElement();
                        vec.x = amb.attribute("r","").toFloat();
                        vec.y = amb.attribute("g","").toFloat();
                        vec.z = amb.attribute("b","").toFloat();
                        fonte.comp_ambiente(vec.x,vec.y,vec.z);

                        QDomElement diff = material.firstChildElement("Diffuse").toElement();
                        vec.x = diff.attribute("r","").toFloat();
                        vec.y = diff.attribute("g","").toFloat();
                        vec.z = diff.attribute("b","").toFloat();
                        fonte.comp_difusa(vec.x,vec.y,vec.z);

                        QDomElement spe = material.firstChildElement("Specular").toElement();
                        vec.x = spe.attribute("r","").toFloat();
                        vec.y = spe.attribute("g","").toFloat();
                        vec.z = spe.attribute("b","").toFloat();
                        fonte.comp_specular(vec.x,vec.y,vec.z);

                        if(!(type=="LIGHT_DIRECTIONAL")){ //se não for direcional tem atenuação
                            QDomElement att = lig.firstChildElement("Attenuation").toElement();
                            vec.x = att.attribute("x","").toFloat();
                            vec.y = att.attribute("y","").toFloat();
                            vec.z = att.attribute("z","").toFloat();
                            //light->setAttenuation(vec);
                        }
                    }

                    fonte.state(enable);
                    this->addlight(fonte);

                    ligs = ligs.nextSibling();
                }
            }
        }
        n=n.nextSibling();
    }
    return true;
}

void Camera::hortoProjection(bool val){
    this->hort = val;
}
