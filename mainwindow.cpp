#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QTimeLine>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>

#include <QStandardItemModel>
#include <GUI\Player\player.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete group;
}


void MainWindow::on_actionOpen_Demo_triggered()
{
    // open file browser
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Demo"), QDir::currentPath(), tr("Demo Files (*.dem)"));
    qDebug() << fileName << endl;

    if( !fileName.isNull()){
        // configure connect Signal, to extract information
        QProcess* process = new QProcess();
        int round = 0;

        float paxo[165][3] = {};
        float meDDn[165][3] = {};

        int pj = 0;
        int mj = 0;
        QObject::connect(process,&QProcess::readyRead,process,[&](){
            QString result = process->readAllStandardOutput();
            QStringList firstList = result.split("\n");

            foreach (const QString &str, firstList) {
                if (str.contains("STARTINFO")){
                    QStringList items = str.split(", ");
                    this->mapName = items[1];

                    // Get Teamname A
                    if (items[2].contains("[READY]")){
                        QStringList name = items[2].split("READY] ");
                        this->teamNameA = name[1];
                    }else{
                        this->teamNameA = items[2];
                    }

                    // Get Teamname B
                    if (items[3].contains("[READY]")){
                        QStringList name = items[3].split("READY] ");
                        this->teamNameB = name[1];
                    }else{
                        this->teamNameB = items[3];
                    }
                }

                if (str.contains("ROUND START")){
                    //qDebug() << str << endl;
                    round = round + 1;
                }

                if (str.contains("PLAYER INFO")){
                    QStringList items = str.split(", ");
                    // items[2] = roundnumber -> round 1
                    if(items[2] == "1"){
                        // items[6] = playername
                        if(items[6] == "Paxo"){
                            paxo[pj][0] = items[1].toFloat(); // tick
                            paxo[pj][1] = items[4].toFloat()/this->mapRatio; // X
                            paxo[pj][2] = items[5].toFloat()/this->mapRatio; // Y
                            pj = pj + 1;
                        }
                        if(items[6] == "meDDn"){
                            meDDn[mj][0] = items[1].toFloat(); // tick
                            meDDn[mj][1] = items[4].toFloat()/this->mapRatio; // X
                            meDDn[mj][2] = items[5].toFloat()/this->mapRatio; // Y
                            mj = mj + 1;
                        }
                    }
                    //qDebug() << str << endl;
                }

            }

        });

        // parse demofile
        QString cmd = "go run parse_demofile.go -demo " + fileName.toUtf8();
        process->start(cmd);
        process->waitForFinished(-1); // sets current thread to sleep and waits for pingProcess end

        // load map as Pixmap
        QString mapPath = ".\\map\\";
        mapPath.append(this->mapName);
        mapPath.append(".jpg");
        QPixmap map(mapPath);
        map = map.scaled(1024/this->mapRatio, 1024/this->mapRatio, Qt::KeepAspectRatio);

        // Add  graphics scene
        this->scene = new QGraphicsScene();
        ui->graphicsView->setScene(this->scene);    // Set grapihcs scene into graphicsView
        this->scene->addPixmap(map);                // Add Map

        ui->teamNameA->setText(this->teamNameA);
        ui->teamNameB->setText(this->teamNameB);

        qDebug() << round << endl;





        // animate test
        Player* playerPaxo = new Player();
        playerPaxo->set_color(QColor(0, 0, 255));
        playerPaxo->set_name("Paxo");
        playerPaxo->setPos(QPoint(paxo[0][1],paxo[0][2]));
        this->scene->addItem(playerPaxo);

        Player* playerMeddn = new Player();
        playerMeddn->set_color(QColor(0, 0, 255));
        playerMeddn->set_name("meDDn");
        //playerMeddn->setPos(meDDn[0][1],meDDn[0][2]);
        this->scene->addItem(playerMeddn);


        QPropertyAnimation *pPropAnimation = new QPropertyAnimation(playerPaxo, "pos");
        pPropAnimation->setDuration(10000);
        pPropAnimation->setStartValue(QPointF(paxo[0][1], paxo[0][2]));

        QPropertyAnimation *pPropAnimation2 = new QPropertyAnimation(playerMeddn, "pos");
        pPropAnimation2->setDuration(10000);
        pPropAnimation2->setStartValue(QPointF(meDDn[0][1], meDDn[0][2]));

        for (int i = 0; i < 100; ++i){
            // tick, X, Y
            pPropAnimation->setKeyValueAt(i / 100.0, QPointF(paxo[i][1], paxo[i][2]));
            pPropAnimation2->setKeyValueAt(i / 100.0, QPointF(meDDn[i][1], meDDn[i][2]));
        }

        // Sequentiell
        QSequentialAnimationGroup *sGroup = new QSequentialAnimationGroup();
        QPauseAnimation *delay = new QPauseAnimation(2000, pPropAnimation2);

        sGroup->addAnimation(delay);
        sGroup->addAnimation(pPropAnimation2);

        // Parallel
        this->group = new QParallelAnimationGroup();
        this->group->addAnimation(pPropAnimation);
        this->group->addAnimation(sGroup);

    }
}



void MainWindow::on_playButton_clicked()
{
    if(this->group != nullptr){
        if(this->gAnimationStarted == false){
            this->group->start();
        }else{
            this->group->setPaused(false);
        }

    }
}

void MainWindow::on_stopButton_clicked()
{
    if(this->group != nullptr){
        this->group->setPaused(true);
        this->gAnimationStarted = true;
    }
}
