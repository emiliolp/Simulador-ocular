/**
 *Implementaci�n de los m�todos de la clase SimulatorWindow
 */
#include "SimulatorWindow.hpp"

//Llamamos a la ventana de ayuda de uso de la aplicaci�n
int SimulatorWindow::callPopup(){

    pop = new HelpDialog((Fl::w()/2)-207,(Fl::h()/2)-200,440,400,"Ayuda");
    pop->resizable(pop);
    pop->clear_border();
    pop->color(Fl_Color(7));
    pop->show();

    return(Fl::run());

}

//M�todo lanzado al seleccionar el modo exploraci�n
void SimulatorWindow::chooseExploration(Fl_Widget*w) {

    //Inicializamos los valores de patolog�a y �ngulo de rotaci�n
    leftEyePath = NORMAL;
    leftAngle = 0;
    rightEyePath = NORMAL;
    rightAngle = 0;

    leftChoice->activate();
    checkLeftOc->activate();

    eyeChoice->value(0);
    examAChoice->value(0);
    patChoice->value(0);

    if (leftEyePath != NORMAL)
    {
        leftAChoice->activate();
        checkLeftOc->activate();
        leftOChoice->activate();
    }

    rightChoice->activate();
    checkRightOc->activate();

    if (rightEyePath != NORMAL)
    {
        rightAChoice->activate();
        checkRightOc->activate();
        righOChoice->activate();
    }

    //Deshabilitamos los controles del modo examen
    eyeChoice->deactivate();
    patChoice->deactivate();
    examAChoice->deactivate();
    patButton->deactivate();
    checkAnsButton->deactivate();

    examMode = 0;

}

//M�todo lanzado al seleccionar el modo examen
void SimulatorWindow::chooseExam(Fl_Widget*w){

    //Inicializamos los valores de los controles del modo exploraci�n
    leftChoice->value(0);
    leftEyePath = NORMAL;
    leftAChoice->value(0);
    leftAngle = 0;
    leftOChoice->value(0);
    checkLeftOc->value(0);
    leftOccluder = TRANSPARENTE;

    rightChoice->value(0);
    rightEyePath = NORMAL;
    rightAChoice->value(0);
    rightAngle = 0;
    checkRightOc->value(0);
    righOChoice->value(0);
    rightOccluder = TRANSPARENTE;

    leftChoice->deactivate();
    leftAChoice->deactivate();
    checkLeftOc->deactivate();
    leftOChoice->deactivate();
    rightChoice->deactivate();
    rightAChoice->deactivate();
    checkRightOc->deactivate();
    righOChoice->deactivate();

    //Activamos los controles del modo examen
    eyeChoice->activate();
    patChoice->activate();
    examAChoice->activate();
    patButton->activate();

    examMode = 1;

}

