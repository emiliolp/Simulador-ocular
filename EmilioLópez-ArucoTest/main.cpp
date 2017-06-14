#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <aruco/aruco.h>
#include <iostream>
#include "Point3D.hpp"
#include "TypeV.hpp"
#include "TypeF.hpp"
#include <string.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl_Round_Button.H>
#include <Fl/Fl_Check_Button.H>
#include <cstdlib>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include "wtypes.h"
#include <FL/Fl_Pixmap.H>
#include "help_button.hpp"
#include "aruco_calibration_board_a4.h"
#include <conio.h>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>

#define NORMAL      0
#define ENDOTROPIA  1
#define EXOTROPIA   2
#define HIPERTROPIA 3
#define HIPOTROPIA  4

#define TRANSPARENTE 0
#define OPACO        1

#define WIDTH   1195
#define HEIGHT  521
#define HPANEL  100

using namespace cv;
using namespace aruco;
using namespace std;
using namespace aruco_test;

string TheInputVideo;
VideoCapture TheVideoCapturer;
vector<Marker> TheMarkers;      //vector de marcadores encontrados
vector<TypeV> VvLente, VvFace, VvEye, VnFace, VnEye, VnLente;
vector<TypeF> VfLente, VfFace, VfEye;
Mat TheInputImage,TheUndInputImage,TheResizedImage;
CameraParameters TheCameraParams;
bool TheCaptureFlag=true, popupActive = false;
MarkerDetector PPDetector;
Size TheGlWindowSize;
Point3D MarkerPx, MarkerCm;
float TheMarkerSize=-1, rotx1, roty1, rotz1, rotz2, rotx2, roty2, rotx, roty, rotz, rot_ant = 0.0;
int econt=0, CameraPosition, curr_string_iz = 0, curr_string_de = 0;
Marker Marker1;
char cadena[200], buffer[200];
int fill_resize_rostro = 0, eye_test, pat_test, angle_test, oclusor_iz = TRANSPARENTE, oclusor_de = TRANSPARENTE, modo_examen = 0;
Fl_Box *textBox, *fl_boxbox;
Fl_Text_Display *display;
Fl_Window *pop; //*margin_right,
Fl_Window *win_calib;
MarkerMap TheMarkerMapConfig;
aruco::CameraParameters camp;//camera parameters estimated

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 128.0 };
GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
GLfloat rot_angle_y = 0.0;
GLfloat rot_angle_x = 0.0;
GLfloat ROT_INC = 0.1;
Fl_Choice *choice_iz, *choice_de, *choice_ian, *choice_dan, *choice_ojo, *choice_pat, *choice_ean, *choice_oiz, *choice_ode;
Fl_Button *b_paciente, *b_comprobar, *b_help, *b_close, *b_marker, *b_calib, *b_capture_calib, *b_start_calib, *b_finish_calib;
int choice_ian45 = 0, choice_dan45 = 0, angle_iz = 5, angle_de = 5, eye, patologia, angle;
Fl_Check_Button *check_oiz, *check_ode;
Point2f MarkerAnt;
vector < vector<cv::Point2f>  > calib_p2d;
vector < vector<cv::Point3f>  > calib_p3d;

