#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <opencv2/calib3d/calib3d.hpp>
#include <aruco/aruco.h>
#include "SimulatorWindow.hpp"
#include <GL/glut.h>
#include <FL/Fl_Round_Button.H>
#include <Fl/Fl_Check_Button.H>
#include <FL/Fl_JPEG_Image.H>

using namespace cv;
using namespace aruco;
using namespace std;
using namespace aruco_test;

VideoCapture videoCapturer;
Mat inputImage;
CameraParameters cameraParams;
bool popupActive = false, flagCalibration = false, noExisteTag = true;
Point3D markerPx, markerCm;
float rotAnt = 0.0;
int econt=0, leftEyePath = NORMAL, rightEyePath = NORMAL, leftAChoice45 = 0, rightAChoice45 = 0, leftAngle = 0, rightAngle = 0, eye, path, angle;
int fillResizeFace = 0, eyeTest, patTest, angleTest, leftOccluder = TRANSPARENTE, rightOccluder = TRANSPARENTE, examMode = 0;
Marker markerChoosed;
Fl_Box *boxCalibra1, *boxCalibra2, *boxCalibra3, *boxCalibra4;
Fl_Window *pop, *winCalib;
MarkerMap theMarkerMapConfig;
CameraParameters camp;//camera parameters estimated
Fl_Choice *leftChoice, *rightChoice, *leftAChoice, *rightAChoice, *eyeChoice, *patChoice, *examAChoice, *leftOChoice, *righOChoice;
Fl_Button *patButton, *checkAnsButton, *helpButton, *closeButton, *markerButton, *calibButton, *startButton, *finishButton;
Fl_Check_Button *checkLeftOc, *checkRightOc;
Point2f markerAnt;
vector < vector<cv::Point2f>  > calibP2d;
vector < vector<cv::Point3f>  > calibP3d;
Fl_JPEG_Image *calibImage1, *calibImage2, *calibImage3, *calibImage4;

// MAIN
int main() {

    SimulatorWindow win(Fl::w(), Fl::h(), "Simulador motilidad ocular");
    win.resizable(win);
    win.color(Fl_Color(236));
    win.show();

    try{
        videoCapturer.open(0);

        if(videoCapturer.open(0) != 1)
            fl_message("Error al abrir la cámara","Aceptar",0,0);
            ///cout << "Video not opened" << endl;


        if(!videoCapturer.isOpened()){

            ///cerr<<"Could not open video"<<endl;
            fl_message("No se ha podido la cámara","Aceptar",0,0);
            return -1;

        }

        try{
            cameraParams.readFromXMLFile("C:\\ProgramData\\Simulador_motilidad_ocular\\camera.yml");
        }
        catch (std::exception &ex){
            win.callPopup();
        }

    }
    catch (std::exception &ex)
    {
        fl_message("No se ha podido la cámara","Aceptar",0,0);
    }
    return(Fl::run());
}

