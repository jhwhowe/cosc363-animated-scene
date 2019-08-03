/*
 * Cosc363 Assignment 1
 * Author: James Howe
 * 
 */

#include <math.h>
#include <GL/freeglut.h>
#include "loadBMP.h"
#include "loadTGA.h"

#define NUM_TEXTURES 16
#define WALL_PX 40
#define WALL_PZ 50
#define WALL_NX -40
#define WALL_NZ -30

GLUquadric *q;    //Used for creating cylindrical objects
GLuint txId[NUM_TEXTURES];
const int tableN = 18;  // Total number of vertices for table
const int diamN = 5; //number of vertices for diamond

//---- Globals relating to the camera ----
float  eye_x = 0,  eye_y = 6,  eye_z = 15;    //Initial camera position
float look_x = 0, look_y = 6, look_z = 0;    //"Look-at" point along -z direction
float robot_eye_x = 0, robot_eye_y = 7.4, robot_eye_z = 0; //first person camera
float robot_look_x = 1.0, robot_look_y = 6, robot_look_z = 0;    //robot looking direction
float  theta = 0;                              //Look angle
int step = 0;								   //camera motion


//----------------------------------------
float DRSF = 3.14159265/180.0; //Scale factor to convert degrees to radians
float camAngle = 0; //angle of rotation of security camera
bool camDirection = 1; //Camera rotating clockwise (0) or CCW (1)
float robotMoveX = 0;
float robotMoveZ = 0; //robot movement in x and z
int robotTurn = 0; //Which turn the robot is up to
float robotRotate = 0.0; //angle that robot turns
bool turning = 0; //if the robot is turning currently = 1
bool robotPickup = 0; //is the robot picking something up
float robotLean = 0; //angle that the robot is leaning
float armAngle = 0.0; //angle that the arms are at
bool hold = 0; //is the robot holding something
bool putDown = 0; //tell robot to put thing down
float spin = 0.0;
int trayLocation = 0; //which table the tray is on
bool colour = 1; //if colours should be set or not (for shadows)
bool firstPerson = 0; //if camera is first person or not

void loadTexture(void)	 
{
	glGenTextures(NUM_TEXTURES, txId);   //Get N texture IDs 
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture name for the following OpenGL texture
	loadBMP("./Textures/MetalFloor.bmp");  //Floor texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture name for the following OpenGL texture
	loadBMP("./Textures/Wallbmp.bmp");  //wall texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture name for the following OpenGL texture
	loadTGA("./Textures/antarcticdawn/back.tga");  //skybox back
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[3]);  //Use this texture name for the following OpenGL texture
	loadTGA("./Textures/antarcticdawn/bottom.tga");  //skybox bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[4]);  //Use this texture name for the following OpenGL texture
	loadTGA("./Textures/antarcticdawn/front.tga");  //skybox front
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[5]);  //Use this texture name for the following OpenGL texture
	loadTGA("./Textures/antarcticdawn/left.tga");  //skybox left
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[6]);  //Use this texture name for the following OpenGL texture
	loadTGA("./Textures/antarcticdawn/right.tga");  //skybox right
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[7]);  //Use this texture name for the following OpenGL texture
	loadTGA("./Textures/antarcticdawn/top.tga");  //skybox top
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[8]);  //Use this texture name for the following OpenGL texture
	loadBMP("./Textures/Wood.bmp");  //Wood for table
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[9]);  //Use this texture name for the following OpenGL texture
	loadBMP("./Textures/dragonball1.bmp");  //Wood for table
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[10]);  //Use this texture name for the following OpenGL texture
	loadBMP("./Textures/dragonball2.bmp");  //Wood for table
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[11]);  //Use this texture name for the following OpenGL texture
	loadBMP("./Textures/dragonball3.bmp");  //Wood for table
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[12]);  //Use this texture name for the following OpenGL texture
	loadBMP("./Textures/dragonball4.bmp");  //Wood for table
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[13]);  //Use this texture name for the following OpenGL texture
	loadBMP("./Textures/dragonball5.bmp");  //Wood for table
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[14]);  //Use this texture name for the following OpenGL texture
	loadBMP("./Textures/dragonball6.bmp");  //Wood for table
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[15]);  //Use this texture name for the following OpenGL texture
	loadBMP("./Textures/dragonball7.bmp");  //Wood for table
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
}