void axis(float size)
{
    glColor3f (1,0,0 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
    glVertex3f(size,0.0f, 0.0f); // ending point of the line
    glEnd( );

    glColor3f ( 0,1,0 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
    glVertex3f( 0.0f,size, 0.0f); // ending point of the line
    glEnd( );


    glColor3f (0,0,1 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
    glVertex3f(0.0f, 0.0f, size); // ending point of the line
    glEnd( );

}

/////////////////////////////////////////////////////////////
// OPENGL WINDOW CLASS
class MyGlWindow : public Fl_Gl_Window {

    // DRAW METHOD
    void draw() {

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
        glRotatef(rotz2,0,0,1);

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
        glRotatef(rotz1,0,0,1);

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
    // HANDLE WINDOW RESIZING
    void resize(int x, int y, int w,int h) {

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
    static void Timer_CB(void *userdata) {

        MyGlWindow *pb = (MyGlWindow*)userdata;
        pb->redraw();
        Fl::repeat_timeout(1.0/5000.0, Timer_CB, userdata);

    }

    //Método para leer el contenido de un fichero .obj
    void loadFile(char *filename, vector<TypeF> &ArrayF, vector<TypeV> &ArrayV, vector<TypeV> &ArrayVn){

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
    void drawShape(vector<TypeF> ArrayF, vector<TypeV> ArrayV, vector<TypeV> ArrayVn, int shape){

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

void initRendering() {

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

public:
    // OPENGL WINDOW CONSTRUCTOR
    MyGlWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {

        //Cargamos las figuras que forman el modelo 3D
        loadFile("figuras/cara.obj", VfFace, VvFace, VnFace);
        loadFile("figuras/ojo.obj", VfEye, VvEye, VnEye);
        loadFile("figuras/oclusor.obj", VfLente, VvLente, VnLente);

        Fl::add_timeout(1.0/5000.0, Timer_CB, (void*)this);       // 24fps timer
        end();
    }
};

/////////////////////////////////////////////////
// CLASE PARA EL VISOR DE LA CÁMARA
class MyGlWindowCam : public Fl_Gl_Window {

    double fg;                       //brillo del primer plano
    double bg;                       //brillo de fondo

    // DRAW METHOD
    void draw() {

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

        }

    }
    // HANDLE WINDOW RESIZING
     void resize(int x, int y, int w,int h) {

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

    static void Timer_CB(void *userdata) {
        MyGlWindowCam *pb = (MyGlWindowCam*)userdata;
        pb->redraw();
        Fl::repeat_timeout(1.0/5000.0, Timer_CB, userdata);
    }

    void vIdle()
    {

        if (TheCaptureFlag) {

            TheVideoCapturer.grab();
            TheVideoCapturer.retrieve( TheInputImage);

            //Validamos que se haya capturado una imagen de la cámara
            if(!TheInputImage.empty()){

                TheUndInputImage.create(TheInputImage.size(),CV_8UC3);
                cv::cvtColor(TheInputImage,TheInputImage,CV_BGR2RGB);

                MarkerDetector::Params params;
                //play with this paramteres if the detection does not work correctly
                params._borderDistThres=.01;//acept markers near the borders
                params._thresParam1=5;
                params._thresParam1_range=5;//search in wide range of values for param1
                params._cornerMethod=MarkerDetector::SUBPIX;//use subpixel corner refinement
                params._subpix_wsize= (15./2000.)*float(TheInputImage.cols) ;//search corner subpix in a 5x5 widow area
                PPDetector.setParams(params);//set the params above
                PPDetector.setDictionary("ARUCO_MIP_36h12");
                ///PPDetector.setDictionary("ARUCO");

                TheMarkers=PPDetector.detect(TheInputImage);
                //vector<Point2f> corners;
                //vector<int> ids;

                cv::resize(TheInputImage,TheResizedImage,TheGlWindowSize);
                strcpy(buffer,cadena);

            }
        }
    }

    //Método para seleccionar un tag
    void detectMarker(){

        Point2f maux;
        float v[50], menor;
        int indice;
        Point2f TheMarkerAux;
        Point2f MarkerAnt, PointMarkerAnt;

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
            Marker1.calculateExtrinsics(0.043,TheCameraParams);

    }

    void calculateEyesPosition(Point2f MarkerC){

        Point3D MarkerEye1, MarkerEye2, MarkerEye;
        Point2f MarkerAux;
        float rot_estrabismo_y, rot_estrabismo_x, angle, desy_camera;

        //Almacenamos la posición (x,y) del tag detectado
        MarkerAux = Marker1.getCenter();
        MarkerPx.readPoint3D(MarkerAux.x,MarkerAux.y,0);

        if (not isnan(MarkerAux.x)){

            MarkerPx.obtenerz(Marker1.getPerimeter());
            //Convertimos la coordenada z a px
            MarkerPx.convertToPixel();
            //Modificamos los valores de las coordenadas del punto, el punto 0,0 se sitúa ahora en el centro de la pantalla
            MarkerPx.translateCentre();
            //Convertimos las coordenadas del punto a cm, este objeto solo se utilizará para visualizar el punto en la pantalla
            //La conversión se realiza sabiendo que el punto (320, 240)px es equivalente a (0.7179, 0.5384)cm
            MarkerCm.setX(((MarkerPx.getX() * 0.62)/320) + 0.05);
            MarkerCm.setY(((MarkerPx.getY() * 0.45)/240) - 0.41);

            //Obtenemos los valores del vector de traslación de la cámara con respecto al ojo izquierdo
            //Aplicamos el desplazamiento del ojo izquierdo
            float desx2 = Marker1.Tvec.at<float>(0,0) + 0.022;
            desy_camera = ((Fl::h()/3) * 0.026458333)/100;                      //Conversión píxeles a m
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
                rotx = (MarkerEye.turnAxisX() * 2 - desy_camera) * 4;
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
                rotx2 = (MarkerEye2.turnAxisX() - rot_estrabismo_x) * 4;
            }
            else{

                angle = atan2(angle_iz/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                rotx2 = MarkerEye2.turnAxisX() - rot_estrabismo_x;
            }
            if (rotx2 < (-25.5))
                    rotx2 = -25.5;
        }

        else if ( curr_string_iz == HIPOTROPIA){

            if (MarkerEye2.turnAxisX() > 0){

                angle = atan2(angle_iz/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                rotx2 = (MarkerEye2.turnAxisX() + rot_estrabismo_x) * 4;
            }
            else{
                angle = atan2(angle_iz/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                rotx2 = MarkerEye2.turnAxisX() + rot_estrabismo_x;
            }
        if (rotx2 > 23.0)
                rotx2 = 23.0;
        }

        else{

            if (MarkerEye2.turnAxisX() > 0)
                rotx2 = MarkerEye2.turnAxisX() * 4;
            else
                rotx2 = MarkerEye2.turnAxisX();

            if (rotx2 > 23.0)
                rotx2 = 23.0;
        }

        //Rotación del ojo en el eje y
        if (curr_string_iz == ENDOTROPIA){

            angle = atan2(angle_iz/7, 12.5);
            rot_estrabismo_y = (angle * 180) / 3.141618;
            roty2 = MarkerEye2.turnAxisY() - rot_estrabismo_y;

            if (roty2 < (-47))
            roty2 = -47;    //Límite rotación eje y
        }
        else if(curr_string_iz == EXOTROPIA){
            angle = atan2(angle_iz/7, 12.5);
            rot_estrabismo_y = (angle * 180) / 3.141618;
            roty2 = MarkerEye2.turnAxisY() + rot_estrabismo_y;
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
                rotx1 = (MarkerEye1.turnAxisX() - rot_estrabismo_x) * 4;
            }
            else{
                angle = atan2(angle_de/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                rotx1 = MarkerEye1.turnAxisX() - rot_estrabismo_x;
            }
            if (rotx1 < (-25.5))
                rotx1 = -25.5;

        }
        else if(curr_string_de == HIPOTROPIA){

            if(MarkerEye2.turnAxisX() > 0){
                angle = atan2(angle_de/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                rotx1 = (MarkerEye1.turnAxisX() + rot_estrabismo_x)*4;
            }
            else{
                angle = atan2(angle_de/7, 12.5);
                rot_estrabismo_x = (angle * 180) / 3.141618;
                rotx1 = MarkerEye1.turnAxisX() + rot_estrabismo_x;
                if (rotx2 > 23.0)
                    rotx2 = 23.0;
            }
        }
        else{

            if (MarkerEye1.turnAxisX() > 0)
               rotx1 = MarkerEye1.turnAxisX() * 4;
            else
                rotx1 = MarkerEye1.turnAxisX();

            if (rotx1 > 23.0)
                rotx1 = 23.0;
        }



        if (curr_string_de == ENDOTROPIA){

            angle = atan2(angle_de/7, 12.5);
            rot_estrabismo_y = (angle * 180) / 3.141618;
            roty1 = MarkerEye1.turnAxisY() + rot_estrabismo_y;

               if (roty1 < -47)
                roty1 = -47;
        }
        else if (curr_string_de == EXOTROPIA){
            angle = atan2(angle_de/7, 12.5);
            rot_estrabismo_y = (angle * 180) / 3.141618;
            roty1 = MarkerEye1.turnAxisY() - rot_estrabismo_y;
        }
        else
            roty1 = MarkerEye1.turnAxisY();

}

public:
    // OPENGL WINDOW CONSTRUCTOR
    MyGlWindowCam(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {
        fg = 1.0;
        bg = 0.0;
        Fl::add_timeout(1.0/5000.0, Timer_CB, (void*)this);       // 24fps timer
        end();
    }
    void SetBrightness(double new_fg, double new_bg)
        { fg = new_fg; bg = new_bg; redraw(); }
};

//CLASE CALIBRADOR
class WinCalibrate : public Fl_Window{

    Fl_Window *win_calibrate;

    private:

        static void getMarker2d_3d(vector<cv::Point2f> &p2d, vector<cv::Point3f> &p3d, const MarkerMap &bc){

            p2d.clear();
            p3d.clear();
            int fidx=-1;
            for(size_t j=0;j<bc.size() &&fidx==-1;j++)
                if (bc[j].id==Marker1.id ) fidx=j;
            if (fidx!=-1){
                for(int j=0;j<4;j++){
                    p2d.push_back(Marker1[j]);
                    p3d.push_back(bc[fidx][j]);
                }
            }
        }

        static void Capture(Fl_Widget *w, void *d){

            vector<cv::Point2f> p2d;
            vector<cv::Point3f> p3d;

            if (Marker1.empty() == 0){

                stringstream sstr;
                sstr.write((char*)default_a4_board,default_a4_board_size);
                TheMarkerMapConfig.fromStream(sstr);

                WinCalibrate::getMarker2d_3d(p2d,p3d,TheMarkerMapConfig);
                if(p3d.size()>0){
                    calib_p2d.push_back(p2d);
                    calib_p3d.push_back(p3d);
                }

                if (calib_p3d.size() > 3)
                    b_finish_calib->activate();

            }

        }

        static void FinishCalibrate(Fl_Widget *w, void *d){

            vector<cv::Mat> vr,vt;
            camp.CamSize=TheInputImage.size();
            cv::calibrateCamera(calib_p3d,calib_p2d,TheInputImage.size(),camp.CameraMatrix,camp.Distorsion,vr,vt);
            camp.saveToFile("camera.yml");
            TheCameraParams.readFromXMLFile("camera.yml");
            win_calib->hide();

        }

    public:

        WinCalibrate(int X,int Y,int W,int H,const char*L=0) : Fl_Window(X,Y,W,H,L) {
        //this->user_data(this);
        b_capture_calib = new Fl_Button(200,358,140,22,"Capturar");
        b_capture_calib->callback(WinCalibrate::Capture, (void*)this);
        b_finish_calib = new Fl_Button(455,358, 140,22, "Terminar");
        b_finish_calib->callback(WinCalibrate::FinishCalibrate, (void*)this);
        b_finish_calib->deactivate();
//        //Test display superior
        Fl_Text_Display::Style_Table_Entry stable[] = {
         // FONT COLOR      FONT FACE   FONT SIZE
         // --------------- ----------- --------------
            {  FL_BLACK, 0, 24 }, // A - Red
            {  FL_BLACK, 0, 18 }, // B - Yellow
            {  FL_BLACK, 0, 14 }, // C - Green
            //{  FL_BLACK, 0, 14 }, // D - Blue
        };
        Fl_Text_Display *disp = new Fl_Text_Display(20, 10, 710, 138, "");
        disp->box(FL_NO_BOX);
        Fl_Text_Buffer *tbuff = new Fl_Text_Buffer();      // text buffer
        Fl_Text_Buffer *sbuff = new Fl_Text_Buffer();      // style buffer
        disp->buffer(tbuff);
        int stable_size = sizeof(stable)/sizeof(stable[0]);
        disp->highlight_data(sbuff, stable, stable_size, 'A', 0, 0);
        // Text
        tbuff->text("Siga las siguientes instrucciones para calibrar la cámara:\n(sólo es necesario realizarlo una vez)\n1-Reproduzca cada una de las siguientes posturas del tag frente a la cámara, por cada vista representada \ndebe presionar el botón Capturar para almacenar la imagen:\n");
                 //\n2-Presione el botón Capturar por cada postura representada\n3-Al finalizar con las capturas presione el botón Terminar\n");
        // Style for text
        sbuff->text("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");

        Fl_Text_Display *disp2 = new Fl_Text_Display(20, 300, 710, 60, "");
        disp2->box(FL_NO_BOX);
        Fl_Text_Buffer *tbuff2 = new Fl_Text_Buffer();      // text buffer
        Fl_Text_Buffer *sbuff2 = new Fl_Text_Buffer();      // style buffer
        disp2->buffer(tbuff2);
        int stable_size2 = sizeof(stable)/sizeof(stable[0]);
        disp2->highlight_data(sbuff2, stable, stable_size2, 'A', 0, 0);
        // Text
        tbuff2->text("2-Tras concluir con la captura de vistas, presione el botón Terminar para finalizar la calibración.\n");
                 //\n2-Presione el botón Capturar por cada postura representada\n3-Al finalizar con las capturas presione el botón Terminar\n");
        // Style for text
        sbuff2->text("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");

    }

};


class PopupDialog : public Fl_Window {

    Fl_Window *popup;

private:

    static int showCalibrate(){

    win_calib = new WinCalibrate((Fl::w()/2)-400,(Fl::h()/2)-200,800,400,"Instrucciones de calibración");
    win_calib->resizable(pop);
    win_calib->color(Fl_Color(7));
                      // initialize image lib
    Fl_Box        box(390,-30,14,500);     // widget that will contain image
    Fl_JPEG_Image jpg("calibra.jpg");      // load jpeg image into ram
    box.image(jpg);
    win_calib->show();

    return(Fl::run());

}

    static void Close(Fl_Widget*w, void *d){
        pop->hide();
        popupActive = false;
        if (!TheCameraParams.isValid())
            fl_message("Es necesario realizar las tareas de calibración\nantes de empezar a utilizar el simulador","Aceptar",0,0);
    }

    static void ShowMarker(Fl_Widget*w, void *d){
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

    static void CallCalibrate(Fl_Widget*w, void *d){

        showCalibrate();

    }


public:
    // APP WINDOW CONSTRUCTOR
    PopupDialog(int X,int Y,int W,int H,const char*L=0) : Fl_Window(X,Y,W,H,L) {

        Fl_Button *b_close = new Fl_Button(130,(Fl::h()/2)-197,140,22,"Cerrar");
        b_close->callback(PopupDialog::Close, (void*)this);

        Fl_Button *b_marker = new Fl_Button(341,61,50,22,"tag");
        b_marker->callback(PopupDialog::ShowMarker, (void*)this);
        b_marker->box(FL_NO_BOX);
        b_marker->labelcolor(FL_BLUE);

        Fl_Button *b_calib = new Fl_Button(341,88,100,22,"calibración");
        b_calib->callback(PopupDialog::CallCalibrate, (void*)this);
        b_calib->box(FL_NO_BOX);
        b_calib->labelcolor(FL_BLUE);

        Fl_Text_Display::Style_Table_Entry stable[] = {
         // FONT COLOR      FONT FACE   FONT SIZE
         // --------------- ----------- --------------
            {  FL_BLACK,         0, 24 }, // A - Red
            {  FL_BLACK, 0, 14 }, // B - Yellow
            {  FL_BLACK,  0, 14 }, // C - Green
            {  FL_BLACK,        0, 14 }, // D - Blue
        };
        Fl_Text_Display *disp = new Fl_Text_Display(20, 25, 336, 165, "");
        disp->box(FL_NO_BOX);
        Fl_Text_Buffer *tbuff = new Fl_Text_Buffer();      // text buffer
        Fl_Text_Buffer *sbuff = new Fl_Text_Buffer();      // style buffer
        disp->buffer(tbuff);
        int stable_size = sizeof(stable)/sizeof(stable[0]);
        disp->highlight_data(sbuff, stable, stable_size, 'A', 0, 0);
        // Text
        tbuff->text("Guía de uso:\n1-Imprimir la imagen  recortable adjunta y montar el\n2-Completar la configuración de cámara mediante la\n3-Seleccionar modo de simulación\n4-Mover el tag delante de la cámara\n");
        // Style for text
        sbuff->text("AAAAAAAAAAAA\nBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\nCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\nDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\nDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");

        fl_boxbox = new Fl_Box(130,(Fl::h()/2)-80,300,62,"");

    }
};

// APP WINDOW CLASS
class MyAppWindow : public Fl_Window {
    MyGlWindow *mygl;                    // opengl window
    MyGlWindowCam *mygl_cam;
    Fl_Window *mywindow;

    public:
    static int callPopup(){

    pop = new PopupDialog((Fl::w()/2)-220,(Fl::h()/2)-200,440,230,"Ayuda");
    pop->resizable(pop);
    pop->clear_border();
    pop->color(Fl_Color(7));
    pop->show();

    return(Fl::run());

}

private:
    // Someone changed one of the sliders
    static void Choice_Exploracion(Fl_Widget*w, void *d) {

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

        textBox->label("");

    }
    static void Choice_Examen(Fl_Widget*w, void *d){

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
    static void Choice_iz(Fl_Widget*w, void *d) {

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

  static void Choice_de(Fl_Widget*w,void *d){

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

  static void Angle_iz(Fl_Widget*w,void *d){

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

  static void Angle_de(Fl_Widget*w,void *d){

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

  static void Gen_paciente(Fl_Widget*w, void *d){

    b_comprobar->activate();

    eye = rand() % 2;
    patologia = rand() % 4 + 1;
    textBox->label("");

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

  static void Choice_eye(Fl_Widget *w, void *d){

    Fl_Choice *ch = (Fl_Choice*)w;

    if(strcmp(ch->text(), "Ojo izquierdo") == 0)
        eye_test = 0;

    else if (strcmp(ch->text(), "Ojo derecho") == 0)
        eye_test = 1;

  }

  static void Choice_pat(Fl_Widget *w, void *d){

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

  static void Choice_angle(Fl_Widget *w, void *d){

    Fl_Choice *ch = (Fl_Choice*)w;

    if (strcmp(ch->text(), "15º") == 0)
        angle_test = 0;

    else if (strcmp(ch->text(), "30º") == 0)
        angle_test = 1;

    else if (strcmp(ch->text(), "45º") == 0)
        angle_test = 2;
  }

    static void Check_answer(Fl_Widget*w, void *d){

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

    static void Check_oclusor_izquierdo(Fl_Widget*w, void *d){


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

    static void Check_oclusor_derecho(Fl_Widget*w, void *d){

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

    static void Oclusor_iz(Fl_Widget*w, void *d){

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

    static void Oclusor_de(Fl_Widget*w, void *d){

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

    static void Show_help(Fl_Widget*w, void *d){

        popupActive = true;
        callPopup();

    }

public:
    // APP WINDOW CONSTRUCTOR
    MyAppWindow(int W,int H,const char*L=0) : Fl_Window(W,H,L) {

        mygl     = new MyGlWindow(300, 0, w()-600, h()-120);
        mygl_cam = new MyGlWindowCam(1366-300,0, 300, 187.5);

        Fl_Group *o = new Fl_Group(10,672,800,25);
        {
            Fl_Round_Button* o = new Fl_Round_Button(32, 672, 113, 25, "Exploración");
            o->type(102);
            o->down_box(FL_ROUND_DOWN_BOX);
            o->setonly();
            o->callback(MyAppWindow::Choice_Exploracion, (void*)this);

        }

        {
            Fl_Round_Button* o = new Fl_Round_Button(680, 672, 113, 25, "Examen");
            o->type(102);
            o->down_box(FL_ROUND_DOWN_BOX);
            o->callback(MyAppWindow::Choice_Examen, (void*)this);
        }
        o->end();

        choice_iz = new Fl_Choice(137,698,140,22,"Ojo izquierdo:");
        choice_iz->add("Sin patología");
        choice_iz->add("Endotropia");
        choice_iz->add("Exotropia");
        choice_iz->add("Hipertropia");
        choice_iz->add("Hipotropia");
        choice_iz->callback(MyAppWindow::Choice_iz, (void*)o);
        choice_iz->value(0);
        choice_iz->color(FL_BLUE);

        choice_ian = new Fl_Choice(360,698,70,22,"Ángulo:");
        choice_ian->add("15º");
        choice_ian->add("30º");
        choice_ian45 = choice_ian->add("45º");
        choice_ian->value(0);
        choice_ian->callback(MyAppWindow::Angle_iz, (void*)o);
        choice_ian->deactivate();
        choice_ian->color(FL_BLUE);

        check_oiz = new Fl_Check_Button(450, 698, 73, 22, "Oclusor:");
        check_oiz->callback(MyAppWindow::Check_oclusor_izquierdo, (void*)o);
        choice_oiz = new Fl_Choice(525, 698, 113, 22, "");
        choice_oiz->add("Transparente");
        choice_oiz->add("Opaco");
        choice_oiz->value(0);
        choice_oiz->callback(MyAppWindow::Oclusor_iz, (void*)o);
        choice_oiz->deactivate();
        choice_oiz->color(FL_BLUE);

        choice_de = new Fl_Choice(137,728,140,22,"Ojo derecho: ");
        choice_de->add("Sin patología");
        choice_de->add("Endotropia");
        choice_de->add("Exotropia");
        choice_de->add("Hipertropia");
        choice_de->add("Hipotropia");
        choice_de->callback(MyAppWindow::Choice_de, (void*)o);
        choice_de->value(0);
        choice_de->color(FL_BLUE);

        choice_dan = new Fl_Choice(360,728,70,22,"Ángulo:");
        choice_dan->add("15º");
        choice_dan->add("30º");
        choice_dan45 = choice_dan->add("45º");
        choice_dan->value(0);
        choice_dan->callback(MyAppWindow::Angle_de, (void*)o);
        choice_dan->deactivate();
        choice_dan->color(FL_BLUE);

        check_ode = new Fl_Check_Button(450, 728, 73, 22, "Oclusor");
        check_ode->callback(MyAppWindow::Check_oclusor_derecho, (void*)o);

        choice_ode = new Fl_Choice(525, 728, 113, 22, "");
        choice_ode->add("Transparente");
        choice_ode->add("Opaco");
        choice_ode->value(0);
        choice_ode->callback(MyAppWindow::Oclusor_de, (void*)o);
        choice_ode->deactivate();
        choice_ode->color(FL_BLUE);

        textBox = new Fl_Box(1075, 728, 200, 20);
        textBox->labelsize(13);

        //Modo examen
        choice_ojo = new Fl_Choice(775,698,140,22,"Diagnóstico:");
        choice_ojo->add("Ojo izquierdo");
        choice_ojo->add("Ojo derecho");
        choice_ojo->value(0);
        choice_ojo->deactivate();
        choice_ojo->callback(MyAppWindow::Choice_eye, (void*)o);
        choice_ojo->color(FL_BLUE);

        choice_pat = new Fl_Choice(925, 698, 140, 22,"");
        choice_pat->add("Endotropia");
        choice_pat->add("Exotropia");
        choice_pat->add("Hipertropia");
        choice_pat->add("Hipotropia");
        choice_pat->value(0);
        choice_pat->callback(MyAppWindow::Choice_pat, (void*)o);
        choice_pat->deactivate();
        choice_pat->color(FL_BLUE);

        choice_ean = new Fl_Choice(1077, 698, 140,22,"");
        choice_ean->add("15º");
        choice_ean->add("30º");
        choice_ean->add("45º");
        choice_ean->value(0);
        choice_ean->callback(MyAppWindow::Choice_angle, (void*)o);
        choice_ean->deactivate();
        choice_ean->color(FL_BLUE);

        b_paciente = new Fl_Button(685,728,140,22,"Generar paciente");
        b_paciente->color(FL_BLUE);
        b_paciente->labelcolor(FL_WHITE);
        b_paciente->callback(MyAppWindow::Gen_paciente,(void*)o);
        b_paciente->deactivate();

        b_comprobar = new Fl_Button(875, 728,140,22,"Comprobar");
        b_comprobar->color(FL_BLUE);
        b_comprobar->labelcolor(FL_WHITE);
        b_comprobar->callback(MyAppWindow::Check_answer, (void*)o);
        b_comprobar->deactivate();

        b_help = new Fl_Button(1229, 698, 52, 52, "");
        b_help->color(FL_BLUE);
        b_help->callback(MyAppWindow::Show_help, (int*)o);
        static Fl_Pixmap G_cat(cat_xpm);
        b_help->image(G_cat);

        Fl_Window *margin_left = new Fl_Window(0,0,400,h()-120);
        margin_left->color(FL_BLACK);
        margin_left->end();
        Fl_Window *margin_right = new Fl_Window(1366-400, 187.5, 400, h()-(187+120));
        margin_right->color(FL_BLACK);
        margin_right->end();
    }

};

// MAIN
int main() {
    int horizontal = 0;
   int vertical = 0;

   MyAppWindow win(1366, 768, "Simulador motilidad ocular");
    win.resizable(win);
    win.color(Fl_Color(236));
    win.show();

    try{
        CameraPosition   = 1;
        TheInputVideo = "live";
        if(TheInputVideo=="live"){

            cout << "Choosed camera live" <<endl;
            TheVideoCapturer.open(0);

            if(TheVideoCapturer.open(0) != 1)
                cout << "Video not opened" << endl;

        }
        else{

            cout << "Capture image" << endl;
            TheVideoCapturer.open(TheInputVideo);

        }
        if(!TheVideoCapturer.isOpened()){

            cerr<<"Could not open video"<<endl;
            return -1;

        }

        //read first image
        cout << "Reading first image" << endl;
        TheVideoCapturer>>TheInputImage;
        //read camera paramters if passed
        cout << "Reading camera parameters" << endl;

        //Cargamos las coordenadas de los puntos que forman la cara en el vector para después dibujarla
        cout << "Loading face" << endl;
        //loadFaceFile();
        //Cargamos las coordenadas del ojo
        cout << "Loading eyes" << endl;
        //loadEyeFile();

        cout<< "Loading oclusor"<<endl;
        //loadLenteFile();

        try{
            TheCameraParams.readFromXMLFile("camera.yml");
        }
        catch (std::exception &ex){
            win.callPopup();
        }

    }
    catch (std::exception &ex)
    {
        cout<<"Exception:"<<ex.what()<<endl;
    }
    return(Fl::run());
}
