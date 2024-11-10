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
    ui->deleteFramePopUp->setVisible(false); // Hide deletion confirmation popup

    // Set up the canvas
    canvas = ui->uiCanvas;
    canvas->setFixedSize(512, 512); //Setting locked size to a power of two so zoom in conversions are easy
    model.SizeChange(64);

    model.AddInitialFrame(canvas); // Add the first frame from the canvas into the list of frames

    //model.DuplicateFrame(canvas->getPixmap()); //Give first frame to model as well

    //Connect the signal for draw and erase
    connect(ui->drawButton, &QToolButton::clicked, canvas, &Canvas::drawActivated);
    connect(ui->eraseButton, &QToolButton::clicked, canvas, &Canvas::eraseActivated);

    // Connect the signal for custom color selection
    connect(ui->customColor, &QPushButton::clicked, this, &MainWindow::updateColorWithCustom);

    // Connect the signal for quick access colors
    connect(ui->white, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor("white")); });
    connect(ui->gray, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor(127, 127, 127)); });
    connect(ui->black, &QPushButton::clicked, this, [this]() { updateColorWithPreset(QColor("black")); });

    // Center canvas in the main window
    canvas->move(370, 0);

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
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Project"), "", tr("JSON Files (*.json);;All Files (*)"));

    if (!filePath.isEmpty()) {
        // Get the list of frames from the model
        QList<QPixmap*> pixmapList = model->getPixmapList();

        qDebug() << "Saving frame at index:" << pixmapList.size();

        // Save all frames using the JsonReader
        if (JsonReader::savePixmapsToJson(pixmapList, filePath))
        {
            QMessageBox::information(this, tr("Save Successful"), tr("Project saved successfully!"));
        } else
        {
            QMessageBox::warning(this, tr("Save Failed"), tr("Could not save the project."));
        }
    }
}

void MainWindow::onLoadTriggered()
{
    // Open QFileDialog for user to choose a filepath
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("JSON Files (*.json);;All Files (*)"));

    if (!filePath.isEmpty()) {
        QList<QPixmap*> pixmapList;

        // Load the project frames using the JsonReader
        if (JsonReader::loadPixmapsFromJson(pixmapList, filePath)) {
            // Successfully loaded frames, update the model with the new list
            //model->setPixmapList(pixmapList);

            model->SelectFrame(0);


            // Optionally update the canvas with the first frame if there are frames
            if (!pixmapList.isEmpty()) {
                //canvas->setPixmap(pixmapList.first());
                //canvas->setPixmap(pixmapList.at(0));
                canvas->repaint();
            }

            QMessageBox::information(this, tr("Load Successful"), tr("Project loaded successfully!"));
        } else {
            QMessageBox::warning(this, tr("Load Failed"), tr("Could not load the project."));
        }
    }
}


void MainWindow::FrameListChanged(int newIndex, QPixmap* newMap) {
    canvas->setPixmap(newMap);
    newIndex++;
    newIndex--;
    // Scroller highlight (at newIndex)
}

void MainWindow::UpdateAnimation(QList<QPixmap*> newPixMap) {
    // Animation preview update
    QList<QPixmap*> pixMap = newPixMap;
}

void MainWindow::DeleteFramePopUp() {
    ui->deleteFramePopUp->setVisible(true);
}

void MainWindow::DeleteFramePopUpClose() {
    ui->deleteFramePopUp->setVisible(false);
}
