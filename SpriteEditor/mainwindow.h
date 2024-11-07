#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
<<<<<<< HEAD
#include <QColorDialog>
=======
#include "model.h"
#include "canvas.h"
>>>>>>> 110aae1f3c07af8e309e1bf14129ade534c5686c

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Model& model, int canvasSize = 64, QWidget *parent = nullptr) ;
    ~MainWindow();

private slots:
<<<<<<< HEAD
    void getCustomColor();
=======
    void updateCanvasDisplay();  // Slot to update the canvas display
>>>>>>> 110aae1f3c07af8e309e1bf14129ade534c5686c

private:
    Ui::MainWindow *ui;
    Canvas *canvas;  // Pointer to Canvas
    Model *model;    // Pointer to Model
};

#endif // MAINWINDOW_H
