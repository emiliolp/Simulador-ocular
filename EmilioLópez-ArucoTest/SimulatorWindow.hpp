#ifndef _SIMULATORWINDOW_H_
#define _SIMULATORWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Round_Button.H>
#include <Fl/Fl_Check_Button.H>
#include "help_button.hpp"
#include "FaceWindow.hpp"
#include "CamWindow.hpp"
#include "wtypes.h"
#include <FL/Fl_Pixmap.H>

extern Fl_Choice *choice_iz, *choice_de, *choice_ian, *choice_dan, *choice_ojo, *choice_pat, *choice_ean, *choice_oiz, *choice_ode;
extern Fl_Button *b_paciente, *b_comprobar, *b_help, *b_close, *b_marker, *b_calib, *b_start_calib, *b_finish_calib;
extern Fl_Check_Button *check_oiz, *check_ode;
extern int choice_dan45, choice_ian45, eye, patologia, angle, eye_test, pat_test, angle_test;
extern Fl_Window *pop;

class SimulatorWindow : public Fl_Window {

    private:
        FaceWindow *figureGl;                    // opengl window
        CamWindow  *viewer;
        Fl_Group   *grUI;

    public:
        SimulatorWindow(int W,int H,const char*L=0) : Fl_Window(W,H,L) {

        figureGl = new FaceWindow(300, 0, w()-600, h()-120);
        ///mygl_cam = new MyGlWindowCam(1366-300,0, 300, 187.5);
        viewer   = new CamWindow(1366-300,0, 300, 225);

        grUI = new Fl_Group(10,672,800,25);
        {
            Fl_Round_Button* grUI = new Fl_Round_Button(32, 672, 113, 25, "Exploración");
            grUI->type(102);
            grUI->down_box(FL_ROUND_DOWN_BOX);
            grUI->setonly();
            grUI->callback(SimulatorWindow::Choice_Exploracion);

        }

        {
            Fl_Round_Button* grUI = new Fl_Round_Button(680, 672, 113, 25, "Examen");
            grUI->type(102);
            grUI->down_box(FL_ROUND_DOWN_BOX);
            grUI->callback(SimulatorWindow::Choice_Examen);
        }
        grUI->end();

        choice_iz = new Fl_Choice(137,698,140,22,"Ojo izquierdo:");
        choice_iz->add("Sin patología");
        choice_iz->add("Endotropia");
        choice_iz->add("Exotropia");
        choice_iz->add("Hipertropia");
        choice_iz->add("Hipotropia");
        choice_iz->callback(SimulatorWindow::Choice_iz);
        choice_iz->value(0);
        choice_iz->color(FL_BLUE);

        choice_ian = new Fl_Choice(360,698,70,22,"Ángulo:");
        choice_ian->add("15º");
        choice_ian->add("30º");
        choice_ian45 = choice_ian->add("45º");
        choice_ian->value(0);
        choice_ian->callback(SimulatorWindow::Angle_iz);
        choice_ian->deactivate();
        choice_ian->color(FL_BLUE);

        check_oiz = new Fl_Check_Button(450, 698, 73, 22, "Oclusor:");
        check_oiz->callback(SimulatorWindow::Check_oclusor_izquierdo);
        choice_oiz = new Fl_Choice(525, 698, 113, 22, "");
        choice_oiz->add("Transparente");
        choice_oiz->add("Opaco");
        choice_oiz->value(0);
        choice_oiz->callback(SimulatorWindow::Oclusor_iz);
        choice_oiz->deactivate();
        choice_oiz->color(FL_BLUE);

        choice_de = new Fl_Choice(137,728,140,22,"Ojo derecho: ");
        choice_de->add("Sin patología");
        choice_de->add("Endotropia");
        choice_de->add("Exotropia");
        choice_de->add("Hipertropia");
        choice_de->add("Hipotropia");
        choice_de->callback(SimulatorWindow::Choice_de);
        choice_de->value(0);
        choice_de->color(FL_BLUE);

        choice_dan = new Fl_Choice(360,728,70,22,"Ángulo:");
        choice_dan->add("15º");
        choice_dan->add("30º");
        choice_dan45 = choice_dan->add("45º");
        choice_dan->value(0);
        choice_dan->callback(SimulatorWindow::Angle_de);
        choice_dan->deactivate();
        choice_dan->color(FL_BLUE);

        check_ode = new Fl_Check_Button(450, 728, 73, 22, "Oclusor");
        check_ode->callback(SimulatorWindow::Check_oclusor_derecho);

        choice_ode = new Fl_Choice(525, 728, 113, 22, "");
        choice_ode->add("Transparente");
        choice_ode->add("Opaco");
        choice_ode->value(0);
        choice_ode->callback(SimulatorWindow::Oclusor_de);
        choice_ode->deactivate();
        choice_ode->color(FL_BLUE);

        //Modo examen
        choice_ojo = new Fl_Choice(775,698,140,22,"Diagnóstico:");
        choice_ojo->add("Ojo izquierdo");
        choice_ojo->add("Ojo derecho");
        choice_ojo->value(0);
        choice_ojo->deactivate();
        choice_ojo->callback(SimulatorWindow::Choice_eye);
        choice_ojo->color(FL_BLUE);

        choice_pat = new Fl_Choice(925, 698, 140, 22,"");
        choice_pat->add("Endotropia");
        choice_pat->add("Exotropia");
        choice_pat->add("Hipertropia");
        choice_pat->add("Hipotropia");
        choice_pat->value(0);
        choice_pat->callback(SimulatorWindow::Choice_pat);
        choice_pat->deactivate();
        choice_pat->color(FL_BLUE);

        choice_ean = new Fl_Choice(1077, 698, 140,22,"");
        choice_ean->add("15º");
        choice_ean->add("30º");
        choice_ean->add("45º");
        choice_ean->value(0);
        choice_ean->callback(SimulatorWindow::Choice_angle);
        choice_ean->deactivate();
        choice_ean->color(FL_BLUE);

        b_paciente = new Fl_Button(685,728,140,22,"Generar paciente");
        b_paciente->color(FL_BLUE);
        b_paciente->labelcolor(FL_WHITE);
        b_paciente->callback(SimulatorWindow::Gen_paciente);
        b_paciente->deactivate();

        b_comprobar = new Fl_Button(875, 728,140,22,"Comprobar");
        b_comprobar->color(FL_BLUE);
        b_comprobar->labelcolor(FL_WHITE);
        b_comprobar->callback(SimulatorWindow::Check_answer);
        b_comprobar->deactivate();

        b_help = new Fl_Button(1229, 698, 52, 52, "");
        b_help->color(FL_BLUE);
        b_help->callback(SimulatorWindow::Show_help);
        static Fl_Pixmap G_cat(cat_xpm);
        b_help->image(G_cat);

        Fl_Window *margin_left = new Fl_Window(0,0,400,h()-120);
        margin_left->color(FL_BLACK);
        margin_left->end();
        Fl_Window *margin_right = new Fl_Window(1366-400, 225, 400, h()-(225+120));
        margin_right->color(FL_BLACK);
        margin_right->end();
    }

    public: static int callPopup();
    private:
        static void Choice_Exploracion(Fl_Widget*w);
        static void Choice_Examen(Fl_Widget*w);
        static void Choice_iz(Fl_Widget*w);
        static void Choice_de(Fl_Widget*w);
        static void Angle_iz(Fl_Widget*w);
        static void Angle_de(Fl_Widget*w);
        static void Gen_paciente(Fl_Widget*w);
        static void Choice_eye(Fl_Widget *w);
        static void Choice_pat(Fl_Widget *w);
        static void Choice_angle(Fl_Widget *w);
        static void Check_answer(Fl_Widget*w);
        static void Check_oclusor_izquierdo(Fl_Widget*w);
        static void Check_oclusor_derecho(Fl_Widget*w);
        static void Oclusor_iz(Fl_Widget*w);
        static void Oclusor_de(Fl_Widget*w);
        static void Show_help(Fl_Widget*w);

};

#endif // _SIMULATORWINDOW_H_
