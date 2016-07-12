#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <aruco/aruco.h>
#include <iostream>
#include "Point3D.hpp"
#include "TypeV.hpp"
#include "TypeF.hpp"
#include <string.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <stdlib.h>

using namespace cv;
using namespace aruco;
using namespace std;
using namespace aruco_test;

int   main_window;
string TheInputVideo;
string TheIntrinsicFile;    //fichero .yml
VideoCapture TheVideoCapturer;
vector<Marker> TheMarkers;      //vector de marcadores encontrados
TypeV *VvFace,*VvEye,*VnFace,*VnEye;    //vectores con los vértices y la normal de las figuras
TypeF *VfFace,*VfEye;                   //vectores con la posición de los vértices que forman la figura
Mat TheInputImage,TheUndInputImage,TheResizedImage;
CameraParameters TheCameraParams;
bool TheCaptureFlag=true;
MarkerDetector PPDetector;
Size TheGlWindowSize;
Point3D MarkerPx;
float TheMarkerSize=-1, rotx1, roty1, rotz1,rotz2, rotx2,roty2;
float _angle = -70.0f,vX,vY,vZ,width,height;
int econt=0, fcount1=0,fcount2=0, CameraPosition;
GLUI *glui;
Marker Marker1;
char *cadena="OK", buffer[200];


void loadFaceFile(){
    FILE *file;
    int d, vcount1=0,vncount1=0;
    //fichero obj del que obtenemos las coordenadas de la cabeza
    file = fopen("figuras/cara.obj","r");

    if(file == NULL){
        cerr<<"Error: Can not open face file"<<endl;
        exit(0);
    }

    char lineHeader[128];
    int res = fscanf(file,"%s",lineHeader);
    //cout<<"lineHeader: "<<lineHeader<<endl;
    while(1){
        //leemos el fichero línea por línea hasta que llega al final
        res=fscanf(file,"%s",lineHeader);
        if(res == EOF){
            break;
        }
        //contamos el número de f, v y vn (los parámetros que nos interesan)
        if(strcmp(lineHeader,"f") == 0){
            fcount1++;
        }
        else if(strcmp(lineHeader,"v") == 0){
            vcount1++;
        }
        else if(strcmp(lineHeader,"vn") == 0){
            vncount1++;
        }
    }
    fclose(file);

    VvFace = new TypeV[vcount1];
    VnFace = new TypeV[vncount1];
    VfFace = new TypeF[fcount1];

    file = fopen("figuras/cara.obj","r");

    int reso = fscanf(file,"%s",lineHeader);
    int x=0,j=0,k=0;
    float px,py,pz;
    int pa,pb,pc,pan,pbn,pcn;

    //leemos otra vez el fichero para guardar los valores
    while(1){

        reso = fscanf(file,"%s",lineHeader);

        if (reso == EOF){
            break;
        }

        if (strcmp(lineHeader,"v")==0){
            fscanf(file,"%f %f %f",&px,&py,&pz);
            VvFace[x].readTypeV(px,py,pz);
            x++;
        }

        else if(strcmp(lineHeader,"vn") == 0){
            fscanf(file,"%f %f %f",&px,&py,&pz);
            VnFace[k].readTypeV(px,py,pz);
            k++;
        }

        else if(strcmp(lineHeader,"f") == 0){
            fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d",&pa,&d,&pan,&pb,&d,&pbn,&pc,&d,&pcn);
            VfFace[j].readTypeF(pa,pb,pc,pan,pbn,pcn);
            j++;
        }
    }

    fclose(file);

}

