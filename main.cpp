/*
 *  main.cpp
 *  BoidTest
 *
 *  Created by maruyama on 11/04/26.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>
#include <GLUT/glut.h>
#include <list>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <fstream.h>
#include "boid.h"

#ifndef parameter_H
#include "parameter.h"
#endif

static const double PI = 6*asin( 0.5 );

Boid boids[N];
Vector3D preyPosition(0.5,0.5,0.5);
unsigned long int timeStep = 0;
const unsigned int oneStep = 200000;
unsigned long int ind = 0;

std::ofstream ofData;
std::ofstream ofParam;

int  dragMouseL = 0;
int  lastMouseX;
int  lastMouseY;


void update_boids()
{
    Vector3D dv[N];
    for(int i=0; i<N; i++){
        Vector3D dv_coh;
        Vector3D dv_sep;
        Vector3D dv_ali;
        int neivers_num_coh = 0;
        int neivers_num_sep = 0;
        int neivers_num_ali = 0;
        for(int j=0; j<N; j++){
            if( i != j ){
                // Cohesion
                if (boids[i].isInsideCohesionArea(boids[j])){
                    neivers_num_coh ++;
                    dv_coh += boids[j].position;
                }
                // Separation
                if (boids[i].isInsideSeparationArea(boids[j])) {
                    neivers_num_sep ++;
                    dv_sep += (boids[i].position - boids[j].position).getUnity();
                }
                // Alignment
                if (boids[j].isInsideAlignmentArea(boids[j])) {
                    neivers_num_ali ++;
                    dv_ali += boids[j].velocity;
                }
            }
        }
        if (neivers_num_coh != 0) {
            dv_coh = dv_coh / neivers_num_coh - boids[i].position;
        }
        if (neivers_num_sep != 0) {
            //dv_sep = dv_sep / neivers_num_sep;
        }
        if (neivers_num_ali != 0) {
            dv_ali = dv_ali / neivers_num_ali - boids[i].velocity;
        }
        dv[i] = COEFF_COHESION*dv_coh + COEFF_SEPARATION*dv_sep +COEFF_ALIGNMENT*dv_ali;
    }
    
    for(int i=0; i<N; i++) {
        boids[i].velocity += dv[i];
        /*
        if(PREY){
            boids[i].velocity += (preyPosition - boids[i].position).getUnity() * preyF;
        }
         */
        
        if(boids[i].velocity.getAbs()>0. and boids[i].velocity.getAbs()>MAX_VELOCITY){
            boids[i].velocity = boids[i].velocity.getUnity() * MAX_VELOCITY;
        }else if(boids[i].velocity.getAbs()>0. and boids[i].velocity.getAbs()<MIN_VELOCITY){
            boids[i].velocity = boids[i].velocity.getUnity() * MIN_VELOCITY;
        }
        
        //update boid
        boids[i].position += boids[i].velocity;
        
        //Boundary conditon
        if(boids[i].position.x < 0.0) {
            boids[i].position.x = FIELD_SIZE + boids[i].position.x;
        }
        if(boids[i].position.y < 0.0) {
            boids[i].position.y = FIELD_SIZE + boids[i].position.y;
        }
        if(boids[i].position.z < 0.0) {
            boids[i].position.z = FIELD_SIZE + boids[i].position.z;
        }
        if(boids[i].position.x > FIELD_SIZE) {
            boids[i].position.x = boids[i].position.x - FIELD_SIZE;
        }
        if(boids[i].position.y > FIELD_SIZE) {
            boids[i].position.y = boids[i].position.y - FIELD_SIZE;
        }
        if(boids[i].position.z > FIELD_SIZE) {
            boids[i].position.z = boids[i].position.z - FIELD_SIZE;
        }
        
        //output data
        if(LOGGING and timeStep%outputStep==0){
            ofData << ind++ << "," << i << " "
            << boids[i].position.x * 2.0 - 1.0 << " "
            << boids[i].position.y * 2.0 - 1.0 << " "
            << boids[i].position.z * 2.0 - 1.0 << " ";
            if(boids[i].live){
                ofData << 0;
            }else{
                ofData << 1;
            }
            ofData << ";\n";
            if(stopPeriod && ind==10000){
                exit(0);
            }
        }
    }
}

