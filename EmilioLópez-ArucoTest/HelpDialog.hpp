#ifndef _HELPDIALOG_H_
#define _HELPDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Button.H>

extern Fl_Window *winCalib, *pop;
extern bool popupActive;

namespace aruco_test{

    class HelpDialog : public Fl_Window {

        private:
            Fl_Button *bCalib, *bMarker, *bClose;
            Fl_Text_Buffer *styleBuffer, *textBuffer;
            Fl_Text_Display *container;

        public:
    // APP WINDOW CONSTRUCTOR
    HelpDialog(int X,int Y,int W,int H,const char*L=0) : Fl_Window(X,Y,W,H,L) {

        bClose = new Fl_Button(130,(Fl::h()/2)-197,140,22,"Cerrar");
        bClose->callback(HelpDialog::Close, (void*)this);

        bMarker = new Fl_Button(341,61,50,22,"tag");
        bMarker->callback(HelpDialog::ShowMarker, (void*)this);
        bMarker->box(FL_NO_BOX);
        bMarker->labelcolor(FL_BLUE);

        bCalib = new Fl_Button(341,88,100,22,"calibración");
        bCalib->callback(HelpDialog::CallCalibrate, (void*)this);
        bCalib->box(FL_NO_BOX);
        bCalib->labelcolor(FL_BLUE);

        Fl_Text_Display::Style_Table_Entry stable[] = {
         // FONT COLOR      FONT FACE   FONT SIZE
         // --------------- ----------- --------------
            {  FL_BLACK,         0, 24 }, // A - Red
            {  FL_BLACK, 0, 14 }, // B - Yellow
            {  FL_BLACK,  0, 14 }, // C - Green
            {  FL_BLACK,        0, 14 }, // D - Blue
        };
        container = new Fl_Text_Display(20, 25, 336, 165, "");
        container->box(FL_NO_BOX);
        textBuffer = new Fl_Text_Buffer();      // text buffer
        Fl_Text_Buffer *styleBuffer = new Fl_Text_Buffer();      // style buffer
        container->buffer(textBuffer);
        int stable_size = sizeof(stable)/sizeof(stable[0]);
        container->highlight_data(styleBuffer, stable, stable_size, 'A', 0, 0);
        // Text
        ///textBuffer->text("Guía de uso:\n1-Imprimir la imagen  recortable adjunta y montar el\n2-Completar la configuración de cámara mediante la\n3-Seleccionar modo de simulación\n4-Mover el tag delante de la cámara\n");
        textBuffer->text("Guía de uso:\n1-Imprimir la imagen  recortable adjunta y montar el\n2-Completar la configuración de cámara mediante la\n3-Seleccionar modo de simulación  \n4-Mover el tag delante de la cámara\n");
        // Style for text
        styleBuffer->text("AAAAAAAAAAAA\nBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\nEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");

        ///fl_boxbox = new Fl_Box(130,(Fl::h()/2)-80,300,62,"");

    }
        static int showCalibrate();
        static void Close(Fl_Widget*w, void *d);
        static void ShowMarker(Fl_Widget*w, void *d);
        static void CallCalibrate(Fl_Widget*w, void *d);

    };

}
#endif
