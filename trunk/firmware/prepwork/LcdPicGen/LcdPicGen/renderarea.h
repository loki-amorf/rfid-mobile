#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

public:
    void setImage(QImage img);

private:
    QImage image;
};

#endif // RENDERAREA_H
