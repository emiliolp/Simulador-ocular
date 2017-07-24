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

extern Marker markerChoosed;
extern bool noExisteTag, flagCalibration;
extern MarkerMap theMarkerMapConfig;
extern vector < vector<cv::Point2f>  > calibP2d;
extern vector < vector<cv::Point3f>  > calibP3d;
extern aruco::CameraParameters camp;
extern Mat inputImage;
extern CameraParameters cameraParams;
extern Fl_Window *winCalib;
extern Fl_JPEG_Image *calibImage1, *calibImage2, *calibImage3, *calibImage4;
extern Fl_Box *boxCalibra1, *boxCalibra2, *boxCalibra3, *boxCalibra4;

namespace aruco_test{
	class CalibWindow : public Fl_Window{
	private:
		Fl_Text_Display *container;
        Fl_Text_Buffer *bufferStyle, *bufferText;
        Fl_Button *bCapture;
	public:
        CalibWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Window(X,Y,W,H,L) {
        //this->user_data(this);
        bCapture = new Fl_Button(300,350,140,22,"Capturar");
        bCapture->callback(CalibWindow::Capture, (void*)this);

//        //Test display superior
        Fl_Text_Display::Style_Table_Entry stable[] = {
         // FONT COLOR      FONT FACE   FONT SIZE
         // --------------- ----------- --------------
            {  FL_BLACK, 0, 24 }, // A - Red
            {  FL_BLACK, 0, 18 }, // B - Yellow
            {  FL_BLACK, 0, 14 }, // C - Green
            //{  FL_BLACK, 0, 14 }, // D - Blue
        };
        container = new Fl_Text_Display(20, 10, 710, 138, "");
        container->box(FL_NO_BOX);
        bufferText = new Fl_Text_Buffer();      // text buffer
        bufferStyle = new Fl_Text_Buffer();      // style buffer
        container->buffer(bufferText);
        int stableSize = sizeof(stable)/sizeof(stable[0]);
        container->highlight_data(bufferStyle, stable, stableSize, 'A', 0, 0);
        // Text
        // Style for text
        bufferText->text("Siga las siguientes instrucciones para calibrar la cámara:\n(sólo es necesario realizarlo una vez)\n1-Reproduzca cada una de las siguientes vistas del tag frente a la cámara en el cuadrado amarillo.\n2-Pulse Capturar para almacenar cada imagen.\n");
        bufferStyle->text("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");

    }

		static void getMarker2d3d(vector<cv::Point2f> &p2d, vector<cv::Point3f> &p3d, const MarkerMap &bc);
		static void Capture(Fl_Widget *w, void *d);

	};
}
#endif

        container = new Fl_Text_Display(20, 10, 710, 138, "");
        container->box(FL_NO_BOX);
        bufferText = new Fl_Text_Buffer();      // text buffer
        bufferStyle = new Fl_Text_Buffer();      // style buffer
        container->buffer(bufferText);
        int stable_size = sizeof(stable)/sizeof(stable[0]);
        container->highlight_data(bufferStyle, stable, stable_size, 'A', 0, 0);
        // Text
        // Style for text
        bufferText->text("Siga las siguientes instrucciones para calibrar la cámara:\n(sólo es necesario realizarlo una vez)\n1-Reproduzca cada una de las siguientes vistas del tag frente a la cámara en el cuadrado amarillo.\n2-Pulse Capturar para almacenar cada imagen.\n");
        bufferStyle->text("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");

    }

		static void getMarker2d_3d(vector<cv::Point2f> &p2d, vector<cv::Point3f> &p3d, const MarkerMap &bc);
		static void Capture(Fl_Widget *w, void *d);

	};
}
#endif
