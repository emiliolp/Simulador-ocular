#include "FaceWindow.hpp"

using namespace aruco_test;

void FaceWindow::draw() {

        GLfloat mat_transparent[] = { 0.75, 0.75, 0.0, 1.0 };
        GLfloat mat_zero[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat mat_solid[] = { 1.0, 1, 1, 1.0 };
        GLfloat mat_emission[] = { 1.0, 1, 1, 0.6 };

        if (fill_resize_rostro == 0){
            resize(Fl_Gl_Window::x(), Fl_Gl_Window::y(), Fl_Gl_Window::w(), Fl_Gl_Window::h());
            fill_resize_rostro = 1;
        }

        //Inicializamos los valores de rotación si la configuración de la cámara no es válida
        if (!TheCameraParams.isValid() || popupActive == true){

            rotx1 = 0.0;
            roty1 = 0.0;
            rotz1 = 0.0;
            rotx2 = 0.0;
            roty2 = 0.0;
            rotz2 = 0.0;
            rotx  = 0.0;
            roty  = 0.0;
            rotz  = 0.0;
        }

        //Establecemos los puntos de luz
        initRendering();
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE); //Se normalizan automáticamente las normales
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);

        //Mostramos los puntos de luz establecidos
        glPushMatrix();
        glRotatef(roty, 0,1,0);
        glRotatef(rotx, 1,0,0);
        glRotatef(rotz, 0,0,1);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glTranslatef(light_position[0],light_position[1],light_position[2]);
        glPopMatrix();
        glPushMatrix();

        //Dibuja en la pantalla el rostro
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_solid);
        glLoadIdentity();
        glTranslatef(0.0f,-0.3f,-1.0f);
        glScalef(0.25f,0.25f,0.25f);
        drawShape(VfFace,VvFace,VnFace,1);
        glPopMatrix();

        //Dibuja en la pantalla el ojo izquierdo
        glPushMatrix();
        glLoadIdentity();
        //glTranslatef(2.91*0.145f,1.041*0.165f,-1.8f);
        glTranslatef(0.42195f, 0.171765f, -1.8f);
        glScalef(0.25f,0.25f,0.25f);

        //Rotamos ojo izquierdo
        glRotatef(rotx2,1,0,0);
        glRotatef(roty2,0,1,0);
        ///glRotatef(rotz2,0,0,1);

        //Dibuja la forma
        drawShape(VfEye,VvEye,VnEye,2);
        glPopMatrix();

        //Dibuja en la pantalla el ojo derecho
        glPushMatrix();
        glLoadIdentity();
        //glTranslatef(2.91*-0.145f,1.041*0.165f,-1.8f);
        glTranslatef(-0.42195f, 0.171765, -1.8);
        glScalef(0.25f,0.25f,0.25f);

        //Rotamos ojo derecho
        glRotatef(rotx1,1,0,0);
        glRotatef(roty1,0,1,0);
        ///glRotatef(rotz1,0,0,1);

        //Dibuja la forma
        drawShape(VfEye,VvEye,VnEye,2);
        glPopMatrix();

        //Dibuja Oclusor izquierdo si se ha marcado la opción y está en modo exploración
        if (check_oiz->value() == 1 && modo_examen == 0){

            glPushMatrix();
            glLoadIdentity();
            //glTranslatef(2.91*0.145f,1.041*0.165f,-1.0f);
            glTranslatef(0.42195f,0.171765f,-1.0f);

            if (oclusor_iz == TRANSPARENTE){

                glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent);
                glEnable (GL_BLEND);
                glDepthMask (GL_FALSE);
                glBlendFunc (GL_SRC_ALPHA, GL_ONE);

            }
            else if (oclusor_iz == OPACO){

                glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_solid);
            }

            glScalef(0.0075f,0.0075f,0.0075f);
            drawShape(VfLente,VvLente,VnLente,3);

            if (oclusor_iz == TRANSPARENTE){
                glDepthMask (GL_TRUE);
                glDisable (GL_BLEND);
            }

            glPopMatrix();

        }

        //Dibujamos el oclusor derecho si se encuentra en modo exploración
        if (check_ode->value() == 1 && modo_examen == 0){

            glPushMatrix();

            glLoadIdentity();
            //glTranslatef(2.91*-0.145f,1.041*0.165f,-1.0f);
            glTranslatef(-0.42195f,0.171765f,-1.0f);

            if (oclusor_de == TRANSPARENTE){

                glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent);
                glEnable (GL_BLEND);
                glDepthMask (GL_FALSE);
                glBlendFunc (GL_SRC_ALPHA, GL_ONE);

            }
            else if (oclusor_de == OPACO){

                glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_solid);

            }

            glScalef(0.0075f,0.0075f,0.0075f);
            drawShape(VfLente,VvLente,VnLente,3);

            if (oclusor_de == TRANSPARENTE){
                glDepthMask (GL_TRUE);
                glDisable (GL_BLEND);
            }

            glPopMatrix();

        }

    }

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
    void FaceWindow::Timer_CB(void *userdata) {

        FaceWindow *pb = (FaceWindow*)userdata;
        pb->redraw();
        Fl::repeat_timeout(1.0/5000.0, Timer_CB, userdata);

    }

    //Método para leer el contenido de un fichero .obj
    void FaceWindow::loadFile(char *filename, vector<TypeF> &ArrayF, vector<TypeV> &ArrayV, vector<TypeV> &ArrayVn){

        FILE *file;
        int d;
        TypeV AuxV, AuxVn;
        TypeF AuxVf;
        char lineHeader[128];
        int x=0,j=0,k=0;
        float px,py,pz;
        int pa,pb,pc,pan,pbn,pcn;

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
                AuxV.readTypeV(px,py,pz);
                ArrayV.push_back(AuxV);
            }

            //Obtenemos las normales
            else if(strcmp(lineHeader,"vn") == 0){
                fscanf(file,"%f %f %f",&px,&py,&pz);
                AuxVn.readTypeV(px,py,pz);
                ArrayVn.push_back(AuxVn);
            }

            //La posición en el espacio de los vértices anteriores
            else if(strcmp(lineHeader,"f") == 0){
                fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d",&pa,&d,&pan,&pb,&d,&pbn,&pc,&d,&pcn);
                AuxVf.readTypeF(pa,pb,pc,pan,pbn,pcn);
                ArrayF.push_back(AuxVf);
            }
        }

        fclose(file);

    }

    //Método para dibujar en la pantalla una figura
    void FaceWindow::drawShape(vector<TypeF> ArrayF, vector<TypeV> ArrayV, vector<TypeV> ArrayVn, int shape){

        int auxv,auxvn;
        //En cada iteración se dibuja un triángulo
        for(int i=0;i<ArrayF.size();i++){

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
        auxvn = (ArrayF[i].getAn() - 1);
        //Una vez tenemos la posición, buscamos el valor en ArrayVn
        glNormal3f(ArrayVn[auxvn].getX(),ArrayVn[auxvn].getY(),ArrayVn[auxvn].getZ());

        //Para cada posición de ArrayF obtenemos el número del vértice que se dibujará (este número viene indicado por el orden en el que aparece en el fichero .obj)
        auxv = (ArrayF[i].getA() - 1);
        //Una vez que obtenemos la posición buscamos las coordenadas del vértice en ArrayV
        glVertex3f(ArrayV[auxv].getX(),ArrayV[auxv].getY(),ArrayV[auxv].getZ());

        //En cada posición de ArrayF obtenemos el número de la normal que será dibujada
        auxvn = (ArrayF[i].getBn() - 1);
        //Una vez tenemos la posición, buscamos el valor en ArrayVn
        glNormal3f(ArrayVn[auxvn].getX(),ArrayVn[auxvn].getY(),ArrayVn[auxvn].getZ());

        //Repetimos esta operación para los otros dos vértices que forman un triángulo
        auxv = (ArrayF[i].getB() - 1);
        glVertex3f(ArrayV[auxv].getX(),ArrayV[auxv].getY(),ArrayV[auxv].getZ());

        //En cada posición de ArrayF obtenemos el número de la normal que será dibujada
        auxvn = (ArrayF[i].getCn() - 1);
        //Una vez tenemos la posición, buscamos el valor en ArrayVn
        glNormal3f(ArrayVn[auxvn].getX(),ArrayVn[auxvn].getY(),ArrayVn[auxvn].getZ());

        auxv = (ArrayF[i].getC() - 1);
        glVertex3f(ArrayV[auxv].getX(),ArrayV[auxv].getY(),ArrayV[auxv].getZ());

        glEnd();
    }
}

void FaceWindow::initRendering() {

    GLfloat lightColor0[] = {0.5f,0.5f,0.5f,1.0f};
    GLfloat lightPos0[] = {0.0,0.0,1.0,0.0};

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    if (rot_ant == 0.0)
       rot_ant = rotx2;

    if(rot_ant != rotx2){
        rot_ant = rotx2;
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
    }
    else
        glDisable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);
}
