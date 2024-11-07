#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include "model.h"
#include "canvas.h"

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
    void updateCanvasDisplay();             // Update the canvas display
    void updateColorWithCustom();           // Update the color to custom color
    void updateColorWithPreset(QColor);     // Update the color to quick access color

private:
    Ui::MainWindow *ui;
    Canvas *canvas;  // Pointer to Canvas
    Model *model;    // Pointer to Model
};

#endif // MAINWINDOW_H
