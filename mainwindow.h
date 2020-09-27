#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QParallelAnimationGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString mapName = "";
    float mapRatio = 1.285; // 1024 / 800 = 1.28
    QString teamNameA = "";
    QString teamNameB = "";

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_Demo_triggered();

    void on_playButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene = nullptr;
    QParallelAnimationGroup *group = nullptr;
    bool gAnimationStarted = false;
};
#endif // MAINWINDOW_H
