/**
 * Definición de la clase CamWindow
 * Determina el área de la ventana que representa el visor de la cámara
 */
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

using namespace simulator;
using namespace aruco;
using namespace cv;

extern bool flagCalibration, noExisteTag;
extern VideoCapture videoCapturer;
extern Mat inputImage;
extern Marker markerChoosed;
extern int cameraError;
extern CameraParameters cameraParams;
extern Point2f markerAnt;
extern vector < vector<cv::Point3f>  > calibP3d;
extern Fl_Window *winCalib;

namespace simulator{

    /**\brief Clase que representa el visor de la cámara
     *
     */
    class CamWindow : public Fl_Gl_Window{

        private:

            double brightFirstLevel;                 /**<@brief brillo del primer plano*/
            double brightBack;                       /**<@brief brillo de fondo*/
            vector<Marker> markersDetected;          /**<@brief marcadores detectados*/
            Mat undInputImage;                       /**<@brief imagen tomada por la cámara*/
            Mat resizedImage;                        /**<@biref imagen redimensionada*/
            MarkerDetector detector;                 /**<@biref detector del tag en la imagen*/
            Size windowSize;                         /**<@brief tamaño de la ventana*/

            /**
             *@brief Dibuja un punto identificando el tag en el visor de la cámara
             *
             *La forma de plasmar la imagen captada por la cámara en el visor ha sido tomada del fichero aruco_test_gl.cpp de la librería ArUco
             */
            void draw();

            /**
             *@brief Redimensiona la ventana del visor
             *@param x punto del eje x donde comienza el visor
             *@param y punto del eje y donde comienza el visor
             *@param w ancho de la ventana
             *@param h altura de la ventana
             */
            void resize(int x, int y, int w,int h);

            /**
             *@brief Método para actualizar la imagen del visor
             */
            static void updateScene(void *userdata);

            /**
             *@brief Método que toma la imagen a mostrar en el visor
             *
             *Los parámetros iniciales de la cámara han sido tomados del fichero aruco_calibration.cpp perteneciente a la librería ArUco
             */
            void detectMarker();

            /**
             *@brief Método para seleccionar el tag a mostrar y obtener los parámetros extrínsecos
             */
            void selectMarker();

            /**
             *@brief Actualiza el brillo de la imagen mostrada en el visor
             */
            void setBrightness(double newFg, double newBg);

        public:

            /**
             *@brief Constructor de la clase
             *
             *Crea una instancia de la clase que representa el visor de la cámara
             *@param X punto del eje x donde comienza el visor
             *@param Y punto del eje y donde comienza el visor
             *@param W ancho de la ventana
             *@param H altura de la ventana
             *@param L título de la ventana
             */
            CamWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {

                brightFirstLevel = 1.0;
                brightBack = 0.0;
                Fl::add_timeout(1.0/5000.0, updateScene, (void*)this);
                end();
        }

    };

}
#endif
