/**
 * Definici�n de la clase HelpDialog
 * Representa la ventana de ayuda que muestra las instrucciones de uso de la aplicaci�n
 */
#ifndef _HELPDIALOG_H_
#define _HELPDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Button.H>
#include "CalibWindow.hpp"

extern Fl_Window *winCalib, *pop;
extern bool popupActive;

namespace simulator{

    /**\brief Clase que representa la ventana de ayuda de la aplicaci�n
     *
     */
    class HelpDialog : public Fl_Window {

        private:

            Fl_Button *bCalib;              /**<@brief enlace para visualizar el tag*/
            Fl_Button *bMarker;             /**<@brief enlace para calibrar la c�mara*/
            Fl_Button *bClose;              /**<@brief bot�n para cerrar la ventana*/
            Fl_Text_Buffer *styleBuffer;    /**<@brief estilo del texto mostrado en la ventana*/
            Fl_Text_Buffer *textBuffer;     /**<@brief texto mostrado en la ventana*/
            Fl_Text_Display *container;     /**<@brief �rea que contiene el texto*/

            /**
             *@brief Inicializa los valores de la ventana de calibraci�n
             *
             * Especifica dimensiones de la ventana e im�genes a mostrar
             */
            static int showCalibrate();

            /**
             *@brief Cierra la ventana de ayuda
             *@param w elemento de control seleccionado
             */
            static void close(Fl_Widget*w);

            /**
             *@brief Muestra la imagen del tag que se debe utilizar
             *@param w elemento de control seleccionado
             */
            static void showMarker(Fl_Widget*w);

            /**
             *@brief M�todo para llamar a la ventana de calibraci�n
             *@param w elemento de control seleccionado
             */
            static void callCalibrate(Fl_Widget*w);

        public:

            /**
             *@brief Constructor de la clase
             *
             *Crea una instancia de la clase que representa la ventana de di�logo
             *@param X punto del eje x donde comienza la ventana
             *@param Y punto del eje y donde comienza la ventana
             *@param W ancho de la ventana
             *@param H altura de la ventana
             *@param L t�tulo de la ventana
             */
            HelpDialog(int X,int Y,int W,int H,const char*L=0) : Fl_Window(X,Y,W,H,L) {

            //Bot�n de cerrar
            bClose = new Fl_Button(135,(Fl::h()/2)-20,140,22,"Cerrar");
            bClose->callback(HelpDialog::close);

            //Enlace para mostrar el marcador
            bMarker = new Fl_Button(341,91,50,22,"tag");
            bMarker->callback(HelpDialog::showMarker);
            bMarker->box(FL_NO_BOX);
            bMarker->labelcolor(FL_BLUE);

            //Mostrar gu�a de calibraci�n
            bCalib = new Fl_Button(341,119,100,22,"calibraci�n");
            bCalib->callback(HelpDialog::callCalibrate);
            bCalib->box(FL_NO_BOX);
            bCalib->labelcolor(FL_BLUE);

            Fl_Text_Display::Style_Table_Entry stable[] = {

                {  FL_BLACK, 0, 25 },   //Color del texto, tama�o de la fuente
                {  FL_BLACK, 0, 18 },
                {  FL_BLACK, 0, 14 },
            };

            container = new Fl_Text_Display(20, 25, 336, 340, "");
            container->box(FL_NO_BOX);
            textBuffer = new Fl_Text_Buffer();                       // buffer texto
            Fl_Text_Buffer *styleBuffer = new Fl_Text_Buffer();      // buffer estilo
            container->buffer(textBuffer);
            int stableSize = sizeof(stable)/sizeof(stable[0]);
            container->highlight_data(styleBuffer, stable, stableSize, 'A', 0, 0);
            //Texto
            textBuffer->text("Simulador motilidad ocular\nGu�a de uso:\n1-Imprimir la imagen  recortable adjunta y montar el\n2-Completar la configuraci�n de c�mara mediante la\n3-Seleccionar modo de simulaci�n  \n4-Mover el tag delante de la c�mara\n_________________________________________\nTrabajo Fin de Grado de la Universidad de C�rdoba\nDirectores: Antonio Ara�zo Azofra\n                  Fernando Labella Quesada\nAutor:         Emilio L�pez Pi�a\n");
            //Estilo del texto
            styleBuffer->text("AAAAAAAAAAAAAAAAAAAAAAAAAA\nBBBBBBBBBBBB\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");

        }

    };

}
#endif
