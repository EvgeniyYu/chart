#ifndef DATAREADER_H
#define DATAREADER_H

#include <vector>
#include <string>

enum ERROR {FILE_OPEN_ERROR, LINE_ERROR};

struct Result
{
    ERROR error;
    int line;
    Result(): line(0) {}
    Result(ERROR _error, int _line = 0):
        error(_error), line(_line) {}
    ~Result() {}
};


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
private:
    std::string sFileName;
protected:
    std::string sHeader;
    std::vector<Point> points;
    bool readFile(Result &result);
    std::string getError(ERROR err);
public:
    DataReader(const std::string& _sFileName);
    ~DataReader();
};

#endif // DATAREADER_H