//Almacenamos las coordenadas del fichero del ojo en diversos vectores de igual forma que en la función anterior
void loadEyeFile(){

    FILE *file;
    char lineHeader[128];
    int d, vcount2=0, vncount2=0,x=0,j=0,k=0,posicion2,a2,b2,q2,d2,e2,h2,g2;
    int pa,pb,pc,pan,pbn,pcn;
    float px,py,pz;

     file = fopen("figuras/ojo.obj","r");

     if(file == NULL){
        cerr<<"Error: Can not open eye file"<<endl;
        exit(0);
    }
    
     int res = fscanf(file,"%s",lineHeader);
     
     while(1){
        res=fscanf(file,"%s",lineHeader);
        if(res == EOF){
            break;
        }

        if(strcmp(lineHeader,"f") == 0){
            fcount2++;
        }
        else if(strcmp(lineHeader,"v") == 0){
            vcount2++;
        }
        else if(strcmp(lineHeader,"vn") == 0){
            vncount2++;
        }
     }

     fclose(file);

     VvEye = new TypeV[vcount2];
     VnEye = new TypeV[vncount2];
     VfEye = new TypeF[fcount2];

     file = fopen("figuras/ojo.obj","r");

     res = fscanf(file,"%s",lineHeader);
     
     while(1){
        res = fscanf(file,"%s",lineHeader);

        if (res == EOF){
            break;
        }

        if (strcmp(lineHeader,"v")==0){
            fscanf(file,"%f %f %f",&px,&py,&pz);
            VvEye[x].readTypeV(px,py,pz);
            x++;
        }

        else if(strcmp(lineHeader,"vn") == 0){
            fscanf(file,"%f %f %f",&px,&py,&pz);
            VnEye[k].readTypeV(px,py,pz);
            k++;
        }

        else if(strcmp(lineHeader,"f") == 0){
            fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d",&pa,&d,&pan,&pb,&d,&pbn,&pc,&d,&pcn);
            VfEye[j].readTypeF(pa,pb,pc,pan,pbn,pcn);
            j++;
        }
     }

    fclose(file);

 }

 //Aplicamos luces
void drawLight(){
    //Luz ambiente
    GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    //Posición de la luz
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Posición (4, 0, 8)
    //GLfloat lightPos0[] = {1.0f, 0.0f, 2.0f, 1.0f}; //Posición (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    
    //Dirección de la luz
    GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f}; //Color (0.5, 0.2, 0.2)
    //Viene de dirección (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    //GLfloat lightPos1[] = {-0.25f, 0.15f, 0.15f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}

