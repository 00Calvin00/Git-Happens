#include "mainwindow.h"
#include "model.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model model;  // Create an instance of Model
    MainWindow w(model, 64);  // Pass the model to the MainWindow and set default canvas size
    w.show();
    return a.exec();
}
