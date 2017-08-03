/**
 *Implementaci�n de los m�todos de la clase SimulatorWindow
 */
#include "HelpDialog.hpp"

using namespace simulator;

//M�todo que inicializa los valores de la ventana de calibraci�n
int HelpDialog::showCalibrate(){

    winCalib = new CalibWindow((Fl::w()/2)-390,(Fl::h()/2)-200,780,400,"Instrucciones de calibraci�n");
    winCalib->resizable(pop);
    winCalib->color(Fl_Color(7));

    //Establecemos las �reas donde se incluyen las cuatro capturas del tag
    boxCalibra1 = new Fl_Box(110,0,14,500);
    boxCalibra2 = new Fl_Box(290,0,14,500);
    boxCalibra3 = new Fl_Box(470,0,14,500);
    boxCalibra4 = new Fl_Box(650,0,14,500);

    calibImage1 = new Fl_JPEG_Image("capturas_tag/1.jpg");
    calibImage2 = new Fl_JPEG_Image("capturas_tag/2.jpg");
    calibImage3 = new Fl_JPEG_Image("capturas_tag/3.jpg");
    calibImage4 = new Fl_JPEG_Image("capturas_tag/4.jpg");

    boxCalibra1->image(calibImage1);
    boxCalibra2->image(calibImage2);
    boxCalibra3->image(calibImage3);
    boxCalibra4->image(calibImage4);

    calibImage2->inactive();
    calibImage3->inactive();
    calibImage4->inactive();

    calibP2d.clear();
    calibP3d.clear();

    //Mostramos la ventana de calibraci�n
    winCalib->show();

    return(Fl::run());

}

//M�todo utilizado para cerrar las instrucciones de uso
void HelpDialog::close(Fl_Widget*w){

    pop->hide();
    popupActive = false;
    flagCalibration = false;
    if (!cameraParams.isValid())
        fl_message("Es necesario realizar las tareas de calibraci�n\nantes de empezar a utilizar el simulador");
}

//M�todo para mostrar la imagen del tag incluida al instalar la aplicaci�n
void HelpDialog::showMarker(Fl_Widget*w){

    char buffer[MAX_PATH];
    char *tag = NULL;
    std::string path;
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    path = "rundll32.exe C:\\WINDOWS\\system32\\shimgvw.dll,ImageView_Fullscreen " + string( buffer ).substr( 0, pos) + "\\capturas_tag\\marcador.jpg";
    char *cstr = new char[path.length() + 1];
    strcpy(cstr, path.c_str());
    WinExec(cstr, SW_SHOW);
}

//Llamamos a la ventana de calibraci�n
void HelpDialog::callCalibrate(Fl_Widget*w){

    //Si hay una ventana abierta de calibraci�n la cerramos para que no haya duplicidad de ventana
    if (flagCalibration == true){

        if(winCalib->shown()){

            winCalib->hide();
        }
    }

    flagCalibration = true;
    showCalibrate();

}
