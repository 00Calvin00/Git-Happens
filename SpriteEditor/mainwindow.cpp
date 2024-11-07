#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include "jsonreader.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(Model& model, int canvasSize, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(&model)
{
    ui->setupUi(this);
    this->showMaximized();

    connect(ui->customColor, &QPushButton::clicked, this, &MainWindow::getCustomColor);

    this->showMaximized(); //Fill the screen with the sprite editor

    canvas = ui->uiCanvas;
    canvas->setFixedSize(canvasSize * 10, canvasSize * 10); //need to swap out 10 for scale variable

    // Center canvas in the main window
    int centerX = (this->width() - canvas->width()) / 2;
    int centerY = (this->height() - canvas->height()) / 2;
    canvas->move(centerX, centerY);

    // Connect the signal for drawing updates
    connect(canvas, &Canvas::updateCanvas, this, &MainWindow::updateCanvasDisplay);

    //connect(ui->redButton, &QPushButton::clicked, &model, &Model::verifyClickRed); Example from another project. Do we need to connect the view to the canvas as well similar to this to handle when the user clicks the canvas?

    // Initialize the display with the current canvas state
    updateCanvasDisplay();

    // Connect the save Action from the File Menu to a slot
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onActionSaveTriggered);
}

void MainWindow::updateCanvasDisplay()
{
    canvas->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getCustomColor()
{
    // Open the QColorDialog and get the selected color
    QColor selectedColor = QColorDialog::getColor(Qt::white, this, "Select a Color");

    if (selectedColor.isValid()) {
        // If the color is valid, set the background color of the button
        ui->customColor->setStyleSheet(QString("background-color: %1").arg(selectedColor.name()));
    }
}

void MainWindow::onActionSaveTriggered()
{
    // Open a QFileDialog to let the user choose where to save the file
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("Save Pixmap"),
                                                    "",
                                                    tr("JSON Files (*.json);;All Files (*)"));

    // Check if a file path was selected
    if (!filePath.isEmpty()) {
        // Get the QPixmap from your canvas
        QPixmap pixmap = canvas->getPixmap();  // Assuming canvas is a pointer to your Canvas instance

        // Pass the file path and pixmap to your save function
        if (JsonReader::savePixmapToJson(pixmap, filePath)) {
            QMessageBox::information(this, tr("Save Successful"), tr("File saved successfully!"));
        } else {
            QMessageBox::warning(this, tr("Save Failed"), tr("Could not save the file."));
        }
    }
}
