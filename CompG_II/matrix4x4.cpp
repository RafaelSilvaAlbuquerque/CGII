#include "matrix4x4.h"
#include <stdlib.h>
#include <stdio.h>

Matrix4x4::Matrix4x4()
{
    this->resetmatrix();
}

void Matrix4x4::resetmatrix(){
    for(int i=0;i<=15;i++){
        if(i%5 == 0){
            this->matriz[i] = 1.0;
            this->invmatriz[i] = 1.0;
        }
        else{
            this->matriz[i] = 0.0;
            this->invmatriz[i] = 0.0;
        }
    }
}


Matrix4x4 Matrix4x4::getmatrix(){

    Matrix4x4 result;
    result.setmatrix(this->matriz);
    return result;
}

Matrix4x4 Matrix4x4::getinvmatrix(){

    Matrix4x4 result;
    result.setmatrix(this->invmatriz);
    return result;
}

void Matrix4x4::setmatrix(float *vetor){

    for(int i=0;i<=15;i++){
        this->matriz[i] = vetor[i];
    }
}

Matrix4x4 Matrix4x4::multmatrix(Matrix4x4 m1,Matrix4x4 m2){

    Matrix4x4 result;

    result.matriz[0] = m1.matriz[0]*m2.matriz[0] + m1.matriz[1]*m2.matriz[4] + m1.matriz[2]*m2.matriz[8] + m1.matriz[3]*m2.matriz[12];
    result.matriz[1] = m1.matriz[0]*m2.matriz[1] + m1.matriz[1]*m2.matriz[5] + m1.matriz[2]*m2.matriz[9] + m1.matriz[3]*m2.matriz[13];
    result.matriz[2] = m1.matriz[0]*m2.matriz[2] + m1.matriz[1]*m2.matriz[6] + m1.matriz[2]*m2.matriz[10] + m1.matriz[3]*m2.matriz[14];
    result.matriz[3] = m1.matriz[0]*m2.matriz[3] + m1.matriz[1]*m2.matriz[7] + m1.matriz[2]*m2.matriz[11] + m1.matriz[3]*m2.matriz[15];

    result.matriz[4] = m1.matriz[4]*m2.matriz[0] + m1.matriz[5]*m2.matriz[4] + m1.matriz[6]*m2.matriz[8] + m1.matriz[7]*m2.matriz[12];
    result.matriz[5] = m1.matriz[4]*m2.matriz[1] + m1.matriz[5]*m2.matriz[5] + m1.matriz[6]*m2.matriz[9] + m1.matriz[7]*m2.matriz[13];
    result.matriz[6] = m1.matriz[4]*m2.matriz[2] + m1.matriz[5]*m2.matriz[6] + m1.matriz[6]*m2.matriz[10] + m1.matriz[7]*m2.matriz[14];
    result.matriz[7] = m1.matriz[4]*m2.matriz[3] + m1.matriz[5]*m2.matriz[7] + m1.matriz[6]*m2.matriz[11] + m1.matriz[7]*m2.matriz[15];

    result.matriz[8] = m1.matriz[8]*m2.matriz[0] + m1.matriz[9]*m2.matriz[4] + m1.matriz[10]*m2.matriz[8] + m1.matriz[11]*m2.matriz[12];
    result.matriz[9] = m1.matriz[8]*m2.matriz[1] + m1.matriz[9]*m2.matriz[5] + m1.matriz[10]*m2.matriz[9] + m1.matriz[11]*m2.matriz[13];
    result.matriz[10] = m1.matriz[8]*m2.matriz[2] + m1.matriz[9]*m2.matriz[6] + m1.matriz[10]*m2.matriz[10] + m1.matriz[11]*m2.matriz[14];
    result.matriz[11] = m1.matriz[8]*m2.matriz[3] + m1.matriz[9]*m2.matriz[7] + m1.matriz[10]*m2.matriz[11] + m1.matriz[11]*m2.matriz[15];

    result.matriz[12] = m1.matriz[12]*m2.matriz[0] + m1.matriz[13]*m2.matriz[4] + m1.matriz[14]*m2.matriz[8] + m1.matriz[15]*m2.matriz[12];
    result.matriz[13] = m1.matriz[12]*m2.matriz[1] + m1.matriz[13]*m2.matriz[5] + m1.matriz[14]*m2.matriz[9] + m1.matriz[15]*m2.matriz[13];
    result.matriz[14] = m1.matriz[12]*m2.matriz[2] + m1.matriz[13]*m2.matriz[6] + m1.matriz[14]*m2.matriz[10] + m1.matriz[15]*m2.matriz[14];
    result.matriz[15] = m1.matriz[12]*m2.matriz[3] + m1.matriz[13]*m2.matriz[7] + m1.matriz[14]*m2.matriz[11] + m1.matriz[15]*m2.matriz[15];

    return result;
}