//Create a textured floor plane made up of small squares
void floor(void)
{
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glPushMatrix();
	glTranslatef(0.0, -0.01, 0.0);
		
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	//load floor texture
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	for(int i = -200; i <= 200; i +=5){
		for(int j = -200; j <= 200; j += 5){
			glBegin(GL_QUADS);			//A grid of 200x200 squares on the xz plane
				glTexCoord2f(0, 0);
				glVertex3f(j, 0, i);
				glTexCoord2f(1, 0);
				glVertex3f(j + 5, 0, i);
				glTexCoord2f(1,1);
				glVertex3f(j + 5, 0, i + 5);
				glTexCoord2f(0,1);
				glVertex3f(j, 0, i + 5);
			glEnd();
		}
	}
	//Unbind the floor texture
	glBindTexture(GL_TEXTURE_2D, 0);
		
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);

	glPopMatrix();
}

//Make a robot that will do something
void robotWheel(void)
{
	if(colour){
		glColor3f(0.1, 0.1, 0.1);		//wheel
	}
	
	glPushMatrix();
	glTranslatef(0.0, 0.5, -0.35);
	gluCylinder(q, 0.5, 0.5, 0.7, 30.0 ,30.0); 
	if(colour){
		glColor3f(.1, 0.1, 0.1);		//
	}
	glPushMatrix();
		glTranslatef(0.0,0.0,0.7);
		gluDisk(q,0.0,0.5,30.0,30.0); 
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0,0.0,0.0);
		gluDisk(q,0.0,0.5,30.0,30.0);
	glPopMatrix(); 

	glPopMatrix();
}

void robotBody(void)
{
	//right wheel strut
	glPushMatrix(); 
		if(colour){
			glColor3f(0.8,0.8,0.8);
		}
		glTranslatef(0.0, 1.25, -0.45);
        glScalef(0.2,1.5,0.1);	
	    glutSolidCube(1.0);  
	glPopMatrix();
	
	//left wheel strut
	glPushMatrix();
		glTranslatef(0.0, 1.25, 0.45);
        glScalef(0.2,1.5,0.1);	
	    glutSolidCube(1.0);  
	glPopMatrix();
	
	//axle
	glPushMatrix(); 
		glTranslatef(0.0, 0.5, 0.0);
        glScalef(0.1,0.1,1.0);	
	    glutSolidCube(1.0);  
	glPopMatrix();
	
	//body
	if(colour){
		glColor3f(0.4,0.4,0.4);
	}
	glPushMatrix();
		glTranslatef(0.0, 1.25, 0.0);
        glRotatef(-90,1.0, 0.0, 0.0);
		gluCylinder(q, 0.5, 0.75, 2, 30.0 ,30.0);   
	glPopMatrix();
	
	//T H I C C 
	glPushMatrix();
		glTranslatef(-0.4, 1.5, -0.15);	
		glutSolidSphere(0.25, 30, 30);  
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-0.4, 1.5, 0.15);	
		glutSolidSphere(0.25, 30, 30);  
	glPopMatrix();
	
	//head
	glPushMatrix();
	if(colour){
		glColor3f(0.5,0.5,0.5);
	}
		glTranslatef(0.0, 3.7, 0.0);
        glScalef(1.0,1.0,1.5);	
	    glutSolidSphere(0.7, 30, 30);  
	glPopMatrix();

	//eyes
	glPushMatrix();
	if(colour){
		glColor3f(0.4,0.,0.);
	}
		glTranslatef(0.65, 3.7, -0.2);
	    glutSolidSphere(0.1, 30, 30); 
	    glTranslatef(0.0, 0.0, 0.4);
	    glutSolidSphere(0.1, 30, 30);  
	glPopMatrix();
	
	//antenna
	glPushMatrix(); 
	if(colour){
		glColor3f(0.4,0.4,0.4);
	}
		glTranslatef(0.0, 4.3, 0.0);
		glPushMatrix();
			glRotatef(-90,1.0, 0.0, 0.0);
			gluCylinder(q, 0.1, 0.1, 1, 30.0 ,30.0); 
	    glPopMatrix();
	    glTranslatef(0.0, 1, 0.0);
	    glutSolidSphere(0.2, 30, 30); 
	glPopMatrix();
}

