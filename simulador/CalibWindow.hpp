/**
 * Definición de la clase CalibWindow
 * Representa la ventana de instrucciones de calibración y los controles para realizar tal tarea
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
//Posiciones tomadas en la calibración
extern vector < vector<cv::Point2f>  > calibP2d;
extern vector < vector<cv::Point3f>  > calibP3d;
//Parámetros de la cámara
extern CameraParameters camp;
extern Mat inputImage;
extern CameraParameters cameraParams;
//Ventana de calibración
extern Fl_Window *winCalib;
//Controles de la pantalla
extern Fl_JPEG_Image *calibImage1, *calibImage2, *calibImage3, *calibImage4;
extern Fl_Box *boxCalibra1, *boxCalibra2, *boxCalibra3, *boxCalibra4;

namespace simulator{

    /**\brief Clase que representa la ventana con las instrucciones de calibración
     *
     */
	class CalibWindow : public Fl_Window{

        private:

            Fl_Text_Display *container;                 /**<@brief área de texto de la imagen*/
            Fl_Text_Buffer *styleBuffer;                /**<@brief estilo del texto mostrado*/
            Fl_Text_Buffer *textBuffer;                 /**<@brief texto mostrado en la ventana*/
            Fl_Button *bCapture;                        /**<@brief botón de captura*/

            /**
             *@brief Método para almacenar la posición obtenida del tag en la calibración
             *
             *Este método ha sido tomado de la función del mismo nombre del fichero aruco_calibration.cpp de la librería ArUco
             *@param p2d punto 2d seleccionado
             *@param p3d punto 3d seleccionado
             *@param bc  configuración del tag
             */
            static void getMarker2d3d(vector<cv::Point2f> &p2d, vector<cv::Point3f> &p3d, const MarkerMap &bc);

            /**
             *@brief Método ejecutado tras capturar una imagen en la calibración
             *
             *Para realizar la calibración a partir de los puntos tomados del tag y la generación del fichero se ha tomado la función main
             *del fichero aruco_calibration.cpp de la librería ArUco
             *@param w elemento seleccionado
             */
            static void capture(Fl_Widget *w);

        public:

            /**
             *@brief Constructor de la clase,
             *crea una instancia de la clase que representa la ventana de calibración
             *@param X punto del eje x donde en el que se inicia la ventana
             *@param Y punto del eje y donde se inicia la ventana
             *@param W ancho de la ventana
             *@param H altura de la ventana
             *@param L título de la ventana
             */
            CalibWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Window(X,Y,W,H,L) {

                bCapture = new Fl_Button(300,350,140,22,"Capturar");
                bCapture->callback(CalibWindow::capture);

                Fl_Text_Display::Style_Table_Entry stable[] = {
                    //Especificamos color de la fuente y tamaño
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

            textBuffer->text("Siga las siguientes instrucciones para calibrar la cámara:\n(sólo es necesario realizarlo una vez)\n1-Reproduzca cada una de las siguientes vistas del tag frente a la cámara en el cuadrado amarillo.\n2-Pulse Capturar para almacenar cada imagen.\n");
            styleBuffer->text("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");

        }

	};
}
#endif
