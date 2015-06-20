#include "algebra.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

Algebra::Algebra()
{
}

float Algebra::modulo(float var){

    if(var < 0.0){
        var = var*(-1.0);
    }

    return var;
}

//calcula norma de um vetor
//parametro (float vetor)
float Algebra::vet_norma(vect vetor){

    return sqrt((vetor.x*vetor.x)+(vetor.y*vetor.y)+(vetor.z*vetor.z));
}

//calcula vetor unitário
//parametro (float vetor)
vect Algebra::vet_unit(vect vetor){

    float norma_vetor = vet_norma(vetor);

    vect result;

    result.x = vetor.x*(1/norma_vetor);
    result.y = vetor.y*(1/norma_vetor);
    result.z = vetor.z*(1/norma_vetor);
    result.v = 0.0;

    return result;
}

//calcula o produto vetorial do primeiro vetor pelo segundo
//parametro (float primeiro vetor, float segundo vetor)
vect Algebra::vet_prtvetorial(vect vetor1,vect vetor2){

    vect result;

    result.x = (vetor1.y*vetor2.z)-(vetor1.z*vetor2.y);
    result.y = (vetor1.z*vetor2.x)-(vetor1.x*vetor2.z);
    result.z = (vetor1.x*vetor2.y)-(vetor1.y*vetor2.x);

    return result;
}

//calcula o produto escalar do primeiro pelo segundo vetor
//parametro (float primeiro vetor, float segundo vetor)
float Algebra::vet_prtescalar(vect vetor1,vect vetor2){
    return ((vetor1.x*vetor2.x)+(vetor1.y*vetor2.y)+(vetor1.z*vetor2.z));
}

//calcula produto da primeira matriz pela segundo
//parametro (float primeira matriz, float segunda matriz)
//float* Algebra::matriz_prodt(float matriz1,float matriz2){

//}

//calcula o vetor distáncia subtraindo o segundo vetor do primeiro
//parametro (float primeiro vetor, float segundo vetor)

vect Algebra::vet_dist(vect vetor1,vect vetor2){

    vect result;

    result.x = (vetor1.x-vetor2.x);
    result.y = (vetor1.y-vetor2.y);
    result.z = (vetor1.z-vetor2.z);

    return result;
}

vect Algebra::invert(vect vetor){

    vetor.x = vetor.x*-1.0;
    vetor.y = vetor.y*-1.0;
    vetor.z = vetor.z*-1.0;

    return vetor;
}

vect Algebra::subvetores(vect vetor1,vect vetor2){

    vect result;

    result.x = vetor1.x-vetor2.x;
    result.y = vetor1.y-vetor2.y;
    result.z = vetor1.z-vetor2.z;

    return result;
}

//num elevado a exp
//parametro (float número a ser elevado, int expoente da potência)
float Algebra::potencia(float num,int exp){

    float var=1;

    for(int i=1;i<=exp;i++){
        var = var*num;
    }

    if(exp == 0){
        return 1;
    }

    return var;
}



/*
float* Algebra::pmatriz_vetor(float (*matriz)[4],float *vetor){

    float *result = (float*)calloc(3,sizeof(float));
    result[0] = 0.0;
    result[1] = 0.0;
    result[2] = 0.0;
    result[3] = 0.0;

    for(int i=0;i<=3;i++){
        for(int j=0;j<=3;j++){
            result[i] = result[i] + matriz[i][j] * vetor[j];
        }
    }
}
*/

//calcula o produto de um vetor por uma matriz
//parametro (float vetor de entrada, float matriz de entrada)
//void Algebra::Mat_vet(float *vetor,float (*matriz)[4]){
//
//}
//------------------------------------------------------------------------------------------------
//calcula vetor unitário
//parametro (float vetor)
