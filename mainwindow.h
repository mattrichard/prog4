#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_as_triggered();
    void on_actionSave_triggered();

    void on_actionGrayscale_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionSmooth_triggered();
    void on_actionGradient_triggered();
    void on_actionLaplacian_triggered();
    void on_actionBrighten_triggered();
    void on_actionDarken_triggered();
    void on_actionSharpen_triggered();
    void on_actionNegate_triggered();
    void on_actionBinary_Threshold_triggered();
    void on_actionNoise_triggered();
    void on_actionEmboss_triggered();
    void on_actionEnhanceContrast_triggered();
    void on_actionReduce_Contrast_triggered();
    void on_actionPosterize_triggered();
    void on_actionGamma_triggered();
    void on_actionGaussian_triggered();
    void on_actionSmooth_Sequential_triggered();
    void on_actionGrayscale_Sequential_triggered();
    void on_actionGradient_Sequential_triggered();
    void on_actionBrighten_Sequential_triggered();
    void on_actionDarken_Sequential_triggered();
    void on_actionLaplacian_Sequential_triggered();
    void on_actionNoise_Sequential_triggered();
    void on_actionBinary_Threshold_Sequential_triggered();
    void on_actionNegate_Sequential_triggered();
    void on_actionSharpen_Sequential_triggered();
    void on_actionGamma_Sequential_triggered();
    void on_actionEnhanceContrast_Sequential_triggered();
    void on_actionReduce_Contrast_Sequential_triggered();
    void on_actionEmboss_Sequential_triggered();
    void on_actionPosterize_Sequential_triggered();
    void on_actionGaussian_Sequential_triggered();
    void on_actionSet_Thread_Count_triggered();
    void on_actionFFT_triggered();
    void on_actionFFT_Sequential_triggered();

private:
    void clear_undo_stack();
    void clear_redo_stack();
    void clear_stacks();
    void save_image(QString fileName = QString());
    void set_image(QImage *newImage, double time);
    void undo();
    void redo();
    void update_undo_redo_actions();

    Ui::MainWindow *ui;

    int thread_count;

    QImage* image;
    QString imageFileName;

    QList<QImage*> undoStack;
    QList<QImage*> redoStack;
};

#endif // MAINWINDOW_H
