#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <aruco/aruco.h>
#include <iostream>
#include "Point3D.hpp"
#include "TypeV.hpp"
#include "TypeF.hpp"
#include "CalibWindow.hpp"
#include "HelpDialog.hpp"
#include "FaceWindow.hpp"
#include "CamWindow.hpp"
#include "SimulatorWindow.hpp"
#include <string.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl_Round_Button.H>
#include <Fl/Fl_Check_Button.H>
#include <cstdlib>
//#include <FL/Fl_Text_Buffer.H>
//#include <FL/Fl_Text_Display.H>
#include "wtypes.h"
#include <FL/Fl_Pixmap.H>
//#include "help_button.hpp"
//#include "aruco_calibration_board_a4.h"
#include <conio.h>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>

#define NORMAL      0
#define ENDOTROPIA  1
#define EXOTROPIA   2
#define HIPERTROPIA 3
#define HIPOTROPIA  4

#define TRANSPARENTE 0
#define OPACO        1

#define WIDTH   1195
#define HEIGHT  521
#define HPANEL  100

using namespace cv;
using namespace aruco;
using namespace std;
using namespace aruco_test;

string TheInputVideo;
VideoCapture TheVideoCapturer;
vector<Marker> TheMarkers;      //vector de marcadores encontrados
vector<TypeV> VvLente, VvFace, VvEye, VnFace, VnEye, VnLente;
vector<TypeF> VfLente, VfFace, VfEye;
Mat TheInputImage,TheUndInputImage,TheResizedImage;
CameraParameters TheCameraParams;
bool TheCaptureFlag=true, popupActive = false, flagCalibration = false, noExisteTag = true;
MarkerDetector PPDetector;
Size TheGlWindowSize;
Point3D MarkerPx, MarkerCm;
float TheMarkerSize=-1, rotx1, roty1, rotz1, rotz2, rotx2, roty2, rotx, roty, rotz, rot_ant = 0.0;
int econt=0, CameraPosition, curr_string_iz = 0, curr_string_de = 0;
Marker Marker1;
char cadena[200], buffer[200];
int fill_resize_rostro = 0, eye_test, pat_test, angle_test, oclusor_iz = TRANSPARENTE, oclusor_de = TRANSPARENTE, modo_examen = 0;
Fl_Box *boxCalibra1, *boxCalibra2, *boxCalibra3, *boxCalibra4;
Fl_Window *pop; //*margin_right,
Fl_Window *winCalib;
MarkerMap TheMarkerMapConfig;
aruco::CameraParameters camp;//camera parameters estimated

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 128.0 };
GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
GLfloat rot_angle_y = 0.0;
GLfloat rot_angle_x = 0.0;
GLfloat ROT_INC = 0.1;
Fl_Choice *choice_iz, *choice_de, *choice_ian, *choice_dan, *choice_ojo, *choice_pat, *choice_ean, *choice_oiz, *choice_ode;
Fl_Button *b_paciente, *b_comprobar, *b_help, *b_close, *b_marker, *b_calib, *b_start_calib, *b_finish_calib;
int choice_ian45 = 0, choice_dan45 = 0, angle_iz = 5, angle_de = 5, eye, patologia, angle;
Fl_Check_Button *check_oiz, *check_ode;
Point2f MarkerAnt;
vector < vector<cv::Point2f>  > calib_p2d;
vector < vector<cv::Point3f>  > calib_p3d;
Fl_JPEG_Image *calibImage1, *calibImage2, *calibImage3, *calibImage4;
