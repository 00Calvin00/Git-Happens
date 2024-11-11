#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include "jsonreader.h"
#include "canvassizepopup.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(Model& model, int canvasSize, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(&model)
{
    ui->setupUi(this);
    this->showMaximized(); // Fill the screen with the sprite editor
    ui->deleteFramePopUp->setVisible(false); // Hide deletion confirmation popup

    // Create and show the modal dialog
    CanvasSizePopup dialog(nullptr);
    if (dialog.exec() == QDialog::Accepted) {
        QString selectedSize = dialog.getSelectedSize();
        int scale = 8; // Default size

        if (selectedSize == "8x8") scale = 8;
        else if (selectedSize == "16x16") scale = 16;
        else if (selectedSize == "32x32") scale = 32;
        else if (selectedSize == "64x64") scale = 64;

        initializeCanvas(scale); // Initialize canvas with chosen resolution
    } else {
        close(); // Close app if no size is selected
    }

    // Set up the canvas
    canvas = ui->uiCanvas;
    canvas->setFixedSize(512, 512); //Setting locked size to a power of two so zoom in conversions are easy
    model.SizeChange(64);
    model.DuplicateFrame(canvas->getPixmap()); //Give first frame to model as well

    //Connect the signal for draw and erase
    connect(ui->drawButton, &QToolButton::clicked, canvas, &Canvas::drawActivated);
    connect(ui->eraseButton, &QToolButton::clicked, canvas, &Canvas::eraseActivated);

    // Connect the signal for custom color selection
    connect(ui->customColor, &QPushButton::clicked, this, &MainWindow::updateColorWithCustom);

    // Connect the signal for quick access colors
    connect(ui->white, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor("white")); });
    connect(ui->gray, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(127, 127, 127)); });
    connect(ui->black, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor("black")); });

    // Connect the signal for drawing updates
    connect(canvas, &Canvas::updateCanvas, this, &MainWindow::updateCanvasDisplay);

    // Initialize the display with the current canvas state
    updateCanvasDisplay();

    // Connect the UI frame actions to the correct methods
    connect(ui->addFrameButton, &QPushButton::clicked, &model, &Model::AddFrame);
    connect(ui->deleteFrameButton, &QPushButton::clicked, this, &MainWindow::DeleteFramePopUp);
    connect(ui->deleteConfirmation->button(QDialogButtonBox::Yes), &QPushButton::clicked, &model, &Model::DeleteFrame);
    connect(ui->deleteConfirmation->button(QDialogButtonBox::Yes), &QPushButton::clicked, this, &MainWindow::DeleteFramePopUpClose);
    connect(ui->deleteConfirmation->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &MainWindow::DeleteFramePopUpClose);
    // ui->deleteFramePopUp->setVisible(true);
    // Connect back signals from model to slots here
    connect(&model, &Model::SendFrameListChanged, this, &MainWindow::FrameListChanged);

    // Connect the frame actions to update the animation(only when we change the list, not our position)
    connect(&model, &Model::SendUpdateAnimation, this, &MainWindow::UpdateAnimation);

    // Connect the save Action from the File Menu to a save action slot
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveTriggered);

    // Connect the load Action from the File Menu to a load action slot
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onLoadTriggered);
}

void MainWindow::initializeCanvas(int scale)
{
    // Create new Canvas with the chosen resolution
    canvas = new Canvas(this, 512, scale); // scale adjusted based on canvasSize
    canvas->setFixedSize(512, 512); // Keeps canvas display size fixed

    // Center canvas in the main window
    canvas->move(370, 0);

    //setCentralWidget(canvas); // Add canvas to the main window

    // canvas->setFixedSize(canvasSize * 8, canvasSize * 8); // Adjust size based on canvasSize
    // model->SizeChange(canvasSize);
    //model->DuplicateFrame(canvas->getPixmap());
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

    if (canvas->penColor.isValid())
    {
        // Set the color of the button to show current custom color being used
        ui->customColor->setStyleSheet(QString("background-color: %1").arg(canvas->penColor.name()));
    }
}

void MainWindow::updateColorWithPreset(QColor color)
{
    canvas->penColor = color;
}

void MainWindow::onSaveTriggered()
{
    // Open QFileDialog for user to choose a filepath
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Pixmap"), "", tr("JSON Files (*.json);;All Files (*)"));

    // Check if a path was actually selected
    if (!filePath.isEmpty())
    {

        // Get current QPixmap from canvas
        QPixmap pixmap = canvas->getPixmap();

        // Save Json file using QPixmap and the user-selected file path
        if (JsonReader::savePixmapToJson(pixmap, filePath))
        {
            QMessageBox::information(this, tr("Save Successful"), tr("File saved successfully!"));
        } else
        {
            QMessageBox::warning(this, tr("Save Failed"), tr("Could not save the file."));
        }
    }
}

void MainWindow::onLoadTriggered(){
    // Open QFileDialog for user to choose a filepath to load the pixmap from
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Pixmap"), "", tr("JSON Files (*.json);;All Files (*)"));

    // Check if a path was selected
    if (!filePath.isEmpty())
        {
        // Load the pixmap using JsonReader
        QPixmap pixmap;
        JsonReader jsonReader;

        if (jsonReader.loadPixmapFromJson(pixmap, filePath))
        {
            // Successfully loaded the pixmap, update the canvas
            canvas->setPixmap(pixmap); // Assuming you have a setter for pixmap in your Canvas class

            // Optionally, you can update the canvas display (it might already be done in setPixmap)
            canvas->repaint();

            QMessageBox::information(this, tr("Load Successful"), tr("File loaded successfully!"));
        } else
        {
            QMessageBox::warning(this, tr("Load Failed"), tr("Could not load the file."));
        }
    }
}

void MainWindow::FrameListChanged(int newIndex, QPixmap newMap) {
    canvas->setPixmap(newMap);
    newIndex++;
    newIndex--;
    // Scroller highlight (at newIndex)
}

void MainWindow::UpdateAnimation(QList<QPixmap> newPixMap) {
    // Animation preview update
    QList<QPixmap> pixMap = newPixMap;
}

void MainWindow::DeleteFramePopUp() {
    ui->deleteFramePopUp->setVisible(true);
}

void MainWindow::DeleteFramePopUpClose() {
    ui->deleteFramePopUp->setVisible(false);
}
