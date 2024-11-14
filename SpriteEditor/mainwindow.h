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
    explicit MainWindow(FrameManager& model, int canvasSize = 64, QWidget *parent = nullptr) ;
    Canvas *canvas;  // Pointer to Canvas
    Background *background;
    ~MainWindow();

private slots:
    void updateCanvasDisplay();             // Update the canvas display
    void updateColorWithCustom();           // Update the color to custom color
    void updateColorWithPreset(QColor);     // Update the color to quick access color


    /**
     * @brief Saves the current project to a JSON file, including frames and scale.
     *
     * Opens a file dialog for the user to choose a save location and file name.
     * Encodes the current frames and canvas scale in JSON format and writes this data
     * to the specified file. Displays a success or error message based on the outcome.
     */
    void onSaveTriggered();

    /**
     * @brief Loads a project from a JSON file, setting frames and scale on the canvas.
     *
     * Opens a file dialog for the user to select a JSON file. If the file is valid,
     * it reads the frame data and scale, updates the model with the loaded frames,
     * and sets the canvas to the appropriate scale.
     */
    void onLoadTriggered();

    void onNewTriggered();

    /**
     * @brief Adds an initial frame to the project’s frame list of pixmaps.
     *
     * Retrieves the starting frame from the canvas and adds it to the model’s pixmap list
     * and updates the canvas to display this initial frame.
     *
     * @param initialFrame A pointer to the QPixmap representing the initial frame to add.
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
