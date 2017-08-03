/**
 * Definición de la clase FaceWindow
 * Representa el área de la ventana principal de la aplicación donde se muestra el rostro humano
 */
#ifndef _FACEWINDOW_H_
#define _FACEWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <Fl/Fl_Check_Button.H>
#include <FL/gl.h>
#include "TypeV.hpp"
#include "TypeF.hpp"
#include <aruco/aruco.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "Point3D.hpp"

#define TRANSPARENTE 0
#define OPACO        1

#define NORMAL      0
#define ENDOTROPIA  1
#define EXOTROPIA   2
#define HIPERTROPIA 3
#define HIPOTROPIA  4

using namespace simulator;
using namespace aruco;
using namespace cv;

extern int fillResizeFace, examMode, leftOccluder, rightOccluder;
extern bool popupActive;
extern CameraParameters cameraParams;
extern float rotAnt;
extern Fl_Check_Button *checkLeftOc, *checkRightOc;
extern Marker markerChoosed;
extern int leftEyePath, rightEyePath;
extern int leftAngle, rightAngle;
extern Fl_Window *pop;

using namespace aruco;

namespace simulator{

    /**\brief Clase que representa el área en la que se muestra el modelo 3d
     *
     */
    class FaceWindow : public Fl_Gl_Window {

    private:

        vector<TypeV> vvLente;  /**<array con los vértices que forman el oclusor*/
        vector<TypeV> vvFace;   /**<array con los vértices que forman la figura de la cara*/
        vector<TypeV> vvEye;    /**<array con los vértices que forma un ojo*/
        vector<TypeV> vnFace;   /**<array formado por los índices de las normales de la cara*/
        vector<TypeV> vnEye;    /**<array formado por los índices de las normales de un ojo*/
        vector<TypeV> vnLente;  /**<array formado por los índices de las normales del oclusor*/
        vector<TypeF> vfLente;  /**<array compuesto por los triángulos que forman la figura del oclusor*/
        vector<TypeF> vfFace;   /**<array compuesto por los triángulos que forman la figura de la cara*/
        vector<TypeF> vfEye;    /**<array compuesto por los triángulos que forman la figura del ojo*/

        /**
         *@brief Método para actualizar la imagen del visor
         */
        static void updateScene(void *userdata);

        /**
         *@brief Dibuja en la pantalla el rostro humano
         *
         *Incluye el movimiento de los ojos y la iluminación de la escena
         */
        void draw();

        /**
         *@brief Redimensiona la área que muestra el rostro humano
         *@param x punto del eje x donde comienza el visor
         *@param y punto del eje y donde comienza el visor
         *@param w ancho de la ventana
         *@param h altura de la ventana
         */
        void resize(int x, int y, int w,int h);

        /**
         *@brief Método  que dibuja en pantalla una determinada figura a partir de los vértices de la misma
         *@param arrayF  vector que contiene la posición en el que dibujar cada uno de los vértices de la figura
         *@param arrayV  vector que contiene todos los vértices de la figura
         *@param arrayVn vector formado por los índices de las normales para los vértices (1-Rostro 2-Ojo 3-Oclusor)
         *@param shape   identificador de la figura a representar
         */
        void drawShape(vector<TypeF> arrayF, vector<TypeV> arrayV, vector<TypeV> arrayVn, int shape);

        /**
         *@brief Establece la información de la iluminación: color, intensidad y punto de origen
         *@param light rotación del punto de luz con respecto a la posición inicial
         */
        void initRendering(Point3f light);

        /**
         *@brief Lee el contenido de los ficheros de imagen que forman el modelo 3d
         *
         *Almacena la información en función de la tipología de los datos (vértices, posición o normales)
         *@param filename nombre del fichero a leer
         *@param arrayF   vector que contiene la información de las líneas de tipo f, especifica el orden en el que se dibuja cada vértice
         *@param arrayV   vector formado por la información de las líneas de tipo v, indica las coordenadas de los vértices
         *@param arrayVn  vector formado por el contenido de las líneas de tipo vn, indica los índices de las normales de los vértices
         */
        void loadFile(char *filename, vector<TypeF> &arrayF, vector<TypeV> &arrayV, vector<TypeV> &arrayVn);

        /**
         *@brief Calcula el ángulo de rotación de los ojos y del punto de luz
         *
         *El ángulo de rotación depende de que un ojo sufra una determinada patología.
         *Se especifica la rotación con respecto a los ejes x e y
         *@param leftRot rotación que experimenta el ojo izquierdo respecto a su posición inicial
         *@param rightRot rotación que experimenta el ojo derecho respecto a la posición inicial
         *@param lightRot rotación que se aprecia en el haz de luz principal proveniente del tag
         *@param marker tag seleccionado y se tomará como referencia para calcular la rotación de los ojos
         */
        void calculateEyesPosition(Point3f &leftRot, Point3f &rightRot, Point3f &lightRot, Marker marker);

        public:

            /**
             *@brief Constructor de la clase,
             *crea una instancia de la clase que representa el área de la ventana donde se muestra el rostro humano
             *@param X punto situado en el eje x donde comienza el área
             *@param Y punto situado en el eje y donde comienza el área
             *@param W ancho del área
             *@param H altura del área
             *@param L título del área
             */
            FaceWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {

                //Cargamos las figuras que forman el modelo 3D
                loadFile("figuras/cara.obj", vfFace, vvFace, vnFace);
                loadFile("figuras/ojo.obj", vfEye, vvEye, vnEye);
                loadFile("figuras/oclusor.obj", vfLente, vvLente, vnLente);

                Fl::add_timeout(1.0/5000.0, updateScene, (void*)this);       // 24fps timer
                end();
        }

    };

}
#endif // _FACEWINDOW_H_
