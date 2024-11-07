#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"

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
