#ifndef _CAMWINDOW_H_
#define _CAMWINDOW_H_
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <iostream>
#include <cstdlib>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <aruco/aruco.h>
#include <FL/gl.h>
#include <FL/fl_ask.H>
#include "Point3D.hpp"

#define NORMAL      0
#define ENDOTROPIA  1
#define EXOTROPIA   2
#define HIPERTROPIA 3
#define HIPOTROPIA  4

using namespace aruco_test;
using namespace aruco;
using namespace std;
using namespace cv;

extern bool TheCaptureFlag, flagCalibration, noExisteTag, flagCalibration, noExisteTag;
extern VideoCapture TheVideoCapturer;
extern Mat TheInputImage, TheUndInputImage, TheResizedImage;
extern MarkerDetector PPDetector;
extern vector<Marker> TheMarkers;
extern char cadena[200], buffer[200];
extern Size TheGlWindowSize;
extern Marker Marker1;
extern int econt;
extern int curr_string_iz, curr_string_de, angle_iz, angle_de;
extern CameraParameters TheCameraParams;
extern Point3D MarkerPx, MarkerCm;
extern float rotx1, roty1, rotz1, rotz2, rotx2, roty2, rotx, roty, rotz, TheMarkerSize;
extern Point2f MarkerAnt;
extern vector < vector<cv::Point3f>  > calib_p3d;

namespace aruco_test{

    class CamWindow : public Fl_Gl_Window{

        double fg;                       //brillo del primer plano
        double bg;                       //brillo de fondo

        // OPENGL WINDOW CONSTRUCTOR
        public:
    CamWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {
        fg = 1.0;
        bg = 0.0;
        Fl::add_timeout(1.0/5000.0, Timer_CB, (void*)this);       // 24fps timer
        end();
    }
    void SetBrightness(double new_fg, double new_bg)
        { fg = new_fg; bg = new_bg; redraw(); }

        void draw();
        void resize(int x, int y, int w,int h);
        static void Timer_CB(void *userdata);
        void vIdle();
        void detectMarker();
        void calculateEyesPosition(Point2f MarkerC);

    };

}
#endif
