#include "dataReader.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

DataReader::DataReader(const std::string& _sFileName): sFileName(_sFileName)
{
}

DataReader::~DataReader()
{

}

int getCountLines(const std::string& fname)
{
    std::ifstream ifs(fname.c_str());

    return std::count(std::istreambuf_iterator<char>(ifs),
                      std::istreambuf_iterator<char>(), '\n');
}

bool DataReader::readFile(Result &result)
{
    if (points.size())
        points.clear();

    const int fsize = getCountLines(sFileName);
    points.reserve(fsize);

    std::fstream fs;
    fs.open(sFileName.c_str(), std::ios::in);
    if (!fs.is_open())
    {
        result.error = FILE_OPEN_ERROR;
        return false;
    }

    std::string line;
    int num_line = 0;
    double a, b;
    std::size_t found;

    while (std::getline(fs, line))
    {
        num_line++;

        found = line.find("#");
        if (found != std::string::npos)
        {
            //display only three lines of header
            if (num_line <= 3)
                sHeader += line + "\n";
            continue;
        }

        if (line.length() < 2)    //allow empty strings
            continue;

        std::istringstream iss(line);
        if (!(iss >> a >> b))
        {
            fs.close();
            result.error = LINE_ERROR;
            result.line = num_line;
            return false;
        }
        else
        {
            points.push_back(Point(a, b));
        }
    }
    fs.close();

    //remove symbol '#' in header
    sHeader.erase(std::remove(sHeader.begin(), sHeader.end(), '#'), sHeader.end());

    return true;
}

std::string DataReader::getError(ERROR err)
{
    switch (err)
    {
        case FILE_OPEN_ERROR:
            return "File cannot be open";
        case LINE_ERROR:
            return "Line error";
        default:
            return "";
    }
}
