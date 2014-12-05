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

    clear_stacks();

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
        {
            clear_stacks();
            update_undo_redo_actions();
            ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
        }
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

    set_image(newImage);
}

void MainWindow::on_actionSmooth_triggered()
{
    if(image == NULL)
        return;

    QImage *newImage = smooth(*image, thread_count);

    set_image(newImage);
}

void MainWindow::on_actionUndo_triggered()
{
    undo();
}

void MainWindow::on_actionRedo_triggered()
{
    redo();
}

void MainWindow::clear_undo_stack()
{
    while(!undoStack.isEmpty())
    {
        QImage* temp = undoStack.back();
        undoStack.pop_back();
        delete temp;
    }
}

void MainWindow::clear_redo_stack()
{
    while(!redoStack.isEmpty())
    {
        QImage* temp = redoStack.back();
        redoStack.pop_back();
        delete temp;
    }
}

void MainWindow::clear_stacks()
{
    clear_undo_stack();
    clear_redo_stack();
}

void MainWindow::save_image(QString fileName)
{
    image->save(fileName.isEmpty() ? imageFileName : fileName);
}

void MainWindow::set_image(QImage *newImage)
{
    if(image != NULL)
    {
        undoStack.push_back(image);
        clear_redo_stack();

        image = newImage;

        ui->imageLabel->setPixmap(QPixmap::fromImage(*image));

        // Undo stack can only save 15 images
        if(undoStack.size() > 15)
        {
            QImage* temp = undoStack.front();
            undoStack.pop_front();
            delete temp;
        }

        update_undo_redo_actions();
    }
}

void MainWindow::undo()
{
    redoStack.push_back(image);

    image = undoStack.back();
    undoStack.pop_back();

    ui->imageLabel->setPixmap(QPixmap::fromImage(*image));

    update_undo_redo_actions();
}

void MainWindow::redo()
{
    undoStack.push_back(image);

    image = redoStack.back();
    redoStack.pop_back();

    ui->imageLabel->setPixmap(QPixmap::fromImage(*image));

    update_undo_redo_actions();
}

void MainWindow::update_undo_redo_actions()
{
    ui->actionUndo->setEnabled(!undoStack.isEmpty());
    ui->actionRedo->setEnabled(!redoStack.isEmpty());
}

void MainWindow::on_actionGradient_triggered()
{
    if(image == NULL)
        return;

    QImage* newImage = gradient(*image, thread_count);

    set_image(newImage);
}