void robotArms(void)
{
	glPushMatrix();
	//shoulders
	glPushMatrix();
	if(colour){
		glColor3f(0.5,0.5,0.5);
	}
		glScalef(1.0, 1.0, 1.4);
		glTranslatef(0.0, 2.8, 0.45);	
		glutSolidSphere(0.25, 30, 30);  
		glTranslatef(0.0, 0.0, -0.9);	
		glutSolidSphere(0.25, 30, 30); 
	glPopMatrix();
	
	//arms
	glPushMatrix();
		glTranslatef(0.0, 2, 0.8);
		glScalef(0.2,1.8,0.2);
		glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0, 2, -0.8);
		glScalef(0.2,1.8,0.2);
		glutSolidCube(1.0);
	glPopMatrix();
	
	glPopMatrix();
}

void normal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3 )
{
	  float nx, ny, nz;
	  nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
	  ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
	  nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(-nx, -ny, -nz);
}

//wowa a table
void table(void){
	glBindTexture(GL_TEXTURE_2D, txId[8]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	float vx[tableN] = {0,8,8.0,7.5,7.5,5.5,5.5,5,4.0,3.0,2.0,1.5,1,1,4.5,18,20.0,0.0};
	float vy[tableN] = {0,0,0.5,1.0,1.5,2,2.5,2.5,3.5,4.0,4.5,5,11,38,39.0,39,41.5,41.5};
	float vz[tableN] = {0};
	float wx[tableN], wy[tableN], wz[tableN]; 
	float angStep = 1.0*DRSF;  //Rotate in 1 deg steps (converted to radians)
	
	if(colour){
		glColor3f(1.0, 1.0, 1.0);
	}
	
	for (int j = 0; j < 360; j++){
		
		for(int i = 0; i < tableN; i++){
			wx[i] = vx[i] * cos(angStep) + vz[i] * sin(angStep);
			wy[i] = vy[i];
			wz[i] = -vx[i] * sin(angStep) + vz[i] * cos(angStep);
		}
		
		for(int i = 0; i < tableN; i++){
			glBegin(GL_TRIANGLE_STRIP);
			if(i > 0){
				normal( wx[i-1], wy[i-1], wz[i-1], vx[i-1], vy[i-1], vz[i-1], vx[i], vy[i], vz[i]);
			}
			glTexCoord2f((float) j/360, (float) i/ tableN);
			glVertex3f(vx[i], vy[i], vz[i]);
			
			if(i > 0){
				normal( wx[i-1], wy[i-1], wz[i-1], vx[i], vy[i], vz[i], wx[i], wy[i], wz[i]);
			}
			glTexCoord2f((float) (j+1)/36, (float)(i)/tableN);
			glVertex3f(wx[i], wy[i], wz[i]);
		}
	
		glEnd();
		
		for(int k = 0; k < tableN; k++){
			vx[k] = wx[k];
			vy[k] = wy[k];
			vz[k] = wz[k];
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Make a wall
void wall(void)
{
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glColor3f(1.0, 1.0, 1.0);
 	glBegin(GL_QUADS);
	
		//front
		glTexCoord2f(0, 1);
		glVertex3f(WALL_NX, 15, WALL_NZ);
		glTexCoord2f(0, 0);
		glVertex3f(WALL_NX, 0, WALL_NZ);
		glTexCoord2f(1, 0);
		glVertex3f(WALL_PX, 0, WALL_NZ);
		glTexCoord2f(1, 1);
		glVertex3f(WALL_PX, 15, WALL_NZ);
		
		//back
		glTexCoord2f(0, 1);
		glVertex3f(WALL_NX, 15, WALL_PZ);
		glTexCoord2f(0, 0);
		glVertex3f(WALL_NX, 0, WALL_PZ);
		glTexCoord2f(1, 0);
		glVertex3f(WALL_PX, 0, WALL_PZ);
		glTexCoord2f(1, 1);
		glVertex3f(WALL_PX, 15, WALL_PZ);
		
		//left
		glTexCoord2f(0, 1);
		glVertex3f(WALL_NX, 15, WALL_NZ);
		glTexCoord2f(0, 0);
		glVertex3f(WALL_NX, 0, WALL_NZ);
		glTexCoord2f(1, 0);
		glVertex3f(WALL_NX, 0, WALL_PZ);
		glTexCoord2f(1, 1);
		glVertex3f(WALL_NX, 15, WALL_PZ);
		
		//right
		glTexCoord2f(0, 1);
		glVertex3f(WALL_PX, 15, WALL_NZ);
		glTexCoord2f(0, 0);
		glVertex3f(WALL_PX, 0, WALL_NZ);
		glTexCoord2f(1, 0);
		glVertex3f(WALL_PX, 0, WALL_PZ);
		glTexCoord2f(1, 1);
		glVertex3f(WALL_PX, 15, WALL_PZ);
		
		glBindTexture(GL_TEXTURE_2D, 0);

	glEnd();
}

//Draw a skybox
void skybox(void)
{
	glBindTexture(GL_TEXTURE_2D, txId[2]); //back
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(250, -250, -250);
		glTexCoord2f(1, 0);
		glVertex3f(-250, -250, -250);
		glTexCoord2f(1, 1);
		glVertex3f(-250, 250, -250);
		glTexCoord2f(0, 1);
		glVertex3f(250, 250, -250);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, txId[3]); //bottom
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(250, -250, -250);
		glTexCoord2f(1, 0);
		glVertex3f(-250, -250, -250);
		glTexCoord2f(1, 1);
		glVertex3f(-250, -250, 250);
		glTexCoord2f(0, 1);
		glVertex3f(250, -250, 250);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, txId[4]); //front
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(250, -250, 250);
		glTexCoord2f(1, 0);
		glVertex3f(-250, -250, 250);
		glTexCoord2f(1, 1);
		glVertex3f(-250, 250, 250);
		glTexCoord2f(0, 1);
		glVertex3f(250, 250, 250);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, txId[5]); //left
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0);
		glVertex3f(-250, -250, -250);
		glTexCoord2f(1, 1);
		glVertex3f(-250, 250, -250);
		glTexCoord2f(0, 1);
		glVertex3f(-250, 250, 250);
		glTexCoord2f(0, 0);
		glVertex3f(-250, -250, 250);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, txId[6]); //right
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0);
		glVertex3f(250, -250, -250);
		glTexCoord2f(1, 1);
		glVertex3f(250, 250, -250);
		glTexCoord2f(0, 1);
		glVertex3f(250, 250, 250);
		glTexCoord2f(0, 0);
		glVertex3f(250, -250, 250);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, txId[7]); //top
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);
		glVertex3f(-250, 250, -250);
		glTexCoord2f(1, 0);
		glVertex3f(-250, 250, 250);
		glTexCoord2f(0, 0);
		glVertex3f(250, 250, 250);
		glTexCoord2f(0, 1);
		glVertex3f(250, 250, -250);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void cameraBody(void)
{
	glPushMatrix();
	glTranslatef(0.0,0.0,0.5);
	//camera body
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glColor3f(0.6,0.6,0.6);
	glPushMatrix();
      glScalef(1.0,1.0, 2.5);	
	  glutSolidCube(1.0);  
	glPopMatrix();  
	
	//camera lens
	glColor3f(0.2,0.2, 0.2);
	glPushMatrix();
      glTranslatef(0.0,0.0,1.25);	
	  gluCylinder(q, 0.3, 0.3, 0.2, 30.0 ,30.0);  
	  glTranslatef(0.0,0.0,0.2);
	  gluDisk(q,0.0,0.3,30.0,30.0);
	glPopMatrix(); 
	
	glRotatef(-30.0, 1.0, 0.0, 0.0);
	glPopMatrix();
}

