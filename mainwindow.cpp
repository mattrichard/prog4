#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <omp.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

#include "ip_algorithms.h"
#include "chris_algorithms.h"
#include "ian_algorithms.h"

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

    double start = omp_get_wtime();
    QImage* newImage = grayscale(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionSmooth_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage *newImage = smooth(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
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

void MainWindow::set_image(QImage *newImage, double time)
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

        ui->statusBar->showMessage(QString("Total time: %1 seconds").arg(time));
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

    double start = omp_get_wtime();
    QImage* newImage = gradient(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionLaplacian_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = laplacian(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionBrighten_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = brighten(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionDarken_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = darken(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionSharpen_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = sharpen(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionNegate_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = negate(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionEmboss_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = emboss(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionBinary_Threshold_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = binary_threshold( *grayscale(*image, thread_count) , thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionEnhanceContrast_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = enhance_contrast(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionNoise_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = noise(*image , thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionReduce_Contrast_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = reduce_contrast(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionPosterize_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = posterize(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionGamma_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = gamma(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionGaussian_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = gaussian(*image, thread_count);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionSmooth_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage *newImage = smooth(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionGrayscale_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = grayscale(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionGradient_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = gradient(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionBrighten_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = brighten(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionDarken_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = darken(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionLaplacian_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = laplacian(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionNoise_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = noise(*image , 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionBinary_Threshold_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = binary_threshold( *grayscale(*image, 1) , 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionNegate_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = negate(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionSharpen_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = sharpen(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionGamma_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = gamma(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionEnhanceContrast_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = enhance_contrast(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionReduce_Contrast_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = reduce_contrast(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionEmboss_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = emboss(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionPosterize_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = posterize(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionGaussian_Sequential_triggered()
{
    if(image == NULL)
        return;

    double start = omp_get_wtime();
    QImage* newImage = gaussian(*image, 1);
    double end = omp_get_wtime();

    set_image(newImage, end - start);
}

void MainWindow::on_actionSet_Thread_Count_triggered()
{
    thread_count = QInputDialog::getInt(this, "Set Thread Count", "Thread Count", thread_count, 2, 16);
}
