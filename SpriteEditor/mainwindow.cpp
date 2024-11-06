#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"

MainWindow::MainWindow(Model& model, int canvasSize, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(&model), canvas(new Canvas(this))
{
    ui->setupUi(this);
    this->showMaximized(); //Fill the screen with the sprite editor

    // Set up the Canvas widget inside the QGraphicsView (this could be done in the UI Designer as well)
    canvas->setParent(ui->uiCanvas);
    canvas->resize(canvasSize, canvasSize);

    // Connect the signal for drawing updates
    connect(canvas, &Canvas::canvasUpdated, this, &MainWindow::updateCanvasDisplay);

    // Initialize the display with the current canvas state
    updateCanvasDisplay();
}

void MainWindow::updateCanvasDisplay() {
    // Get the current pixmap from the canvas and update the QLabel in the UI
    QPixmap pixmap = canvas->getPixmap();
    ui->canvasLabel->setPixmap(pixmap);  // Use the QLabel from the UI designer
    ui->canvasLabel->adjustSize();  // Adjust size of QLabel to fit the pixmap
}

MainWindow::~MainWindow()
{
    delete ui;
    // Don't need to delete `model` or `canvas` because they're managed by the MainWindow
}
