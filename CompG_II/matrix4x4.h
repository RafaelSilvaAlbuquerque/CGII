#ifndef MATRIX4X4_H
#define MATRIX4X4_H
#include <vect.h>
#include <math.h>

class Matrix4x4
{
public:
    Matrix4x4();

    float matriz[16];
    float invmatriz[16];
    bool enable = false;

    void printMatrix();
    void printInvMatrix();
    Matrix4x4 multmatrix(Matrix4x4 m1,Matrix4x4 m2);
    void escala(float x,float y,float z);
    void translate(float x,float y,float z);
    vect transform(vect vetor);
    vect invtransform(vect vetor);
    Matrix4x4 transposta(Matrix4x4 matriz1);
    void setmatrix(float *vetor);
    Matrix4x4 getmatrix();
    Matrix4x4 getinvmatrix();
    vect multvetmatrix(Matrix4x4 matrix,vect vetor);
    void resetmatrix();
    void rotate_xaxis(float angulo);
    void rotate_yaxis(float angulo);
    void rotate_zaxis(float angulo);

};

#endif // MATRIX4X4_H