void idle(void){
	//std::cout << "timeStep : " << timeStep << "\n";
	//std::cout << "clock()  : " << clock() << "\n";
	if(realTime){
		if( clock()>= (timeStep+1)*oneStep ){
			std::cout << "Cant display on this resolution\n";
			exit(-1);
		}else if(clock()<timeStep*oneStep){
			return;
		}
	}
	
	if( PREY && (timeStep%CHANGE_PREY)==0 ) {
		preyPosition.x = (double)rand()/(double)RAND_MAX;
		preyPosition.y = (double)rand()/(double)RAND_MAX;
		preyPosition.z = (double)rand()/(double)RAND_MAX;
	}
	
	timeStep++;
    update_boids();
    glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for(float i=0; i<=1.1; i+=0.1){
		glVertex3f(0.0, i, 0);
		glVertex3f(1.0, i, 0);
		glVertex3f(i, 0.0, 0);
		glVertex3f(i, 1.0, 0);
	}
//	glColor3d(1.0, 0.0, 0.0);
//	glVertex3f(0.0, 0.0, 0.0);
//	glVertex3f(1.0, 0.0, 0.0);
//	glColor3d(0.0, 1.0, 0.0);
//	glVertex3f(0.0, 0.0, 0.0);
//	glVertex3f(0.0, 1.0, 0.0);
//	glColor3d(0.0, 0.0, 1.0);
//	glVertex3f(0.0, 0.0, 0.0);
//	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
	
	for(int i=0; i<N; i++){
		if(boids[i].live){
			glColor3d(1.0, 0.0, 0.0);
		}else{
			glColor3d(0.0, 0.0, 1.0);
		}
		boids[i].draw();
	}
   
    glColor3d(1.0, 0.0, 1.0);
    boids[0].draw();
    
	if(PREY){
		glColor3d(1.0,1.0,0.0);
		glBegin(GL_POLYGON);
		for(int i=0; i<360; i+=10){
			glVertex3f(preyPosition.x+cos(i*2*PI/360)/50,
					   preyPosition.y+sin(i*2*PI/360)/50,
					   preyPosition.z);
		}
		glEnd();
	}
	
	//glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	gluLookAt(1.2, -1.7, 1.7,
              0.3, 0.7, 0.0,
              0.0, 0.0, 1.0);
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                dragMouseL = 1;
            }else if (state == GLUT_UP) {
                dragMouseL = 0;
            }
                      
			break;
		case GLUT_RIGHT_BUTTON:
			break;
		default:
			break;
	}
    lastMouseX = x;
    lastMouseY = y;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'c':
			preyPosition.x = (double)rand()/RAND_MAX;
			preyPosition.y = (double)rand()/RAND_MAX;
			preyPosition.z = (double)rand()/RAND_MAX;
			break;
		default:
			break;
	}
}

void  mousemove( int mx, int my )
{
    if ( dragMouseL == 1 ) {
        float deltaRotZ = ( mx - lastMouseX ) * 1.0;
        float deltaRotX = ( my - lastMouseY ) * 1.0;
        glMatrixMode( GL_MODELVIEW );
        glTranslatef(0.5, 0.5, 0.5 );
        glRotatef( deltaRotZ, 0.,  0.,  1. );
        //glRotatef( deltaRotZ, -1.53,  4.08,  6.57 );
        //glRotatef( deltaRotX, 2.4,  0.9, -0. );
        glTranslatef(-0.5, -0.5, -0.5);
    }
    
    lastMouseX = mx;
    lastMouseY = my;
    glutPostRedisplay();
}


void init(void)
{
	ofData.open(dataFileName.c_str());
	ofParam.open(paramFileName.c_str());

	ofParam
	<< "N = " << N << "\n"
    
	<< "#Separation" << "\n"
    << "force: " << COEFF_SEPARATION << "\n"
    << "area distance: " << SIGHT_DISTANCE_SEPARATION << "\n"
    << "area angle: " << SIGHT_ANGLE_SEPARATION << "\n"
    
    << "#Alignment" << "\n"
    << "force: " << COEFF_ALIGNMENT << "\n"
    << "area distance: " << SIGHT_DISTANCE_ALIGNMENT << "\n"
    << "area angle: " << SIGHT_ANGLE_ALIGNMENT << "\n"
    
    << "#Cohesion" << "\n"
    << "force: " << COEFF_COHESION << "\n"
    << "area distance: " << SIGHT_DISTANCE_COHESION << "\n"
    << "area angle: " << SIGHT_ANGLE_COHESION << "\n"
	
    << "preyF = " << preyF << "\n"
	<< "toLiveMin = " << toLiveMin << "\n"
	<< "toLiveMax = " << toLiveMax << "\n"

    << "#Velocity" << "\n"
	<< "min: " << MIN_VELOCITY << "\n"
	<< "max: " << MAX_VELOCITY << "\n"
	
    << "CHANGE_PREY = " << CHANGE_PREY << "\n"
	<< "outputStep = " << outputStep << "\n"
	<< "exitStep = " << exitStep << "\n"
	<< "stopPeriod = " << stopPeriod << "\n"
	<< "PREY = " << PREY << "\n"
	<< "realTime = " << realTime << "\n";

    glClearColor(1.0, 1.0, 1.0, 1.0);
    srand(12345);
	for(int i=0; i<N; i++){
        boids[i].position.x = drand48()*0.2 - 0.1 + FIELD_SIZE/2.;
        boids[i].position.y = drand48()*0.2 - 0.1 + FIELD_SIZE/2.;
        boids[i].position.z = 0.;
        
        boids[i].velocity.x = 0.001; //( drand48()*2.0 - 1.0 ) * (MAX_VELOCITY - MIN_VELOCITY) + MIN_VELOCITY;
        boids[i].velocity.y = 0.; //( drand48()*2.0 - 1.0 ) * (MAX_VELOCITY - MIN_VELOCITY) + MIN_VELOCITY;
        boids[i].velocity.z = 0.; //( drand48()*2.0 - 1.0 ) * (MAX_VELOCITY - MIN_VELOCITY) + MIN_VELOCITY;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize(960, 640);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
    glutMotionFunc(mousemove);
	glutKeyboardFunc(keyboard);
    init();
	glutMainLoop();
	return 0;
}