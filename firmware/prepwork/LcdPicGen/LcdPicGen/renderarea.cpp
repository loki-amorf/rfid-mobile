#include "renderarea.h"
#include <QImage>
#include <QPainter>

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent)
{
}

void RenderArea::setImage(QImage img)
{
    this->image = img.scaled(this->size());
}

void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 0, this->image);

    int w = this->width();
    int h = this->height();

    //painter.setPen(Qt::DashLine);
    painter.setPen(QColor(255, 0, 0));
    // grid (horizontal)
    for (int y = h / 4; y < h; y += h / 4) {
        painter.drawLine(0, y, w, y);
    }
    // grid (vertical)
    for (int x = w / 8; x < w; x += w / 8) {
        painter.drawLine(x, 0, x, h);
    }
}
