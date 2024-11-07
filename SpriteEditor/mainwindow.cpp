#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

    connect(ui->customColor, &QPushButton::clicked, this, &MainWindow::getCustomColor);
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
