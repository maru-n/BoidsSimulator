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

//const int N = 256;
const int N = 32*32;

const double FIELD_SIZE = 1.0;

const double SIGHT_DISTANCE_SEPARATION = 0.04;
const double SIGHT_DISTANCE_ALIGNMENT = 0.07;
const double SIGHT_DISTANCE_COHESION = 0.07;
const double SIGHT_ANGLE_SEPARATION = 1.0;
const double SIGHT_ANGLE_ALIGNMENT = 1.0;
const double SIGHT_ANGLE_COHESION = 1.0;

const double COEFF_SEPARATION = 0.15; // 10^-4
const double COEFF_COHESION = 0.; // 3x 10^-4
const double COEFF_ALIGNMENT = 0.; // 10^-4

const double preyF = 0.000002;

const int toLiveMin = 10; //if( toLiveMin <= seeing boids <= toLiveMax )
const int toLiveMax = 25; //the boid tweet

const double MIN_VELOCITY = 0.01;
const double MAX_VELOCITY = 0.01;

const bool LOGGING = false;
const int CHANGE_PREY = 3000; //step
const int outputStep = 50;
const int exitStep = 5000; //step, when only the next param is true, it's work.

const bool stopPeriod = false;
const bool PREY = false;
const bool realTime = false; //don't use this

#endif