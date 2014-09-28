/*
  ==============================================================================
  
    syAmp
	
	bilinearTransform.cpp
	
	created by syor - April 2014

	* A VST - Analog Amp.
	* Build a z-domain model from s-domain model with bilinear transform
	* This model is a tonestack circuit of Fender Bassman'59 (David Yeh's Paper)

  ==============================================================================
*/

#include "bilinearTransform.h"


BilinearTransform::BilinearTransform()
{	

}

BilinearTransform::~BilinearTransform()
{
	resetBuffer();
	resetCoeffs();
}

void BilinearTransform::setSampleRate(float sampleRate)
{
	sampleRate_ = sampleRate;
	c_ = 2*sampleRate_;
}

void BilinearTransform::resetBuffer()
{	
	in_z_1 = 0.f;
	in_z_2 = 0.f;
	in_z_3 = 0.f;
	
	out_ = 0.f;
	out_z_1 = 0.f;
	out_z_2 = 0.f;
	out_z_3 = 0.f;
}

void BilinearTransform::resetCoeffs()
{	
	a_0 = 0.f;
	a_1 = 0.f;
	a_2 = 0.f;
	a_3 = 0.f;
	b_1	= 0.f;
	b_2 = 0.f;
	b_3 = 0.f;
	
	A_0 = 0.f;
	A_1 = 0.f;
	A_2 = 0.f; 
	A_3 = 0.f;
	B_0	= 0.f;
	B_1 = 0.f;
	B_2 = 0.f;
	B_3 = 0.f;
}

void BilinearTransform::getLow(float low)
{
	l_ = low;
}

void BilinearTransform::getMid(float mid)
{
	m_ = mid;
}

void BilinearTransform::getHigh(float high)
{
	t_ = high;
}

void BilinearTransform::setCoeffs()
{
	float C1 = 22.0e-12f;
	float C2 = 22.0e-12f;
	float C3 = 22.0e-12f;
	float R1 = 1.0e6f;
	float R2 = 1.0e6f;
	float R3 = 10.0e3f;
	float R4 = 100.0e3f;
	//H(s) =(b1s + b2s2 + b3s3)/ a0 + a1s + a2s2 + a3s3
	
	b_1 = t_*C1*R1 + m_*C3*R3 + l_*C1*R2 + l_*C2*R2 + C1*R3 + C2*R3;
	
	b_2 = t_*C1*C2*R1*R4 + t_*C1*C3*R1*R4 - m_*m_*C1*C3*R3*R3 + m_*m_*C2*C3*R3*R3
			+ m_*C1*C3*R1*R3 + m_*C1*C3*R3*R3 + m_*C2*C3*R3*R3
			+ l_*(C1*C2*R1*R2 + C1*C2*R2*R4 + C1*C3*R2*R4)
			+ l_*m_*(C1*C3*R2*R3 + C2*C3*R2*R3)
			+ (C1*C2*R1*R3 + C1*C2*R3*R4 + C1*C3*R3*R4);
	
	b_3 = l_*m_*C1*C2*C3*R1*R2*R3 + l_*m_*C1*C2*C3*R2*R3*R4 - m_*m_*C1*C2*C3*R1*R3*R3 + m_*m_*C1*C2*C3*R3*R3*R4
			+ m_*C1*C2*C3*R1*R3*R3 + m_*C1*C2*C3*R3*R3*R4
			+ t_*C1*C2*C3*R1*R3*R4 - t_*m_*C1*C2*C3*R1*R3*R4
			+ t_*l_*C1*C2*C3*R1*R2*R4;
			
	a_0 = 1;
	
	a_1 = (C1*R1 + C1*R3 + C2*R3 + C2*R4 + C3*R4) + m_*C3*R3 + l_*(C1*R2 + C2*R2);
	
	a_2 = m_*C1*C3*R1*R3 - m_*C2*C3*R3*R4 + m_*C1*C3*R3*R3 + m_*C2*C3*R3*R3 + l_*m_*C1*C3*R2*R3 + l_*m_*C2*C3*R2*R3
			- m_*m_*C1*C3*R3*R3 + m_*m_*C2*C3*R3*R3 + l_*C1*C2*R2*R4
			+ l_*C1*C2*R1*R2 + l_*C1*C3*R2*R4 + l_*C2*C3*R2*R4
			+ (C1*C2*R1*R4 + C1*C3*R1*R4 + C1*C2*R3*R4
			+ C1*C2*R1*R3 + C1*C3*R3*R4 + C2*C3*R3*R4);

	a_3 = l_*m_*(C1*C2*C3*R1*R2*R3 + C1*C2*C3*R2*R3*R4) - m_*m_*C1*C2*C3*R1*R3*R3 + m_*m_*C1*C2*C3*R3*R3*R4
			+ m_*C1*C2*C3*R3*R3*R4 + m_*C1*C2*C3*R1*R3*R3
			- m_*C1*C2*C3*R1*R3*R4 + l_*C1*C2*C3*R1*R2*R4 + C1*C2*C3*R1*R3*R4;
	
	
	B_0 = -b_1*c_ - b_2*c_*c_ - b_3*c_*c_*c_;
	B_1 = -b_1*c_ + b_2*c_*c_ + 3*b_3*c_*c_*c_;
	B_2 = b_1*c_ + b_2*c_*c_ - 3*b_3*c_*c_*c_;
	B_3 = b_1*c_ - b_2*c_*c_ + b_3*c_*c_*c_;

	A_0 = -a_0 - a_1*c_ - a_2*c_*c_ - a_3*c_*c_*c_;
	A_1 = -3*a_0 - a_1*c_ + a_2*c_*c_ + 3*a_3*c_*c_*c_;
	A_2 = -3*a_0 + a_1*c_ + a_2*c_*c_ - 3*a_3*c_*c_*c_;
	A_3 = -a_0 + a_1*c_ - a_2*c_*c_ + a_3*c_*c_*c_;
}

void BilinearTransform::process(float &in)
{
	out_ = (B_0*in + B_1*in_z_1 + B_2*in_z_2 + B_3*in_z_3 - A_1*out_z_1 - A_2*out_z_2 - A_3*out_z_3)/A_0;
	
	in_z_1 = in;
	in_z_2 = in_z_1;
	in_z_3 = in_z_2;
	
	out_z_1 = out_;
	out_z_2 = out_z_1;
	out_z_3 = out_z_2;
	
	in = out_;

}

