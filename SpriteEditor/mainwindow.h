/* Reviewed by: Kenna */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QTimer>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include "framemanager.h"
#include "canvas.h"
#include "background.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(FrameManager& model, int canvasSize, QWidget *parent = nullptr) ;
    ~MainWindow();
    Canvas *canvas;  // Pointer to Canvas
    Background *background;

private slots:
    void updateCanvasDisplay();
    void updateColorWithCustom();
    void updateColorWithPreset(QColor);
    void onSaveTriggered();

    /**
     * @brief Loads a project from a JSON file, setting frames and scale on the canvas.
     *
     * Opens a file dialog for the user to select a JSON file. If the file is valid,
     * it reads the frame data and scale, updates the model with the loaded frames,
     * and sets the canvas to the appropriate scale.
     */
    void onLoadTriggered();

    void OnNewTriggered();

    /**
     * @brief onNewTriggered
     */

    void FrameListChanged(int newIndex, QPixmap* newFrame);
    void OnFrameSelected(int);
    void UpdateSelectedFrameIcon();
    void DeleteFramePopUp();
    void DeleteFramePopUpClose();
    void IteratePreview();

private:
    void initializeCanvas(int canvasSize);
    Ui::MainWindow *ui;
    FrameManager *model;    // Pointer to Model
    int curPreviewIndex = 0;
    int fps = 5;
    QTimer* previewIterationTimer;
    QTimer* selectedFrameTimer;
};

#endif // MAINWINDOW_H
