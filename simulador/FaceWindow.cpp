/**
 *Implementación de los métodos de la clase FaceWindow
 */
#include "FaceWindow.hpp"

using namespace simulator;

//Método que dibuja el rostro humano en la pantalla
void FaceWindow::draw() {

    Point3f leftEyeRot, rightEyeRot, lightRot;

    //Materiales de la iluminación
    float matTransparent[] = { 0.75, 0.75, 0.0, 1.0 };
    float matZero[] = { 0.0, 0.0, 0.0, 1.0 };
    float matSolid[] = { 1.0, 1, 1, 1.0 };
    float matEmission[] = { 1.0, 1, 1, 0.6 };

    //Posición de la fuente de luz
    float lightPos[] = { 0.0, 0.0, 1.0, 0.0 };

    //Calculamos la rotación de cada ojo y del punto de luz
    calculateEyesPosition(leftEyeRot, rightEyeRot, lightRot, markerChoosed);

    if (fillResizeFace == 0){
        resize(Fl_Gl_Window::x(), Fl_Gl_Window::y(), Fl_Gl_Window::w(), Fl_Gl_Window::h());
        fillResizeFace = 1;
    }

    //Inicializamos los valores de rotación si la configuración de la cámara no es válida
    if (!cameraParams.isValid() || popupActive == true){

        leftEyeRot.x  = 0.0;
        leftEyeRot.y  = 0.0;
        leftEyeRot.z  = 0.0;
        rightEyeRot.x = 0.0;
        rightEyeRot.y = 0.0;
        rightEyeRot.z = 0.0;
        lightRot.x    = 0.0;
        lightRot.y    = 0.0;
        lightRot.z    = 0.0;
    }

    //Establecemos los puntos de luz
    initRendering(lightRot);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE); //Se normalizan automáticamente las normales
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    //Mostramos los puntos de luz establecidos
    glPushMatrix();
    glRotatef(lightRot.y, 0,1,0);
    glRotatef(lightRot.x, 1,0,0);
    glRotatef(lightRot.z, 0,0,1);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glTranslatef(lightPos[0],lightPos[1],lightPos[2]);
    glPopMatrix();
    glPushMatrix();

    //Dibuja en la pantalla el rostro
    glMaterialfv(GL_FRONT, GL_EMISSION, matZero);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matSolid);
    glLoadIdentity();
    glTranslatef(0.0f,-0.3f,-1.0f);
    glScalef(0.25f,0.25f,0.25f);
    drawShape(vfFace,vvFace,vnFace,1);
    glPopMatrix();

    //Dimensiones y posición del ojo izquierdo
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.42195f, 0.171765f, -1.8f);
    glScalef(0.25f,0.25f,0.25f);

    //Rotamos ojo izquierdo
    glRotatef(leftEyeRot.x,1,0,0);
    glRotatef(leftEyeRot.y,0,1,0);

    //Dibuja el ojo izquierdo
    drawShape(vfEye,vvEye,vnEye,2);
    glPopMatrix();

    //Dimensiones y posición del ojo derecho
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-0.42195f, 0.171765, -1.8);
    glScalef(0.25f,0.25f,0.25f);

    //Rotamos ojo derecho
    glRotatef(rightEyeRot.x,1,0,0);
    glRotatef(rightEyeRot.y,0,1,0);

    //Dibuja la forma
    drawShape(vfEye,vvEye,vnEye,2);
    glPopMatrix();

    //Dibuja Oclusor izquierdo si se ha marcado la opción y está en modo exploración
    if (checkLeftOc->value() == 1 && examMode == 0){

        glPushMatrix();
        glLoadIdentity();
        glTranslatef(0.42195f,0.171765f,-1.0f);

        if (leftOccluder == TRANSPARENTE){  //Oclusor izquierdo transparente

            glMaterialfv(GL_FRONT, GL_EMISSION, matZero);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, matTransparent);
            glEnable (GL_BLEND);
            glDepthMask (GL_FALSE);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE);

        }
        else if (leftOccluder == OPACO){    //Oclusor izquierdo opaco

            glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, matSolid);
        }

        //Dibujamos la lente
        glScalef(0.0075f,0.0075f,0.0075f);
        drawShape(vfLente,vvLente,vnLente,3);

        if (leftOccluder == TRANSPARENTE){  //Oclusor izquierdo transparente
            glDepthMask (GL_TRUE);
            glDisable (GL_BLEND);
        }

        glPopMatrix();

    }

    //Dibujamos el oclusor derecho si se encuentra en modo exploración
    if (checkRightOc->value() == 1 && examMode == 0){

        glPushMatrix();
        glLoadIdentity();
        glTranslatef(-0.42195f,0.171765f,-1.0f);

        if (rightOccluder == TRANSPARENTE){         //Oclusor izquierdo transparente

            glMaterialfv(GL_FRONT, GL_EMISSION, matZero);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, matTransparent);
            glEnable (GL_BLEND);
            glDepthMask (GL_FALSE);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE);

        }
        else if (rightOccluder == OPACO){           //Oclusor derecho opaco

            glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, matSolid);

        }

        //Dibujamos el oculsor derecho
        glScalef(0.0075f,0.0075f,0.0075f);
        drawShape(vfLente,vvLente,vnLente,3);

        if (rightOccluder == TRANSPARENTE){
            glDepthMask (GL_TRUE);
            glDisable (GL_BLEND);
        }

        glPopMatrix();

    }

}

