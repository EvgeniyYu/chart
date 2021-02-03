#ifndef _CWINDOW_
#define _CWINDOW_

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/Xos.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include "dataReader.h"

class ChartWindow: public DataReader
{
private:    
	int screen;
	Display *display;
    Window win, root_window;
	XEvent event;
	unsigned int depth;
	XSetWindowAttributes attrs;
    GC gc;
    XGCValues gcv;

    double minX, maxX, minY, maxY;

    bool run();
    void draw();
public:
    ChartWindow(const std::string& fileName);
	~ChartWindow();
    bool init();
    bool start();
};


#endif
