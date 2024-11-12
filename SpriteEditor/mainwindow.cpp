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

    // Create the hashmap for quick access colors
    QHash<QPushButton*, QColor> colorMap = {
        {ui->white, QColor("white")}, {ui->gray, QColor(127, 127, 127)}, {ui->black, QColor("black")},
        {ui->darkBrown, QColor(60, 10, 0)}, {ui->brown, QColor(86, 39, 4)}, {ui->lightBrown, QColor(139, 70, 0)},
        {ui->burgundy, QColor(129, 0, 0)}, {ui->darkRed, QColor(185, 0, 0)}, {ui->red, QColor(245, 0, 0)},
        {ui->burntOrange, QColor(191, 54, 12)}, {ui->orange, QColor(230, 81, 0)}, {ui->neonOrange, QColor(255, 111, 0)},
        {ui->gold, QColor(255, 150, 38)}, {ui->yellow, QColor(255, 200, 9)}, {ui->neonYellow, QColor(255, 255, 0)},
        {ui->forestGreen, QColor(0, 63, 0)}, {ui->green, QColor(0, 150, 0)}, {ui->neonGreen, QColor(0, 235, 0)},
        {ui->darkTeal, QColor(5, 60, 88)}, {ui->teal, QColor(0, 128, 128)}, {ui->cyan, QColor(0, 245, 245)},
        {ui->darkBlue, QColor(1, 3, 147)},{ui->blue, QColor(0, 0, 245)}, {ui->skyBlue, QColor(0, 170, 255)},
        {ui->indigo, QColor(70, 0, 127)}, {ui->violet, QColor(148, 0, 211)}, {ui->purple, QColor(205, 0, 211)},
        {ui->plum, QColor(109, 0, 82)}, {ui->berry, QColor(170, 0, 127)}, {ui->magenta, QColor(255, 0, 160)}
    };

    // Connect color buttons to repective color values iteratively
    for (auto button = colorMap.begin(); button != colorMap.end(); ++button)
    {
        QColor color = button.value();
        connect(button.key(), &QPushButton::clicked, this, [this, color]() {updateColorWithPreset(color);});
    }

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
        QList<QPixmap*> pixmapList = model->getPixmapListValues();

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
        //QList<QPixmap*> pixmapList;
        // Get the model's pixmap list

        QList<QPixmap*> pixmapList = model->getPixmapListObjects();

        // Load the project frames using the JsonReader
        if (JsonReader::loadPixmapsFromJson(pixmapList, filePath)) {
            // Successfully loaded frames, update the model with the new
            //Load is not properly replacing the model instances pixmaps with the loaded pixmaps. There are
            // two pixmaps in memory at this time, the loaded one and the model instance's pixmap

            model->setPixmapList(pixmapList);

            // Update the model with the loaded pixmapList
            QList<QPixmap*> modelPixmapList = model->getPixmapListValues();

            //Debugging for the current model frame contents
            // for (QPixmap* frame : modelPixmapList) {
            //     QPixmap tempPixmapCopy = *frame; // Copy the pixmap
            //     QImage imageCopy = tempPixmapCopy.toImage();

            //     // Check the top-left pixel color of the copied image
            //     QColor topLeftColor = QColor(imageCopy.pixel(0, 0));
            //     qDebug() << "Top-left pixel color in Model Frames:" << topLeftColor.name();
            // }

            // Update the canvas with the first frame if there are frames
            if (model->getPixmapListValues().size() > 0) {
                model->SelectFrame(0); // Select frame updates the index to the current one, emits a sendframeListChanged signal
                // to the model, and triggers the FrameListChanged slot in the mainwindow, that sets the canvas pixmap to the new map at
                // index 0 of the loaded project.
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
    ui->frameNavigator->clear();

    for (QPixmap* framePtr : model->getPixmapListValues())
    {
        if (framePtr)
        {
        QListWidgetItem *scaledFrame = new QListWidgetItem(QIcon(framePtr->scaled(100, 100)), "");
        ui->frameNavigator->addItem(scaledFrame);
        }
    }

    ui->frameNavigator->setCurrentRow(newIndex);
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
