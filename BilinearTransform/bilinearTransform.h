/*
  ==============================================================================
  
    syAmp
	
	bilinearTransform.h
	
	created by syor - April 2014

	* A VST - Analog Amp.
	* Build a z-domain model from s-domain model with bilinear transform
	* This model is a tonestack circuit of Fender Bassman'59 (David Yeh's Paper)

  ==============================================================================
*/

#ifndef BILINEARTRANSFORM_H
#define BILINEARTRANSFORM_H

#include <math.h>

class BilinearTransform
{
public:
	BilinearTransform();
	~BilinearTransform();
	
	void setSampleRate(float sampleRate_);
	
	void resetBuffer();
	void resetCoeffs();
	
	void getLow(float low);
	void getMid(float mid);
	void getHigh(float high);
	
	void setCoeffs();
	
	void process(float &in);
  
private:
	float sampleRate_;
	
	float c_;
	
	float l_;
	float m_;
	float t_;
	
	float a_0;
	float a_1;
	float a_2;
	float a_3;
	float b_1;
	float b_2;
	float b_3;
	
	float A_0;
	float A_1;
	float A_2;
	float A_3;
	float B_0;
	float B_1;
	float B_2;
	float B_3;
	
	float in_z_1;
	float in_z_2;
	float in_z_3;
	float out_;
	float out_z_1;
	float out_z_2;
	float out_z_3;
	
};

#endif