vect Matrix4x4::multvetmatrix(Matrix4x4 matrix,vect vetor){

    vect result;

    result.x = (vetor.x * matrix.matriz[0])+(vetor.y * matrix.matriz[1])+(vetor.z * matrix.matriz[2])+(vetor.v * matrix.matriz[3]);
    result.y = (vetor.x * matrix.matriz[4])+(vetor.y * matrix.matriz[5])+(vetor.z * matrix.matriz[6])+(vetor.v * matrix.matriz[7]);
    result.z = (vetor.x * matrix.matriz[8])+(vetor.y * matrix.matriz[9])+(vetor.z * matrix.matriz[10])+(vetor.v * matrix.matriz[11]);
    result.v = (vetor.x * matrix.matriz[12])+(vetor.y * matrix.matriz[13])+(vetor.z * matrix.matriz[14])+(vetor.v * matrix.matriz[15]);

    return result;
}


void Matrix4x4::translate(float x,float y,float z){

    this->matriz[3] = x;
    this->matriz[7] = y;
    this->matriz[11] = z;

    this->invmatriz[3] = -1*x;
    this->invmatriz[7] = -1*y;
    this->invmatriz[11] = -1*z;
}

void Matrix4x4::escala(float x,float y,float z){

    this->matriz[0] = this->matriz[0]*x;
    this->matriz[5] = this->matriz[5]*y;
    this->matriz[10] = this->matriz[10]*z;

    this->invmatriz[0] = this->invmatriz[0]/x;
    this->invmatriz[5] = this->invmatriz[5]/y;
    this->invmatriz[10] = this->invmatriz[10]/z;
}

void Matrix4x4::rotate_xaxis(float angulo){

    float pi = 3.14159265359;
    float radianos = (pi*angulo)/180.0;

    this->matriz[5] = cos(radianos);
    this->matriz[6] = -1*sin(radianos);
    this->matriz[9] = sin(radianos);
    this->matriz[10] = cos(radianos);

    this->invmatriz[5] = cos(-1*radianos);
    this->invmatriz[6] = -1*sin(-1*radianos);
    this->invmatriz[9] = sin(-1*radianos);
    this->invmatriz[10] = cos(-1*radianos);
}

void Matrix4x4::rotate_yaxis(float angulo){

    float pi = 3.14159265359;
    float radianos = (pi*angulo)/180.0;

    this->matriz[0] = cos(radianos);
    this->matriz[2] = sin(radianos);
    this->matriz[8] = -1*sin(radianos);
    this->matriz[10] = cos(radianos);

    this->invmatriz[0] = cos(-1*radianos);
    this->invmatriz[2] = sin(-1*radianos);
    this->invmatriz[8] = -1*sin(-1*radianos);
    this->invmatriz[10] = cos(-1*radianos);
}

void Matrix4x4::rotate_zaxis(float angulo){

    float pi = 3.14159265359;
    float radianos = (pi*angulo)/180.0;

    this->matriz[0] = cos(radianos);
    this->matriz[1] = -1*sin(radianos);
    this->matriz[4] = sin(radianos);
    this->matriz[5] = cos(radianos);

    this->invmatriz[0] = cos(-1*radianos);
    this->invmatriz[1] = -1*sin(-1*radianos);
    this->invmatriz[4] = sin(-1*radianos);
    this->invmatriz[5] = cos(-1*radianos);
}

