#include <QMenu>
#include <QFileDialog>
#include <QLabel>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMenu *menuFile = new QMenu("&File");
    menuFile->addAction("&Open file", this, SLOT(slotOpen()), Qt::CTRL + Qt::Key_O);
    menuFile->addAction("&Exit", this, SLOT(slotExit()));
    QMenu *menuHelp = new QMenu("&Help");
    menuHelp->addAction("&About", this, SLOT(slotAbout()), Qt::Key_F1);
    menuBar()->addMenu(menuFile);
    menuBar()->addMenu(menuHelp);

}


void MainWindow::slotOpen()
{
    QString sFilter;
    QString sFileName = QFileDialog::getOpenFileName(0
                                                     ,tr("Open chart file")
                                                     ,"File"
                                                     ,"*.ssd *.rsd"
                                                     ,&sFilter);
    if (!sFileName.isEmpty())
    {
        chartWindow = new ChartWindow(sFileName.toStdString());
        chartWindow->init();

        bool res = chartWindow->start();
        if (res)
        {
            delete chartWindow;
            chartWindow = NULL;
        }
    }
}

void MainWindow::slotExit()
{
    QCoreApplication::quit();
}

void MainWindow::slotAbout()
{
    QDialog *wAbout = new QDialog(this);
    QString sInfo = QString("Product: Visual representation of measurements\n")
            .append("Author: Yurevich Evgeniy\n")
            .append("e-mail: evgeniy.yurevich@gmail.com\n")
            .append("Date of issue: 30.01.2021\n");
    QLabel *label = new QLabel(sInfo);
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(label);
    wAbout->setWindowTitle("About");
    wAbout->setLayout(vlayout);
    wAbout->setModal(true);
    wAbout->show();
}


MainWindow::~MainWindow()
{
    delete ui;
}

