#ifndef WINDOWCHART_H
#define WINDOWCHART_H

#include <QWidget>
#include "dataReader.h"

class WindowChart: public DataReader, public QWidget
{
public:
    WindowChart(const QString& sFileName, QWidget *widget = NULL);
    ~WindowChart();
};


#endif // WINDOWCHART_H
