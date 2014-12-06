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

private:
    void clear_undo_stack();
    void clear_redo_stack();
    void clear_stacks();
    void save_image(QString fileName = QString());
    void set_image(QImage *newImage);
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
