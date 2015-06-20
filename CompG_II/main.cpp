#include "mainwindow.h"
#include "display.h"
#include <QApplication>
#include <QLabel>
#include "iluminacao.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    int linha=800,coluna=800;

    QImage image(linha, coluna, QImage::Format_RGB32);

    Display tela;

    QLabel myLabel;
    myLabel.setPixmap(QPixmap::fromImage(tela.draw(image,linha,coluna)));
    myLabel.show();

    return a.exec();


}
