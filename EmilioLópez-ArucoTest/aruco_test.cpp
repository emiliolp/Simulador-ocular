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

#define NORMAL 1
#define ESTRABISMO 2

using namespace cv;
using namespace aruco;
using namespace std;
using namespace aruco_test;

int   main_window, obj=0;
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
float TheMarkerSize=-1, rotx1, roty1, rotz1,rotz2, rotx2,roty2,rotx,roty,rotz;
float _angle = -70.0f,vX,vY,vZ,width,height;
int econt=0, fcount1=0,fcount2=0, CameraPosition, curr_string_iz = 0, curr_string_de = 0;
GLUI *glui;
Marker Marker1;
char cadena[200], buffer[200];

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 128.0 };
GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
GLfloat rot_angle_y = 0.0;
GLfloat rot_angle_x = 0.0;
GLfloat ROT_INC = 0.1;

GLUI_RadioGroup *radio;
GLUI_Listbox *listizd, *listder;

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
                strcpy(cadena,"OK");
            
            }
            else{

                strcpy(cadena,"Error: varios tags detectados");
            }
        }
        else{

            if(TheMarkers.size() == 1){

                Marker1 = TheMarkers[0];
                strcpy(cadena,"OK");
                econt = 0;

            }

            else if(TheMarkers.size() > 1){

                econt++;
                cout<<"econt: "<<econt<<endl;

                if(econt >= 20){

                    strcpy(cadena,"Error: varios tags detectados");

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

    Point3D MarkerCm, MarkerEye1, MarkerEye2, MarkerEye;
    Point2f MarkerC;
    float rot_estrabismo;
    
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
        
    //Luz
    float desx = MarkerPx.getX();
    float desy = MarkerPx.getY()-((240*0.165)/0.5384);
    MarkerEye.readPoint3D(desx,desy);
    MarkerEye.setZ(MarkerPx.getZ());
    rotz = MarkerEye.turnAxisZ();
    //Rotación del ojo en el eje x
    rotx = MarkerEye.turnAxisX() * 3;
    //Rotación del ojo en el eje y
    roty = MarkerEye.turnAxisY() * 3;

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
    if ( curr_string_iz == NORMAL)
        
        roty2 = MarkerEye2.turnAxisY() * 1.667;
        
        else if (curr_string_iz == ESTRABISMO){
            //Cuando aplicamos estrabismo al ojo izquierdo (posición de reposo), rotamos el ojo hacia el interior 15º * 1.667 (valor que permite ampliar el campo de visión, como vimos antes)
            rot_estrabismo = ( MarkerEye2.turnAxisY() - 15 ) * 1.667;
        
            if (rot_estrabismo >=  ( -30)){

                roty2 = rot_estrabismo;
            }
            else
                roty2 = -30;    //Límite rotación eje y
        }

        //Desplazamiento del ojo derecho
        float desx1 = MarkerPx.getX()-((320*0.145)/0.9886);
        float desy1 = MarkerPx.getY()-((240*0.165)/0.5384);

        MarkerEye1.readPoint3D(desx1,desy1);
        MarkerEye1.setZ(MarkerPx.getZ());
        //Rotación ejes z, x e y
        rotz1 = MarkerEye1.turnAxisZ();
        rotx1 = MarkerEye1.turnAxisX() * 1.667;

        if ( curr_string_de == NORMAL)

            roty1 = MarkerEye1.turnAxisY() * 1.667;

        else if (curr_string_de == ESTRABISMO){
            
            rot_estrabismo = ( MarkerEye2.turnAxisY() + 15 ) * 1.667;

            if (rot_estrabismo < 30){

                roty1 = rot_estrabismo;
            }
            else
                roty1 = 30;
        }

}

//Dibuja la escena 3D
void drawScene() {

    detectMarker();

    Point2f MarkerDetect = Marker1.getCenter();

    if (not isnan(MarkerDetect.x)){

        calculateEyesPosition();

    }

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE); //Se normalizan automáticamente las normales
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    glMatrixMode(GL_MODELVIEW);
    
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

    glPushMatrix();

        glRotatef(roty, 0,1,0);
        glRotatef(rotx, 1,0,0);
        glRotatef(rotz, 0,0,1);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glTranslatef(light_position[0],light_position[1],light_position[2]);

    glPopMatrix();

    //Dibuja en la pantalla la cara
    glPushMatrix();
     
        glLoadIdentity();
        glTranslatef(0.0f,0.0f,-1.0f);
        glScalef(0.08f,0.08f,0.08f);
        drawShape(VfFace,VvFace,VnFace,fcount1,1);
    
    glPopMatrix();

    //Dibuja en la pantalla el ojo izquierdo
    glPushMatrix();
     
        glLoadIdentity();
        glTranslatef(0.145f,0.165f,-1.3f);
        glScalef(0.08f,0.08f,0.08f);

        //Rotamos ojo izquierdo
        glRotatef(rotx2,1,0,0);
        glRotatef(roty2,0,1,0);
        glRotatef(rotz2,0,0,1);

        //Dibuja la forma
        drawShape(VfEye,VvEye,VnEye,fcount2,2);
    glPopMatrix();

    //Dibuja en la pantalla el ojo derecho
    glPushMatrix();
    
        glLoadIdentity();
        glTranslatef(-0.145f,0.165f,-1.3f);
        glScalef(0.08f,0.08f,0.08f);

        //Rotamos ojo derecho
        glRotatef(rotx1,1,0,0);
        glRotatef(roty1,0,1,0);
        glRotatef(rotz1,0,0,1);

        //Dibuja la forma
        drawShape(VfEye,VvEye,VnEye,fcount2,2);
    
    glPopMatrix();

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

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 128.0 };
    GLfloat light_position[] = { 0.0, 1.5, 1.0, 0.0 };

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
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

    vX = -MarkerPx.getX() * (width/640);
    vY = MarkerPx.getY()  * (height/480);
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

   if (rot_angle_y > 360.0)

      rot_angle_y = 0;

   else

      rot_angle_y += ROT_INC;

   if (rot_angle_x > 360.0)

      rot_angle_x = 0;

   else

      rot_angle_x += ROT_INC / (2 * 3.1416);

}

