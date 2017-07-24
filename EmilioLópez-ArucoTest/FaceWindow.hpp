#ifndef _FACEWINDOW_H_
#define _FACEWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <Fl/Fl_Check_Button.H>
#include <FL/gl.h>
#include "TypeV.hpp"
#include "TypeF.hpp"
#include <aruco/aruco.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#define TRANSPARENTE 0
#define OPACO        1

#define NORMAL      0
#define ENDOTROPIA  1
#define EXOTROPIA   2
#define HIPERTROPIA 3
#define HIPOTROPIA  4

using namespace aruco_test;
using namespace aruco;
using namespace cv;

extern int fillResizeFace, examMode, leftOccluder, rightOccluder;
extern bool popupActive;
extern CameraParameters cameraParams;
extern float rotAnt;
extern Fl_Check_Button *checkLeftOc, *checkRightOc;
extern Marker markerChoosed;
extern int leftEyePath, rightEyePath;
extern int leftAngle, rightAngle;

using namespace aruco;

namespace aruco_test{

    class FaceWindow : public Fl_Gl_Window {

    private:

        vector<TypeV> vvLente, vvFace, vvEye, vnFace, vnEye, vnLente;
        vector<TypeF> vfLente, vfFace, vfEye;

        static void Timer_CB(void *userdata);

        public:
        FaceWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {

        //Cargamos las figuras que forman el modelo 3D
        loadFile("figuras/cara.obj", vfFace, vvFace, vnFace);
        loadFile("figuras/ojo.obj", vfEye, vvEye, vnEye);
        loadFile("figuras/oclusor.obj", vfLente, vvLente, vnLente);

        Fl::add_timeout(1.0/5000.0, Timer_CB, (void*)this);       // 24fps timer
        end();
    }

     void draw();
     void resize(int x, int y, int w,int h);
     void drawShape(vector<TypeF> arrayF, vector<TypeV> arrayV, vector<TypeV> arrayVn, int shape);
     void initRendering(Point3f light);
     void loadFile(char *filename, vector<TypeF> &arrayF, vector<TypeV> &arrayV, vector<TypeV> &arrayVn);
     void calculateEyesPosition(Point3f &leftRot, Point3f &rightRot, Point3f &lightRot, Marker marker);//Point2f MarkerC);

    };

}
#endif // _FACEWINDOW_H_

     void drawShape(vector<TypeF> ArrayF, vector<TypeV> ArrayV, vector<TypeV> ArrayVn, int shape);
     void initRendering();
     //static void Timer_CB(void *userdata);
     void loadFile(char *filename, vector<TypeF> &ArrayF, vector<TypeV> &ArrayV, vector<TypeV> &ArrayVn);

    };

}
#endif // _FACEWINDOW_H_
