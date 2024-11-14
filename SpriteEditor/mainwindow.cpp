/* Reviewed by: Kenna */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include "jsonreader.h"
#include "canvasscalepopup.h"
#include <QMessageBox>

MainWindow::MainWindow(FrameManager& model, int canvasSize, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(&model)
{
    ui->setupUi(this);
    this->showMaximized(); // Fill the screen with the sprite editor
    ui->deleteFramePopUp->setVisible(false); // Hide deletion confirmation popup

    // Set up the canvas
    background = ui->background;
    canvas = ui->uiCanvas;

    // Create and show the modal dialog
    CanvasScalePopup dialog(nullptr);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString selectedSize = dialog.getSelectedSize();
        int scale;

        if (selectedSize == "64x64") scale = 8;
        else if (selectedSize == "32x32") scale = 16;
        else if (selectedSize == "16x16") scale = 32;
        else if (selectedSize == "8x8") scale = 64; //Scale is the size of the cells so 64 means a 64 pixel cell so there will be 8 in the 512x512 canvas

        // Create new Canvas with the chosen resolution
        background->setScale(scale);
        background->update();
        background->repaint();
        canvas->setScale(scale);
    } else {
        close(); // Close app if no size is selected
    }

    // Center canvas in the main window
    background->move(370, 0);
    canvas->move(370, 0);
    model.AddInitialFrame(canvas); // Add the first frame from the canvas into the list of frames
    FrameListChanged(0, model.pixmapList[0]);
    ui->frameNavigator->setDragDropMode(QAbstractItemView::NoDragDrop);

    // AddInitalFrame(model.pixmapList[0]);

    previewIterationTimer = new QTimer(this);
    connect(previewIterationTimer, &QTimer::timeout, this, &MainWindow::IteratePreview);
    previewIterationTimer->start(1000/fps);

    //Connect the signal for draw and erase
    connect(ui->drawButton, &QToolButton::clicked, canvas, &Canvas::drawActivated);
    connect(ui->eraseButton, &QToolButton::clicked, canvas, &Canvas::eraseActivated);

    // Connect the signal for background settings
    // Connect gridToggle
    connect(ui->gridToggle, &QAction::toggled, background, &Background::setGridOn);
    connect(ui->onionSkinningToggle, &QAction::toggled, background, &Background::setOnionSkinningOn);
    connect(ui->checkeredBackgroundToggle, &QAction::toggled, background, &Background::setCheckeredBackgroundOn);

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

    // Connect the signal for drawing updates
    connect(canvas, &Canvas::updateCanvas, this, &MainWindow::updateCanvasDisplay);

    // Initialize the display with the current canvas state
    updateCanvasDisplay();

    // Connect the UI frame actions to the correct methods
    connect(ui->addFrameButton, &QPushButton::clicked, &model, &FrameManager::AddFrame);
    connect(ui->deleteFrameButton, &QPushButton::clicked, this, &MainWindow::DeleteFramePopUp);
    connect(ui->deleteConfirmation->button(QDialogButtonBox::Yes), &QPushButton::clicked, &model, &FrameManager::DeleteFrame);
    connect(ui->deleteConfirmation->button(QDialogButtonBox::Yes), &QPushButton::clicked, this, &MainWindow::DeleteFramePopUpClose);
    connect(ui->deleteConfirmation->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &MainWindow::DeleteFramePopUpClose);
    // ui->deleteFramePopUp->setVisible(true);

    // Connect back signals from model to slots here
    connect(&model, &FrameManager::SendFrameListChanged, this, &MainWindow::FrameListChanged);
    selectedFrameTimer = new QTimer(this);
    connect(selectedFrameTimer, &QTimer::timeout, this, &MainWindow::UpdateSelectedFrameIcon);

    previewIterationTimer->start(1000/fps);

    connect(ui->animationFPSSlider, &QSlider::valueChanged, this, [=](int value){
        ui->animationFPSNumber->setNum(value);
    });

    connect(ui->animationFPSSlider, &QSlider::valueChanged, this, [=](int value){
        if(value == 0) {
            previewIterationTimer->stop();
        }
        else {
            fps = value;
            previewIterationTimer->start(1000/fps);
        }
    });



    // Connect the save Action from the File Menu to a save action slot
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveTriggered);

    // Connect the load Action from the File Menu to a load action slot
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onLoadTriggered);

    // Connect the load Action from the File Menu to a load action slot
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewTriggered);

    // Connect the
    connect(ui->frameNavigator, &QListWidget::currentRowChanged, this, &MainWindow::OnFrameSelected);

    selectedFrameTimer = new QTimer(this);
    connect(selectedFrameTimer, &QTimer::timeout, this, &MainWindow::UpdateSelectedFrameIcon);
    selectedFrameTimer->start(1000/fps);

}


