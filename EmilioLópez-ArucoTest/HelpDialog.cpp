#include "HelpDialog.hpp"
#include "CalibWindow.hpp"

using namespace aruco_test;

int HelpDialog::showCalibrate(){

    winCalib = new CalibWindow((Fl::w()/2)-390,(Fl::h()/2)-200,780,400,"Instrucciones de calibración");
    ///win_calib = new WinCalibrate((Fl::w()/2)-300,(Fl::h()/2)-225,600,450,"Instrucciones de calibración");
    winCalib->resizable(pop);
    winCalib->color(Fl_Color(7));
                      // initialize image lib
    boxCalibra1 = new Fl_Box(110,0,14,500);     // widget that will contain image
    boxCalibra2 = new Fl_Box(290,0,14,500);
    boxCalibra3 = new Fl_Box(470,0,14,500);
    boxCalibra4 = new Fl_Box(650,0,14,500);

    calibImage1 = new Fl_JPEG_Image("1.jpg");      // load jpeg image into ram
    calibImage2 = new Fl_JPEG_Image("2.jpg");
    calibImage3 = new Fl_JPEG_Image("3.jpg");
    calibImage4 = new Fl_JPEG_Image("4.jpg");

    boxCalibra1->image(calibImage1);
    boxCalibra2->image(calibImage2);
    boxCalibra3->image(calibImage3);
    boxCalibra4->image(calibImage4);

    calibImage2->inactive();
    calibImage3->inactive();
    calibImage4->inactive();

    calib_p2d.clear();
    calib_p3d.clear();

    winCalib->show();

    return(Fl::run());

}

    void HelpDialog::Close(Fl_Widget*w, void *d){
        pop->hide();
        popupActive = false;
        flagCalibration = false;
        if (!TheCameraParams.isValid())
            fl_message("Es necesario realizar las tareas de calibración\nantes de empezar a utilizar el simulador","Aceptar",0,0);
    }

    void HelpDialog::ShowMarker(Fl_Widget*w, void *d){
        char buffer[MAX_PATH];
        char *tag = NULL;
        std::string path;
        GetModuleFileName( NULL, buffer, MAX_PATH );
        string::size_type pos = string( buffer ).find_last_of( "\\/" );
        path = "rundll32.exe C:\\WINDOWS\\system32\\shimgvw.dll,ImageView_Fullscreen " + string( buffer ).substr( 0, pos) + "\\marcador.jpg";
        char *cstr = new char[path.length() + 1];
        strcpy(cstr, path.c_str());
        WinExec(cstr, SW_SHOW);
    }

    void HelpDialog::CallCalibrate(Fl_Widget*w, void *d){

        flagCalibration = true;
        showCalibrate();

    }