void cameraBase(void)
{
	glColor3f(0.2,0.2,0.2);
	glPushMatrix();
	  glPushMatrix();
        glTranslatef(0.0, -0.5, 0.0);
        glScalef(0.1,1.0,0.1);	
	    glutSolidCube(1.0);  
	  glPopMatrix(); 
	  
	  glPushMatrix();
	    glTranslatef(0.0, -1.0, -0.6);
	    glScalef(0.1,0.1,1.3);	
	    glutSolidCube(1.0);  
	  glPopMatrix(); 
	  
	  glTranslatef(0.0,-1.0,-1.3);
	  gluCylinder(q, 0.3, 0.3, 0.5, 30.0 ,30.0); 
	  glTranslatef(0.0,0.0,0.5);
	  gluDisk(q,0.0,0.3,30.0,30.0); 
	  
	glPopMatrix(); 
	
}

//A little tray with a cup and a teapot on it :)
void tray(void)
{
	glPushMatrix();
	if(colour){
		glColor3f(0.2, 0.2, 0.2);
	}
		glScalef(0.8, 0.15, 1.6);
		glutSolidCube(1);
	glPopMatrix();
	
	if(colour){
		glColor3f(1, 0.2, 0.2);
	}
	glPushMatrix();
		glTranslatef(0.0, 0.2, -0.3);
		glutSolidTeapot(.2);
	glPopMatrix();
	
	glPushMatrix();
		if(colour){
			glColor3f(0.2, 0.2, 1);
		}
		glTranslatef(0.0, 0.38, 0.3);
		glRotatef(90, 1.0, 0.0, 0.0);
		gluCylinder(q, 0.1, 0.1, 0.3, 30, 30);
	glPopMatrix();
}

