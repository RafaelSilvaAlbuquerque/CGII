#include "iluminacao.h"
#include <stdio.h>
#include <stdlib.h>

Iluminacao::Iluminacao()
{
}

//adiciona uma fonte de luz ao conjunto de luzes existentes
//parametro de entrada  (id da fonte, estado ligado desligado, coordenada da fonte, matriz das componentes da fonte)
void Iluminacao::adlight(light ligh){

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

//calcula a reflexão ambiente para cada uma das componentes
//parametros de entrada (matriz com as componenetes de reflexão do material, matriz resultante)
void Iluminacao::ambiente(float (*L)[3],float (*R)[3],float (*I)[3]){

    float var=0;

    for(int coluna=0;coluna<=2;coluna++){

        var = (L[0][coluna]*R[0][coluna]);

        I[0][coluna] = I[0][coluna]+var;

    }
}

//calcula a reflexão difusa para cada uma das componentes
//parametros de entrada (coordenadas da fonte de luz, vetor nnormal, matriz com as componentes da iluminação, matriz com as componenetes de reflexão do material, matriz resultante)
void Iluminacao::difusa(vect l,vect n,float (*L)[3],float (*R)[3],float (*I)[3]){

    float var=0;

    float con = _algebra.vet_prtescalar(l,n);

    if(con < 0){
        con = 0.0;
    }

    for(int coluna=0;coluna<=2;coluna++){

        var = (L[1][coluna]*R[1][coluna]*con);

        I[1][coluna] = I[1][coluna]+var;
    }
}

//calcula a reflexão specular para cada uma das componentes
//parametros de entrada (vetor do raio refletido, coordenadas do ponto de observação, valor do expoente, matriz resultante)
void Iluminacao::specular(vect r,vect v,float exp,float (*L)[3],float (*R)[3],float (*I)[3]){

    float var=0;
    float con = _algebra.potencia(_algebra.vet_prtescalar(r,v),exp);

    if(_algebra.vet_prtescalar(r,v) < 0){
        con = 0;
    }

    for(int coluna=0;coluna<=2;coluna++){

        var = (L[2][coluna]*R[2][coluna]*con);

        I[2][coluna] = I[2][coluna]+var;
    }
}

//retorna o vetor com a intensidade para cada uma das componentes
//parametros de entrada (ponto iluminado, coordenadas do ponto de observação, vetor normal, valor do expoente para a função specular)
vect Iluminacao::phong(vect ponto_camera,vect ponto_luz,vect vetor_normal,vect raio_refletido,float (*R)[3],float (*L)[3],float exp,float distancia,bool intershadow,bool atenuation){//float *ponto,float *v,float *n, float exp,float (*R)[3],float distancia,bool intershadow

    float I[3][3];//matriz resultante da execução dos calculos para luz ambiente, difusa, specular

    vect result;

    //inicializa a matriz I[3][3] com 0's
    for(int i=0;i<=2;i++){
        for(int j=0;j<=2;j++){
            I[i][j] = 0;
        }
    }

    this->ambiente(L,R,I);

    if(intershadow != true){

        this->difusa(ponto_luz,vetor_normal,L,R,I);
        this->specular(raio_refletido,ponto_camera,exp,L,R,I);
    }


    float invqdist = 1.0;
    if(distancia > 0.0){
        invqdist = (1/(distancia*distancia));
    }

    if(atenuation){
        result.x = I[0][0]+(I[1][0]+I[2][0])*invqdist;
        result.y = I[0][1]+(I[1][1]+I[2][1])*invqdist;
        result.z = I[0][2]+(I[1][2]+I[2][2])*invqdist;
    }
    else{
        result.x = I[0][0]+I[1][0]+I[2][0];
        result.y = I[0][1]+I[1][1]+I[2][1];
        result.z = I[0][2]+I[1][2]+I[2][2];
    }

    return result;
}

