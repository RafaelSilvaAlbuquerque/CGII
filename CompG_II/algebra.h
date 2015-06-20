#ifndef ALGEBRA_H
#define ALGEBRA_H
#include <vect.h>

class Algebra
{
public:
    Algebra();

    //calcula vetor unitário
    //parametro (float vetor)
    vect vet_unit(vect vetor);

    //calcula norma de um vetor
    //parametro (float vetor)
    float vet_norma(vect vetor);

    //calcula o produto vetorial do primeiro vetor pelo segundo
    //parametro (float primeiro vetor, float segundo vetor)
    vect vet_prtvetorial(vect vetor1,vect vetor2);

    //calcula produto matricial da primeira matriz pela segundo
    //parametro (float primeira matriz, float segunda matriz)
    //float matriz_prodt(float matriz1,float matriz2);

    //calcula o vetor distáncia subtraindo o segundo vetor do primeiro
    //parametro (float primeiro vetor, float segundo vetor)
    vect vet_dist(vect vetor1, vect vetor2);

    //calcula o produto escalar do primeiro pelo segundo vetor
    //parametro (float primeiro vetor, float segundo vetor)
    float vet_prtescalar(vect vetor1,vect vetor2);

    //num elevado a exp
    //parametro (float número a ser elevado, int expoente da potência)
    float potencia(float num,int exp);

    vect invert(vect vetor);

    vect subvetores(vect vetor1,vect vetor2);

    float modulo(float var);
};

#endif // ALGEBRA_H