vect Matrix4x4::transform(vect vetor){

    vect result;

    result.x = (vetor.x * this->matriz[0])+(vetor.y * this->matriz[1])+(vetor.z * this->matriz[2])+(vetor.v * this->matriz[3]);
    result.y = (vetor.x * this->matriz[4])+(vetor.y * this->matriz[5])+(vetor.z * this->matriz[6])+(vetor.v * this->matriz[7]);
    result.z = (vetor.x * this->matriz[8])+(vetor.y * this->matriz[9])+(vetor.z * this->matriz[10])+(vetor.v * this->matriz[11]);
    result.v = (vetor.x * this->matriz[12])+(vetor.y * this->matriz[13])+(vetor.z * this->matriz[14])+(vetor.v * this->matriz[15]);

    return result;
}

vect Matrix4x4::invtransform(vect vetor){

    vect result;

    result.x = (vetor.x * this->invmatriz[0])+(vetor.y * this->invmatriz[1])+(vetor.z * this->invmatriz[2])+(vetor.v * this->invmatriz[3]);
    result.y = (vetor.x * this->invmatriz[4])+(vetor.y * this->invmatriz[5])+(vetor.z * this->invmatriz[6])+(vetor.v * this->invmatriz[7]);
    result.z = (vetor.x * this->invmatriz[8])+(vetor.y * this->invmatriz[9])+(vetor.z * this->invmatriz[10])+(vetor.v * this->invmatriz[11]);
    result.v = (vetor.x * this->invmatriz[12])+(vetor.y * this->invmatriz[13])+(vetor.z * this->invmatriz[14])+(vetor.v * this->invmatriz[15]);

    return result;
}

Matrix4x4 Matrix4x4::transposta(Matrix4x4 matriz1){

    Matrix4x4 result;

    result.matriz[0] = matriz1.matriz[0];
    result.matriz[1] = matriz1.matriz[4];
    result.matriz[2] = matriz1.matriz[8];
    result.matriz[3] = matriz1.matriz[12];

    result.matriz[4] = matriz1.matriz[1];
    result.matriz[5] = matriz1.matriz[5];
    result.matriz[6] = matriz1.matriz[9];
    result.matriz[7] = matriz1.matriz[13];

    result.matriz[8] = matriz1.matriz[2];
    result.matriz[9] = matriz1.matriz[6];
    result.matriz[10] = matriz1.matriz[10];
    result.matriz[11] = matriz1.matriz[14];

    result.matriz[12] = matriz1.matriz[3];
    result.matriz[13] = matriz1.matriz[7];
    result.matriz[14] = matriz1.matriz[11];
    result.matriz[15] = matriz1.matriz[15];

    return result;
}

void Matrix4x4::printMatrix(){
    printf("%f %f %f %f\n",this->matriz[0],this->matriz[1],this->matriz[2],this->matriz[3]);
    printf("%f %f %f %f\n",this->matriz[4],this->matriz[5],this->matriz[6],this->matriz[7]);
    printf("%f %f %f %f\n",this->matriz[8],this->matriz[9],this->matriz[10],this->matriz[11]);
    printf("%f %f %f %f\n",this->matriz[12],this->matriz[13],this->matriz[14],this->matriz[15]);
}

void Matrix4x4::printInvMatrix(){
    printf("%f %f %f %f\n",this->invmatriz[0],this->invmatriz[1],this->invmatriz[2],this->invmatriz[3]);
    printf("%f %f %f %f\n",this->invmatriz[4],this->invmatriz[5],this->invmatriz[6],this->invmatriz[7]);
    printf("%f %f %f %f\n",this->invmatriz[8],this->invmatriz[9],this->invmatriz[10],this->invmatriz[11]);
    printf("%f %f %f %f\n",this->invmatriz[12],this->invmatriz[13],this->invmatriz[14],this->invmatriz[15]);
}