//Better summon Shenron to wish that this assignment was better looking
void dragonBalls(void)
{
	glPushMatrix();
		if(colour){
			glColor4f(1, 0.67, 0.0,1.0);
		}
		glBindTexture(GL_TEXTURE_2D, txId[9]);
		glTranslatef(0.0, 0, 0.0);	
		glRotatef(90,1.0,0.0,0.0);
		gluSphere ( q, 0.25, 36, 17 );
		for(int i = 0; i < 6; i++){
			glBindTexture(GL_TEXTURE_2D, txId[10 + i]);
			glPushMatrix();
			glRotatef(i * 60, 0.0, 1.0, 0.0);
			glTranslatef(1.2, 0, 0.0);	
			gluSphere ( q, 0.25, 36, 17 );
			glPopMatrix();
		}
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Keyboard event callback function
void special(int key, int x, int t)
{
	step = 0;
	if(key == GLUT_KEY_LEFT){
		theta += 5 * DRSF;   //in radians
	} else if(key == GLUT_KEY_RIGHT){
		 theta -= 5 * DRSF;
	} else if(key == GLUT_KEY_DOWN){
		 step = -1;
	} else if(key == GLUT_KEY_UP){
		 step = 1;
	}
	glutPostRedisplay();
}

//Was intended to make a first person mode but ran out of time :( :( :(
void keyboard(unsigned char key, int x, int y){
	//~ if(key == ' '){
		//~ firstPerson = 1 - firstPerson; //toggle first person camera
		//~ if(firstPerson){
			//~ eye_x = robot_eye_x, eye_z = robot_eye_z, eye_z = robot_eye_z;
		//~ } else {
			//~ //resets you to centre
			//~ eye_x = 0,  eye_y = 6,  eye_z = 15;  
		//~ }
	//~ }
}

void initialize(void) 
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
    float red[4]  = {1.0, 0.0, 0.0, 1.0};

	q = gluNewQuadric();
	
	loadTexture();
	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_LIGHTING);
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 500);
	
	glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, red);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals( q, GLU_SMOOTH );
	gluQuadricTexture( q, GL_TRUE );
	glClearColor (1.0, 1.0, 1.0, 0.0);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection
}

