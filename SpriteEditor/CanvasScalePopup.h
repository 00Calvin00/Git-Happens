/* Reviewed by: Alex */

#ifndef CANVASSCALEPOPUP_H
#define CANVASSCALEPOPUP_H

#include <QDialog>
#include <QString>
#include <QComboBox>

/**
 * @class CanvasSizeDialog
 * @brief A modal dialog for selecting the initial canvas size.
 *
 * This class provides a simple dialog interface for selecting a predefined canvas size.
 * The dialog includes a combo box with options (e.g., "8x8", "16x16", etc.) and an
 * OK button to confirm the selection.
 *
 * Usage:
 * - Instantiate CanvasSizeDialog and call exec() to display it modally.
 * - Use getSelectedSize() to retrieve the user's choice after the dialog is accepted.
 */
class CanvasScalePopup : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructs a CanvasSizeDialog.
     * @param parent Optional parent widget for this dialog.
     *
     * Initializes the dialog's layout, including a combo box for canvas size selection
     * and an OK button to confirm the selection.
     */
    explicit CanvasScalePopup(QDialog *parent = nullptr);

    /**
     * @brief Retrieves the selected canvas size as a string.
     * @return QString containing the selected canvas size (e.g., "8x8", "16x16").
     *
     * This function returns the user's selected canvas size after the dialog is accepted.
     */
    QString getSelectedSize() const;

private:
    QString selectedSize;  ///< Stores the currently selected canvas size.
    QComboBox *sizeComboBox;  ///< Combo box widget for selecting canvas size.
    QPushButton *okButton;    ///< Button to confirm the selected size.
};

#endif // CANVASSCALEPOPUP_H
