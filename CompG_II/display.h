#ifndef DISPLAY_H
#define DISPLAY_H
#include <QGLWidget>
#include <QImage>
#include <camera.h>
#include <iluminacao.h>
#include "objeto.h"
#include <vect.h>

class Display : public QGLWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = 0);

    QImage draw(QImage image,int li,int co);
    //void initializeGL();
    //void paintGL();
    //void resizeGL(int w, int h);

    Camera cam;
    Iluminacao _light;
    Objeto _obj;
    Algebra _algebra;
    Matrix4x4 _matrix;

signals:

public slots:

};

#endif // DISPLAY_H
