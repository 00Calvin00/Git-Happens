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
    connect(ui->darkBrown, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(60, 10, 0)); });
    connect(ui->brown, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(86, 39, 4)); });
    connect(ui->lightBrown, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(139, 70, 0)); });

    connect(ui->burgundy, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(129, 0, 0)); });
    connect(ui->darkRed, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(185, 0, 0)); });
    connect(ui->red, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(245, 0, 0)); });
    connect(ui->burntOrange, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(191, 54, 12)); });
    connect(ui->orange, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(230, 81, 0)); });
    connect(ui->brightOrange, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(255, 111, 0)); });
    connect(ui->gold, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(255, 150, 38)); });
    connect(ui->yellow, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(255, 200, 9)); });
    connect(ui->brightYellow, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(255, 255, 0)); });
    connect(ui->forestGreen, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(0, 63, 0)); });
    connect(ui->green, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(0, 150, 0)); });
    connect(ui->neonGreen, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(0, 235, 0)); });
    connect(ui->darkTeal, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(5, 60, 88)); });
    connect(ui->teal, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(0, 128, 128)); });
    connect(ui->cyan, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(0, 245, 245)); });
    connect(ui->darkBlue, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(1, 3, 147)); });
    connect(ui->blue, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(0, 0, 245)); });
    connect(ui->skyBlue, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(0, 170, 255)); });
    connect(ui->indigo, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(70, 0, 127)); });
    connect(ui->violet, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(148, 0, 211)); });
    connect(ui->purple, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(205, 0, 211)); });
    connect(ui->plum, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(109, 0, 82)); });
    connect(ui->berry, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(170, 0, 127)); });
    connect(ui->magenta, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(255, 0, 160)); });


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