//Dibuja en la pantalla la figura pasada mediante los vectores que contienen las coordenas
void drawShape(TypeF ArrayF[], TypeV ArrayV[], TypeV ArrayVn[], int count,int shape){
    int auxv,auxvn;
    //En cada iteración se dibuja un triángulo
    for(int i=0;i<count;i++){
        glBegin(GL_TRIANGLES);      //la figura está formada por un conjunto de triángulos de pequeño tamaño

        //si la figura es la cara se le da el color de la piel, si dicha figura es el ojo se le proporciona los colores blanco, azul y negro
        if(shape == 1)
            glColor3f(1.0,0.698039216,0.4);
        else if(shape == 2){
            //tras observar el fichero obj sabemos que estos son los intervalos en los que se dibujan las tres partes del ojo
            if(i >= 0 && i < 128){
                glColor3f(0.5,0.5,1);   //pupila, color azul
            }

            if(i >= 128 && i < 992){
                glColor3f(1,1,1);   //globo ocular, color blanco
            }

            if(i >= 992 && i < 1024){
                glColor3f(0,0,0);   //iris, color negro
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
        //Repetimos esta operación para los otros dos vértices que forman un triángulo
        auxv = (ArrayF[i].getB() - 1);
        glVertex3f(ArrayV[auxv].getX(),ArrayV[auxv].getY(),ArrayV[auxv].getZ());

        auxv = (ArrayF[i].getC() - 1);
        glVertex3f(ArrayV[auxv].getX(),ArrayV[auxv].getY(),ArrayV[auxv].getZ());

        glEnd();
    }
}

void detectMarker(){

    Point2f maux;
    float v[50];
    float menor;
    int indice;
    Point2f TheMarkerAux;
    Point2f MarkerAnt, PointMarkerAnt;

    if(TheMarkers.size() != 0){
        PointMarkerAnt = Marker1.getCenter();

        if(isnan(PointMarkerAnt.x)){         
            if(TheMarkers.size() == 1){
                Marker1 = TheMarkers[0];
                TheMarkerAux = Marker1.getCenter();
                cadena="OK";
            }
            else{

                cadena = "Error: varios tags detectados";
            }
        }
        else{

            if(TheMarkers.size() == 1){

                Marker1 = TheMarkers[0];
                cadena="OK";
                econt = 0;

            }

            else if(TheMarkers.size() > 1){

                econt++;
                cout<<"econt: "<<econt<<endl;

                if(econt >= 20){

                    cadena = "Error: varios tags detectados";;

                }

                else{

                    for (unsigned int m=0;m<TheMarkerSize;m++){

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

}

void calculateEyesPosition(){

    Point3D MarkerCm, MarkerEye1, MarkerEye2;
    Point2f MarkerC;
    
        //Obtenemos el centro del marcador
        MarkerC = Marker1.getCenter();
        //Guardamos su valor en píxeles, sabiendo que el punto 0,0 se encuentra en la esquina superior izquierda de la pantalla
        MarkerPx.readPoint3D(MarkerC.x,MarkerC.y);
        //Obtenemos la coordenada z, su valor está en cm
        MarkerPx.obtenerz(Marker1.getPerimeter());
        //Convertimos la coordenada z a px
        MarkerPx.convertToPixel();
        //Modificamos los valores de las coordenadas del punto, el punto 0,0 se sitúa ahora en el centro de la pantalla
        MarkerPx.translateCentre(CameraPosition);
        //Convertimos las coordenadas del punto a cm, este objeto solo se utilizará para visualizar el punto en la pantalla
        //La conversión se realiza sabiendo que el punto (320, 240)px es equivalente a (0.7179, 0.5384)cm
        MarkerCm.setX((MarkerPx.getX() * 0.9886)/320);
        MarkerCm.setY((MarkerPx.getY() * 0.5384)/240);

        //Aplicamos el desplazamiento del ojo izquierdo
        float desx2 = MarkerPx.getX()+((320*0.145)/0.9886); //pasamos a píxeles
        float desy2 = MarkerPx.getY()-((240*0.165)/0.5384);

        MarkerEye2.readPoint3D(desx2, desy2);
        MarkerEye2.setZ(MarkerPx.getZ());
        //Calculamos la rotación del ojo izquierdo en el eje z
        //Para simular mayor amplitud en los movimientos, ampliar el ángulo de rotación x e y
        //Una amplitud de 180º en el movimiento de los ojos supondría un rango de rotación de -90º a 180º
        //En una persona humana, la amplitud del campo de visión puede ser de 180º, pero el ojo no puede rotar 180º
        //He supuesto una rotación total de 50º (-25º a +25º). El ángulo máximo de rotación al hacer pruebas es 15º, así que se le aplicará un coeficiente de 1,667 para llegar a 25.
        rotz2 = MarkerEye2.turnAxisZ();
        //Rotación del ojo en el eje x
        rotx2 = MarkerEye2.turnAxisX() * 1.667;
        //Rotación del ojo en el eje y
        roty2 = MarkerEye2.turnAxisY() * 1.667;

        //Desplazamiento del ojo derecho
        float desx1 = MarkerPx.getX()-((320*0.145)/0.9886);
        float desy1 = MarkerPx.getY()-((240*0.165)/0.5384);

        MarkerEye1.readPoint3D(desx1,desy1);
        MarkerEye1.setZ(MarkerPx.getZ());
        //Rotación ejes z, x e y
        rotz1 = MarkerEye1.turnAxisZ();
        rotx1 = MarkerEye1.turnAxisX() * 1.667;
        roty1 = MarkerEye1.turnAxisY() * 1.667;
}

//Dibuja la escena 3D
void drawScene() {

    //Inicializamos el renderizado
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Activamos la luz
    glEnable(GL_LIGHT0); //Activamos luz #0
    glEnable(GL_LIGHT1); //Activamos luz #1
    glEnable(GL_NORMALIZE); //Se normalizan automáticamente las normales

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);

    detectMarker();

    Point2f MarkerDetect = Marker1.getCenter();

    if (not isnan(MarkerDetect.x)){

        calculateEyesPosition();

    }
    
    //Dibuja en la pantalla el puntero que muestra la posición del centro del marcador
    // glLoadIdentity();
    // ////glTranslatef(0.0f, 0.0f, -1.0f);
    // ////glTranslatef(0.0f, 0.0f, 0.0f);
    // glTranslatef(0.0f, 0.0f, -1.3f);
    // drawLight();
    // glColor3f(1,0,1);   
    // glPointSize(10.0);
    // glBegin(GL_POINTS);
    //     glVertex3f(-MarkerCm.getX(),MarkerCm.getY(),0);
    //     //glVertex3f(0.117,0.133,0);
    //     //glVertex3f(0,0,0);
    //     //glVertex3f(0.145,0.165,0);
    //     //glVertex3f(0.9886,0.5384,0);
    // glEnd();

    //Dibuja en la pantalla la cara
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-1.0f);
    glScalef(0.08f,0.08f,0.08f);
    drawLight();
    drawShape(VfFace,VvFace,VnFace,fcount1,1);

    //Dibuja en la pantalla el ojo izquierdo
    glLoadIdentity();
    glTranslatef(0.145f,0.165f,-1.3f);
    glScalef(0.08f,0.08f,0.08f);
    drawLight();

    //Rotamos ojo izquierdo
    glRotatef(rotx2,1,0,0);
    glRotatef(roty2,0,1,0);
    glRotatef(rotz2,0,0,1);
    //glRotatef(-25,1,0,0);
    //Dibuja la forma
    drawShape(VfEye,VvEye,VnEye,fcount2,2);

    //Dibuja en la pantalla el ojo derecho
    glLoadIdentity();
    glTranslatef(-0.145f,0.165f,-1.3f);
    glScalef(0.08f,0.08f,0.08f);
    drawLight();
    //Rotamos ojo derecho
    glRotatef(rotx1,1,0,0);
    glRotatef(roty1,0,1,0);
    glRotatef(rotz1,0,0,1);
    cout<<"rotx1: "<<rotx1<<endl;
    cout<<"roty1: "<<roty1<<endl;
    cout<<"rotz1: "<<rotz1<<endl;
    //Dibuja la forma
    drawShape(VfEye,VvEye,VnEye,fcount2,2);

    glutSwapBuffers();
}

void update(int value) {

    _angle += 1.5f;
    if (_angle > 360) {
        _angle -= 360;
    }
    
    glutPostRedisplay();
    glutTimerFunc(50, update, 0);
}

bool readArguments ( int argc,char **argv )
{
    if (argc!=4) {
        cerr<<"Invalid number of arguments"<<endl;
        cerr<<"Usage: (in.avi|live)  intrinsics.yml   size   Posición cámara"<<endl;
        return false;
    }
    TheInputVideo    = argv[1];
    TheIntrinsicFile = argv[2];
    CameraPosition   = atoi(argv[3]);
    return true;
}

//Inicializamos el renderizado
void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Activamos la luz
    glEnable(GL_LIGHT0); //Activamos luz #0
    glEnable(GL_LIGHT1); //Activamos luz #1
    glEnable(GL_NORMALIZE); //Se normalizan automáticamente las normales
}

void vIdle()
{
    if (TheCaptureFlag) {
        //capture image
        TheVideoCapturer.grab();
        TheVideoCapturer.retrieve( TheInputImage);
        TheUndInputImage.create(TheInputImage.size(),CV_8UC3);
        //transform color that by default is BGR to RGB because windows systems do not allow reading BGR images with opengl properly
        cv::cvtColor(TheInputImage,TheInputImage,CV_BGR2RGB);
        //remove distorion in image
        cv::undistort(TheInputImage,TheUndInputImage, TheCameraParams.CameraMatrix, TheCameraParams.Distorsion);
        //detect markers
        PPDetector.detect(TheUndInputImage,TheMarkers, TheCameraParams.CameraMatrix,Mat(),TheMarkerSize,false);
        //resize the image to the size of the GL window
        cv::resize(TheUndInputImage,TheResizedImage,TheGlWindowSize);
    }

    if ( glutGetWindow() != main_window ){
    glutSetWindow(main_window);
    }

    glutPostRedisplay();

    vX = -MarkerPx.getX()*(width/640);
    vY = MarkerPx.getY()*(height/480);
    vZ = MarkerPx.getZ();
    strcpy(buffer,cadena);
    glui->sync_live();
}

//Llamada cuando se redimensiona la ventana
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
    GLUI_EditText   *edittext;
  try{
        if(readArguments(argc,argv)==false)
            return 0;

        if(TheInputVideo=="live"){
            cout << "Choosed camera live" <<endl;
            TheVideoCapturer.open(-1);
            if(TheVideoCapturer.open(-1) != 1)
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
        TheCameraParams.readFromXMLFile(TheIntrinsicFile);
        TheCameraParams.resize(TheInputImage.size());

        //Cargamos las coordenadas de los puntos que forman la cara en el vector para después dibujarla
        cout << "Loading face" << endl;
        loadFaceFile();
        //Cargamos las coordenadas del ojo
        cout << "Loading eyes" << endl;
        loadEyeFile();

        //Obtenemos resolución de pantalla
        Display * pantalla;
        pantalla = XOpenDisplay ( 0 );
        width  = XDisplayWidth  ( pantalla, DefaultScreen ( pantalla ) );
        height = XDisplayHeight ( pantalla, DefaultScreen ( pantalla ) );
        XCloseDisplay ( pantalla );

  /****************************************/
  /*   Initialize GLUT and create window  */
  /****************************************/
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( width,height );
  main_window = glutCreateWindow( "Simulador motilidad ocular" );
  glutDisplayFunc(drawScene);
  glutReshapeFunc(handleResize);

  cout << "Drawing the scene" << endl;

  /****************************************/
  /*          Enable z-buffering          */
  /****************************************/

  glEnable(GL_DEPTH_TEST);

  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/
  glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_BOTTOM);
  GLUI_Master.set_glutIdleFunc(vIdle);
  GLUI_Master.set_glutReshapeFunc(handleResize);

  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );

  GLUI_EditText *counterx_edittext = new GLUI_EditText( glui, "x:", &vX );
  counterx_edittext->disable();

  new GLUI_Column( glui, false );
  GLUI_EditText *countery_edittext = new GLUI_EditText( glui, "y:", &vY );
  countery_edittext->disable();
  new GLUI_Button(glui,"Simulador Motilidad Ocular",0,(GLUI_Update_CB)exit);
  
  new GLUI_Column( glui, false );
  GLUI_EditText *counterz_edittext = new GLUI_EditText( glui, "z:", &vZ );
  counterz_edittext->disable();
  
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );
  new GLUI_Column( glui, false );

  edittext = glui->add_edittext("Estado",GLUI_EDITTEXT_TEXT,buffer);
  edittext->set_w( 600 );
  edittext->disable();

  new GLUI_Button(glui,"Examen",0,(GLUI_Update_CB)exit);
  glui->set_main_gfx_window( main_window );
  TheGlWindowSize=TheInputImage.size();

  glutMainLoop();

  return 0;
  }

    catch (std::exception &ex)
    {
        cout<<"Exception :"<<ex.what()<<endl;
    }
}
