#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "ip_algorithms.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    thread_count = 8;

    image = NULL;
}

MainWindow::~MainWindow()
{
    if(image != NULL)
        delete image;

    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionOpen_triggered()
{
    imageFileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.bmp)"));

    if(!imageFileName.isEmpty())
    {
        if(image != NULL)
            delete image;

        image = new QImage(imageFileName);//, QImage::Format_RGB32);

        if(image->isNull())
            QMessageBox::information(this, tr("prog4"), tr("Unable to load image %1.").arg(imageFileName));
        else
            ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), QString(), tr("Image Files (*.png *.jpg *.bmp)"));

    save_image(fileName);
}

void MainWindow::on_actionSave_triggered()
{
    save_image();
}

void MainWindow::on_actionGrayscale_triggered()
{
    if(image == NULL)
        return;

    QImage* newImage = grayscale(*image, thread_count);

    delete image;
    image = newImage;

    ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void MainWindow::save_image(QString fileName)
{
    image->save(fileName.isEmpty() ? imageFileName : fileName);
}
