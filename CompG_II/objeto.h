#ifndef OBJETO_H
#define OBJETO_H
#include <algebra.h>
#include <string.h>
#include <stdlib.h>
#include <vect.h>
#include <matrix4x4.h>
#include <interface/scene.h>

#include <sstream>
#include <QFileInfo>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <limits>
#include <GL/gl.h>
#include <QFileInfo>

struct ponto{
    float x,y,z;
    struct ponto *proximo;
};

struct cor{
    float r,g,b,alfa;
};

struct aresta{
    ponto ponto1,ponto2;
};

struct face_triangulo{
    aresta aresta1,aresta2,aresta3;
};

struct face_quadrado{
    aresta aresta1,aresta2,aresta3,aresta4;
};

struct normal{
    float x,y,z;
    struct nomal *proximo;
};

struct objeto{

    int indice;
    bool aux = false;

    bool teste = false;

    char nome[10];

    bool reflexivo = false;
    bool refractvo = false;

    float indicerefracao = 0.0;
    float indicereflexao = 0.0;

    float raio = 1.0;//0.01
    float altura = 1.0;

    float R[3][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};

    Matrix4x4 matrix;
    Matrix4x4 invmatrix;

    Matrix4x4 rotateX;
    Matrix4x4 rotateY;
    Matrix4x4 rotateZ;
    Matrix4x4 _translate;
    Matrix4x4 _escale;

    float exp = 5;

    float x=0.0;
    float y=0.0;
    float z=0.0;

    float dimencao = 1.0;

    vect ponto;
    vect normal;

    vect canto_se;
    vect canto_id;

    int id;



    void setraio(float var){
        this->raio = var;
    }

    void indrefracao(float var){
        this->refractvo = true;
        this->indicerefracao = var;
    }

    void indreflexao(float var){
        this->reflexivo = true;
        this->indicereflexao = var;

    }

    void tipo(char tipo[]){
        strcpy(this->nome,tipo);
    }

    void centro(float x,float y,float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void copy_matriz(float (*matriz)[3]){
        for(int i=0;i<=2;i++){
            for(int j=0;j<=2;j++){
                this->R[i][j] = matriz[i][j];
            }
        }
    }

    void vet_normal(float x,float y,float z){
        this->normal.x = x;
        this->normal.y = y;
        this->normal.z = z;
    }

    void pontop(float x,float y,float z){
        this->ponto.x = x;
        this->ponto.y = y;
        this->ponto.z = z;
    }

    void comp_ambiente(float r,float g,float b){

        this->R[0][0] = r;
        this->R[0][1] = g;
        this->R[0][2] = b;
    }

    void comp_difusa(float r,float g,float b){

        this->R[1][0] = r;
        this->R[1][1] = g;
        this->R[1][2] = b;
    }

    void comp_specular(float r,float g,float b,float exp){

        this->R[2][0] = r;
        this->R[2][1] = g;
        this->R[2][2] = b;

        this->exp = exp;
    }

    void ponto_se(float x,float y,float z){
        this->canto_se.x = x;
        this->canto_se.y = y;
        this->canto_se.z = z;
    }


    void ponto_id(float x,float y,float z){
        this->canto_id.x = x;
        this->canto_id.y = y;
        this->canto_id.z = z;
    }

    void reflexao(bool var){
        this->reflexivo = var;
    }

    void refracao(bool var){
        this->refractvo = var;
    }

    void setmatrix(Matrix4x4 matriz1){
        this->matrix = matriz1;
    }

    void setinvmatrix(Matrix4x4 matriz2){
        this->invmatrix = matriz2;
    }

    void rotate_X(float angulo){
        this->rotateX.enable = true;
        this->rotateX.rotate_xaxis(angulo);
    }

    void rotate_Y(float angulo){
        this->rotateY.enable = true;
        this->rotateY.rotate_yaxis(angulo);
    }

    void rotate_Z(float angulo){
        this->rotateZ.enable = true;
        this->rotateZ.rotate_zaxis(angulo);
    }

    void translate(float x,float y,float z){
        this->_translate.enable = true;
        this->_translate.translate(x,y,z);
    }

    void scale(float x,float y,float z){
        this->_escale.enable = true;
        this->_escale.escala(x,y,z);
    }

};

struct objetos{
    int indice;
    objeto obj;
    objetos *prox;
};

struct dvet{
    bool val;
    float t;
    vect ponto;
    vect normal;
    vect pontop;
};

struct vet{
    float *vetor;
};

class Objeto
{
public:
    Objeto();

    //void ler_ply(char *obj);//ler arquivo txt no formato ply

    //bool loadScene(Scene *scene, const string &fileName);

    float bola(vect equacao);
    vect normal_esfera(vect ponto,vect origem);
    vect equacao_esfera(vect vetor_dm,vect eye,objeto esfera);
    vect ponto_esfera(vect vetor_dm,vect eye,vect equacao,objeto esfera);


    float plano(vect vetor_dm,vect eye,vect normal,vect ponto);
    float distancia_plano(float t,vect normal,vect eye,vect vetor_dm,vect ponto);
    vect ponto_plano(float t,vect eye,vect vetor_dm);
    bool teste_plano(vect coordpl,objeto obj);


    vect equacao_cilindro(vect eye,vect raio,objeto obj);
    float cilindro_delta(vect equacao);
    vect ponto_cilindro(vect eye,vect raio,vect equacao);
    vect normal_cilindro(vect ponto,vect origem);


    dvet cubo(vect raio,vect eye,objeto obj);

    bool loadSceneXML(QString fileName);

    objetos *teste = NULL;

    Algebra _algebra;
    Matrix4x4 _matrix;

};

#endif // OBJETO_H
