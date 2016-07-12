Consideracíones previas
-----------------------------------------------------------------
El tamaño del marcador debe ser 35 mm de lado

Instrucciones
-----------------------------------------------------------------
- Reemplazar la carpeta EmilioLópez-ArucoTest por la anterior
- En el terminal:
	mkdir build
	cd build
	cmake ..
	make

- Copiar el fichero de calibración de cámara .yml al directorio build
- Copiar la carpeta figuras al directorio build
- Ejemplo de ejecución (cambiar el nombre de fichero .yml):
	./aruco_test live camera3.yml 1

Vídeo demostración: https://www.dropbox.com/s/hinp42bs6ez294m/DSCN0043.MOV?dl=0

Prerequisitos:
    sudo apt-get install freeglut3-dev
    sudo apt-get install libxmu-dev libxi-dev
    sudo apt-get install libqt4-dev  # necesario?
 
Prerequisitos librería glui:
    Descargar la librería glui:
    http://www.rpmseek.com/rpm-pl/libglui2c2.html?hl=com&cs=libgcc1:RE:0:0:0:0:2420
    http://www.rpmseek.com/rpm-pl/libglui-dev.html?hl=com&cx=0::
    Instalamos:
    sudo dpkg -i libglui2c2_2.36-4_amd64.deb
    sudo dpkg -i libglui-dev_2.36-4_amd64.deb


 
