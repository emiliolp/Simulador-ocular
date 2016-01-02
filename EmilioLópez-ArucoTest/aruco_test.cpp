#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <aruco/aruco.h>
#include <iostream>
#include "Point3D.hpp"
#include "TypeV.hpp"
#include "TypeF.hpp"
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace cv;
using namespace aruco;
using namespace std;
using namespace aruco_test;

string TheInputVideo;
string TheIntrinsicFile;    //fichero .yml
float TheMarkerSize=-1;     //tamaño del marcador
VideoCapture TheVideoCapturer;
vector<Marker> TheMarkers;      //vector de marcadores encontrados
TypeV *VvFace,*VvEye,*VnFace,*VnEye;    //vectores con los vértices y la normal de las figuras
TypeF *VfFace,*VfEye;                   //vectores con la posición de los vértices que forman la figura
Mat TheInputImage,TheUndInputImage,TheResizedImage;
CameraParameters TheCameraParams;
bool TheCaptureFlag=true;
MarkerDetector PPDetector;
Size TheGlWindowSize;
Point3D MarkerPx, MarkerCm, MarkerEye1,MarkerEye2;
Point2f MarkerC;
float rotx1, roty1, rotz1,rotz2, rotx2,roty2;
float _angle = -70.0f;

int fcount1=0,fcount2=0;

void loadFaceFile(){
    FILE *file;
    int d, vcount1=0,vncount1=0;
    //fichero obj del que obtenemos las coordenadas de la cabeza
    file = fopen("figuras/cara.obj","r");

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
     file = fopen("figuras/ojo.obj","r");
     char lineHeader[128];
     int res = fscanf(file,"%s",lineHeader);
     int d, vcount2=0, vncount2=0;
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
     int x=0,j=0,k=0,posicion2,a2,b2,q2,d2,e2,h2,g2;
     float px,py,pz;
     int pa,pb,pc,pan,pbn,pcn;
     
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

//Llamada cuando se presiona una tecla
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 27: //Escape
            exit(0);
    }
}

//Inicializamos el renderizado
void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Activamos la luz
    glEnable(GL_LIGHT0); //Activamos luz #0
    glEnable(GL_LIGHT1); //Activamos luz #1
    glEnable(GL_NORMALIZE); //Se normalizan automáticamente las normales
    //glShadeModel(GL_SMOOTH); //Activamos sombreado liso
}

//Llamada cuando se redimensiona la ventana
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-5, 5, -5, 5, -5, 5);
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
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

void calculateEyesPosition(){
    for (unsigned int m=0;m<TheMarkers.size();m++)
    {
        //Obtenemos el centro del marcador
        MarkerC = TheMarkers[m].getCenter();
        //Guardamos su valor en píxeles, sabiendo que el punto 0,0 se encuentra en la esquina superior izquierda de la pantalla
        MarkerPx.readPoint3D(MarkerC.x,MarkerC.y);
        //Obtenemos la coordenada z, su valor está en cm
        MarkerPx.obtenerz(TheMarkers[m].getPerimeter());
        //Convertimos la coordenada z a cm
        MarkerPx.convertToPixel();
        //Modificamos los valores de las coordenadas del punto, el punto 0,0 se sitúa ahora en el centro de la pantalla
        MarkerPx.translateCentre();
        //Convertimos las coordenadas del punto a cm, este objeto solo se utilizará para visualizar el punto en la pantalla
        //La conversión se realiza sabiendo que el punto (320, 240)px es equivalente a (0.7179, 0.5384)cm
        MarkerCm.setX((MarkerPx.getX() * 0.7179)/320);
        MarkerCm.setY((MarkerPx.getY() * 0.5384)/240);

        //Aplicamos el desplazamiento del ojo izquierdo
        float desx2 = MarkerPx.getX()+((320*0.145f)/0.7179f);
        //float desy2 = MarkerPx.getY()-((240*0.165f)/0.5384f);
        float desy2 = MarkerPx.getY();

        MarkerEye2.readPoint3D(desx2, desy2);
        MarkerEye2.setZ(MarkerPx.getZ());
        //Calculamos la rotación del ojo izquierdo en el eje z
        rotz2 = MarkerEye2.turnAxisZ();
        //Rotación del ojo en el eje x
        rotx2 = MarkerEye2.turnAxisX();
        //Rotación del ojo en el eje y
        roty2 = MarkerEye2.turnAxisY();

        //Desplazamiento del ojo derecho
        float desx1 = MarkerPx.getX()-((320*0.145f)/0.7179f);
        //float desy1 = MarkerPx.getY()-((240*0.165f)/0.5384f);
        float desy1 = MarkerPx.getY();
        MarkerEye1.readPoint3D(desx1,desy1);
        MarkerEye1.setZ(MarkerPx.getZ());
        //Rotación ejes z, x e y
        rotz1 = MarkerEye1.turnAxisZ();
        rotx1 = MarkerEye1.turnAxisX();
        roty1 = MarkerEye1.turnAxisY();
    }
}

//Dibuja la escena 3D
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);

    calculateEyesPosition();
    
    //Dibuja en la pantalla el puntero que muestra la posición del centro del marcador
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -1.0f);
    //glTranslatef(0.0f, 0.0f, -1.3f);
    drawLight();
    glColor3f(1,1,1);   
    glPointSize(10.0);
    glBegin(GL_POINTS);
        glVertex3f(-MarkerCm.getX(),MarkerCm.getY(),0);
    glEnd();

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
    glutTimerFunc(25, update, 0);
}

bool readArguments ( int argc,char **argv )
{
    if (argc!=4) {
        cerr<<"Invalid number of arguments"<<endl;
        cerr<<"Usage: (in.avi|live)  intrinsics.yml   size "<<endl;
        return false;
    }
    TheInputVideo=argv[1];
    TheIntrinsicFile=argv[2];
    TheMarkerSize=atof(argv[3]);
    return true;
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
        //cv::resize(TheUndInputImage,TheResizedImage,TheGlWindowSize);
    }
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    try{
        if(readArguments(argc,argv)==false)
            return 0;

        if(TheInputVideo=="live")
            TheVideoCapturer.open(0);
        else
            TheVideoCapturer.open(TheInputVideo);
        if(!TheVideoCapturer.isOpened()){
            cerr<<"Could not open video"<<endl;
            return -1;
        }

        //read first image
        TheVideoCapturer>>TheInputImage;
        //read camera paramters if passed
        TheCameraParams.readFromXMLFile(TheIntrinsicFile);
        TheCameraParams.resize(TheInputImage.size());

        //Cargamos las coordenadas de los puntos que forman la cara en el vector para después dibujarla
        loadFaceFile();
        //Cargamos las coordenadas del ojo
        loadEyeFile();

        //Initialize GLUT
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        //glutInitWindowSize(640, 480);
        glutInitWindowSize(TheInputImage.size().width,TheInputImage.size().height);
    
        //Create the window
        glutCreateWindow("Aruco");
        initRendering();
    
        //Set handler functions
        glutDisplayFunc(drawScene);
        glutIdleFunc( vIdle );
        glutKeyboardFunc(handleKeypress);
        glutReshapeFunc(handleResize);
    
        glutTimerFunc(25, update, 0); //Add a timer
        TheGlWindowSize=TheInputImage.size();
    
        glutMainLoop();
        return 0;
    }

    catch (std::exception &ex)
    {
        cout<<"Exception :"<<ex.what()<<endl;
    }
    
}