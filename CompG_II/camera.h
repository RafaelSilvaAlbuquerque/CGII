#ifndef CAMERA_H
#define CAMERA_H
#include <algebra.h>
#include <iluminacao.h>
#include <objeto.h>
#include <vect.h>
#include <matrix4x4.h>


#include <sstream>
#include <QFileInfo>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <limits>
#include <GL/gl.h>
#include <QFileInfo>

struct pixel{

    float x_plane;
    float y_plane;
    float z_plane;

    float x_tela;
    float y_tela;

    float distancia = 10000;

    float at = 0.0;

    float dist = 0.0;

    float distrec = 10000;
    float prof;

    int idobj = -1;

    bool intersect=false;

    float cor[3];
    float backup[3];
};
struct vetor{
    float *vet;
};

class Camera
{
public:
    Camera();

    vect coordenadas;//define a posição da camera no mundo
    Matrix4x4 matriz_camera;//define a matriz da câmera
    Matrix4x4 matriz_perspective;
    Matrix4x4 matriz_frustum;
    vect vet_k;//equivalente ao vetor x da câmera
    vect vet_up;//equivalente ao vetor y da câmera
    vect vet_look[3];//define a direção na qual a câmera olha
    pixel *film;//matriz com pixels da tela mapeados para o film plane
    int var;

    bool hort = false;

    bool _shadow = false;

    //amostra os pixels da tela
    //parametros (int número de linhas em pixels, int número de colunas em pixels, int número de linhas do film plane, int número de colunas do film plane)
    void ampixel(float l,float c,float linhas,float colunas);

    //mapea coordenadas de tela para o film plane
    //parametros (int número de linhas em pixels, int número de colunas em pixels, int número de linhas do film plane, int número de colunas do film plane)
    void map(float l,float c,float linhas,float colunas);


    void sistemaCamera(vect at,vect up,vect eye);

    void getRay(int linhas,int colunas);//gera o raio

    void intersect();//calcula a interseção

    void coordcamera(float x,float y,float z);//calcula o sistema de coordenadas da câmera

    bool shadow(int id,vect ponto,vect coord_light);

    void mult(float (*matriz)[4]);

    void addObj(objeto obj);

    void addlight(light ligh);

    void enableshadow(bool var);

    //---------------
    void intersect_esfera(objeto obj,vect raio,vect eye,int indice,int i,int prof);

    void intersect_plano(objeto obj,vect raio,vect eye,int indice,int i,int prof);

    void intersect_cilindro(objeto obj,vect raio,vect eye,int indice,int i,int prof);

    void intersect_cubo(objeto obj,vect raio,vect eye,int indice,int i,int prof);


    void recursao(vect raio,vect eye,int i,int prof,int idobj);

    void atenuacao(bool var);

    objeto maxprox(vect raio,vect eye,int idobj,int i);

    void perspectiva(float fov,float aspect,float znear,float zfar);


    vect raiorefletido(vect ponto_luz,vect vetor_normal);

    vect raiorefratado(vect ponto_luz,vect vetor_normal,float nl,float nt);

    bool loadSceneXML(QString fileName);

    void frustum(float left,float right,float bottom,float top,float near,float far);

    void hortoProjection(bool val);

//------------------------------------------------
    float testeEsfera(vect raio,vect eye,objeto obj);
    float testePlano(vect raio,vect eye,objeto obj);
    float testeCilindro(vect raio,vect eye,objeto obj);
    float testeCubo(vect raio,vect eye,objeto obj);
//------------------------------------------------

    bool atenuation = false;

    int indiceobj = 0;
    objetos *teste = NULL;

    int indice_light = 0;
    setlight *fontes = NULL;


    Iluminacao _light;
    Algebra _algebra;
    Objeto _obj;
    Matrix4x4 _matrix;
};

#endif // CAMERA_H
