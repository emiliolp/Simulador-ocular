/**
 *Implementación de los métodos de la clase SimulatorWindow
 */
#include "CamWindow.hpp"

using namespace simulator;

//Método que configura la imagen a mostrar en el visor
void CamWindow::detectMarker()
{
    videoCapturer.grab();
    videoCapturer.retrieve(inputImage);

    //Validamos que se haya capturado una imagen de la cámara
    if(!inputImage.empty()){

        cameraError = 0;
        undInputImage.create(inputImage.size(),CV_8UC3);
        cv::cvtColor(inputImage,inputImage,CV_BGR2RGB);

        //Parámetros iniciales de la cámara
        MarkerDetector::Params params;
        params._borderDistThres=.01;
        params._thresParam1=5;
        params._thresParam1_range=5;
        params._cornerMethod=MarkerDetector::SUBPIX;
        params._subpix_wsize= (15./2000.)*float(inputImage.cols);
        detector.setParams(params);
        detector.setDictionary("ARUCO_MIP_36h12");  //Diccionario de los tags a detectar

        markersDetected=detector.detect(inputImage);

        cv::resize(inputImage,resizedImage,windowSize);
    }
    else{
        cameraError +=1;

        if (cameraError == 10)  //Si la cámara está inactiva durante más de 3 segundos se muestra un error
            fl_message("La cámara no puede ser utilizada, \ncierre el simulador e inténtelo de nuevo");
    }
}

//Método para seleccionar un tag y calcular los parámetros extrínsecos
void CamWindow::selectMarker(){

    //Si se han encontrado por lo menos un tag
    if(markersDetected.size() != 0){

        //Si sólo hay un tag detectado lo seleccionamos
        if(markersDetected.size() == 1){

            markerChoosed = markersDetected[0];

        }
        //En caso contrario, mostramos un error y se detiene la simulación
        else{
            markerChoosed.clear();
            fl_message("Error: varios tags detectados");

        }
    }

    //Si el marcador 1 se ha asignado y obtener los parámetros extrínsecos
    if ( markerChoosed.empty() == 0 && cameraParams.isValid() )
        markerChoosed.calculateExtrinsics(0.04,cameraParams);

}