void display(void)
{
	float lgtPos[4] = {0.0f, 25.0f, 0.0f, 1.0f};  //Main light position
	float spotlgt_pos[] = {0.0f, 0.0f, 0.0f, 1.0f};  //security light
	float spotdir[] = {0.0f, -1.0f, 2.0f};
	
	float shadowMat[16] = { lgtPos[1],0,0,0, -lgtPos[0],0,-lgtPos[2],-1,0,0,lgtPos[1],0, 0,0,0,lgtPos[1] };
	
	//Set up the camera look direction
	float dir_x = -sin(theta);
	float dir_y = 0;
	float dir_z = -cos(theta);
	int d = 2;
	
	if(!firstPerson){
		eye_x = eye_x + step * dir_x;
		eye_z = eye_z + step * dir_z;
		
		step = 0;

		//~ //If want to limit camera movement do it here :^)
		
		if(eye_x > WALL_PX -12){
			eye_x = WALL_PX - 12;
		} else if (eye_x < WALL_NX + 12){
			eye_x = WALL_NX + 12;
		}
		
		if(eye_z > WALL_PZ - 12){
			eye_z = WALL_PZ - 12;
		} else if (eye_z < WALL_NZ + 12){
			eye_z = WALL_NZ + 12;
		}
    }
	
	look_x = eye_x + d * dir_x;
	look_y = eye_y + d * dir_y;
	look_z = eye_z + d * dir_z;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();
	gluLookAt(eye_x, eye_y, eye_z, look_x, look_y, look_z,   0, 1, 0);
	
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0, 1, 0);	
	glLightfv(GL_LIGHT0, GL_POSITION, lgtPos );   //set light position

	floor();
	skybox();
	wall();
	
	glPushMatrix();
		glTranslatef(0.0, 0.25, 5.0);
		glRotatef(spin, 0.0, 1.0, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		dragonBalls();
	glPopMatrix();
	
	glPushMatrix();
	  glTranslatef(0.0, 12.0, WALL_NZ + 2);
	  glPushMatrix();
	    glRotatef(camAngle, 0.0, 1.0, 0.0);
	    	
		glLightfv(GL_LIGHT1, GL_POSITION, spotlgt_pos);   //spotlight on robot
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);
	
		cameraBody();
	  glPopMatrix();
	  cameraBase();
	glPopMatrix();
	
	glPushMatrix();
	
	glTranslatef(robotMoveX, 0.0, robotMoveZ);
	glRotatef(-robotRotate, 0.0, 1.0, 0.0);
	glScalef(2,2,2);
	
	if(robotPickup){
		glRotatef(-robotLean, 0.0, 0.0, 1.0);
	} else if(putDown){
		glRotatef(-robotLean, 0.0, 0.0, 1.0);
	}
	robotWheel();
	robotBody();
	
	glPushMatrix();
	if(robotPickup || hold || putDown){
		glTranslatef(0.0,2.8,0.0);
		glRotatef(armAngle, 0.0, 0.0, 1.0);
		glTranslatef(0.0, -2.8, 0.0);
	}
	robotArms();
	glPopMatrix();
	
	if(hold){
		if(colour){
			glColor3f(1.0,0.0,1.0);
		}
		glPushMatrix();
			glTranslatef(1.5, 3.0, 0.0);
			tray();
		glPopMatrix();
	}
	glPopMatrix();
	
	//planar shadow stuff
	{
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glColor4f(0.1, 0.1, 0.1, 0.9);
		colour = 0;
		glMultMatrixf(shadowMat);
		glPushMatrix();
			glTranslatef(robotMoveX, 0.0, robotMoveZ);
			glRotatef(-robotRotate, 0.0, 1.0, 0.0);
			glScalef(2,2,2);
			if(robotPickup){
				glRotatef(-robotLean, 0.0, 0.0, 1.0);
			} else if(putDown){
				glRotatef(-robotLean, 0.0, 0.0, 1.0);
			}
			robotWheel();
			robotBody();
			
			glPushMatrix();
			if(robotPickup || hold || putDown){
				glTranslatef(0.0,2.8,0.0);
				glRotatef(armAngle, 0.0, 0.0, 1.0);
				glTranslatef(0.0, -2.8, 0.0);
			}
			robotArms();
			glPopMatrix();
			
			if(hold){
				if(colour){
					glColor3f(1.0,0.0,1.0);
				}
				glPushMatrix();
					glTranslatef(1.5, 3.0, 0.0);
					tray();
				glPopMatrix();
			}
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	colour = 1;
	glEnable(GL_LIGHTING);
}
	//end planar shadow stuff
	
	
	if(!hold){
		glColor3f(1.0,0.0,1.0);
		glPushMatrix();
		if(trayLocation == 0){
			glTranslatef(13.8,4.3, 0.0);
		} else {
			glTranslatef(-13.8,4.3, 10.0);
		}
		glScalef(2,2,2);
		tray();
		glPopMatrix();
	}
	glPushMatrix();
		glTranslatef(14.5,0.0, 0.0);
		glScalef(0.1, 0.1, 0.1);
		table();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(-14.5,0.0, 10.0);
		glScalef(0.1, 0.1, 0.1);
		table();
	glPopMatrix();
	
	glutSwapBuffers();   //Useful for animation
}

