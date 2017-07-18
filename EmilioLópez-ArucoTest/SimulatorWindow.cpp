#include "SimulatorWindow.hpp"
#include "HelpDialog.hpp"

int SimulatorWindow::callPopup(){

    pop = new HelpDialog((Fl::w()/2)-220,(Fl::h()/2)-200,440,230,"Ayuda");
    pop->resizable(pop);
    pop->clear_border();
    pop->color(Fl_Color(7));
    pop->show();

    return(Fl::run());

}

void SimulatorWindow::Choice_Exploracion(Fl_Widget*w) {

        curr_string_iz = NORMAL;
        angle_iz = 0;
        curr_string_de = NORMAL;
        angle_de = 0;

        choice_iz->activate();
        check_oiz->activate();

        choice_ojo->value(0);
        choice_ean->value(0);
        choice_pat->value(0);

        if (curr_string_iz != NORMAL)
        {
            choice_ian->activate();
            check_oiz->activate();
            choice_oiz->activate();
        }

        choice_de->activate();
        check_ode->activate();

        if (curr_string_de != NORMAL)
        {
            choice_dan->activate();
            check_ode->activate();
            choice_ode->activate();
        }

        choice_ojo->deactivate();
        choice_pat->deactivate();
        choice_ean->deactivate();
        b_paciente->deactivate();
        b_comprobar->deactivate();

        modo_examen = 0;

    }
    void SimulatorWindow::Choice_Examen(Fl_Widget*w){

        choice_iz->value(0);
        curr_string_iz = NORMAL;
        choice_ian->value(0);
        angle_iz = 0;
        choice_oiz->value(0);
        check_oiz->value(0);
        oclusor_iz = TRANSPARENTE;

        choice_de->value(0);
        curr_string_de = NORMAL;
        choice_dan->value(0);
        angle_de = 0;
        check_ode->value(0);
        choice_ode->value(0);
        oclusor_de = TRANSPARENTE;

        choice_iz->deactivate();
        choice_ian->deactivate();
        check_oiz->deactivate();
        choice_oiz->deactivate();
        choice_de->deactivate();
        choice_dan->deactivate();
        check_ode->deactivate();
        choice_ode->deactivate();

        choice_ojo->activate();
        choice_pat->activate();
        choice_ean->activate();
        b_paciente->activate();

        modo_examen = 1;

    }
    void SimulatorWindow::Choice_iz(Fl_Widget*w) {

        Fl_Choice *ch = (Fl_Choice*)w;

        if ( strcmp(ch->text(), "Endotropia") == 0){

            if (check_oiz->value() == 1 || (check_oiz->value() == 0 && check_ode->value() == 0)){
                curr_string_iz = ENDOTROPIA;
                angle_iz = 15;
                curr_string_de = NORMAL;
                angle_de = 0;
            }
            else if (check_ode->value() == 1){
                curr_string_de = ENDOTROPIA;
                angle_de = 15;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }

            choice_ian->activate();
            choice_ian45 = choice_ian->add("45º");
            choice_ian->value(0);

        }
        else if (strcmp(ch->text(), "Exotropia") == 0){

            if (check_oiz->value() == 1 || (check_oiz->value() == 0 && check_ode->value() == 0)){
                curr_string_iz = EXOTROPIA;
                angle_iz = 15;
                curr_string_de = NORMAL;
                angle_de = 0;
            }
            else if (check_ode->value() == 1){
                curr_string_de = EXOTROPIA;
                angle_de = 15;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }

            choice_ian->activate();
            choice_ian45 = choice_ian->add("45º");
            choice_ian->value(0);
        }
        else if (strcmp(ch->text(), "Hipertropia") == 0){

            if (check_oiz->value() == 1 || (check_oiz->value() == 0 && check_ode->value() == 0)){
                curr_string_iz = HIPERTROPIA;
                angle_iz = 15;
                curr_string_de = NORMAL;
                angle_de = 0;
            }
            else if (check_ode->value() == 1){
                curr_string_de = HIPERTROPIA;
                angle_de = 15;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }

            choice_ian->activate();
            choice_ian->remove(choice_ian45);
            choice_ian->value(0);
        }
        else if (strcmp(ch->text(), "Hipotropia") == 0){

            if (check_oiz->value() == 1 || (check_oiz->value() == 0 && check_ode->value() == 0)){
                curr_string_iz = HIPOTROPIA;
                angle_iz = 15;
                curr_string_de = NORMAL;
                angle_de = 0;
            }
            else if (check_ode->value() == 1){
                curr_string_de = HIPOTROPIA;
                angle_de = 15;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }

            choice_ian->activate();
            choice_ian->remove(choice_ian45);
            choice_ian->value(0);
        }
        else{

            if (check_oiz->value() == 1 || (check_oiz->value() == 0 && check_ode->value() == 0)){
                curr_string_iz = NORMAL;
                angle_iz = 0;
                curr_string_de = NORMAL;
                angle_de = 0;
            }
            else if (check_ode->value() == 1){
                curr_string_de = NORMAL;
                angle_de = 0;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }

            choice_ian->deactivate();

        }

        choice_de->value(0);
        choice_dan->value(0);
        choice_dan->deactivate();
  }

  void SimulatorWindow::Choice_de(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;

        if ( strcmp(ch->text(), "Endotropia") == 0){

            if (check_ode->value() == 1 || (check_oiz->value() == 0 && check_ode->value() == 0)){
                curr_string_de = ENDOTROPIA;
                angle_de = 15;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }
            else if (check_oiz->value() == 1){
                curr_string_de = NORMAL;
                angle_de = 0;
                curr_string_iz = ENDOTROPIA;
                angle_iz = 15;
            }
            choice_dan->activate();
            choice_dan45 = choice_dan->add("45º");
            choice_dan->value(0);
        }
        else if (strcmp(ch->text(), "Exotropia") == 0){

            if (check_ode->value() == 1 || (check_oiz->value() == 0 && check_ode->value() == 0)){
                curr_string_de = EXOTROPIA;
                angle_de = 15;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }
            else if (check_oiz->value() == 1){
                curr_string_de = NORMAL;
                angle_de = 0;
                curr_string_iz = EXOTROPIA;
                angle_iz = 15;
            }
            choice_dan->activate();
            choice_dan45 = choice_dan->add("45º");
            choice_dan->value(0);
        }
        else if (strcmp(ch->text(), "Hipertropia") == 0){

            if (check_ode->value() == 1 || (check_oiz->value() == 0 && check_ode->value() == 0)){
                curr_string_de = HIPERTROPIA;
                angle_de = 15;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }
            else if (check_oiz->value() == 1){
                curr_string_de = NORMAL;
                angle_de = 0;
                curr_string_iz = HIPERTROPIA;
                angle_iz = 15;
            }
            choice_dan->activate();
            choice_dan->remove(choice_dan45);
            choice_dan->value(0);
        }
        else if (strcmp(ch->text(), "Hipotropia") == 0){

            if (check_ode->value() == 1 || (check_oiz->value() == 0 && check_ode->value() == 0)){
                curr_string_de = HIPOTROPIA;
                angle_de = 15;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }
            else if (check_oiz->value() == 1){
                curr_string_de = NORMAL;
                angle_de = 0;
                curr_string_iz = HIPOTROPIA;
                angle_iz = 15;
            }
            choice_dan->activate();
            choice_dan->remove(choice_dan45);
            choice_dan->value(0);
        }
        else{

            if (check_ode->value() == 1 || (check_oiz->value() == 0 && check_ode->value() == 0)){
                curr_string_de = NORMAL;
                angle_de = 0;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }
            else if (check_oiz->value() == 1){
                curr_string_de = NORMAL;
                angle_de = 0;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }
            choice_dan->deactivate();
            angle_de = 5;
        }

        choice_iz->value(0);
        choice_ian->value(0);
        choice_ian->deactivate();
  }

  void SimulatorWindow::Angle_iz(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;
    if(strcmp(ch->text(), "15º") == 0){
        angle_iz = 15;
    }
    else if (strcmp(ch->text(), "30º") == 0){
        angle_iz = 30;
    }
    else if (strcmp(ch->text(), "45º") == 0){
        angle_iz = 45;
    }
  }

  void SimulatorWindow::Angle_de(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;
    if(strcmp(ch->text(), "15º") == 0){
        angle_de = 15;
    }
    else if (strcmp(ch->text(), "30º") == 0){
        angle_de = 30;
    }
    else if (strcmp(ch->text(), "45º") == 0){
        angle_de = 45;
    }
  }

  void SimulatorWindow::Gen_paciente(Fl_Widget*w){

    b_comprobar->activate();

    eye = rand() % 2;
    patologia = rand() % 4 + 1;

    if ( eye == 0 ){

        curr_string_iz = patologia;
        curr_string_de = NORMAL;

        if ( patologia == ENDOTROPIA or patologia == EXOTROPIA ){
            angle = rand() % 1;

            if ( angle == 0){
                angle_iz = 15;
            }
            else if ( angle == 1){
                angle_iz = 30;
            }
        }
    }
    else if ( eye == 1 ){

        curr_string_de = patologia;
        curr_string_iz = NORMAL;

        if (patologia == HIPERTROPIA or patologia == HIPOTROPIA){
            angle = rand() % 2;

            if (angle == 0){
                angle_de = 15;
            }
            else if (angle == 1){
                angle_de = 30;
            }
            else if (angle == 2){
                angle_de = 45;
            }
        }
  }
}

  void SimulatorWindow::Choice_eye(Fl_Widget *w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if(strcmp(ch->text(), "Ojo izquierdo") == 0)
        eye_test = 0;

    else if (strcmp(ch->text(), "Ojo derecho") == 0)
        eye_test = 1;

  }

  void SimulatorWindow::Choice_pat(Fl_Widget *w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if (strcmp(ch->text(), "Endotropia") == 0)
        pat_test = ENDOTROPIA;

    else if(strcmp(ch->text(), "Exotropia") == 0)
        pat_test = EXOTROPIA;

    else if(strcmp(ch->text(), "Hipertropia") == 0)
        pat_test = HIPERTROPIA;

    else if(strcmp(ch->text(), "Hipotropia") == 0)
        pat_test = HIPOTROPIA;
  }

  void SimulatorWindow::Choice_angle(Fl_Widget *w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if (strcmp(ch->text(), "15º") == 0)
        angle_test = 0;

    else if (strcmp(ch->text(), "30º") == 0)
        angle_test = 1;

    else if (strcmp(ch->text(), "45º") == 0)
        angle_test = 2;
  }

    void SimulatorWindow::Check_answer(Fl_Widget*w){

        if (eye_test == eye and pat_test == patologia and angle_test == angle){

            fl_message("Correcto","Aceptar",0,0);

        }

        else{

            HANDLE hcon;
            hcon = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD dwPos;
            dwPos.X = 0;
            dwPos.Y= 0;
            SetConsoleCursorPosition(hcon,dwPos);
            fl_message("Incorrecto. Inténtelo de nuevo","Aceptar",0,0);
        }

    }

    void SimulatorWindow::Check_oclusor_izquierdo(Fl_Widget*w){


        if(check_oiz->value() == 1){
            choice_oiz->activate();
            check_ode->value(0);
            choice_ode->deactivate();
            choice_ode->value(0);
            oclusor_de = TRANSPARENTE;

            if(curr_string_de != NORMAL){
                curr_string_iz = curr_string_de;
                angle_iz = angle_de;
                curr_string_de = NORMAL;
                angle_de = 0;
            }
        }
        else{
            choice_oiz->deactivate();

            if (choice_iz->value() == 0 && choice_de->value() != 0){
                curr_string_de = curr_string_iz;
                angle_de = angle_iz;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }
        }
    }

    void SimulatorWindow::Check_oclusor_derecho(Fl_Widget*w){

        if(check_ode->value() == 1){
            choice_ode->activate();
            check_oiz->value(0);
            choice_oiz->deactivate();
            choice_oiz->value(0);
            oclusor_iz = TRANSPARENTE;

            if(curr_string_iz != NORMAL){
                curr_string_de = curr_string_iz;
                angle_de = angle_iz;
                curr_string_iz = NORMAL;
                angle_iz = 0;
            }
        }
        else{
            choice_ode->deactivate();
            if (choice_de->value() == 0 && choice_iz->value() != 0){
                curr_string_iz = curr_string_de;
                angle_iz = angle_de;
                curr_string_de = NORMAL;
                angle_de = 0;
            }
        }
    }

    void SimulatorWindow::Oclusor_iz(Fl_Widget*w){

        Fl_Choice *ch = (Fl_Choice*)w;

    if (strcmp(ch->text(), "Transparente") == 0)
        oclusor_iz = TRANSPARENTE;

    else if(strcmp(ch->text(), "Opaco") == 0)
        oclusor_iz = OPACO;

        if(curr_string_de != NORMAL){
            curr_string_iz = curr_string_de;
            angle_iz = angle_de;
            curr_string_de = NORMAL;
            angle_de = 0;
        }

    }

    void SimulatorWindow::Oclusor_de(Fl_Widget*w){

        Fl_Choice *ch = (Fl_Choice*)w;

    if (strcmp(ch->text(), "Transparente") == 0)
        oclusor_de = TRANSPARENTE;

    else if(strcmp(ch->text(), "Opaco") == 0)
        oclusor_de = OPACO;


    if(curr_string_iz != NORMAL){
            curr_string_de = curr_string_iz;
            angle_de = angle_iz;
            curr_string_iz = NORMAL;
            angle_iz = 0;
        }

    }

   void SimulatorWindow::Show_help(Fl_Widget*w){

        popupActive = true;
        callPopup();

    }
