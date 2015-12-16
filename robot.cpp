#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <iostream>

using namespace std;
/*####################
KONTROLER:
esc:    cikis
h :     topografik aydinlatma
q :     sola donus
e :     saga donus
w :     ileri hareket
s :     geri hareket
a :     sola kaydirma
d :     saga kaydirma

O :     kanca hareketi
o :     kanca hareketi
1 ardından + : ilk eklemden yukari hareket
1 ardından - : ilk eklemden asagi hareket
2 ardından + : ikinci eklemden yukari hareket
2 ardından - : ikinci eklemden asagi hareket
3 ardından + : tutucu saga/sola hareket
3 ardından - : tutucu saga/sola hareket
saga/sola imlec tuslar: sahneyi dondur

#######################*/

bool is_colored=false; //renklendirme durumunu tutan degisken
float xc=500, zc=17;   // kamera konumu degiskenleri
float radius = 500.0,angle=0.0; //kameranin hareket yaricapi ve acisi
float forward_backward=210,left_right=170; //arac konumunu kontrolleri
unsigned char joint_choice='0'; //eklem secimi
int shoulder = 0, elbow = 0, wrist = 0,hook=0, antenna=0, direction=0; //arac eklem acilari ve yon acisi 


int data[17][21]={//yukseklik verisi 
	{14,25,45,55,68,70,84,91,97,101,105,105,105,105,110,110,110,110,110,110,110},
	{5,18,43,62,73,82,88,94,99,102,105,105,105,105,110,110,110,110,110,110,110},
	{5,18,38,56,69,77,86,94,99,103,106,105,105,105,110,110,110,110,110,110,110},
	{5,9,31,48,60,71,81,87,95,101,106,105,105,105,110,110,110,110,110,110,110},
	{5,5,18,37,49,56,62,81,91,94,101,105,105,105,110,110,110,110,110,110,110},
	{5,5,12,23,34,40,53,66,77,82,97,103,105,105,109,110,110,110,110,115,115},
	{4,5,8,15,20,24,35,39,40,77,92,101,104,104,105,110,110,110,115,115,115},
	{5,7,22,36,46,48,48,44,50,58,80,96,96,97,106,110,110,115,115,115,115},
	{4,15,31,46,61,68,69,63,53,50,67,82,84,103,108,110,110,115,115,115,115},
	{4,12,31,46,64,78,82,80,69,54,73,71,92,105,108,110,110,115,115,115,115},
	{6,26,35,45,63,75,84,87,84,74,77,80,96,103,108,110,110,110,115,115,115},
	{21,30,46,57,64,76,85,92,92,87,79,80,86,102,106,110,105,110,115,115,115},
	{27,40,48,62,75,84,92,96,97,94,88,80,80,91,104,105,105,105,110,115,115},
	{33,43,55,65,75,87,96,101,101,101,97,92,80,80,98,105,105,105,105,110,115},
	{45,50,58,68,80,91,99,102,105,105,105,99,90,80,80,97,105,105,105,110,100},
	{50,60,65,71,84,95,101,105,105,107,107,106,102,101,92,80,98,104,105,100,100},
	{60,70,76,83,88,96,103,106,107,108,110,109,108,108,106,101,90,100,100,100,100}
};

void idle(void){ //calling this slows the rendering !
	antenna = (antenna + 1);
	if (antenna>=180) {antenna=0;}
	//anten acisinin guncellenmesi

	glutPostRedisplay();

}


void set_color(int height){ //yukseklige ve renklendirme tusuna gore renkleri ayarlayan fonk.
	float colors[3]={1,1,1};
	if(is_colored){
		if (height<50){
			glColor3f(0,1,0);
		}
		else if(height>=50 && height<80){
			glColor3f(1,1,0);
		}
		else if(height>=80){
			glColor3f(1,0,0);
		}

	}
	else glColor3f(1,1,1);
}

