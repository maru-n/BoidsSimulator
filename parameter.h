/*
 *  parameter.h
 *  BoidTest
 *
 *  Created by maruyama on 11/04/26.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef parameter_H
#define parameter_H
#include <string>

const std::string dataFileName("test_data.txt");
const std::string paramFileName("test_param.txt");

const int N = 256;

const float SIGHTDISTANCE = 100;
const float SIGHTANGLE = 0.17;

const double separationF = 0.000001; // 10^-4
const double cohesionF = 0.0005; // 3x 10^-4
const double alignmentF = 0.001; // 10^-4

const double preyF = 0.000002;

const int toLiveMin = 10; //if( toLiveMin <= seeing boids <= toLiveMax )
const int toLiveMax = 25; //the boid tweet

const double INIT_VEL = 0.001;
const double MAX_VEL = 0.005;

const int CHANGE_PREY = 3000; //step
const int outputStep = 50;
const int exitStep = 5000; //step, when only the next param is true, it's work.

const bool stopPeriod = false;
const bool PREY = false;
const bool realTime = false; //don't use this

#endif