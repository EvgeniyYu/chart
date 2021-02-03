#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/Xos.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <thread>
#include <future>
#include <sstream>
#include "cwindow.h"

void ChartWindow::draw()
{
    int x_window, y_window;
    unsigned int width_window, height_window;
    unsigned int border_width_window, depth_window;

    //set color
    int blackpixel = BlackPixel(display, DefaultScreen(display));
    int whitepixel = WhitePixel(display, DefaultScreen(display));
    XColor red, blue;
    Status rc;
    gcv.background = whitepixel;
	gcv.foreground = blackpixel;
	Colormap screen_colormap = DefaultColormap(display, DefaultScreen(display));
	rc = XAllocNamedColor(display, screen_colormap, "red", &red, &red);
	rc = XAllocNamedColor(display, screen_colormap, "blue", &blue, &blue);

    //create context and get window geometry
	gc = XCreateGC(display, DefaultRootWindow(display), GCForeground |
		GCBackground, &gcv);
    XGetGeometry(display, win, &root_window, &x_window, &y_window,
                &width_window, &height_window, &border_width_window, &depth_window);
		
    //set window title name
    std::string sBaseName = sFileName.substr(sFileName.find_last_of("/\\") + 1);
    XStoreName(display, win, sBaseName.c_str());
	
	//set XY-coordinate lines
	const int space = 40;
	const int axis_left = 80;
	const int axis_top = 80;
        const int axis_width = width_window - axis_left - space;
        const int axis_height = height_window - axis_top - space;
	XSetForeground(display, gc, blue.pixel);
	XDrawRectangle(display, win, gc, axis_left, axis_top, axis_width, axis_height);
	
	//draw plot
	XSetForeground(display, gc, red.pixel);
	double dX = maxX - minX;
	double dY = maxY - minY;
    if (points.size() < 2) return;
    for (unsigned int i = 0; i < points.size() - 1; i++)
	{
		double _x = points[i].x;
		double _y = points[i].y;
		double nX = (_x - minX)/dX;
		double nY = (_y - minY)/dY; 
		double w1 = (nX * (double)axis_width) + (double)axis_left;
		double h1 = ((1- nY) * (double)axis_height) + (double)axis_top;
		
		_x = points[i+1].x;
		_y = points[i+1].y;
		nX = (_x - minX)/dX;
		nY = (_y - minY)/dY; 
		double w2 = (nX * (double)axis_width) + (double)axis_left;
		double h2 = ((1 - nY) * (double)axis_height) + (double)axis_top;
				
		XDrawLine(display, win, gc, w1, h1, w2, h2);
	}
	
	//draw X coordinate steps
	XSetForeground(display, gc, blue.pixel);
	int count_steps = 5;
	float step = 0;
	for (int i = 0; i <= count_steps; i++)
	{
		float w1 = axis_left + step;
		float h1 = axis_top + axis_height + 3;
		float w2 = axis_left + step;
		float h2 = axis_top + axis_height - 3;
		step += axis_width/count_steps;
		
		float x_number = (dX * (w1 - axis_left)/axis_width) + minX;
		
		XDrawLine(display, win, gc, w1, h1, w2, h2);

        char str[12];
        sprintf(str, "%.03f", x_number);
		XDrawString(display, win, gc, w1 - 5, h1 + 10, str, strlen(str));
	}

	//draw X label	
	XDrawString(display, win, gc, axis_left + axis_width - 20, axis_top + axis_height + 30, "Time", strlen("Time"));

    //draw Y coordinate steps
	step = 0;	
	for (int i = 0; i <= count_steps; i++)
	{
		float w1 = axis_left - 3;
		float h1 = axis_top + step;
		float w2 = axis_left + 3;
		float h2 = axis_top + step;
		step += axis_height/count_steps;
        float y_number = dY * (1 - (float)(h1 - axis_top)/axis_height) + minY;
		XDrawLine(display, win, gc, w1, h1, w2, h2);
				
		char str[12];
        sprintf(str, "%.01f", y_number);
		XDrawString(display, win, gc, w1 - 70, h1, str, strlen(str));

	}

	//draw Y label
	XDrawString(display, win, gc, axis_left - 70, axis_top - 20, "Frequency", strlen("Frequency"));	

    //draw header
    XSetForeground(display, gc, blackpixel);

    std::string line;
    std::istringstream iss(sHeader);
    int top_pos = axis_top - 65;
    while (std::getline(iss, line))
    {
        XDrawString(display, win, gc, axis_left, top_pos, line.c_str(), strlen(line.c_str()));
        top_pos += 12;
    }
}

//----------------------------------------------------------------
ChartWindow::ChartWindow(const std::string& fileName):
    DataReader(fileName)
{

}

bool ChartWindow::init()
{
    display = XOpenDisplay(NULL);
    if (display == NULL)
	{
        printf("Error: Cannot open display\r\n");
		return false;
	}
	
    screen = DefaultScreen(display);
    depth = DefaultDepth(display, screen);
    root_window = RootWindow(display, screen);
    attrs.border_pixel = BlackPixel(display, screen);
    attrs.background_pixel = WhitePixel(display, screen);
	attrs.colormap = CopyFromParent;
	attrs.event_mask = ExposureMask | KeyPressMask;
	
    win = XCreateWindow(display, root_window, 700, 700, 500, 400, 0, depth, InputOutput, CopyFromParent, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &attrs);
	
    XMapWindow(display, win);
	
    //read points from file
    if (!readFile())
        return false;

    //calculate min and max values of X, Y
    auto minmaxX = std::minmax_element(points.begin(), points.end(),
                                [] (Point& p1, Point& p2) {return p1.x < p2.x;});
    auto minmaxY = std::minmax_element(points.begin(), points.end(),
                                [] (Point& p1, Point& p2) {return p1.y < p2.y;});
    minX = minmaxX.first->x;
    maxX = minmaxX.second->x;
    minY = minmaxY.first->y;
    maxY = minmaxY.second->y;

    return true;
}

ChartWindow::~ChartWindow()
{
    if (display != NULL)
	{
       XDestroyWindow(display, win);
       XCloseDisplay(display);
	}
}
	
bool ChartWindow::start()
{
    auto future = std::async(std::launch::async, &ChartWindow::run, this);
    bool res = future.get();
    return res;
}

bool ChartWindow::run()
{
    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, win, &wmDeleteMessage, 1);
	while (1)
	{
        XNextEvent(display, &event);
		if (event.type == Expose)
            draw();
        else if (event.type == ClientMessage)
        {
            if (event.xclient.data.l[0] == wmDeleteMessage)
            {
                XDestroyWindow(display, win);
                XCloseDisplay(display);
                display = NULL;
                return true;
            }
        }
        else
        {
            break;
        }
	}
    return false;
}
