#include "mainwindow.h"
#include "spritemodel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model spritemodel;  // Create an instance of Model
    MainWindow w(spritemodel, 64);  // Pass the model to the MainWindow and set default canvas size
    w.show();
    return a.exec();
}
