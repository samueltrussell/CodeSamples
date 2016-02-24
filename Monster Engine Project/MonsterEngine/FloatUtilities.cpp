//Function definitions for testing various properties of floats

#include "FloatUtilities.h"
#include <math.h>
#include <float.h>

namespace Engine{
	
	bool AboutEqualsRelative(float i_lhs, float i_rhs, float maxAbsoluteDiff)
	{
		return AboutEqualsRelative(i_lhs, i_rhs, maxAbsoluteDiff, FLT_EPSILON);
	}

	bool AboutEqualsRelative(float i_lhs, float i_rhs, float maxAbsoluteDiff, float maxRelativeDiff)
	{
		// Check if the numbers are really close -- needed
		// when comparing numbers near zero.
		float diff = (float)fabs(i_lhs - i_rhs);
		if (diff <= maxAbsoluteDiff)
			return true;

		i_lhs = (float)fabs(i_lhs);
		i_rhs = (float)fabs(i_rhs);
		float largest = (i_rhs > i_lhs) ? i_rhs : i_lhs;

		if (diff <= largest * maxRelativeDiff)
			return true;
		return false;
	}

	bool IsNan(float i_x)
	{
		volatile float val = i_x;

		return val != val;
	}


}