void display_robot (void) //robotu cizen robot fonksiyon
{
	glTranslatef((GLfloat) forward_backward,120,(GLfloat) left_right ); //robot konumu
	glColor3f(0.0, 0.0, 1.0);

	//glPushMatrix (); 
	//// gövde
	glColor3f(1, 1,1 );
	glRotatef((GLfloat) direction, 0.0, 1.0, 0.0);
	glPushMatrix ();
	glScalef (50.0, 30, 20);
	glutSolidCube (1.0);
	glPopMatrix ();

	//tekerlekler
	glPushMatrix();
	glColor3f(0.2,0.3,0.4);

	glTranslatef(20,-15,10);
	glutSolidTorus(4,8,10,25);

	glTranslatef(-37,0,0);
	glutSolidTorus(4,8,10,25);

	glTranslatef(0,0,-20);
	glutSolidTorus(4,8,10,25);

	glTranslatef(37,0,0);
	glutSolidTorus(4,8,10,25);

	glPopMatrix();

	//antenin dik cubugu
	glPushMatrix();
	glTranslatef(-18,25,0);
	glScalef (5.0, 20, 5);
	glutSolidCube (1.0);
	glPopMatrix();

	//antenin donen kismi
	glPushMatrix();
	glTranslatef(-18, 0, 0.0);
	glRotatef ((GLint) antenna, 0.0, 1.0, 0.0);
	glTranslatef(0,35,0);
	glScalef (30, 5, 5);
	glutSolidCube (1.0);
	glPopMatrix();

	//omuz
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef (25.0, 15, 0.0);
	glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
	glTranslatef (10.0, 0.0, 0.0);
	glPushMatrix ();
	glScalef (20.0, 4, 10);
	glutSolidCube (1.0);
	glPopMatrix ();

	//dirsek
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef (10, 0.0, 0.0);
	glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
	glTranslatef (10, 0.0, 0.0);
	glPushMatrix ();
	glScalef (20, 4, 10);
	glutSolidCube (1.0);
	glPopMatrix ();

	//bilek
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(10, 0.0, 0.0);
	glRotatef((GLfloat) wrist, 1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	//kanca kemigi
	glPushMatrix();
	glScalef(5, 10, 24);
	glutSolidCube(1.0);
	glPopMatrix();

	//sol kanca
	glColor3f(.0, 1.0, 1.0);
	glTranslatef(0, 0.0, 12);
	glRotatef((GLfloat) hook, 0.0, 1.0, 0.0);
	glTranslatef(10, 0, 0);
	glPushMatrix();
	glScalef(20, 5, 5);
	glutSolidCube(1.0);
	glPopMatrix();
	glTranslatef(-10, 0.0, 0);
	glRotatef((GLfloat) hook, 0.0, -1.0, 0);
	glColor3f(.0, 1.0, 1.0);

	//sag kanca
	glTranslatef(0, 0, -24);
	glRotatef((GLfloat) hook, 0.0, -1.0, 0);
	glTranslatef(10, 0, 0);
	glPushMatrix();
	glScalef(20, 5, 5);
	glutSolidCube(1.0);
	glPopMatrix();

	//glPopMatrix ();

}


void draw_triangles(){ //terraini cizen fonk.

	//display                data
	//	 xxxxxxxx 420         jjjjjjjjj 21
	//	z                    i
	//  z                    i
	//  z                    i
	//  z 340                i 17
	int x=0,z=0,i=0,j=0;

	for (x=0, j=0;x<400,j<20;x=x+20,j++){
		for(z=0, i=0;i<16,z<320;z=z+20,i++){ //vertexlerin olusturulup ucgenlerin cizimi
			glBegin(GL_LINE_STRIP);

			set_color(data[i+1][j]);
			glVertex3f ( x,data[i+1][j],z+20 );
			set_color(data[i][j+1]);
			glVertex3f ( x+20,data[i][j+1],z);
			set_color(data[i][j]); 
			glVertex3f ( x,data[i][j],z );

			set_color(data[i+1][j]); 
			glVertex3f ( x,data[i+1][j],z+20);
			set_color(data[i+1][j+1]); 
			glVertex3f ( x+20,data[i+1][j+1],z+20 );
			set_color(data[i][j+1]); 
			glVertex3f ( x+20,data[i][j+1],z );
			glEnd();
		}
	}
	glFlush();

}

void display (void)
{
	//#############################
	GLfloat mat_spec[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shiny[] = {50.0};              //parlaklik değeri
	GLfloat light_pos[] = {0.0, 0.0, .0, 0.0}; //isik konumu
	GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};


	glShadeModel (GL_SMOOTH);
	// glMaterialfv (GL_FRONT, GL_SPECULAR, mat_spec);
	//glMaterialfv (GL_FRONT, GL_SHININESS, mat_shiny);
	glLightfv (GL_LIGHT0, GL_POSITION, light_pos);
	//glLightfv (GL_LIGHT0, GL_DIFFUSE, white_light);
	// glLightfv (GL_LIGHT0, GL_SPECULAR, white_light);

	GLfloat light_position[] = {420.0, 115.0, 340.0, 1.0};
	glLightfv (GL_LIGHT0, GL_POSITION, light_position);
	//glColorMaterial(GL_FRONT,GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable (GL_LIGHT0);
	//#####################################


	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );

	glLoadIdentity();
	gluLookAt(xc+210, 300.0, zc+170,             210.0,110.0,170.0,       0.0, 1.0, 0.0); //kamera konumu 

	draw_triangles();
	//glTranslatef(210,150,170);// robot ilk konumu buradan da ayarlanabilir
	display_robot();
	glFlush();
	//glutSwapBuffers ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (50.0, (GLfloat) w / (GLfloat) h, 100.0, -300);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	//glTranslatef (0.0, 0.0, -5.0);
}

