#include "canvasscalepopup.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QDialog>

/**
 * @brief Constructs the CanvasSizeDialog and sets up the user interface.
 *
 * This constructor initializes the dialog's layout, which includes:
 * - A QComboBox for selecting canvas sizes (options: "8x8", "16x16", etc.).
 * - A QPushButton ("OK") to confirm the selection.
 *
 * The selectedSize attribute is initially set to "8x8" as the default. Upon clicking
 * "OK", the dialog emits an accept() signal, closing the dialog.
 *
 * @param parent The parent widget for this dialog, if any (optional).
 */
CanvasScalePopup::CanvasScalePopup(QDialog *parent)
    : QDialog(parent), selectedSize("8x8")  // Default selection
{
    setWindowTitle("Please Select Pixel Ratio :)");

    resize(300, 100);

    // Initialize the combo box and populate it with canvas size options
    sizeComboBox = new QComboBox(this);
    sizeComboBox->addItems({"8x8", "16x16", "32x32", "64x64"});
    sizeComboBox->setCurrentText(selectedSize); // Default size selection

    // Create the OK button and connect it to the dialog's accept slot
    okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    // Arrange the combo box and OK button vertically
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(sizeComboBox);
    layout->addWidget(okButton);
    setLayout(layout);

    // Update selectedSize whenever the user changes the combo box selection
    connect(sizeComboBox, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        selectedSize = text;
    });
}

/**
 * @brief Retrieves the canvas size selected by the user.
 *
 * This function should be called after the dialog is accepted to obtain the
 * selected canvas size. The result will match one of the predefined sizes
 * (e.g., "8x8", "16x16").
 *
 * @return QString representing the selected canvas size.
 */
QString CanvasScalePopup::getSelectedSize() const {
    return selectedSize;
}
