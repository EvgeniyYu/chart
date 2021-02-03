#ifndef DATAREADER_H
#define DATAREADER_H

#include <vector>
#include <string>


struct Point
{
    double x;
    double y;
    Point(): x(0), y(0) {}
    Point(double _x, double _y): x(_x), y(_y) {}
    ~Point() {}
};

class DataReader
{
protected:
    std::string sFileName;
protected:
    std::string sHeader;
    std::vector<Point> points;
    bool readFile();
public:
    DataReader(const std::string& _sFileName);
    ~DataReader();
};

#endif // DATAREADER_H
