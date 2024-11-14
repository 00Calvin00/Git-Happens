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

/**
 * @class MainWindow
 * @brief The MainWindow class is the primary user interface class, which includes
 * the main window and its associated actions and slots. It manages the canvas display,
 * user interactions, and frame operations for the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for MainWindow
     * @param model Reference to the FrameManager model that manages frames
     * @param canvasSize Initial size for the canvas
     * @param parent Parent widget (optional)
     */
    explicit MainWindow(FrameManager& model, QWidget *parent = nullptr) ;

    /**
     * @brief Destructor for MainWindow
     */
    ~MainWindow();

    Canvas *canvas;
    Background *background;

private slots:
    /**
     * @brief Updates the canvas display after a change in frame or other graphical updates.
     */
    void updateCanvasDisplay();

    /**
     * @brief Opens a custom color picker dialog for the user to select a color.
     */
    void updateColorWithCustom();

    /**
     * @brief Updates the color using a preset color selection.
     * @param QColor The chosen color to be applied.
     */
    void updateColorWithPreset(QColor);

    /**
     * @brief Opens a save dialog and saves the current project.
     *
     * Opens a file dialog for the user to select a location and filename for saving the project as a JSON file.
     * If the location is valid, it writes the current frame data and scale to the selected JSON file,
     * ensuring the model data and canvas settings are saved for future access.
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

    /**
     * @brief Creates a new project by resetting the frames and canvas.
     *
     * Resets the canvas and frame data to create a new project.
     * Clears existing frames and resets the scale, initializing the canvas to default settings
     * for a fresh start on a new project.
     */
    void onNewTriggered();

    /**
     * @brief Updates the frame list when a new frame is added or an existing frame is modified.
     * @param newIndex Index of the frame to be updated
     * @param newFrame Pointer to the new QPixmap frame to be displayed
     */
    void frameListChanged(int newIndex, QPixmap* newFrame);

    /**
     * @brief Selects a frame based on the user's interaction with the frame list.
     * @param index Index of the selected frame
     */
    void onFrameSelected(int);

    /**
     * @brief Updates the icon display of the currently selected frame in the frame list.
     */
    void updateSelectedFrameIcon();

    /**
     * @brief Displays a popup to confirm deletion of a frame.
     */
    void deleteFramePopUp();

    /**
     * @brief Closes the delete frame popup dialog without deleting the frame.
     */
    void deleteFramePopUpClose();

    /**
     * @brief Advances the frame in the preview, providing a looped preview playback of frames.
     */
    void iteratePreview();

private:
    Ui::MainWindow *ui;
    FrameManager *model;
    QTimer* previewIterationTimer;
    QTimer* selectedFrameTimer;
    int previousFrameIndex = 0;
    int fps = 5;
};

#endif // MAINWINDOW_H
