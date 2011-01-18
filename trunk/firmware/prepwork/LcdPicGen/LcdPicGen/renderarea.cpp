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
}
