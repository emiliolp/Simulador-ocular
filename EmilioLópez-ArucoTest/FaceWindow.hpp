#ifndef _FACEWINDOW_H_
#define _FACEWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <Fl/Fl_Check_Button.H>
#include <FL/gl.h>
#include <vector>
#include "TypeV.hpp"
#include "TypeF.hpp"
#include <aruco/aruco.h>

#define TRANSPARENTE 0
#define OPACO        1

using namespace aruco_test;
using namespace aruco;

extern vector<TypeV> VvLente, VvFace, VvEye, VnFace, VnEye, VnLente;
extern vector<TypeF> VfLente, VfFace, VfEye;
extern int fill_resize_rostro, modo_examen, oclusor_iz, modo_examen, oclusor_de;
extern bool popupActive;
extern CameraParameters TheCameraParams;
extern float rotx1, roty1, rotz1, rotz2, rotx2, roty2, rotx, roty, rotz, rot_ant;
extern GLfloat light_position[], mat_shininess[], mat_specular[];
extern Fl_Check_Button *check_oiz, *check_ode;

using namespace aruco;

namespace aruco_test{

    class FaceWindow : public Fl_Gl_Window {

        static void Timer_CB(void *userdata);

        public:
        FaceWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {

        //Cargamos las figuras que forman el modelo 3D
        loadFile("figuras/cara.obj", VfFace, VvFace, VnFace);
        loadFile("figuras/ojo.obj", VfEye, VvEye, VnEye);
        loadFile("figuras/oclusor.obj", VfLente, VvLente, VnLente);

        Fl::add_timeout(1.0/5000.0, Timer_CB, (void*)this);       // 24fps timer
        end();
    }

     void draw();
     void resize(int x, int y, int w,int h);
     void drawShape(vector<TypeF> ArrayF, vector<TypeV> ArrayV, vector<TypeV> ArrayVn, int shape);
     void initRendering();
     //static void Timer_CB(void *userdata);
     void loadFile(char *filename, vector<TypeF> &ArrayF, vector<TypeV> &ArrayV, vector<TypeV> &ArrayVn);

    };

}
#endif // _FACEWINDOW_H_
