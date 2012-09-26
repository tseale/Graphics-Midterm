// controlpts.cpp
// implementation of the controlpts class
// Written by John Quinn

#include "controlpts.h"

controlpts::controlpts(double a, double b, double c)
{
	x=a;
	y=b;
	z=c;
}

double controlpts::getX()
{
	return x;
}
 
double controlpts::getY()
{
	return y;
}

double controlpts::getZ()
{
	return z;
}