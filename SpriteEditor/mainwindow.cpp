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
    // Set up the UI
    ui->setupUi(this);
    this->showMaximized();
    ui->deleteFramePopUp->setVisible(false);

    // Set up the background and canvas
    background = ui->background;
    canvas = ui->uiCanvas;
    background->move(370, 0);
    canvas->move(370, 0);

    // Connect the signals for draw and erase
    connect(ui->drawButton, &QToolButton::clicked, canvas, &Canvas::drawActivated);
    connect(ui->eraseButton, &QToolButton::clicked, canvas, &Canvas::eraseActivated);

    // Connect the signal for drawing updates
    connect(canvas, &Canvas::updateCanvas, this, &MainWindow::updateCanvasDisplay);

    // Connect the signal for custom color selection
    connect(ui->customColor, &QPushButton::clicked, this, &MainWindow::updateColorWithCustom);

    // Create the hashmap for color buttons and their respective color values
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

    // Connect the signals of color buttons to respective color values iteratively
    for (auto button = colorMap.begin(); button != colorMap.end(); ++button)
    {
        QColor color = button.value();
        connect(button.key(), &QPushButton::clicked, this, [this, color]() {updateColorWithPreset(color);});
    }

    // Allow the user to select the pixel size on launch
    CanvasScalePopup dialog(nullptr);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString selectedSize = dialog.getSelectedSize();

        // Scale is the number of pixels per cell (e.g. 64 pixels = 1 cell giving an 8x8 "pixel" canvas)

        if (selectedSize == "64x64") canvas->setScale(8);
        else if (selectedSize == "32x32") canvas->setScale(16);
        else if (selectedSize == "16x16") canvas->setScale(32);
        else if (selectedSize == "8x8") canvas->setScale(64);

        // Update background and canvas with the selected size
        background->setScale(canvas->getScale());
        background->update();
        updateCanvasDisplay();
    } else
    {
        close(); // Default to 8x8 if no size is selected
    }

    // Connect the signals for save, load, and new Actions from the File Menu
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveTriggered);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onLoadTriggered);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewTriggered);

    // Connect the signals for grid, onionSkinning, and checkeredBackground Toggles from the Settings Menu
    connect(ui->gridToggle, &QAction::toggled, background, &Background::setGridOn);
    connect(ui->onionSkinningToggle, &QAction::toggled, background, &Background::setOnionSkinningOn);
    connect(ui->checkeredBackgroundToggle, &QAction::toggled, background, &Background::setCheckeredBackgroundOn);

    // Set up Frame Navigator
    ui->frameNavigator->setDragDropMode(QAbstractItemView::NoDragDrop);
    model.addInitialFrame(canvas);
    frameListChanged(0, model.pixmapList[0]);

    // Set up Frame Navigator timer
    selectedFrameTimer = new QTimer(this);
    selectedFrameTimer->start(1000/fps);
    connect(selectedFrameTimer, &QTimer::timeout, this, &MainWindow::updateSelectedFrameIcon);

    // Connect the signals for Frame Navigator
    connect(&model, &FrameManager::sendFrameListChanged, this, &MainWindow::frameListChanged);
    connect(ui->frameNavigator, &QListWidget::currentRowChanged, this, &MainWindow::onFrameSelected);
    connect(ui->addFrameButton, &QPushButton::clicked, &model, &FrameManager::addFrame);
    connect(ui->deleteFrameButton, &QPushButton::clicked, this, &MainWindow::deleteFramePopUp);
    connect(ui->deleteConfirmation->button(QDialogButtonBox::Yes), &QPushButton::clicked, &model, &FrameManager::deleteFrame);
    connect(ui->deleteConfirmation->button(QDialogButtonBox::Yes), &QPushButton::clicked, this, &MainWindow::deleteFramePopUpClose);
    connect(ui->deleteConfirmation->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &MainWindow::deleteFramePopUpClose);

    // Set up Animation Preview timer
    previewIterationTimer = new QTimer(this);
    previewIterationTimer->start(1000/fps);
    connect(previewIterationTimer, &QTimer::timeout, this, &MainWindow::iteratePreview);

    // Connect the signals for Animation Preview slider
    connect(ui->animationFPSSlider, &QSlider::valueChanged, this, [=](int value){ui->animationFPSNumber->setNum(value);});
    connect(ui->animationFPSSlider, &QSlider::valueChanged, this, [=](int value){
        if(value == 0)
        {
            previewIterationTimer->stop();
        }
        else {
            fps = value;
            previewIterationTimer->start(1000/fps);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete previewIterationTimer;
    delete selectedFrameTimer;
}

void MainWindow::updateCanvasDisplay()
{
    canvas->repaint();
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

    if (!filePath.isEmpty())
    {
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
                model->selectFrame(0); // Select the first frame in the project
                canvas->repaint();

            }

            QMessageBox::information(this, tr("Load Successful"), tr("Project loaded successfully!"));
        } else {
            QMessageBox::warning(this, tr("Load Failed"), tr("Project load failed."));
        }

    }
}

void MainWindow::onNewTriggered()
{
    // Show a message box with Yes and No buttons and get the user's response
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Warning"), tr("Opening a new project will discard any unsaved progress. Continue?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        qApp->quit(); // Quit the current running application
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments().mid(1)); // Open and run a new application
    }
}

void MainWindow::frameListChanged(int newIndex, QPixmap* newFrame)
{
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

void MainWindow::onFrameSelected(int newIndex)
{
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

void MainWindow::updateSelectedFrameIcon()
{
    QPixmap* selectedFrame = model->pixmapList.at(model->currentIndex);

    // Checks if a frame is selected before updating current selected frame
    if (ui->frameNavigator->currentItem() != nullptr) {
        ui->frameNavigator->currentItem()->setIcon(QIcon(selectedFrame->scaled(100, 100)));
    }
}

void MainWindow::deleteFramePopUp()
{
    ui->deleteFramePopUp->setVisible(true);
}

void MainWindow::deleteFramePopUpClose()
{
    ui->deleteFramePopUp->setVisible(false);
}

void MainWindow::iteratePreview()
{
    if(++curPreviewIndex >= model->pixmapList.length())
    {
        curPreviewIndex = 0;
    }

    // Set the pixmap on the label
    ui->animationPreview->setPixmap(*(model->pixmapList.at(curPreviewIndex)));

    // Scale the pixmap to fit within the QLabel's size, if desired
    ui->animationPreview->setScaledContents(true);
}
