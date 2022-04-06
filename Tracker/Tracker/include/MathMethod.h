#pragma once
#include <windows.h>
#include <cmath>
class MathMethod
{
//Fields
public:
    float kp = 0.2;
    float ki = 0.1;
    float kd = 0.1;
    float error_now = 0.0;
    float error_old = 0.0;
    float error_oldold = 0.0;
    float increase = 0.0;

//Methods
public:
	double GetDistance(POINT p1, POINT p2);

    void InitError(LONG SetValue, LONG RealValue);
    LONG UpdateError(LONG SetValue, LONG RealValue);


};