void timer1(int value)
{
	glutPostRedisplay();
	
	//Security camera rotation
	if(camDirection){
		camAngle+= 0.5;
		if(camAngle >= 75){
			camDirection = 0;
		}
	} else { //camDirection = 0;
		camAngle-= 0.5;
		if(camAngle <= -75){
			camDirection = 1;
		}
	}
	
	if(robotPickup){
		//Robot needs to pickup the tray
		if(robotLean <= 15 && hold == 0){
			robotLean++;
		} else if (armAngle <= 90){
			armAngle++;
			if(armAngle == 90){
				hold = 1;
			}
		} else if(hold && robotLean > 0){
				robotLean--;
		} else {
			robotPickup = 0;
		}
	} else if(putDown){
		if(robotLean <= 15 && hold == 1){
			robotLean++;
		}else if(armAngle > 0){
			hold = 0;
			armAngle--;
		} else if(robotLean > 0 && hold == 0){
			robotLean --;
		} else {
			putDown = 0;
		}
		
	} else if(!turning){
		if(robotTurn == 0){
			if(robotMoveX < 10){
				robotMoveX += 0.2;
				if(firstPerson){
					eye_x += 0.2;
				}
			} else {
				robotTurn = 1;
				turning = 1;
				if(trayLocation == 0){
					robotPickup = 1;
				} else if(hold){
					putDown =1;
					trayLocation = 0;
				}
			}
		}
		
		if(robotTurn == 1){
			if(robotMoveZ <= 10){
				robotMoveZ += 0.1;
				if(firstPerson){
					eye_z += 0.1;
				}
			} else {
				robotTurn = 2;
				turning = 1;
				robotLean = 0;
			}
		}
		
		if(robotTurn == 2){
			if(robotMoveX >= -10){
				robotMoveX -= 0.2;
				if(firstPerson){
					eye_x -= 0.2;
				}
			} else {
				robotTurn = 3;
				turning = 1;
				if(trayLocation == 1){
					robotPickup = 1;
				} else if(hold){
					putDown = 1;
					trayLocation = 1;
				}
			}
		}
		
		if(robotTurn == 3){
			if(robotMoveZ >= 0){
				robotMoveZ -= 0.1;
				if(firstPerson){
					eye_z -=0.1;
				}
			} else {
				robotTurn = 0;
				turning = 1;
				robotLean = 0;
			}
		}
	}else if((robotTurn != 0 && robotRotate <= robotTurn * 90) || (robotTurn == 0 && robotRotate > 0)){
		robotRotate += 1;
	} else {
		turning = 0;
	}
	
	if(robotRotate >= 360){
		robotRotate = 0;
	}
	
	spin++;
	
	glutTimerFunc(5, timer1, 0); //time is in ms
}

//---------------------------------------------------------------------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (1920, 1080); 
	glutInitWindowPosition (0, 0);
	glutCreateWindow ("Domo Arigato Mr. Roboto");
	initialize ();
	
    glutTimerFunc(5, timer1, 0);
	glutDisplayFunc(display); 
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