void keyboard(unsigned char key, int x, int y){

	switch (key) {
	case 27: //cikis icin esc
		exit (EXIT_SUCCESS);
		break;

	case 'h': case 'H': //aydinlatma icin
		is_colored=!is_colored;
		break;

	case 'w': case 'W'://ileri vites
		if(forward_backward<=370){  // terain sınır kontrol
			forward_backward=forward_backward+7;}
		break;

	case 's': case 'S'://geri vites
		if(forward_backward>=30){ // terain sınır kontrol
			forward_backward=forward_backward-7;}
		break;

	case 'd': case 'D'://saga kay
		if(left_right<=290){  // terain sınır kontrol
			left_right=left_right+7;		}
		break;

	case 'A': case 'a'://sola kay
		if(left_right>=20){  // terain sınır kontrol
			left_right=left_right-7;		}
		break;

	case 'o': 
		printf(		"%d\n",hook);
		if(hook>=-90){ // kanca kapatma
			hook = (hook - 2) % 360;		}
		break;

	case 'O'://kanca 
		if(hook<30){  //kanca acma
			hook = (hook + 2) % 360; 		}
		break;

	case '1': 
		joint_choice='1'  ;
		break;

	case '2': 
		joint_choice='2'  ;
		break;

	case '3': 
		joint_choice='3'  ;
		break;

	case 'q': case 'Q': //soluna don 
		direction = (direction + 5) % 360;    
		break;

	case 'E': case 'e':          //sagina don
		direction = (direction - 5) % 360;    
		break;

	case '+':
		switch (joint_choice){// eklem hareketleri icin degisken guncellemeleri

		case '1':
			if(shoulder<100)
				shoulder = (shoulder + 5) % 360; 
			break;

		case '2':
			if(elbow<70){elbow = (elbow + 5) % 360;}
			break;

		case '3':
			wrist = (wrist + 5) % 360;
			break;

		}
		break;

	case '-':
		switch (joint_choice){// eklem hareketleri icin degisken guncellemeleri

		case '1':
			if(shoulder>-50)
				shoulder = (shoulder - 5) % 360;
			break;

		case '2':
			if(elbow>-60){
				elbow = (elbow - 5) % 360;}
			break;

		case '3':
			wrist = (wrist - 5) % 360;
			break;
		}
		break;
	}

	glutPostRedisplay();

}

void special_keyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)//sahnenin dondurulmesi (kamera konumunun degismesiyle) aci ayarlamasi
	{ 		angle = angle + 1.0;	}

	else if (key == GLUT_KEY_RIGHT)//sahnenin dondurulmesi (kamera konumunun degismesiyle) aci ayarlamasi
	{angle = angle - 1.0;}

	if (angle > 360.0) { 
		angle = 0.0;}
	xc = radius * cos(angle / 180.0*M_PI); //kamera konumu degiskeninin degistirilmesi
	zc = radius * sin(angle / 180.0*M_PI); //kamera konumu degiskeninin degistirilmesi
	glutPostRedisplay();
}


int main(int argc, char **argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);     
	glutInitWindowSize(1000,800);
	glutInitWindowPosition(0,0);
	glutCreateWindow(">>>>>mars merkez turluyo herkez<<<<<");
	glMatrixMode (GL_PROJECTION);
	glutDisplayFunc (display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keyboard);
	glMatrixMode(GL_MODELVIEW);
	glEnable (GL_DEPTH_TEST);
	glutReshapeFunc (reshape);
	glutIdleFunc(idle);
	glutMainLoop();

	return EXIT_SUCCESS;
}
