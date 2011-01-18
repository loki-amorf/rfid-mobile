#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QVector>

#define MAX_TOLERANCE 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    // initializing
    ui->outputText->clear();
    newImage = QImage(LCD_PIXEL_COLS, LCD_PIXEL_LINES, QImage::Format_Mono);
    MainWindow::totalBytes = 0;
    MainWindow::code.clear();

    // open image file
    QString pictureFileName;
    pictureFileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "",
                                                   tr("Image Files (*.jpg *.png *.bmp)"));
    this->origImage = QImage(pictureFileName);
    if (this->origImage.isNull())
        return;

    // gray -> mono
    newImage = origImage.scaled(newImage.size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    newImage = newImage.convertToFormat(QImage::Format_Mono, Qt::MonoOnly);

    // show in render area
    ui->pictureArea->setImage(newImage);
    ui->pictureArea->update();

    // encoding
    int lcd_x, lcd_y;
    int x, y;
    int tolerance = MAX_TOLERANCE;
    bool encoding = false;
    uchar byte0, byte1;
    uchar entityLen = 0x00;
    QString codeHeader, codeEntity;

    for (lcd_y = 0; lcd_y < 32; ++lcd_y) {
        for (lcd_x = 0; lcd_x < 16; ++lcd_x) {
            //lcd_x, lcd_y -> x, y
            x = (lcd_x > 7) ? (lcd_x - 8) * 2 : lcd_x * 2;
            y = lcd_y + (lcd_x / 8) * 32;

            // read byte0 and byte1
            byte0 = newImage.scanLine(y)[x];
            byte1 = newImage.scanLine(y)[x + 1];

            if (byte0 == 0x00 && byte1 == 0x00) {
                if (encoding) {
                    if (tolerance-- == 0) {
                        encoding = false;
                        tolerance = MAX_TOLERANCE;
                        generateCode(codeHeader, codeEntity);
                    }
                }
            }
            else {
                if (encoding)
                    // write 0x00 to code
                    for (int i = 0; i < (MAX_TOLERANCE - tolerance); ++i) {
                        appendHexStr(codeEntity, 0x00);
                        appendHexStr(codeEntity, 0x00);
                        entityLen += (MAX_TOLERANCE - tolerance);
                    }
                else {
                    // write lcd_y, lcd_x to code
                    appendHexStr(codeHeader, lcd_y);
                    appendHexStr(codeHeader, lcd_x);
                }

                encoding = true;
                tolerance = MAX_TOLERANCE;
                appendHexStr(codeEntity, byte0);
                appendHexStr(codeEntity, byte1);
                entityLen += 2;
            }
        }
        if (encoding) {
            generateCode(codeHeader, codeEntity);
        }
        encoding = false;
        tolerance = MAX_TOLERANCE;
    }

    // cut tail
    MainWindow::code.resize(MainWindow::code.size() - 2);

    // display code result and statics
    ui->outputText->insertPlainText(MainWindow::code);
    char str[64];
    sprintf(str, "\n\nTotal bytes: %d", MainWindow::totalBytes);
    ui->outputText->insertPlainText(str);
}

void MainWindow::appendHexStr(QString &str, uchar value)
{
    char tempStr[20];
    sprintf(tempStr, "0x%2X, ", value);
    if (tempStr[2] == ' ')
        tempStr[2] = '0';
    str.append(tempStr);
    ++MainWindow::totalBytes;
}

void MainWindow::generateCode(QString &header, QString &entity)
{
    uchar entityLen;

    entityLen = entity.count("0x");
    appendHexStr(header, entityLen);

    MainWindow::code.append(header);
    MainWindow::code.append(entity);
    header.clear();
    entity.clear();
}
