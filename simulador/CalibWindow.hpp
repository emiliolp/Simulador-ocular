/**
 * Definici�n de la clase CalibWindow
 * Representa la ventana de instrucciones de calibraci�n y los controles para realizar tal tarea
 */
#ifndef _CALIBWINDOW_H_
#define _CALIBWINDOW_H_

#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Button.H>
#include <opencv2/calib3d/calib3d.hpp>
#include <aruco/aruco.h>
#include <FL/fl_ask.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Box.H>

using namespace cv;
using namespace aruco;

extern Marker markerChoosed;                        //Marcador seleccionado
extern bool noExisteTag, flagCalibration;
extern MarkerMap theMarkerMapConfig;
//Posiciones tomadas en la calibraci�n
extern vector < vector<cv::Point2f>  > calibP2d;
extern vector < vector<cv::Point3f>  > calibP3d;
//Par�metros de la c�mara
extern CameraParameters camp;
extern Mat inputImage;
extern CameraParameters cameraParams;
//Ventana de calibraci�n
extern Fl_Window *winCalib;
//Controles de la pantalla
extern Fl_JPEG_Image *calibImage1, *calibImage2, *calibImage3, *calibImage4;
extern Fl_Box *boxCalibra1, *boxCalibra2, *boxCalibra3, *boxCalibra4;

namespace simulator{

    /**\brief Clase que representa la ventana con las instrucciones de calibraci�n
     *
     */
	class CalibWindow : public Fl_Window{

        private:

            Fl_Text_Display *container;                 /**<@brief �rea de texto de la imagen*/
            Fl_Text_Buffer *styleBuffer;                /**<@brief estilo del texto mostrado*/
            Fl_Text_Buffer *textBuffer;                 /**<@brief texto mostrado en la ventana*/
            Fl_Button *bCapture;                        /**<@brief bot�n de captura*/

            /**
             *@brief M�todo para almacenar la posici�n obtenida del tag en la calibraci�n
             *
             *Este m�todo ha sido tomado de la funci�n del mismo nombre del fichero aruco_calibration.cpp de la librer�a ArUco
             *@param p2d punto 2d seleccionado
             *@param p3d punto 3d seleccionado
             *@param bc  configuraci�n del tag
             */
            static void getMarker2d3d(vector<cv::Point2f> &p2d, vector<cv::Point3f> &p3d, const MarkerMap &bc);

            /**
             *@brief M�todo ejecutado tras capturar una imagen en la calibraci�n
             *
             *Para realizar la calibraci�n a partir de los puntos tomados del tag y la generaci�n del fichero se ha tomado la funci�n main
             *del fichero aruco_calibration.cpp de la librer�a ArUco
             *@param w elemento seleccionado
             */
            static void capture(Fl_Widget *w);

        public:

            /**
             *@brief Constructor de la clase,
             *crea una instancia de la clase que representa la ventana de calibraci�n
             *@param X punto del eje x donde en el que se inicia la ventana
             *@param Y punto del eje y donde se inicia la ventana
             *@param W ancho de la ventana
             *@param H altura de la ventana
             *@param L t�tulo de la ventana
             */
            CalibWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Window(X,Y,W,H,L) {

                bCapture = new Fl_Button(300,350,140,22,"Capturar");
                bCapture->callback(CalibWindow::capture);

                Fl_Text_Display::Style_Table_Entry stable[] = {
                    //Especificamos color de la fuente y tama�o
                    {  FL_BLACK, 0, 24 },
                    {  FL_BLACK, 0, 18 },
                    {  FL_BLACK, 0, 14 },
            };
            container = new Fl_Text_Display(20, 10, 710, 138, "");  //Contenedor del texto
            container->box(FL_NO_BOX);
            textBuffer = new Fl_Text_Buffer();      //buffer texto
            styleBuffer = new Fl_Text_Buffer();     //buffer estilo
            container->buffer(textBuffer);
            int stableSize = sizeof(stable)/sizeof(stable[0]);
            container->highlight_data(styleBuffer, stable, stableSize, 'A', 0, 0);

            textBuffer->text("Siga las siguientes instrucciones para calibrar la c�mara:\n(s�lo es necesario realizarlo una vez)\n1-Reproduzca cada una de las siguientes vistas del tag frente a la c�mara en el cuadrado amarillo.\n2-Pulse Capturar para almacenar cada imagen.\n");
            styleBuffer->text("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");

        }

	};
}
#endif
