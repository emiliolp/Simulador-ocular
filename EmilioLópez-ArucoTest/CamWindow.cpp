#include "CamWindow.hpp"
#include "Point3D.hpp"

using namespace aruco_test;

void CamWindow::vIdle()
{
    videoCapturer.grab();
    videoCapturer.retrieve( inputImage);

    //Validamos que se haya capturado una imagen de la cámara
    if(!inputImage.empty()){

        TheUndInputImage.create(inputImage.size(),CV_8UC3);
        cv::cvtColor(inputImage,inputImage,CV_BGR2RGB);

        MarkerDetector::Params params;
        //play with this paramteres if the detection does not work correctly
        params._borderDistThres=.01;//acept markers near the borders
        params._thresParam1=5;
        params._thresParam1_range=5;//search in wide range of values for param1
        params._cornerMethod=MarkerDetector::SUBPIX;//use subpixel corner refinement
        params._subpix_wsize= (15./2000.)*float(inputImage.cols) ;//search corner subpix in a 5x5 widow area
        PPDetector.setParams(params);//set the params above
        PPDetector.setDictionary("ARUCO_MIP_36h12");

        TheMarkers=PPDetector.detect(inputImage);

        cv::resize(inputImage,TheResizedImage,TheGlWindowSize);
    }
    ///else
       /// fl_message("Error al tomar la imagen de la cámara");

}

//Método para seleccionar un tag
void CamWindow::detectMarker(){

    Point2f maux;
    float v[50], menor;
    int indice;
    Point2f TheMarkerAux;
    Point2f MarkerAnt, PointMarkerAnt;

    //Si se han encontrado tags
    if(TheMarkers.size() != 0){

        PointMarkerAnt = markerChoosed.getCenter();

        if(isnan(PointMarkerAnt.x)){                        //Si es el primer tag detectado en la ejecución

            if(TheMarkers.size() == 1){                     //Si sólo hay un tag de tectado lo seleccionamos

                markerChoosed = TheMarkers[0];
                TheMarkerAux = markerChoosed.getCenter();

            }
            else{                                           //En caso contrario, mostramos un error

                fl_message("Error: varios tags detectados");

            }
        }
        else{                                               //Si no se trata del primer tag

            if(TheMarkers.size() == 1){

                markerChoosed = TheMarkers[0];                //Si sólo hay un tag de tectado lo seleccionamos
                econt = 0;

            }

            else if(TheMarkers.size() > 1){                 //Si encontramos varios tags

                econt++;

                if(econt >= 20){                            //Durante 5 segundos el número de tags es mayor que 1

                    fl_message("Error: varios tags detectados");

                }

                else{

                    for (unsigned int m=0;m<TheMarkers.size();m++){         //Seleccionamos el tag más cercano al anterior detectado

                        TheMarkerAux = TheMarkers[m].getCenter();
                        maux.x = PointMarkerAnt.x - TheMarkerAux.x;
                        maux.y = PointMarkerAnt.y - TheMarkerAux.y;
                        v[m] = maux.x + maux.y;

                    }

                    menor = v[0];
                    indice = 0;

                    for(unsigned int m=0; m<TheMarkers.size(); m++){

                        if(v[m] < menor){
                            menor = v[m];
                            indice = m;
                        }
                    }

                    markerChoosed = TheMarkers[indice];

                }
            }
        }
    }

    else
        econt = 0;

    //Si el marcador 1 se ha asignado calculamos
    if ( markerChoosed.empty() == 0 && cameraParams.isValid() )
        markerChoosed.calculateExtrinsics(0.04,cameraParams);

}



