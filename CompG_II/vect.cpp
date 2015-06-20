#include "vect.h"

vect::vect()
{
}

void vect::set(float x,float y,float z,float v){
    this->x = x;
    this->y = y;
    this->z = z;
    this->v = v;
}

void vect::set(float x,float y,float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

