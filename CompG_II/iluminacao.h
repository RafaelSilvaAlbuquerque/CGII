#ifndef ILUMINACAO_H
#define ILUMINACAO_H
#include <algebra.h>
#include <stdlib.h>
#include <vect.h>

struct light{

    bool estado = false;
    vect coordenadas;
    float L[3][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};

    void state(bool var){
        this->estado = var;
    }

    void coord(float x,float y,float z){
        this->coordenadas.x = x;
        this->coordenadas.y = y;
        this->coordenadas.z = z;
        this->coordenadas.v = 1.0;
    }

    void comp_ambiente(float r,float g,float b){
        this->L[0][0] = r;
        this->L[0][1] = g;
        this->L[0][2] = b;
    }

    void comp_difusa(float r,float g,float b){
        this->L[1][0] = r;
        this->L[1][1] = g;
        this->L[1][2] = b;
    }

    void comp_specular(float r,float g,float b){
        this->L[2][0] = r;
        this->L[2][1] = g;
        this->L[2][2] = b;
    }
};

struct setlight{
    int id;
    light lig;
    setlight *prox;
};

class Iluminacao
{
public:
    Iluminacao();

    float coordlight[4];
    float L[3][3];

    int indice_light = 0;
    setlight *fontes = NULL;

    //adiciona uma fonte de luz ao conjunto de luzes existentes
    //parametro de entrada  (id da fonte, estado ligado desligado, coordenada da fonte, matriz das componentes da fonte)
    void adlight(light ligh);

    //retorna o vetor com a intensidade para cada uma das componentes
    //parametros de entrada (fonte de luz, coordenadas do ponto de observação, vetor normal, valor do expoente para a função specular, matriz com as componentes de reflexão)
    vect phong(vect ponto_camera,vect ponto_luz,vect vetor_normal,vect raio_refletido,float (*R)[3],float (*L)[3],float exp,float distancia,bool intershadow,bool atenuation);

    //calcula a reflexão ambiente para cada uma das componentes
    //parametros de entrada (matriz com as componenetes de reflexão do material, matriz resultante)
    void ambiente(float (*L)[3],float (*R)[3],float (*I)[3]);

    //calcula a reflexão difusa para cada uma das componentes
    //parametros de entrada (coordenadas da fonte de luz, vetor nnormal, matriz com as componentes da iluminação, matriz com as componenetes de reflexão do material, matriz resultante)
    void difusa(vect l,vect n,float (*L)[3],float (*R)[3],float (*I)[3]);

    //calcula a reflexão specular para cada uma das componentes
    //parametros de entrada (vetor do raio refletido, coordenadas do ponto de observação, valor do expoente, matriz resultante)
    void specular(vect r,vect v,float exp,float (*L)[3],float (*R)[3],float (*I)[3]);



    Algebra _algebra;
};

#endif // ILUMINACAO_H
