#ifndef VECT_H
#define VECT_H

class vect
{
public:
    vect();

    float x=0.0,y=0.0,z=0.0,v=0.0;

    void set(float x,float y,float z,float v);
    void set(float x,float y,float z);
};

#endif // VECT_H