void CamWindow::draw() {

    //Capturamos la imagen de la cámara y detectamos los marcadores disponibles
    detectMarker();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //Dibuja la imagen en el visor
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowSize.width, 0, windowSize.height, -1.0, 1.0);
    glViewport(0, 0, windowSize.width , windowSize.height);
    glDisable(GL_TEXTURE_2D);
    glPixelZoom( 1, -1);
    glRasterPos3f( 0, windowSize.height  - 0.5, -1.0 );
    glDrawPixels ( windowSize.width , windowSize.height , GL_RGB , GL_UNSIGNED_BYTE , resizedImage.ptr(0) );

    double proj_matrix[16];
    proj_matrix[0] = 2;
    proj_matrix[1] = -0;
    proj_matrix[2] = 0;
    proj_matrix[3] = 0;
    proj_matrix[4] = 0;
    proj_matrix[5] = -3.1;
    proj_matrix[6] = 0;
    proj_matrix[7] = 0;
    proj_matrix[8] = 0.1;
    proj_matrix[9] = 0.1;
    proj_matrix[10] = -1;
    proj_matrix[11] = -1;
    proj_matrix[12] = 0;
    proj_matrix[13] = -0;
    proj_matrix[14] = -0.1;
    proj_matrix[15] = 0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixd(proj_matrix);

    //Seleccionamos únicamente un tag mostraremos error si se encuentran varios
    selectMarker();

    //Almacenamos la posición (x,y) del tag detectado
    Point2f MarkerAux = markerChoosed.getCenter();
    Point3D MarkerPx, MarkerCm;
    MarkerPx.readPoint3D(MarkerAux.x,MarkerAux.y,0);
    if (not isnan(MarkerAux.x)){
        MarkerPx.translateCentre();
        MarkerCm.setX(((MarkerPx.getX() * 0.65)/320) + 0.06);
        MarkerCm.setY(((MarkerPx.getY() * 0.42)/240) - 0.38);
    }

    //Sólo indicamos el indicador del tag cuando éste cambia de posición
    if( MarkerCm.getX() != markerAnt.x && MarkerCm.getY() != markerAnt.y){

        glTranslatef(0.0f,0.0f,-1.3f);

        glColor3f(1,0.0,0.0);
        glPointSize(10.0);
        glBegin(GL_POINTS);
        glVertex3f(MarkerCm.getX(), -MarkerCm.getY(), 0.0f); // origin of the line
        glEnd( );

        markerAnt.x = MarkerCm.getX();
        markerAnt.y = MarkerCm.getY();

        if (flagCalibration == true)
            noExisteTag = false;

        //Simulamos que se presiona tecla shift para evitar apagado de pantalla
        keybd_event(VK_SHIFT,0x10,0 , 0);
        keybd_event(VK_SHIFT,0x10, KEYEVENTF_KEYUP,0);
    }
    else if(flagCalibration == true){

        noExisteTag = true;

    }

    //Si se está calibrando marcar el área con recuadro de color amarillo
    if (flagCalibration == true){

        if (winCalib->shown()){

            glLoadIdentity();

            glColor3f(1,1.0,0.0);
            glPointSize(10.0);

            if (calibP3d.size() == 0){          //Si se realiza la primera captura

                glBegin(GL_LINES);
                glVertex3f(-0.5, 0.2, 0.0f);
                glVertex3f(-1.0, 0.2, 0.0f);
                glEnd( );

                glBegin(GL_LINES);
                glVertex3f(-0.5, 1.0, 0.0f);
                glVertex3f(-0.5, 0.2, 0.0f);
                glEnd( );
            }

            else if (calibP3d.size() == 1){    //Si se realiza la segunda captura

                glBegin(GL_LINES);
                glVertex3f(-0.5, -0.2, 0.0f);
                glVertex3f(-1.0, -0.2, 0.0f);
                glEnd( );

                glBegin(GL_LINES);
                glVertex3f(-0.5, -1.0, 0.0f);
                glVertex3f(-0.5, -0.2, 0.0f);
                glEnd( );
            }

            else if (calibP3d.size() == 2){    //Si se realiza la tercera captura

                glBegin(GL_LINES);
                glVertex3f(0.4, -0.2, 0.0f);
                glVertex3f(1.0, -0.2, 0.0f);
                glEnd( );

                glBegin(GL_LINES);
                glVertex3f(0.4, -0.2, 0.0f);
                glVertex3f(0.4, -1.0, 0.0f);
                glEnd( );
            }

            else if (calibP3d.size() == 3){    //Si se realiza la cuarta captura

                glBegin(GL_LINES);
                glVertex3f(0.4, 0.2, 0.0f);
                glVertex3f(1.0, 0.2, 0.0f);
                glEnd( );

                glBegin(GL_LINES);
                glVertex3f(0.4, 1.0, 0.0f);
                glVertex3f(0.4, 0.2, 0.0f);
                glEnd( );
            }
        }
    }
}

//Redimensionado del visor de la cámara
void CamWindow::resize(int x, int y, int w,int h) {

    Fl_Gl_Window::resize(x,y,w,h);
    windowSize=Size(w,h);

    if (w*3%4!=0) {
        w+=w*3%4;
        resize(x,y,w,windowSize.height);
    }
    else {

        if (undInputImage.rows!=0)
            cv::resize(undInputImage,resizedImage,windowSize);
    }

}

//Método para actualizar la imagen del visor
void CamWindow::updateScene(void *userdata) {

    CamWindow *pb = (CamWindow*)userdata;
    pb->redraw();
    Fl::repeat_timeout(1.0/5000.0, updateScene, userdata);

}

//Actualiza el brillo de la imagen mostrada en el visor
void CamWindow::setBrightness(double newFg, double newBg){

    brightFirstLevel = newFg;
    brightBack = newBg;
    redraw();

}
