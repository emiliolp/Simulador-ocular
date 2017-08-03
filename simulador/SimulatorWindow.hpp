/**
 * Definición de la clase SimulatorWindow
 * Determina la ventana principal de la aplicación que engloba el rostro humano,
 * el visor de la cámara y los controles de usuario
 */
#ifndef _SIMULATORWINDOW_H_
#define _SIMULATORWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Round_Button.H>
#include <Fl/Fl_Check_Button.H>
#include <FL/Fl_Pixmap.H>
#include "help_button.hpp"
#include "FaceWindow.hpp"
#include "CamWindow.hpp"
#include "HelpDialog.hpp"

extern Fl_Choice *leftChoice, *rightChoice, *leftAChoice, *rightAChoice, *eyeChoice, *patChoice, *examAChoice, *leftOChoice, *righOChoice;
extern Fl_Button *patButton, *checkAnsButton, *helpButton, *closeButton, *markerButton, *calibButton, *startButton, *finishButton;
extern Fl_Check_Button *checkLeftOc, *checkRightOc;
extern int rightAChoice45, leftAChoice45, eye, path, angle, eyeTest, patTest, angleTest;
extern Fl_Window *pop;

namespace simulator{
/**\brief Clase que representa la ventana principal de la aplicación
 *
 */
    class SimulatorWindow : public Fl_Window {

        private:
            FaceWindow *figureGl; /**<@brief Ventana rostro humano*/
            CamWindow  *viewer;   /**<@brief Visor de la cámara*/
            Fl_Group   *grUI;     /**<@brief Controles de usuario*/

            /**
             *@brief Método lanzado al seleccionar el modo exploración
             *@param w elemento seleccionado
             */
            static void chooseExploration(Fl_Widget*w);

            /**
             *@brief Método lanzado al seleccionar el modo examen
             *@param w elemento seleccionado
             */
            static void chooseExam(Fl_Widget*w);

            /**
             *@brief Método ejecutado al seleccionar la patología del ojo izquierdo
             *@param w elemento seleccionado
             */
            static void chooseLeftEye(Fl_Widget*w);

            /**
             *@brief Método ejecutado al seleccionar patología en el ojo derecho
             *@param w elemento seleccionado
             */
            static void chooseRightEye(Fl_Widget*w);

            /**
             *@brief Método para elegir el ángulo de rotación del ojo izquierdo
             *@param w elemento seleccionado
             */
            static void chooseLeftAngle(Fl_Widget*w);

            /**
             *@brief Método para elegir el ángulo de rotación del ojo derecho
             *@param w elemento seleccionado
             */
            static void chooseRightAngle(Fl_Widget*w);

            /**
             *@brief Método para generar paciente
             *@param w elemento seleccionado
             */
            static void generatePatient(Fl_Widget*w);

            /**
             *@brief Método ejecutado al seleccionar el ojo en el modo examen
             *@param w elemento seleccionado
             */
            static void chooseEye(Fl_Widget *w);

            /**
             *@brief Método para elegir patología en el modo examen
             *@param w elemento seleccionado
             */
            static void choosePath(Fl_Widget *w);

            /**
             *@brief Método ejecutado al seleccionar el ángulo de rotación en el modo examen
             *@param w elemento seleccionado
             */
            static void chooseAngle(Fl_Widget *w);

            /**
             *@brief Comprobamos la respuesta del usuario
             *@param w elemento seleccionado
             */
            static void checkAnswer(Fl_Widget*w);

            /**
             *@brief Método lanzado al seleccionar el oclusor del ojo izquierdo
             *@param w elemento seleccionado
             */
            static void controlLeftOc(Fl_Widget*w);

            /**
             *@brief Método lanzado al seleccionar el oclusor del ojo derecho
             *@param w elemento seleccionado
             */
            static void controlRightOc(Fl_Widget*w);

            /**
             *@brief Método para seleccionar el material del oclusor izquierdo
             *@param w elemento seleccionado
             */
            static void chooseMatLOc(Fl_Widget*w);

            /**
             *@brief Método para seleccionar el material del oclusor derecho
             *@param w elemento seleccionado
             */
            static void chooseMatROc(Fl_Widget*w);

            /**
             *@brief Método que muestra la guía de uso al pulsar sobre el botón de ayuda
             *@param w elemento seleccionado
             */
            static void showHelp(Fl_Widget*w);

        public:

