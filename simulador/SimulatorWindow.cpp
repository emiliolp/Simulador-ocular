/**
 *Implementación de los métodos de la clase SimulatorWindow
 */
#include "SimulatorWindow.hpp"

//Llamamos a la ventana de ayuda de uso de la aplicación
int SimulatorWindow::callPopup(){

    pop = new HelpDialog((Fl::w()/2)-207,(Fl::h()/2)-200,440,400,"Ayuda");
    pop->resizable(pop);
    pop->clear_border();
    pop->color(Fl_Color(7));
    pop->show();

    return(Fl::run());

}

//Método lanzado al seleccionar el modo exploración
void SimulatorWindow::chooseExploration(Fl_Widget*w) {

    //Inicializamos los valores de patología y ángulo de rotación
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

//Método lanzado al seleccionar el modo examen
void SimulatorWindow::chooseExam(Fl_Widget*w){

    //Inicializamos los valores de los controles del modo exploración
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

//Método ejecutado al seleccionar la patología del ojo izquierdo
void SimulatorWindow::chooseLeftEye(Fl_Widget*w) {

    Fl_Choice *ch = (Fl_Choice*)w;

    if ( strcmp(ch->text(), "Endotropia") == 0){

        //Por defecto seleccionamos 15º de desviación
        if (checkLeftOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            leftEyePath = ENDOTROPIA;
            leftAngle = 15;
            rightEyePath = NORMAL;
            rightAngle = 0;
        }
        //Si se ha marcado el oclusor en el ojo derecho, la patología pasa a este
        else if (checkRightOc->value() == 1){
            rightEyePath = ENDOTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }

        leftAChoice->activate();
        leftAChoice45 = leftAChoice->add("45º");    //45º disponible en endotropia
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
        leftAChoice45 = leftAChoice->add("45º");
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
        leftAChoice->remove(leftAChoice45); //Para hipertropia no se dispone de rotación 45º
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
        leftAChoice->remove(leftAChoice45); //Para hipotropia no se dispone de rotación 45º
        leftAChoice->value(0);
    }
    else{
        //Se ha marcado la opción sin patología
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

//Método ejecutado al seleccionar patología en el ojo derecho
void SimulatorWindow::chooseRightEye(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if ( strcmp(ch->text(), "Endotropia") == 0){

        //Inicialmente se marcan 15º como grado de desviación del ojo derecho
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
        rightAChoice45 = rightAChoice->add("45º");
        rightAChoice->value(0);
    }
    else if (strcmp(ch->text(), "Exotropia") == 0){

        //Inicialmente se marcan 15º como grado de desviación del ojo derecho
        if (checkRightOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            rightEyePath = EXOTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
        //Si el oclusor del ojo izquierdo está marcado cambiamos la patología de ojo
        else if (checkLeftOc->value() == 1){
            rightEyePath = NORMAL;
            rightAngle = 0;
            leftEyePath = EXOTROPIA;
            leftAngle = 15;
        }
        rightAChoice->activate();
        rightAChoice45 = rightAChoice->add("45º");  //45º disponible en exotropia
        rightAChoice->value(0);
    }
    else if (strcmp(ch->text(), "Hipertropia") == 0){

        //Marcamos 15º como rotación inicial
        if (checkRightOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            rightEyePath = HIPERTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
        //Si el oclusor del ojo izquierdo está marcado cambiamos la patología de ojo
        else if (checkLeftOc->value() == 1){
            rightEyePath = NORMAL;
            rightAngle = 0;
            leftEyePath = HIPERTROPIA;
            leftAngle = 15;
        }
        rightAChoice->activate();
        rightAChoice->remove(rightAChoice45);   //45º no disponibles en hipertropia
        rightAChoice->value(0);
    }
    else if (strcmp(ch->text(), "Hipotropia") == 0){

        //Inicialmente se marcan 15º como rotación del ojo derecho
        if (checkRightOc->value() == 1 || (checkLeftOc->value() == 0 && checkRightOc->value() == 0)){
            rightEyePath = HIPOTROPIA;
            rightAngle = 15;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
        //Si el oclusor está marcado cambiamos la patología del ojo
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
        //Si no hay patologías marcadas la rotación de los ojos pasa a ser normal
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

//Método para elegir el ángulo de rotación del ojo izquierdo
void SimulatorWindow::chooseLeftAngle(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;
    if(strcmp(ch->text(), "15º") == 0){
        leftAngle = 15;
    }
    else if (strcmp(ch->text(), "30º") == 0){
        leftAngle = 30;
    }
    else if (strcmp(ch->text(), "45º") == 0){
        leftAngle = 45;
    }
  }

  //Método para elegir el ángulo de rotación del ojo derecho
void SimulatorWindow::chooseRightAngle(Fl_Widget*w){

    Fl_Choice *ch = (Fl_Choice*)w;
    if(strcmp(ch->text(), "15º") == 0){
        rightAngle = 15;
    }
    else if (strcmp(ch->text(), "30º") == 0){
        rightAngle = 30;
    }
    else if (strcmp(ch->text(), "45º") == 0){
        rightAngle = 45;
    }
  }

//Método para generar paciente
void SimulatorWindow::generatePatient(Fl_Widget*w){

    checkAnsButton->activate();

    //Selecionamos un ojo al azar
    eye = rand() % 2;
    //Elegimos una patología al azar
    path = (rand() % 4) + 1;

    //Si el elegido es el ojo izquierdo
    if ( eye == 0 ){

        leftEyePath = path;
        rightEyePath = NORMAL;
        rightAngle = 0;

        //Asignamos la patología y el ángulo de rotación
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

        //Asignamos la patología y el ángulo de rotación
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

//Método ejecutado al seleccionar el ojo en el modo examen
void SimulatorWindow::chooseEye(Fl_Widget *w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if(strcmp(ch->text(), "Ojo izquierdo") == 0)
        eyeTest = 0;

    else if (strcmp(ch->text(), "Ojo derecho") == 0)
        eyeTest = 1;

  }

//Método para elegir patología en el modo examen
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

//Método ejecutado al seleccionar el ángulo de rotación en el modo examen
void SimulatorWindow::chooseAngle(Fl_Widget *w){

    Fl_Choice *ch = (Fl_Choice*)w;

    if (strcmp(ch->text(), "15º") == 0)
        angleTest = 0;

    else if (strcmp(ch->text(), "30º") == 0)
        angleTest = 1;

    else if (strcmp(ch->text(), "45º") == 0)
        angleTest = 2;
  }

//Comprobamos la respuesta del usuario
void SimulatorWindow::checkAnswer(Fl_Widget*w){

    //El ojo seleccionado, patología y ángulo de rotación son los correctos
    if (eyeTest == eye and patTest == path and angleTest == angle){

        fl_message("Correcto","Aceptar",0,0);

    }

    //En caso contraio mostramos mensaje de error
    else{

        fl_message("Incorrecto. Inténtelo de nuevo","Aceptar",0,0);
    }

}

//Método lanzado al seleccionar el oclusor del ojo izquierdo
void SimulatorWindow::controlLeftOc(Fl_Widget*w){

    //Se activa el oclusor
    if(checkLeftOc->value() == 1){

        //Marcamos por defecto el oclusor transparente
        leftOChoice->activate();
        checkRightOc->value(0);
        righOChoice->deactivate();
        righOChoice->value(0);
        rightOccluder = TRANSPARENTE;

        //Si existe enfermedad en el ojo contrario invertimos la patología
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

        //La patología vuelve al ojo inicial
        if (leftChoice->value() == 0 && rightChoice->value() != 0){
            rightEyePath = leftEyePath;
            rightAngle = leftAngle;
            leftEyePath = NORMAL;
            leftAngle = 0;
        }
    }
}

//Método lanzado al seleccionar el oclusor del ojo derecho
void SimulatorWindow::controlRightOc(Fl_Widget*w){

    //Se activa el oclusor
    if(checkRightOc->value() == 1){

        //Marcamos por defecto el oclusor transparente
        righOChoice->activate();
        checkLeftOc->value(0);
        leftOChoice->deactivate();
        leftOChoice->value(0);
        leftOccluder = TRANSPARENTE;

        //Si existe enfermedad en el ojo contrario invertimos la patología
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

        //La patología vuelve al ojo inicial
        if (rightChoice->value() == 0 && leftChoice->value() != 0){
            leftEyePath = rightEyePath;
            leftAngle = rightAngle;
            rightEyePath = NORMAL;
            rightAngle = 0;
        }
    }
}

//Método para seleccionar el material del oclusor izquierdo
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

//Método para seleccionar el material del oclusor derecho
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

//Método que lanza la guía de uso
void SimulatorWindow::showHelp(Fl_Widget*w){

    //Si la ventana de ayuda ya se encontraba abierta la cerramos
    if (popupActive == true)
    {
        pop->hide();
    }

    popupActive = true;
    callPopup();

}

//Cierra todas las ventanas de la aplicación si están activas
void SimulatorWindow::closeWindow(Fl_Widget*w){

    ((Fl_Window*)w)->hide();

    if (flagCalibration)
        winCalib->hide();

    if (popupActive){
        pop->hide();
    }

}
