#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"

MainWindow::MainWindow(Model& model, int canvasSize, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(&model), canvas(new Canvas(this, canvasSize))
{
    ui->setupUi(this);
    this->showMaximized(); //Fill the screen with the sprite editor

    canvas = ui->uiCanvas;
    canvas->resize(canvasSize, canvasSize);

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
