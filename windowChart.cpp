#include <QChart>
#include <QChartView>
#include <QLabel>
#include <QVBoxLayout>
#include <QtCharts>
#include <QLineSeries>
#include <QPointF>
#include <QFileInfo>
#include "windowChart.h"

using namespace QtCharts;

WindowChart::WindowChart(const QString& sFileName, QWidget *widget): DataReader(sFileName.toStdString()), QWidget(widget)
{
    QString qsHeader;

    QChartView* chartView = new QChartView(this);
    QChart* chart = new QChart();

    QFileInfo fileInfo(sFileName);
    this->setWindowTitle(QString("Chart filename: ").append(fileInfo.fileName()));

    Result result;
    if (!readFile(result))
    {
        //error
        qsHeader = QString("ERROR: ").append(QString::fromStdString(getError(result.error)));
        if (result.error == LINE_ERROR)
            qsHeader.append(": Line ").append(QString::number(result.line));
    }
    else
    {
        QLineSeries* series = new QLineSeries();
        for (unsigned int i = 0; i < points.size(); i++)
        {
            *series << QPointF(points[i].x, points[i].y);
        }
        chart->addSeries(series);
        chart->createDefaultAxes();

        chart->setTitle(QString("Chart: ").append(fileInfo.fileName()));
        chartView->setChart(chart);

        qsHeader = QString::fromStdString(sHeader);
    }

    QLabel *label = new QLabel(qsHeader);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(label);
    vlayout->addWidget(chartView);

    this->setLayout(vlayout);

    this->resize(600, 500);
    this->show();

}


WindowChart::~WindowChart()
{

}