//M�todo ejecutado al seleccionar la patolog�a del ojo izquierdo
void SimulatorWindow::chooseLeftEye(Fl_Widget*w) {

    Fl_Choice *ch = (Fl_Choice*)w;

    if ( strcmp(ch->text(), "Endotropia") == 0){

        //Por defecto seleccionamos 15� de desviaci�n
        if (checkLeftOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            leftEyePath = ENDOTROPIA;
            leftAngle = 15;
            rightEyePath = NORMAL;
            rightAngle = 0;
        }
        //Si se ha marcado el oclusor en el ojo derecho, la patolog�a pasa a este
        else if (checkRightOc->value() == 1){
            rightEyePath = ENDOTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }

        leftAChoice->activate();
        leftAChoice45 = leftAChoice->add("45�");    //45� disponible en endotropia
        leftAChoice->value(0);

    }
    else if (strcmp(ch->text(), "Exotropia") == 0){

        if (checkLeftOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            leftEyePath = EXOTROPIA;
            leftAngle = 15;
            rightEyePath = NORMAL;
            rightAngle = 0;
        }
        else if (checkRightOc->value() == 1){
            rightEyePath = EXOTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }

        leftAChoice->activate();
        leftAChoice45 = leftAChoice->add("45�");
        leftAChoice->value(0);
    }
    else if (strcmp(ch->text(), "Hipertropia") == 0){

        if (checkLeftOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            leftEyePath = HIPERTROPIA;
            leftAngle = 15;
            rightEyePath = NORMAL;
            rightAngle = 0;
        }
        else if (checkRightOc->value() == 1){
            rightEyePath = HIPERTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }

        leftAChoice->activate();
        leftAChoice->remove(leftAChoice45); //Para hipertropia no se dispone de rotaci�n 45�
        leftAChoice->value(0);
    }
    else if (strcmp(ch->text(), "Hipotropia") == 0){

        if (checkLeftOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            leftEyePath = HIPOTROPIA;
            leftAngle = 15;
            rightEyePath = NORMAL;
            rightAngle = 0;
        }
        else if (checkRightOc->value() == 1){
            rightEyePath = HIPOTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }

        leftAChoice->activate();
        leftAChoice->remove(leftAChoice45); //Para hipotropia no se dispone de rotaci�n 45�
        leftAChoice->value(0);
    }
    else{
        //Se ha marcado la opci�n sin patolog�a
        if (checkLeftOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            leftEyePath = NORMAL;
            leftAngle = 0;
            rightEyePath = NORMAL;
            rightAngle = 0;
        }
        //El oclusor se encuentra en el ojo derecho
        else if (checkRightOc->value() == 1){
            rightEyePath = NORMAL;
            rightAngle = 0;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }

        leftAChoice->deactivate();

    }

    rightChoice->value(0);
    rightAChoice->value(0);
    rightAChoice->deactivate();
}