//Método para redimensionar el área de la pantalla con el rostro humano
 void FaceWindow::resize(int x, int y, int w,int h) {

    Fl_Gl_Window::resize(x,y,w,h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (w<=h){
        glOrtho(-1.5,1.5,-1.5*(GLfloat)h/(GLfloat)w, 1.5*(GLfloat)h/(GLfloat)w,-10.0,10.0);
    }
    else{
        glOrtho(-1.5*(GLfloat)w/(GLfloat)h,1.5*(GLfloat)w/(GLfloat)h, -1.5,1.5,-10.0,10.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    redraw();
}

//Establecemos la periodicidad con la que se actualiza el modelo 3D
void FaceWindow::updateScene(void *userdata) {

    FaceWindow *pb = (FaceWindow*)userdata;
    pb->redraw();
    Fl::repeat_timeout(1.0/5000.0, updateScene, userdata);

}

//Método para leer el contenido de un fichero .obj
void FaceWindow::loadFile(char *filename, vector<TypeF> &arrayF, vector<TypeV> &arrayV, vector<TypeV> &arrayVn){

    FILE *file;
    int d;
    TypeV auxV, auxVn;
    TypeF auxVf;
    char lineHeader[128];
    int x=0,j=0,k=0;
    float px,py,pz;
    int pa,pb,pc,pan,pbn,pcn;

    //Abrimos el fichero en modo lectura
    file = fopen(filename,"r");
    if(file == NULL){
        cerr<<"Error: Can not open file"<<endl;
        exit(0);
    }

    //leemos cada línea del fichero
    while(1){

        int res = fscanf(file,"%s",lineHeader);

        if (res == EOF){
            break;
        }

        //Obtenemos los vértices del dibujo
        if (strcmp(lineHeader,"v")==0){
            fscanf(file,"%f %f %f",&px,&py,&pz);
            auxV.readTypeV(px,py,pz);
            arrayV.push_back(auxV);
        }

        //Obtenemos las normales
        else if(strcmp(lineHeader,"vn") == 0){
            fscanf(file,"%f %f %f",&px,&py,&pz);
            auxVn.readTypeV(px,py,pz);
            arrayVn.push_back(auxVn);
        }

        //La posición en el espacio de los vértices anteriores
        else if(strcmp(lineHeader,"f") == 0){
            fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d",&pa,&d,&pan,&pb,&d,&pbn,&pc,&d,&pcn);
            auxVf.readTypeF(pa,pb,pc,pan,pbn,pcn);
            arrayF.push_back(auxVf);
        }
    }

    fclose(file);

}

//Método para dibujar en la pantalla una figura
void FaceWindow::drawShape(vector<TypeF> arrayF, vector<TypeV> arrayV, vector<TypeV> arrayVn, int shape){

    int auxv,auxvn;
    //En cada iteración se dibuja un triángulo
    for(int i=0;i<arrayF.size();i++){

        glBegin(GL_TRIANGLES);      //la figura está formada por un conjunto de triángulos de pequeño tamaño

        //si la figura es la cara se le da el color de la piel, si dicha figura es el ojo se le proporciona los colores blanco, azul y negro
        if(shape == 1)
            glColor4f(1.0,0.698039216,0.4,0.3);
        else if(shape == 2){
            //tras observar el fichero obj sabemos que estos son los intervalos en los que se dibujan las tres partes del ojo
            if(i >= 0 && i < 700){
                glColor3f(0,0,0);   //pupila, color negro
            }

            if(i >= 700 && i < 1700){
                glColor3f(0.5,0.5,1);   //iris, color azul
            }

            if(i >= 1700 && i < 9800){
                glColor3f(1,1,1);   //globo ocular, color blanco
            }
        }

        //En cada posición de ArrayF obtenemos el número de la normal que será dibujada
        auxvn = (arrayF[i].getAn() - 1);
        //Una vez tenemos la posición, buscamos el valor en ArrayVn
        glNormal3f(arrayVn[auxvn].getX(),arrayVn[auxvn].getY(),arrayVn[auxvn].getZ());

        //Para cada posición de ArrayF obtenemos el número del vértice que se dibujará (este número viene indicado por el orden en el que aparece en el fichero .obj)
        auxv = (arrayF[i].getA() - 1);
        //Una vez que obtenemos la posición buscamos las coordenadas del vértice en ArrayV
        glVertex3f(arrayV[auxv].getX(),arrayV[auxv].getY(),arrayV[auxv].getZ());

        //En cada posición de ArrayF obtenemos el número de la normal que será dibujada
        auxvn = (arrayF[i].getBn() - 1);
        //Una vez tenemos la posición, buscamos el valor en ArrayVn
        glNormal3f(arrayVn[auxvn].getX(),arrayVn[auxvn].getY(),arrayVn[auxvn].getZ());

        //Repetimos esta operación para los otros dos vértices que forman un triángulo
        auxv = (arrayF[i].getB() - 1);
        glVertex3f(arrayV[auxv].getX(),arrayV[auxv].getY(),arrayV[auxv].getZ());

        //En cada posición de ArrayF obtenemos el número de la normal que será dibujada
        auxvn = (arrayF[i].getCn() - 1);
        //Una vez tenemos la posición, buscamos el valor en ArrayVn
        glNormal3f(arrayVn[auxvn].getX(),arrayVn[auxvn].getY(),arrayVn[auxvn].getZ());

        auxv = (arrayF[i].getC() - 1);
        glVertex3f(arrayV[auxv].getX(),arrayV[auxv].getY(),arrayV[auxv].getZ());

        glEnd();
    }
}

//Establecemos material de la iluminación
void FaceWindow::initRendering(Point3f Light) {

    float lightPosition[] = { 0.0, 0.0, 1.0, 0.0 };
    float matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    float matShininess[] = { 128.0 };

    float lightColor0[] = {0.5f,0.5f,0.5f,1.0f};
    float lightPos0[] = {0.0,0.0,1.0,0.0};

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos0);

    //Activamos los puntos de luz anteriomente establecidos
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    if (rotAnt == 0.0){
       rotAnt = Light.x;
    }

    //Si la rotación del ojo no ha cambiado, se ha pausado la exploración, oscurecemos el rsotro
    if(rotAnt != Light.x){
        rotAnt = Light.x;
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
    }
    else{
        glDisable(GL_LIGHT0);
    }

    glEnable(GL_DEPTH_TEST);
}

//Método para calcular los ángulos de rotación de los ojos
void FaceWindow::calculateEyesPosition(Point3f &leftRot, Point3f &rightRot, Point3f &lightRot, Marker marker){

    Point3D rightEyePos, leftEyePos, lightPos;
    Point2f markerAux;
    Point3f leftTranslation, rightTranslation, lightTranslation;  //Rotación ojos y luz
    float translationCamera;                                      //Distancia correción posición de la cámara

    //Almacenamos la posición (x,y) del tag detectado
    markerAux = marker.getCenter();

    if (not isnan(markerAux.x)){

        //Obtenemos los valores del vector de traslación de la cámara con respecto al ojo izquierdo
        //Aplicamos el desplazamiento del ojo izquierd
        //Posición x del tag sumándole el desplazamiento del ojo izquierdo (0.022) respecto al punto 0,0
        leftTranslation.x = 0.022 + marker.Tvec.at<float>(0,0);
        translationCamera = ((Fl::h()/2) * 0.026458333)/100;    //Distancia existente entre cámara y ojos, pasamos de px a m
        //Posición y del tag restándole el desplazamiento del ojo en el eje y  (0.009) del centro (0,0)
        leftTranslation.y = 0.009 + (-(marker.Tvec.at<float>(1,0))) + translationCamera;
        //Posición z del tag
        leftTranslation.z = marker.Tvec.at<float>(2,0);
        leftEyePos.readPoint3D(leftTranslation.x, leftTranslation.y, leftTranslation.z);

        //Obtenemos los valores del vector de traslación de la cámara con al centro (punto de luz)
        lightTranslation.x = marker.Tvec.at<float>(0,0);
        lightTranslation.y = (-(marker.Tvec.at<float>(1,0)) + translationCamera);
        lightTranslation.z = marker.Tvec.at<float>(2,0);
        lightPos.readPoint3D(lightTranslation.x, lightTranslation.y, lightTranslation.z);

        //Rotación del punto de luz
        if (lightPos.turnAxisX() > 0)
            lightRot.x = (lightPos.turnAxisX() * 2 - translationCamera) * 4;
        else
            lightRot.x = lightPos.turnAxisX() * 2 - translationCamera;

        lightRot.y = lightPos.turnAxisY() * 2;
    }

    //Rotación del ojo izquierdo en el eje x
    switch (leftEyePath){

        case HIPERTROPIA:

            if (leftEyePos.turnAxisX() > 0){
                leftRot.x = (leftEyePos.turnAxisX() * 4) - leftAngle;
            }
            else{
                leftRot.x = leftEyePos.turnAxisX() - leftAngle;
            }

            if (leftRot.x < (-30))
                leftRot.x = -30;

            break;

        case HIPOTROPIA:

            if (leftEyePos.turnAxisX() > 0){
                leftRot.x = (leftEyePos.turnAxisX() * 4) + leftAngle;
            }
            else{
                leftRot.x = leftEyePos.turnAxisX() + leftAngle;
            }

            if (leftRot.x > 30.0)
                leftRot.x = 30.0;

            break;

        default:    //Ojo sin patología que afecta a la rotación con respecto al eje x

            if (leftEyePos.turnAxisX() > 0)
                leftRot.x = leftEyePos.turnAxisX() * 4;
            else
                leftRot.x = leftEyePos.turnAxisX();

            if (leftRot.x > 23.0)   //Límite de rotación al ojo
                leftRot.x = 23.0;
        }

        //Rotación del ojo izquierdo en el eje y
        switch (leftEyePath){

            case ENDOTROPIA:

                //Aplicamos el ángulo de desvío aplicado
                leftRot.y = leftEyePos.turnAxisY() - leftAngle;

                if (leftRot.y < (-47))
                    leftRot.y = -47;    //Límite rotación eje y
                break;

            case EXOTROPIA:
                leftRot.y = leftEyePos.turnAxisY() + leftAngle;

                if (rightRot.y > 52)
                    rightRot.y = 52;

                break;

            //No se le ha aplicado patología que afecta a la rotación con respecto al eje y
            default:
                leftRot.y = leftEyePos.turnAxisY();
        }

        if (not isnan(markerAux.x)){
            //Desplazamiento del tag con respecto a la cámara
            rightTranslation.x = - 0.022 + marker.Tvec.at<float>(0,0);
            rightTranslation.y = 0.009 + (-(marker.Tvec.at<float>(1,0))) + translationCamera;
            rightTranslation.z = marker.Tvec.at<float>(2,0);

            rightEyePos.readPoint3D(rightTranslation.x,rightTranslation.y,rightTranslation.z);

        }

        //Obtenemos la rotación del ojo derecho con respecto al eje x
        switch (rightEyePath){

            case HIPERTROPIA:

                if (rightEyePos.turnAxisX() > 0)
                    rightRot.x = (rightEyePos.turnAxisX() * 4) - rightAngle;

                else
                    rightRot.x = rightEyePos.turnAxisX() - rightAngle;

                  if (rightRot.x < (-30.0))
                    rightRot.x = -30.0;

                break;

            case HIPOTROPIA:

                if(rightEyePos.turnAxisX() > 0)
                    rightRot.x = (rightEyePos.turnAxisX() * 4) + rightAngle;

                else{
                   rightRot.x = rightEyePos.turnAxisX() + rightAngle;
                }

                  if (rightRot.x > 30.0)
                    rightRot.x = 30.0;

                break;

            default:

                if (rightEyePos.turnAxisX() > 0)
                    rightRot.x = rightEyePos.turnAxisX() * 4;
                else
                    rightRot.x = rightEyePos.turnAxisX();

                if (rightRot.x > 23.0)      //Límite de rotación del ojo
                    rightRot.x = 23.0;
        }


        //Obtenemos el ángulo de rotación del ojo derecho con respecto al eje y
        switch (rightEyePath){

            case ENDOTROPIA:
                rightRot.y = rightEyePos.turnAxisY() + rightAngle;

               if (rightRot.y > 47)        //Límite de rotación del ojo
                    rightRot.y = 47;

                break;

            case EXOTROPIA:
                rightRot.y = rightEyePos.turnAxisY() - rightAngle;

                if (rightRot.y < -52)
                    rightRot.y = -52;
                break;

            default:
                rightRot.y = rightEyePos.turnAxisY();
        }

}
