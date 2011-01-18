#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QVector>

#define LCD_PIXEL_LINES     (64)
#define LCD_PIXEL_COLS      (128)

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int totalBytes;
    QImage origImage;
    QImage newImage;
    QString code;
    void appendHexStr(QString &str, uchar value);
    void generateCode(QString &header, QString &entity);

private slots:
    void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H
