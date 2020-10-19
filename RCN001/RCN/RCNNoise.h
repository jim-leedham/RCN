/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNNoise.h
Description:    Contains a 2D Simplex noise function comparable to Perlin 
				noise but with lower computational overhead. 

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNNOISE_H_
#define _RCN_RCNNOISE_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNNoise
{
public:

	static void		Initialise();

	static double	GetNoise(double fX, double fY);

private:

	static int		FastFloor(double dX);

	static int		ms_iGradients[12][3];
	static int		ms_iPermutations[512];
	static int		ms_iPermutationsMod12[512];
};

#endif	// _RCN_RCNNOISE_H_