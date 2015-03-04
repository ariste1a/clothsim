#include "key.h"


key::key()
{
	cubicCoeff = new Vector3(); 
}

key::key(key* copy)
{
	this->time = copy->time; 
	this->value = copy->value; 
	this->tanIn = copy->tanIn; 
	this->tanOut = copy->tanOut;
	this->cubicCoeff = copy->cubicCoeff;
	this->lerpTime = copy->lerpTime; 
	this->in = copy->in; 
	this->out = copy->out; 
	this->precomSpan = copy->precomSpan; 
}
void key::precomputeCoeff(Matrix34 hermite, float p0, float p1, float t0, float t1, float v0, float v1)
{
	Vector3 params = *new Vector3(p0, p1, (t1-t0)*v0, (t1-t0)*v1);
	Vector3 res = *new Vector3(); 
	hermite.VectorCross(params, *cubicCoeff); 
}

void key::inverseLerp(float t)
{
	//lerpTime = (t - t0) / (t1 - t0); 
	lerpTime = (this->time-t) * precomSpan;
}

void key::inverseLerp(float t0, float t, float t1)
{
	lerpTime = (t - t0) / (t1 - t0); 
	//lerpTime = (this->time - t) * precomSpan;
}

float key::evalSpan(float t)
{
	float a = cubicCoeff->x;
	float b = cubicCoeff->y;
	float c = cubicCoeff->z;
	float d = cubicCoeff->w;
	float u = lerpTime; 
	float x; 
		
	x = (a*u*u*u) + (b*u*u) + (c*u) + d;
	//return ((a * u + b) * u + c) * u + d;
	return x; 
}

key::~key()
{
}