void CamWindow::draw() {

        //Capturamos la imagen de la cámara y detectamos los marcadores disponibles
        vIdle();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        //Dibuja la imagen en el visor
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, TheGlWindowSize.width, 0, TheGlWindowSize.height, -1.0, 1.0);
        glViewport(0, 0, TheGlWindowSize.width , TheGlWindowSize.height);
        glDisable(GL_TEXTURE_2D);
        glPixelZoom( 1, -1);
        glRasterPos3f( 0, TheGlWindowSize.height  - 0.5, -1.0 );
        glDrawPixels ( TheGlWindowSize.width , TheGlWindowSize.height , GL_RGB , GL_UNSIGNED_BYTE , TheResizedImage.ptr(0) );

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

        //Seleccionamos únicamente un tag en el caso de que la cámara detecte varios
        detectMarker();

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

            keybd_event(VK_SHIFT,0x10,0 , 0); //Simulamos que se presiona tecla shift para evitar apagado de pantalla
            keybd_event(VK_SHIFT,0x10, KEYEVENTF_KEYUP,0);
        }
        else if(flagCalibration == true){

            noExisteTag = true;

        }

        //Si se está calibrando marcar el área
        if (flagCalibration == true){

            glLoadIdentity();

            glColor3f(1,1.0,0.0);
            glPointSize(10.0);

            if (calibP3d.size() == 0){      //Si se realiza la primera captura

                glBegin(GL_LINES);
                glVertex3f(-0.5, 0.2, 0.0f);
                glVertex3f(-1.0, 0.2, 0.0f);
                glEnd( );

                glBegin(GL_LINES);
                glVertex3f(-0.5, 1.0, 0.0f);
                ///glVertex3f(-0.5, 0.7, 0.0f);
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
    // HANDLE WINDOW RESIZING
     void CamWindow::resize(int x, int y, int w,int h) {

        Fl_Gl_Window::resize(x,y,w,h);
        TheGlWindowSize=Size(w,h);

        if (w*3%4!=0) {
            w+=w*3%4;//resize to avoid padding
            resize(x,y,w,TheGlWindowSize.height);
        }
        else {

            if (TheUndInputImage.rows!=0)
                cv::resize(TheUndInputImage,TheResizedImage,TheGlWindowSize);
        }

    }

    void CamWindow::timerCB(void *userdata) {
        CamWindow *pb = (CamWindow*)userdata;
        pb->redraw();
        Fl::repeat_timeout(1.0/5000.0, timerCB, userdata);
    }


        //Si se han encontrado tags
        if(TheMarkers.size() != 0){

            PointMarkerAnt = Marker1.getCenter();

            if(isnan(PointMarkerAnt.x)){                    //Si es el primer tag detectado en la ejecución

                if(TheMarkers.size() == 1){                 //Si sólo hay un tag de tectado lo seleccionamos

                    Marker1 = TheMarkers[0];
                    TheMarkerAux = Marker1.getCenter();

                }
                else{                                       //En caso contrario, mostramos un error

                    fl_message("Error: varios tags detectados");

                }
            }
            else{                                           //Si no se trata del primer tag

                if(TheMarkers.size() == 1){

                    Marker1 = TheMarkers[0];                //Si sólo hay un tag de tectado lo seleccionamos
                    strcpy(cadena,"OK");
                    econt = 0;

                }

                else if(TheMarkers.size() > 1){             //Si encontramos varios tags

                    econt++;

                    if(econt >= 20){                        //Durante 5 segundos el número de tags es mayor que 1

                        fl_message("Error: varios tags detectados");

                    }

                    else{

                        for (unsigned int m=0;m<TheMarkerSize;m++){         //Seleccionamos el tag más cercano al anterior detectado

                            TheMarkerAux = TheMarkers[m].getCenter();
                            maux.x = PointMarkerAnt.x - TheMarkerAux.x;
                            maux.y = PointMarkerAnt.y - TheMarkerAux.y;
                            v[m] = maux.x + maux.y;

                        }

                        menor = v[0];
                        indice = 0;

                        for(unsigned int m=0; m<TheMarkerSize; m++){

                            if(v[m] < menor){
                                menor = v[m];
                                indice = m;
                            }
                        }

                        Marker1 = TheMarkers[indice];

                    }
                }
            }
        }

        else
            econt = 0;

        //Si el marcador 1 se ha asignado calculamos
        if ( Marker1.empty() == 0 && TheCameraParams.isValid() )
            ///Marker1.calculateExtrinsics(0.043,TheCameraParams);
            Marker1.calculateExtrinsics(0.04,TheCameraParams);

    }

    void CamWindow::calculateEyesPosition(Point2f MarkerC){

        Point3D MarkerEye1, MarkerEye2, MarkerEye;
        Point2f MarkerAux;
        float rot_estrabismo_y, rot_estrabismo_x, angle, desy_camera;

        //Almacenamos la posición (x,y) del tag detectado
        MarkerAux = Marker1.getCenter();
        MarkerPx.readPoint3D(MarkerAux.x,MarkerAux.y,0);

        if (not isnan(MarkerAux.x)){

            //Modificamos los valores de las coordenadas del punto, el punto 0,0 se sitúa ahora en el centro de la pantalla
            MarkerPx.translateCentre();
            //Convertimos las coordenadas del punto a cm, este objeto solo se utilizará para visualizar el punto en la pantalla
            //La conversión se realiza sabiendo que el punto (320, 240)px es equivalente a (0.7179, 0.5384)cm
            MarkerCm.setX(((MarkerPx.getX() * 0.65)/320) + 0.06);
            MarkerCm.setY(((MarkerPx.getY() * 0.42)/240) - 0.38);

            //Obtenemos los valores del vector de traslación de la cámara con respecto al ojo izquierdo
            //Aplicamos el desplazamiento del ojo izquierdo
            ///cout<<"Marker1.x: "<<Marker1.Tvec.at<float>(0,0)<<endl;
            ///cout<<"Marker1.y: "<<Marker1.Tvec.at<float>(1,0)<<endl;
            ///cout<<"Marker1.z: "<<Marker1.Tvec.at<float>(2,0)<<endl;

            float desx2 = Marker1.Tvec.at<float>(0,0) + 0.022;
            ///desy_camera = ((Fl::h()/6) * 0.026458333)/100;                      //Conversión píxeles a m
            desy_camera = ((Fl::h()/2) * 0.026458333)/100;                      //Conversión píxeles a m
            float desy2 = -(Marker1.Tvec.at<float>(1,0) - 0.009 - desy_camera);
            float desz2 = Marker1.Tvec.at<float>(2,0);
            MarkerEye2.readPoint3D(desx2, desy2, desz2);

            //Obtenemos los valores del vector de traslación de la cámara con al centro (punto de luz)
            float desx = Marker1.Tvec.at<float>(0,0);
            float desy = -(Marker1.Tvec.at<float>(1,0) - desy_camera);
            float desz = Marker1.Tvec.at<float>(2,0);
            MarkerEye.readPoint3D(desx, desy, desz);

            //Rotación del punto de luz
            if (MarkerEye.turnAxisX() > 0)
                rotx = (MarkerEye.turnAxisX() * 2 - desy_camera) * 3;
            else
                rotx = MarkerEye.turnAxisX() * 2 - desy_camera;

            roty = MarkerEye.turnAxisY() * 2;
            rotz = MarkerEye.turnAxisZ();

            rotz2 = MarkerEye2.turnAxisZ();
        }

        //Rotación del ojo en el eje x
        if ( curr_string_iz == HIPERTROPIA){

            if (MarkerEye2.turnAxisX() > 0){

                angle = atan2(angle_iz/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                ///rotx2 = (MarkerEye2.turnAxisX() - rot_estrabismo_x) * 4;
                rotx2 = (MarkerEye2.turnAxisX() - angle_iz) * 4;
            }
            else{

                angle = atan2(angle_iz/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                ///rotx2 = MarkerEye2.turnAxisX() - rot_estrabismo_x;
                rotx2 = MarkerEye2.turnAxisX() - angle_iz;
            }
            if (rotx2 < (-25.5))
                    rotx2 = -25.5;
        }

        else if ( curr_string_iz == HIPOTROPIA){

            if (MarkerEye2.turnAxisX() > 0){

                angle = atan2(angle_iz/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                ///rotx2 = (MarkerEye2.turnAxisX() + rot_estrabismo_x) * 4;
                rotx2 = (MarkerEye2.turnAxisX() + angle_iz) * 4;
            }
            else{
                angle = atan2(angle_iz/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                ///rotx2 = MarkerEye2.turnAxisX() + rot_estrabismo_x;
                rotx2 = MarkerEye2.turnAxisX() + angle_iz;
            }
        if (rotx2 > 23.0)
                rotx2 = 23.0;
        }

        else{

            if (MarkerEye2.turnAxisX() > 0)
                //rotx2 = MarkerEye2.turnAxisX() * 2;
                rotx2 = MarkerEye2.turnAxisX() * 3;
            else
                rotx2 = MarkerEye2.turnAxisX();

            if (rotx2 > 23.0)
                rotx2 = 23.0;
        }

        //Rotación del ojo en el eje y
        if (curr_string_iz == ENDOTROPIA){

            angle = atan2(angle_iz/7, 12.5);
            rot_estrabismo_y = (angle * 180) / 3.141618;
            ///roty2 = MarkerEye2.turnAxisY() - rot_estrabismo_y;
            roty2 = MarkerEye2.turnAxisY() - angle_iz;

            if (roty2 < (-47))
            roty2 = -47;    //Límite rotación eje y
        }
        else if(curr_string_iz == EXOTROPIA){
            angle = atan2(angle_iz/7, 12.5);
            rot_estrabismo_y = (angle * 180) / 3.141618;
            ///roty2 = MarkerEye2.turnAxisY() + rot_estrabismo_y;
            roty2 = MarkerEye2.turnAxisY() + angle_iz;
        }
        else{
            roty2 = MarkerEye2.turnAxisY();
        }

        if (not isnan(MarkerAux.x)){
            //Desplazamiento del ojo derecho
            float desx1 = Marker1.Tvec.at<float>(0,0) - 0.022;
            float desy1 = -(Marker1.Tvec.at<float>(1,0) - 0.009 - desy_camera);
            float desz1 = Marker1.Tvec.at<float>(2,0);

            MarkerEye1.readPoint3D(desx1,desy1,desz1);
            //Rotación ejes z, x e y
            rotz1 = MarkerEye1.turnAxisZ();

        }
        if ( curr_string_de == HIPERTROPIA ){

            if (MarkerEye2.turnAxisX() > 0){
                angle = atan2(angle_de/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                ///rotx1 = (MarkerEye1.turnAxisX() - rot_estrabismo_x) * 4;
                rotx1 = (MarkerEye1.turnAxisX() - angle_de) * 4;
            }
            else{
                angle = atan2(angle_de/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                ///rotx1 = MarkerEye1.turnAxisX() - rot_estrabismo_x;
                rotx1 = MarkerEye1.turnAxisX() - angle_de;
            }
            if (rotx1 < (-25.5))
                rotx1 = -25.5;

        }
        else if(curr_string_de == HIPOTROPIA){

            if(MarkerEye2.turnAxisX() > 0){
                angle = atan2(angle_de/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                ///rotx1 = (MarkerEye1.turnAxisX() + rot_estrabismo_x)*4;
                rotx1 = (MarkerEye1.turnAxisX() + angle_de)*4;
            }
            else{
                angle = atan2(angle_de/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                ///rotx1 = MarkerEye1.turnAxisX() + rot_estrabismo_x;
                rotx1 = MarkerEye1.turnAxisX() + angle_de;
                if (rotx2 > 23.0)
                    rotx2 = 23.0;
            }
        }
        else{

            if (MarkerEye1.turnAxisX() > 0)
               //rotx1 = MarkerEye1.turnAxisX() * 2;
               rotx1 = MarkerEye1.turnAxisX() * 3;
            else
                rotx1 = MarkerEye1.turnAxisX();

            if (rotx1 > 23.0)
                rotx1 = 23.0;
        }



        if (curr_string_de == ENDOTROPIA){

            angle = atan2(angle_de/7, 12.5);
            rot_estrabismo_y = (angle * 180) / 3.141618;
            ///roty1 = MarkerEye1.turnAxisY() + rot_estrabismo_y;
            roty1 = MarkerEye1.turnAxisY() + angle_de;

               if (roty1 < -47)
                roty1 = -47;
        }
        else if (curr_string_de == EXOTROPIA){
            angle = atan2(angle_de/7, 12.5);
            rot_estrabismo_y = (angle * 180) / 3.141618;
            ///roty1 = MarkerEye1.turnAxisY() - rot_estrabismo_y;
            roty1 = MarkerEye1.turnAxisY() - angle_de;
        }
        else
            roty1 = MarkerEye1.turnAxisY();

}


void CamWindow::draw() {

        //Capturamos la imagen de la cámara y detectamos los marcadores disponibles
        vIdle();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        //Dibuja la imagen en el visor
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, TheGlWindowSize.width, 0, TheGlWindowSize.height, -1.0, 1.0);
        glViewport(0, 0, TheGlWindowSize.width , TheGlWindowSize.height);
        glDisable(GL_TEXTURE_2D);
        glPixelZoom( 1, -1);
        glRasterPos3f( 0, TheGlWindowSize.height  - 0.5, -1.0 );
        glDrawPixels ( TheGlWindowSize.width , TheGlWindowSize.height , GL_RGB , GL_UNSIGNED_BYTE , TheResizedImage.ptr(0) );

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

        //Seleccionamos únicamente un tag en el caso de que la cámara detecte varios
        detectMarker();
        Point2f MarkerDetect = Marker1.getCenter();

        //Calculamos la rotación de los ojos para la posición del tag detectado
        if (not isnan(MarkerDetect.x)){
            calculateEyesPosition(MarkerDetect);
        }
        else{
            MarkerDetect.x = 0.0;
            MarkerDetect.y = 0.0;
            calculateEyesPosition(MarkerDetect);
        }

        //Sólo indicamos el indicador del tag cuando éste cambia de posición
        if( MarkerCm.getX() != MarkerAnt.x && MarkerCm.getY() != MarkerAnt.y){

            glTranslatef(0.0f,0.0f,-1.3f);

            glColor3f(1,0.0,0.0);
            glPointSize(10.0);
            glBegin(GL_POINTS);
            glVertex3f(MarkerCm.getX(), -MarkerCm.getY(), 0.0f); // origin of the line
            glEnd( );

            MarkerAnt.x = MarkerCm.getX();
            MarkerAnt.y = MarkerCm.getY();

            if (flagCalibration == true)
                noExisteTag = false;

            keybd_event(VK_SHIFT,0x10,0 , 0); //Simulamos que se presiona tecla shift para evitar apagado de pantalla
            keybd_event(VK_SHIFT,0x10, KEYEVENTF_KEYUP,0);
        }
        else if(flagCalibration == true){

            noExisteTag = true;

        }

        //Si se está calibrando marcar el área
        if (flagCalibration == true){

            glLoadIdentity();

            glColor3f(1,1.0,0.0);
            glPointSize(10.0);

            if (calib_p3d.size() == 0){      //Si se realiza la primera captura

                glBegin(GL_LINES);
                glVertex3f(-0.5, 0.2, 0.0f);
                glVertex3f(-1.0, 0.2, 0.0f);
                glEnd( );

                glBegin(GL_LINES);
                glVertex3f(-0.5, 1.0, 0.0f);
                ///glVertex3f(-0.5, 0.7, 0.0f);
                glVertex3f(-0.5, 0.2, 0.0f);
                glEnd( );
            }

            else if (calib_p3d.size() == 1){    //Si se realiza la segunda captura

                glBegin(GL_LINES);
                glVertex3f(-0.5, -0.2, 0.0f);
                glVertex3f(-1.0, -0.2, 0.0f);
                glEnd( );

                glBegin(GL_LINES);
                glVertex3f(-0.5, -1.0, 0.0f);
                glVertex3f(-0.5, -0.2, 0.0f);
                glEnd( );
            }

            else if (calib_p3d.size() == 2){    //Si se realiza la tercera captura

                glBegin(GL_LINES);
                glVertex3f(0.4, -0.2, 0.0f);
                glVertex3f(1.0, -0.2, 0.0f);
                glEnd( );

                glBegin(GL_LINES);
                glVertex3f(0.4, -0.2, 0.0f);
                glVertex3f(0.4, -1.0, 0.0f);
                glEnd( );
            }

            else if (calib_p3d.size() == 3){    //Si se realiza la cuarta captura

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
    // HANDLE WINDOW RESIZING
     void CamWindow::resize(int x, int y, int w,int h) {

        Fl_Gl_Window::resize(x,y,w,h);
        TheGlWindowSize=Size(w,h);

        if (w*3%4!=0) {
            w+=w*3%4;//resize to avoid padding
            resize(x,y,w,TheGlWindowSize.height);
        }
        else {

            if (TheUndInputImage.rows!=0)
                cv::resize(TheUndInputImage,TheResizedImage,TheGlWindowSize);
        }

    }

    void CamWindow::Timer_CB(void *userdata) {
        CamWindow *pb = (CamWindow*)userdata;
        pb->redraw();
        Fl::repeat_timeout(1.0/5000.0, Timer_CB, userdata);
    }