void control_cb( int control )
{
  
    //Activar/Desactivar simulación de patologías
    if (radio->get_int_val() == 1){

        listizd->disable();
        listder->disable();

    }
    
    else if (radio->get_int_val() == 0){

        listizd->enable();
        listder->enable();

    }
  
}


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


        //Inicializa y crea la ventana
        glutInit(&argc, argv);
        glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
        glutInitWindowSize( width,height );
        main_window = glutCreateWindow( "Simulador motilidad ocular" );
        initRendering();
        glutDisplayFunc(drawScene);
        glutReshapeFunc(handleResize);

        cout << "Drawing the scene" << endl;

        glEnable(GL_DEPTH_TEST);

        //Código GLUI
        glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_BOTTOM);
        GLUI_Master.set_glutIdleFunc(vIdle);
        GLUI_Master.set_glutReshapeFunc(handleResize);

        //Panel invisible para la interfaz
        GLUI_Panel *panel1 = new GLUI_Panel(glui, "", GLUI_PANEL_NONE );

        //Panel posición tag
        GLUI_Panel *pos_panel = new GLUI_Panel( panel1, "",GLUI_PANEL_NONE);
        GLUI_EditText *counterx_edittext = new GLUI_EditText( pos_panel, "x:", &vX );
        counterx_edittext->disable();

        new GLUI_Column( pos_panel, false );
        GLUI_EditText *countery_edittext = new GLUI_EditText( pos_panel, "y:", &vY );
        countery_edittext->disable();
  
        new GLUI_Column( pos_panel, false );
        GLUI_EditText *counterz_edittext = new GLUI_EditText( pos_panel, "z:", &vZ );
        counterz_edittext->disable();

        //Panel modo de visualización
        GLUI_Panel *obj_panel = new GLUI_Panel( panel1, "Modo de visualizacion" );
        radio = new GLUI_RadioGroup( obj_panel,&obj,1,control_cb );
        new GLUI_RadioButton( radio, "Simulador Motilidad Ocular" );
        new GLUI_Column(radio, true);
        new GLUI_RadioButton( radio, "Examen");
        obj_panel->set_alignment( GLUI_ALIGN_CENTER );
  
        new GLUI_Column( panel1, false );

        GLUI_Panel *est_panel = new GLUI_Panel(panel1, "", GLUI_PANEL_NONE );

        edittext = glui->add_edittext_to_panel(est_panel,"Estado",GLUI_EDITTEXT_TEXT,buffer);
        edittext->set_w( 600 );
        edittext->disable();
        edittext->set_alignment( GLUI_ALIGN_RIGHT );

        GLUI_Panel *obj_panelsim = new GLUI_Panel(panel1,"Simulacion Patologias");
        listizd = new GLUI_Listbox( obj_panelsim, "Ojo izquierdo:", &curr_string_iz );
        listizd->add_item( 1, "Sin anomalias" );
        listizd->add_item( 2, "Estrabismo");
        new GLUI_Column(obj_panelsim);
        listder = new GLUI_Listbox( obj_panelsim, "Ojo derecho: ", &curr_string_de );
        listder->add_item( 1, "Sin anomalias" );
        listder->add_item( 2, "Estrabismo");

        //new GLUI_Button(glui,"Examen",0,(GLUI_Update_CB)exit);
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
