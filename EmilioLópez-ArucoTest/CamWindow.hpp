#ifndef _CAMWINDOW_H_
#define _CAMWINDOW_H_
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <aruco/aruco.h>
#include <FL/gl.h>
#include <FL/fl_ask.H>
#include "Point3D.hpp"

using namespace aruco_test;
using namespace aruco;
using namespace cv;

extern bool flagCalibration, noExisteTag;
extern VideoCapture videoCapturer;
extern Mat inputImage;
extern Marker markerChoosed;
extern int econt;
extern CameraParameters cameraParams;
extern Point2f markerAnt;
extern vector < vector<cv::Point3f>  > calibP3d;

namespace aruco_test{

    class CamWindow : public Fl_Gl_Window{

        private:
        double fg;                       //brillo del primer plano
        double bg;                       //brillo de fondo
        vector<Marker> TheMarkers;
        Mat TheUndInputImage, TheResizedImage;
        MarkerDetector PPDetector;
        Size TheGlWindowSize;

        // OPENGL WINDOW CONSTRUCTOR
        public:
        CamWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {
        fg = 1.0;
        bg = 0.0;
        Fl::add_timeout(1.0/5000.0, timerCB, (void*)this);       // 24fps timer
        end();
    }
    void SetBrightness(double new_fg, double new_bg)
        { fg = new_fg; bg = new_bg; redraw(); }

        void draw();
        void resize(int x, int y, int w,int h);
        static void timerCB(void *userdata);
        void vIdle();
        void detectMarker();

    };

}
#endif

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
