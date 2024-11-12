#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QTimer>
#include "spritemodel.h"
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
    Canvas *canvas;  // Pointer to Canvas
    ~MainWindow();

private slots:
    void updateCanvasDisplay();             // Update the canvas display
    void updateColorWithCustom();           // Update the color to custom color
    void updateColorWithPreset(QColor);     // Update the color to quick access color
    void onSaveTriggered();
    void onLoadTriggered();
    void FrameListChanged(int newIndex, QPixmap* newMap);
    void onFrameSelected(int i);
    void UpdateAnimation(QList<QPixmap*> newPixMap);
    void DeleteFramePopUp();
    void DeleteFramePopUpClose();
    void IteratePreview();

private:
    Ui::MainWindow *ui;
    Model *model;    // Pointer to Model
    int curPreviewIndex = 0;
    int fps = 60;
    QTimer* previewIterationTimer;
};

#endif // MAINWINDOW_H
