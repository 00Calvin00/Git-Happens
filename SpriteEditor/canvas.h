#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    QPixmap getPixmap() const;  // Method to return the pixmap

signals:
    void canvasUpdated();  // Signal to notify that the canvas has been updated

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void draw(QMouseEvent *event);

    QColor color;
    QPixmap *pixmap;
    bool pressed;
    int canvasSize;
};

#endif // CANVAS_H