            /**
             *@brief Constructor de la clase,
             *crea una instancia de la clase que representa la ventana principal
             *@param w ancho de la ventana
             *@param h altura de la ventana
             *@param l título de la ventana
             */
            SimulatorWindow(int W,int H,const char*L=0) : Fl_Window(W,H,L) {

                figureGl = new FaceWindow(300, 0, w()-600, h()-120);
                viewer   = new CamWindow(w()-300,0, 300, 225);

                //Radiobuttons exploración y examen
                grUI = new Fl_Group(10,672,800,25);
                {
                    Fl_Round_Button* grUI = new Fl_Round_Button(32, 672, 113, 25, "Exploración");
                    grUI->type(102);
                    grUI->down_box(FL_ROUND_DOWN_BOX);
                    grUI->setonly();
                    grUI->callback(SimulatorWindow::chooseExploration);

                }

                {
                    Fl_Round_Button* grUI = new Fl_Round_Button(680, 672, 113, 25, "Examen");
                    grUI->type(102);
                    grUI->down_box(FL_ROUND_DOWN_BOX);
                    grUI->callback(SimulatorWindow::chooseExam);
                }
                grUI->end();

                //Selección patología ojo izquierdo
                leftChoice = new Fl_Choice(137,698,140,22,"Ojo izquierdo:");
                leftChoice->add("Sin patología");
                leftChoice->add("Endotropia");
                leftChoice->add("Exotropia");
                leftChoice->add("Hipertropia");
                leftChoice->add("Hipotropia");
                leftChoice->callback(SimulatorWindow::chooseLeftEye);
                leftChoice->value(0);
                leftChoice->color(FL_BLUE);

                //Selección ángulo ojo izquierdo
                leftAChoice = new Fl_Choice(360,698,70,22,"Ángulo:");
                leftAChoice->add("15º");
                leftAChoice->add("30º");
                leftAChoice45 = leftAChoice->add("45º");
                leftAChoice->value(0);
                leftAChoice->callback(SimulatorWindow::chooseLeftAngle);
                leftAChoice->deactivate();
                leftAChoice->color(FL_BLUE);

                //Oclusor ojo izquierdo
                checkLeftOc = new Fl_Check_Button(450, 698, 73, 22, "Oclusor:");
                checkLeftOc->callback(SimulatorWindow::controlLeftOc);

                //Material oclusor ojo izquierdo
                leftOChoice = new Fl_Choice(525, 698, 113, 22, "");
                leftOChoice->add("Transparente");
                leftOChoice->add("Opaco");
                leftOChoice->value(0);
                leftOChoice->callback(SimulatorWindow::chooseMatLOc);
                leftOChoice->deactivate();
                leftOChoice->color(FL_BLUE);

                //Elección patología ojo derecho
                rightChoice = new Fl_Choice(137,728,140,22,"Ojo derecho: ");
                rightChoice->add("Sin patología");
                rightChoice->add("Endotropia");
                rightChoice->add("Exotropia");
                rightChoice->add("Hipertropia");
                rightChoice->add("Hipotropia");
                rightChoice->callback(SimulatorWindow::chooseRightEye);
                rightChoice->value(0);
                rightChoice->color(FL_BLUE);

                //Ángulo ojo derecho
                rightAChoice = new Fl_Choice(360,728,70,22,"Ángulo:");
                rightAChoice->add("15º");
                rightAChoice->add("30º");
                rightAChoice45 = rightAChoice->add("45º");
                rightAChoice->value(0);
                rightAChoice->callback(SimulatorWindow::chooseRightAngle);
                rightAChoice->deactivate();
                rightAChoice->color(FL_BLUE);

                //Oclusor ojo drecho
                checkRightOc = new Fl_Check_Button(450, 728, 73, 22, "Oclusor");
                checkRightOc->callback(SimulatorWindow::controlRightOc);

                //Material oclusor ojo derecho
                righOChoice = new Fl_Choice(525, 728, 113, 22, "");
                righOChoice->add("Transparente");
                righOChoice->add("Opaco");
                righOChoice->value(0);
                righOChoice->callback(SimulatorWindow::chooseMatROc);
                righOChoice->deactivate();
                righOChoice->color(FL_BLUE);

                //Modo examen
                //Ojo
                eyeChoice = new Fl_Choice(775,698,140,22,"Diagnóstico:");
                eyeChoice->add("Ojo izquierdo");
                eyeChoice->add("Ojo derecho");
                eyeChoice->value(0);
                eyeChoice->deactivate();
                eyeChoice->callback(SimulatorWindow::chooseEye);
                eyeChoice->color(FL_BLUE);

                //Patología
                patChoice = new Fl_Choice(925, 698, 140, 22,"");
                patChoice->add("Endotropia");
                patChoice->add("Exotropia");
                patChoice->add("Hipertropia");
                patChoice->add("Hipotropia");
                patChoice->value(0);
                patChoice->callback(SimulatorWindow::choosePath);
                patChoice->deactivate();
                patChoice->color(FL_BLUE);

                //Ángulo de rotación
                examAChoice = new Fl_Choice(1077, 698, 140,22,"");
                examAChoice->add("15º");
                examAChoice->add("30º");
                examAChoice->add("45º");
                examAChoice->value(0);
                examAChoice->callback(SimulatorWindow::chooseAngle);
                examAChoice->deactivate();
                examAChoice->color(FL_BLUE);

                //Botón de generar paciente
                patButton = new Fl_Button(685,728,140,22,"Generar paciente");
                patButton->color(FL_BLUE);
                patButton->labelcolor(FL_WHITE);
                patButton->callback(SimulatorWindow::generatePatient);
                patButton->deactivate();

                //Botón de comprobar la respuesta
                checkAnsButton = new Fl_Button(875, 728,140,22,"Comprobar");
                checkAnsButton->color(FL_BLUE);
                checkAnsButton->labelcolor(FL_WHITE);
                checkAnsButton->callback(SimulatorWindow::checkAnswer);
                checkAnsButton->deactivate();

                //Botón de ayuda
                helpButton = new Fl_Button(1229, 698, 52, 52, "");
                helpButton->color(FL_BLUE);
                helpButton->callback(SimulatorWindow::showHelp);
                static Fl_Pixmap G_cat(cat_xpm);
                helpButton->image(G_cat);

                Fl_Window *margin_left = new Fl_Window(0,0,400,h()-120);
                margin_left->color(FL_BLACK);
                margin_left->end();
                Fl_Window *margin_right = new Fl_Window(1366-400, 225, 400, h()-(225+120));
                margin_right->color(FL_BLACK);
                margin_right->end();
            }

            /**
             *@brief Llamamos a la ventana de ayuda de uso de la aplicación
             */
            static int callPopup();

            /**
             *@brief Método ejecutado al cerrar la ventana principal, cierra el resto de ventanas abiertas
             */
            static void closeWindow(Fl_Widget*w);

    };
}
#endif // _SIMULATORWINDOW_H_