//M�todo ejecutado al seleccionar patolog�a en el ojo derecho
void SimulatorWindow::chooseRightEye(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if ( strcmp(ch->text(), "Endotropia") == 0){

        //Inicialmente se marcan 15� como grado de desviaci�n del ojo derecho
        if (checkRightOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            rightEyePath = ENDOTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
        else if (checkLeftOc->value() == 1){
            rightEyePath = NORMAL;
            rightAngle = 0;
            leftEyePath = ENDOTROPIA;
            leftAngle = 15;
        }
        rightAChoice->activate();
        rightAChoice45 = rightAChoice->add("45�");
        rightAChoice->value(0);
    }
    else if (strcmp(ch->text(), "Exotropia") == 0){

        //Inicialmente se marcan 15� como grado de desviaci�n del ojo derecho
        if (checkRightOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            rightEyePath = EXOTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
        //Si el oclusor del ojo izquierdo est� marcado cambiamos la patolog�a de ojo
        else if (checkLeftOc->value() == 1){
            rightEyePath = NORMAL;
            rightAngle = 0;
            leftEyePath = EXOTROPIA;
            leftAngle = 15;
        }
        rightAChoice->activate();
        rightAChoice45 = rightAChoice->add("45�");  //45� disponible en exotropia
        rightAChoice->value(0);
    }
    else if (strcmp(ch->text(), "Hipertropia") == 0){

        //Marcamos 15� como rotaci�n inicial
        if (checkRightOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            rightEyePath = HIPERTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
        //Si el oclusor del ojo izquierdo est� marcado cambiamos la patolog�a de ojo
        else if (checkLeftOc->value() == 1){
            rightEyePath = NORMAL;
            rightAngle = 0;
            leftEyePath = HIPERTROPIA;
            leftAngle = 15;
        }
        rightAChoice->activate();
        rightAChoice->remove(rightAChoice45);   //45� no disponibles en hipertropia
        rightAChoice->value(0);
    }
    else if (strcmp(ch->text(), "Hipotropia") == 0){

        //Inicialmente se marcan 15� como rotaci�n del ojo derecho
        if (checkRightOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            rightEyePath = HIPOTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
        //Si el oclusor est� marcado cambiamos la patolog�a del ojo
        else if (checkLeftOc->value() == 1){
            rightEyePath = NORMAL;
            rightAngle = 0;
            leftEyePath = HIPOTROPIA;
            leftAngle = 15;
        }
        rightAChoice->activate();
        rightAChoice->remove(rightAChoice45);
        rightAChoice->value(0);
    }
    else{
        //Si no hay patolog�as marcadas la rotaci�n de los ojos pasa a ser normal
        if (checkRightOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            rightEyePath = NORMAL;
            rightAngle = 0;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
        else if (checkLeftOc->value() == 1){
            rightEyePath = NORMAL;
            rightAngle = 0;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
        rightAChoice->deactivate();
        rightAngle = 5;
    }

    leftChoice->value(0);
    leftAChoice->value(0);
    leftAChoice->deactivate();
}

//M�todo para elegir el �ngulo de rotaci�n del ojo izquierdo
void SimulatorWindow::chooseLeftAngle(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;
    if(strcmp(ch->text(), "15�") == 0){
        leftAngle = 15;
    }
    else if (strcmp(ch->text(), "30�") == 0){
        leftAngle = 30;
    }
    else if (strcmp(ch->text(), "45�") == 0){
        leftAngle = 45;
    }
  }

  //M�todo para elegir el �ngulo de rotaci�n del ojo derecho
void SimulatorWindow::chooseRightAngle(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;
    if(strcmp(ch->text(), "15�") == 0){
        rightAngle = 15;
    }
    else if (strcmp(ch->text(), "30�") == 0){
        rightAngle = 30;
    }
    else if (strcmp(ch->text(), "45�") == 0){
        rightAngle = 45;
    }
  }

//M�todo para generar paciente
void SimulatorWindow::generatePatient(Fl_Widget*w){

    checkAnsButton->activate();

    //Selecionamos un ojo al azar
    eye = rand() % 2;
    //Elegimos una patolog�a al azar
    path = (rand() % 4) + 1;

    //Si el elegido es el ojo izquierdo
    if ( eye == 0 ){

        leftEyePath = path;
        rightEyePath = NORMAL;
        rightAngle = 0;

        //Asignamos la patolog�a y el �ngulo de rotaci�n
        if ( path == ENDOTROPIA or path == EXOTROPIA ){

            angle = rand() % 3;

            if ( angle == 0){
                leftAngle = 15;
            }
            else if ( angle == 1){
                leftAngle = 30;
            }
            else if ( angle == 2){
                leftAngle = 45;
            }
        }
        else if( path == HIPERTROPIA or path == HIPOTROPIA ){

            angle = rand() % 2;

            if ( angle == 0){
                leftAngle = 15;
            }
            else if ( angle == 1){
                leftAngle = 30;
            }

        }
    }

    //Si se selecciona el ojo derecho
    else if ( eye == 1 ){

        rightEyePath = path;
        leftEyePath = NORMAL;
        leftAngle = 0;

        //Asignamos la patolog�a y el �ngulo de rotaci�n
        if ( path == ENDOTROPIA or path == EXOTROPIA ){

            angle = rand() % 3;

            if ( angle == 0){
                rightAngle = 15;
            }
            else if ( angle == 1){
                rightAngle = 30;
            }
            else if ( angle == 2){
                rightAngle = 45;
            }
        }

        else if (path == HIPERTROPIA or path == HIPOTROPIA){

            angle = rand() % 2;

            if (angle == 0){
                rightAngle = 15;
            }
            else if (angle == 1){
                rightAngle = 30;
            }
        }
  }
}

//M�todo ejecutado al seleccionar el ojo en el modo examen
void SimulatorWindow::chooseEye(Fl_Widget *w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if(strcmp(ch->text(), "Ojo izquierdo") == 0)
        eyeTest = 0;

    else if (strcmp(ch->text(), "Ojo derecho") == 0)
        eyeTest = 1;

  }

//M�todo para elegir patolog�a en el modo examen
void SimulatorWindow::choosePath(Fl_Widget *w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if (strcmp(ch->text(), "Endotropia") == 0)
        patTest = ENDOTROPIA;

    else if(strcmp(ch->text(), "Exotropia") == 0)
        patTest = EXOTROPIA;

    else if(strcmp(ch->text(), "Hipertropia") == 0)
        patTest = HIPERTROPIA;

    else if(strcmp(ch->text(), "Hipotropia") == 0)
        patTest = HIPOTROPIA;
  }

//M�todo ejecutado al seleccionar el �ngulo de rotaci�n en el modo examen
void SimulatorWindow::chooseAngle(Fl_Widget *w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if (strcmp(ch->text(), "15�") == 0)
        angleTest = 0;

    else if (strcmp(ch->text(), "30�") == 0)
        angleTest = 1;

    else if (strcmp(ch->text(), "45�") == 0)
        angleTest = 2;
  }

//Comprobamos la respuesta del usuario
void SimulatorWindow::checkAnswer(Fl_Widget*w){

    //El ojo seleccionado, patolog�a y �ngulo de rotaci�n son los correctos
    if (eyeTest == eye and patTest == path and angleTest == angle){

        fl_message("Correcto","Aceptar",0,0);

    }

    //En caso contraio mostramos mensaje de error
    else{

        fl_message("Incorrecto. Int�ntelo de nuevo","Aceptar",0,0);
    }

}

//M�todo lanzado al seleccionar el oclusor del ojo izquierdo
void SimulatorWindow::controlLeftOc(Fl_Widget*w){

    //Se activa el oclusor
    if(checkLeftOc->value() == 1){

        //Marcamos por defecto el oclusor transparente
        leftOChoice->activate();
        checkRightOc->value(0);
        righOChoice->deactivate();
        righOChoice->value(0);
        rightOccluder = TRANSPARENTE;

        //Si existe enfermedad en el ojo contrario invertimos la patolog�a
        if(rightEyePath != NORMAL){
            leftEyePath = rightEyePath;
            leftAngle = rightAngle;
            rightEyePath = NORMAL;
            rightAngle = 0;
        }
    }
    //Desactivamos el oclusor
    else{

        leftOChoice->deactivate();

        //La patolog�a vuelve al ojo inicial
        if (leftChoice->value() == 0 && rightChoice->value() != 0){
            rightEyePath = leftEyePath;
            rightAngle = leftAngle;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
    }
}

//M�todo lanzado al seleccionar el oclusor del ojo derecho
void SimulatorWindow::controlRightOc(Fl_Widget*w){

    //Se activa el oclusor
    if(checkRightOc->value() == 1){

        //Marcamos por defecto el oclusor transparente
        righOChoice->activate();
        checkLeftOc->value(0);
        leftOChoice->deactivate();
        leftOChoice->value(0);
        leftOccluder = TRANSPARENTE;

        //Si existe enfermedad en el ojo contrario invertimos la patolog�a
        if(leftEyePath != NORMAL){

            rightEyePath = leftEyePath;
            rightAngle = leftAngle;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
    }

    //Desactivamos el oclusor
    else{

        righOChoice->deactivate();

        //La patolog�a vuelve al ojo inicial
        if (rightChoice->value() == 0 && leftChoice->value() != 0){
            leftEyePath = rightEyePath;
            leftAngle = rightAngle;
            rightEyePath = NORMAL;
            rightAngle = 0;
        }
    }
}

//M�todo para seleccionar el material del oclusor izquierdo
void SimulatorWindow::chooseMatLOc(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if (strcmp(ch->text(), "Transparente") == 0)
        leftOccluder = TRANSPARENTE;

    else if(strcmp(ch->text(), "Opaco") == 0)
        leftOccluder = OPACO;

    if(rightEyePath != NORMAL){

        leftEyePath = rightEyePath;
        leftAngle = rightAngle;
        rightEyePath = NORMAL;
        rightAngle = 0;
    }

}

//M�todo para seleccionar el material del oclusor derecho
void SimulatorWindow::chooseMatROc(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if (strcmp(ch->text(), "Transparente") == 0)
        rightOccluder = TRANSPARENTE;

    else if(strcmp(ch->text(), "Opaco") == 0)
        rightOccluder = OPACO;


    if(leftEyePath != NORMAL){

            rightEyePath = leftEyePath;
            rightAngle = leftAngle;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }

    }

//M�todo que lanza la gu�a de uso
void SimulatorWindow::showHelp(Fl_Widget*w){

    //Si la ventana de ayuda ya se encontraba abierta la cerramos
    if (popupActive == true)
    {
        pop->hide();
    }

    popupActive = true;
    callPopup();

}

//Cierra todas las ventanas de la aplicaci�n si est�n activas
void SimulatorWindow::closeWindow(Fl_Widget*w){

    ((Fl_Window*)w)->hide();

    if (flagCalibration)
        winCalib->hide();

    if (popupActive){
        pop->hide();
    }

}
