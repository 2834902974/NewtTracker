#include "MathMethod.h"

double MathMethod::GetDistance(POINT p1, POINT p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void MathMethod::InitError(LONG SetValue, LONG RealValue)
{
    increase = 0.0;
    error_old = SetValue - RealValue;
    error_oldold = SetValue - RealValue;
}

LONG MathMethod::UpdateError(LONG SetValue, LONG RealValue)
{
    error_now = SetValue - RealValue;
    //Incremental PID
    increase = kp * (error_now - error_old) + ki * error_now + kd * (error_now - 2 * error_old + error_oldold); 
    error_oldold = error_old;
    error_old = error_now;
    return (LONG)increase;
}