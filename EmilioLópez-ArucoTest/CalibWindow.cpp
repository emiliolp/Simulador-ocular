#include "CalibWindow.hpp"
#include "aruco_calibration_board_a4.h"

using namespace aruco_test;

void CalibWindow::getMarker2d_3d(vector<cv::Point2f> &p2d, vector<cv::Point3f> &p3d, const MarkerMap &bc){

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

        void CalibWindow::Capture(Fl_Widget *w, void *d){

            vector<cv::Point2f> p2d;
            vector<cv::Point3f> p3d;

            if (Marker1.empty() == 0 && noExisteTag == false){

                stringstream sstr;
                sstr.write((char*)default_a4_board,default_a4_board_size);
                TheMarkerMapConfig.fromStream(sstr);

                CalibWindow::getMarker2d_3d(p2d,p3d,TheMarkerMapConfig);
                if(p3d.size()>0){
                    calib_p2d.push_back(p2d);
                    calib_p3d.push_back(p3d);
                }

                if (calib_p3d.size() > 3){      //Si se han realizado las 4 capturas finalizamos la calibración
                    vector<cv::Mat> vr,vt;
                    camp.CamSize=TheInputImage.size();
                    cv::calibrateCamera(calib_p3d,calib_p2d,TheInputImage.size(),camp.CameraMatrix,camp.Distorsion,vr,vt);
                    camp.saveToFile("C:\\ProgramData\\Simulador_motilidad_ocular\\camera.yml");
                    TheCameraParams.readFromXMLFile("C:\\ProgramData\\Simulador_motilidad_ocular\\camera.yml");
                    winCalib->hide();
                    flagCalibration = false;
                    fl_message("Calibración realizada con éxito");
                }

                if (calib_p3d.size() == 1){

                    calibImage1->inactive();
                    calibImage2 = new Fl_JPEG_Image("2.jpg");
                    boxCalibra2->image(calibImage2);

                }
                else if (calib_p3d.size() == 2){

                    calibImage2->inactive();
                    calibImage3 = new Fl_JPEG_Image("3.jpg");
                    boxCalibra3->image(calibImage3);

                }
                else if (calib_p3d.size() == 3){

                    calibImage3->inactive();
                    calibImage4 = new Fl_JPEG_Image("4.jpg");
                    boxCalibra4->image(calibImage4);

                }

                winCalib->redraw();

            }

        }
