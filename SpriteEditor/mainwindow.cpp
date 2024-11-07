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
    this->showMaximized(); // Fill the screen with the sprite editor

    // Connect the signal for custom color selection
    connect(ui->customColor, &QPushButton::clicked, this, &MainWindow::updateColorWithCustom);

    // Connect the signal for quick access colors
    connect(ui->white, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor("white")); });
    connect(ui->gray, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(127, 127, 127)); });
    connect(ui->black, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor("black")); });

    // Set up the canvas
    canvas = ui->uiCanvas;
    canvas->setFixedSize(canvasSize * 10, canvasSize * 10); //need to swap out 10 for scale variable

    // Center canvas in the main window
    int centerX = (this->width() - canvas->width()) / 2;
    int centerY = (this->height() - canvas->height()) / 2;
    canvas->move(centerX, centerY);

    // Connect the signal for drawing updates
    connect(canvas, &Canvas::updateCanvas, this, &MainWindow::updateCanvasDisplay);

    // Initialize the display with the current canvas state
    updateCanvasDisplay();

    // Connect the save Action from the File Menu to a save action slot
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveTriggered);

    // Connect the load Action from the File Menu to a load action slot
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onLoadTriggered);
}

void MainWindow::updateCanvasDisplay()
{
    canvas->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateColorWithCustom()
{
    // Open the QColorDialog and get the selected color
    canvas->penColor = QColorDialog::getColor(Qt::white, this, "Select a Color");

    if (canvas->penColor.isValid()) {
        // Set the color of the button to show current custom color being used
        ui->customColor->setStyleSheet(QString("background-color: %1").arg(canvas->penColor.name()));
    }
}

void MainWindow::onSaveTriggered()
{
    // Open QFileDialog for user to choose a filepath
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Pixmap"), "", tr("JSON Files (*.json);;All Files (*)"));

    // Check if a path was actually selected
    if (!filePath.isEmpty()) {

        // Get current QPixmap from canvas
        QPixmap pixmap = canvas->getPixmap();

        // Save Json file using QPixmap and the user-selected file path
        if (JsonReader::savePixmapToJson(pixmap, filePath)) {
            QMessageBox::information(this, tr("Save Successful"), tr("File saved successfully!"));
        } else {
            QMessageBox::warning(this, tr("Save Failed"), tr("Could not save the file."));
        }
    }

void MainWindow::onLoadTriggered()
{
    // Open QFileDialog for user to choose a filepath to load the pixmap from
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Pixmap"), "", tr("JSON Files (*.json);;All Files (*)"));

    // Check if a path was selected
    if (!filePath.isEmpty()) {
        // Load the pixmap using JsonReader
        QPixmap pixmap;
        JsonReader jsonReader;

        if (jsonReader.loadPixmapFromJson(pixmap, filePath)) {
            // Successfully loaded the pixmap, update the canvas
            canvas->setPixmap(pixmap); // Assuming you have a setter for pixmap in your Canvas class

            // Optionally, you can update the canvas display (it might already be done in setPixmap)
            canvas->repaint();

            QMessageBox::information(this, tr("Load Successful"), tr("File loaded successfully!"));
        } else {
            QMessageBox::warning(this, tr("Load Failed"), tr("Could not load the file."));
        }
    }
}

  
void MainWindow::updateColorWithPreset(QColor color)
{
    canvas->penColor = color;
}