void MainWindow::updateCanvasDisplay()
{
    canvas->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete previewIterationTimer;
    delete selectedFrameTimer;
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
        if (JsonReader::savePixmapsToJson(pixmapList, canvas, filePath))
        {
            QMessageBox::information(this, tr("Save Successful"), tr("Project saved successfully!"));
        } else
        {
            QMessageBox::warning(this, tr("Save Failed"), tr("Project save failed."));
        }
    }
}

void MainWindow::onLoadTriggered()
{
    // Open QFileDialog for user to choose a filepath
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("JSON Files (*.json);;All Files (*)"));

    if (!filePath.isEmpty())
    {
        // Get the model's pixmap list
        QList<QPixmap*> pixmapList = model->getPixmapListObjects();

        // Load the project frames using the JsonReader
        if (JsonReader::loadPixmapsFromJson(pixmapList, canvas, background, filePath))
        {
            // Update the model with the loaded pixmapList
            model->setPixmapList(pixmapList);

            // Update the canvas with the first frame if there are frames
            if (model->getPixmapListValues().size() > 0)
            {
                model->SelectFrame(0); // Select the first frame in the project
                canvas->repaint();

            }

            QMessageBox::information(this, tr("Load Successful"), tr("Project loaded successfully!"));
        } else {
            QMessageBox::warning(this, tr("Load Failed"), tr("Project load failed."));
        }

    }
}

void MainWindow::onNewTriggered() {
    // Show a message box with Yes and No buttons and get the user's response
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Warning"), tr("Opening a new project will discard any unsaved progress. Continue?"),
                                  QMessageBox::Yes | QMessageBox::No);

    // Check if the user clicked Yes
    if (reply == QMessageBox::Yes) {
        qApp->quit(); // Quit the current running application
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments().mid(1)); // Open and run a new application
    }
}

void MainWindow::FrameListChanged(int newIndex, QPixmap* newFrame) {
    if (model->currentIndex != 0)
    {
        QPixmap* previousFrame = model->pixmapList.at(model->currentIndex-1);
        background->setPixmap(previousFrame);
    }
    else {
        background->setPixmap(newFrame);
    }

    canvas->setPixmap(newFrame);
    ui->frameNavigator->clear();
    model->currentIndex = newIndex;

    for (QPixmap* framePtr : model->pixmapList)
    {
        QListWidgetItem *scaledFrame = new QListWidgetItem(QIcon(framePtr->scaled(100, 100)), "");
        ui->frameNavigator->addItem(scaledFrame);
    }



    ui->frameNavigator->setCurrentRow(model->currentIndex);
}

void MainWindow::OnFrameSelected(int newIndex) {
    // Check if the index is within bounds of the list
    if (newIndex == -1) {
        if (model->currentIndex != 0) {
            model->currentIndex--;
        }
    }
    else {
        model->currentIndex = newIndex;
    }

    QPixmap* selectedFrame = model->pixmapList.at(model->currentIndex);
    if (model->currentIndex != 0)
    {
        QPixmap* previousFrame = model->pixmapList.at(model->currentIndex-1);
        background->setPixmap(previousFrame);
    }
    else {
        background->setPixmap(selectedFrame);
    }
    // Set the selected frame on the canvas
    canvas->setPixmap(selectedFrame);
}

void MainWindow::UpdateSelectedFrameIcon(){
    QPixmap* selectedFrame = model->pixmapList.at(model->currentIndex);

    // Checks if a frame is selected before updating current selected frame
    if (ui->frameNavigator->currentItem() != nullptr) {
        ui->frameNavigator->currentItem()->setIcon(QIcon(selectedFrame->scaled(100, 100)));
    }
}

void MainWindow::DeleteFramePopUp() {
    ui->deleteFramePopUp->setVisible(true);
}

void MainWindow::DeleteFramePopUpClose() {
    ui->deleteFramePopUp->setVisible(false);
}

void MainWindow::IteratePreview() {
    if(++curPreviewIndex >= model->pixmapList.length()) {
        curPreviewIndex = 0;
    }

    // Set the pixmap on the label
    ui->animationPreview->setPixmap(*(model->pixmapList.at(curPreviewIndex)));

    // Scale the pixmap to fit within the QLabel's size, if desired
    ui->animationPreview->setScaledContents(true);
}
