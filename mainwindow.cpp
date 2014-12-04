#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QFileDialog"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    exit(0);
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.bmp)"));

    if(!fileName.isEmpty())
    {
        image = new QImage(fileName);
        //QSize size = image->size();

        if(image->isNull())
            QMessageBox::information(this, tr("prog4"), tr("Unable to load image %1.").arg(fileName));
        else
        {
            ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
            //ui->imageLabel->resize(size);
        }
    }
}
