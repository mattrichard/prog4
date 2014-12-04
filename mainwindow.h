#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

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

private:
    void save_image(QString fileName = QString());
    Ui::MainWindow *ui;

    int thread_count;

    QImage* image;
    QString imageFileName;
};

#endif // MAINWINDOW_H
