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
	
	bool outf = false;
	if(timeStep%outputStep==0){
		outf = true;
	}
	
	if( PREY && (timeStep%CHANGE_PREY)==0 ) {
		preyPosition.x = (double)rand()/(double)RAND_MAX;
		preyPosition.y = (double)rand()/(double)RAND_MAX;
		preyPosition.z = (double)rand()/(double)RAND_MAX;
	}
	
	timeStep++;
	
	for(int i=0; i<N; i++){
		
		//Cohesion
		Vector3D v_coh;
		//Separation
		Vector3D v_sep;
		Vector3D v_rad(SIGHTDISTANCE, SIGHTDISTANCE, SIGHTDISTANCE);
		//Alignment
		Vector3D v_ali;
		int sightNum = 0;
		for(int j=0; j<N; j++){
			if( i != j ){
				if( boids[i].canSee(boids[j]) ){
					sightNum ++;
					v_coh += boids[j].position;
					v_sep -= ( boids[j].position - boids[i].position ).getUnity() *
						(SIGHTDISTANCE - ( boids[j].position - boids[i].position ).getAbs());
					v_ali += boids[j].velocity;
				}
			}
		}
	
		if(sightNum!=0){
			v_coh /= sightNum;
			v_ali /= sightNum;
			v_sep /= sightNum;
	//		if(sightNum>=toLiveMin && sightNum<=toLiveMax){
	//			boids[i].live = true;
	//		}else{
	//			boids[i].live = false;
	//		}

			boids[i].velocity += ((v_coh - boids[i].position) * cohesionF);
			boids[i].velocity += ( v_sep * separationF );
			boids[i].velocity += ((v_ali - boids[i].velocity) * alignmentF);
		}
		
		if(PREY){
			boids[i].velocity += (preyPosition - boids[i].position).getUnity() * preyF;
		}
		
		if(boids[i].velocity.getAbs()>MAX_VEL){
			boids[i].velocity = boids[i].velocity.getUnity() * MAX_VEL;
		}
		
		//update boids
		boids[i].position += boids[i].velocity;
		
		//loop at boundary
		if(boids[i].position.x < 0.0) {
			boids[i].position.x = 1.0 + boids[i].position.x;
		}
		if(boids[i].position.y < 0.0) {
			boids[i].position.y = 1.0 + boids[i].position.y;
		}
		if(boids[i].position.z < 0.0) {
			boids[i].position.z = 1.0 + boids[i].position.z;
		}
		if(boids[i].position.x > 1.0) {
			boids[i].position.x = boids[i].position.x - 1.0;
		}
		if(boids[i].position.y > 1.0) {
			boids[i].position.y = boids[i].position.y - 1.0;
		}
		if(boids[i].position.z > 1.0) {
			boids[i].position.z = boids[i].position.z - 1.0;
		}
		
		//output data
		if(outf){
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
	gluLookAt(1.2, -1.7, 1.7, 0.3, 0.7, 0.0, 0.0, 0.0, 1.0);
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			break;
		default:
			break;
	}
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

void init(void)
{
	ofData.open(dataFileName.c_str());
	ofParam.open(paramFileName.c_str());

	ofParam
	<< "N = " << N << "\n"
	<< "SIGHTDISTANCE = " << SIGHTDISTANCE << "\n"
	<< "SIGHTANGLE = " << SIGHTANGLE << "\n"
	<< "separationF = " << separationF << "\n"
	<< "cohesionF = " << cohesionF << "\n"
	<< "alignmentF = " << alignmentF << "\n"
	<< "preyF = " << preyF << "\n"
	<< "toLiveMin = " << toLiveMin << "\n"
	<< "toLiveMax = " << toLiveMax << "\n"
	<< "INIT_VEL = " << INIT_VEL << "\n"
	<< "MAX_VEL = " << MAX_VEL << "\n"
	<< "CHANGE_PREY = " << CHANGE_PREY << "\n"
	<< "outputStep = " << outputStep << "\n"
	<< "exitStep = " << exitStep << "\n"
	<< "stopPeriod = " << stopPeriod << "\n"
	<< "PREY = " << PREY << "\n"
	<< "realTime = " << realTime << "\n";

	glClearColor(1.0, 1.0, 1.0, 1.0);
	
	for(int i=0; i<N; i++){
//		boids[i].position.x = (double)rand()/(double)RAND_MAX;
//		boids[i].position.y = (double)rand()/(double)RAND_MAX;
//		boids[i].position.z = (double)rand()/(double)RAND_MAX;
        boids[i].position.y = 0;
        boids[i].position.z = 0;
        boids[i].position.x = (double)rand()/(double)RAND_MAX/7.;

		boids[i].velocity.x = ( (double)rand()*2.0/(double)RAND_MAX - 1.0 ) * INIT_VEL;
		boids[i].velocity.y = ( (double)rand()*2.0/(double)RAND_MAX - 1.0 ) * INIT_VEL;
		boids[i].velocity.z = ( (double)rand()*2.0/(double)RAND_MAX - 1.0 ) * INIT_VEL;
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
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();
	return 0;
}