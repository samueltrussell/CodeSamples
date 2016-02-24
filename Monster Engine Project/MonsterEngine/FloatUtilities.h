//Function declarations for Testing various properties of floats

#ifndef FLOAT_EQUALITY_H
#define FLOAT_EQUALITY_H

namespace Engine{
	
	bool AboutEqualsRelative(float i_lhs, float i_rhs, float maxAbsoluteDiff);
	bool AboutEqualsRelative(float i_lhs, float i_rhs, float maxAbsoluteDiff, float maxRelativeDiff);
	bool IsNaN(float i_x);


}//end namespace Engine

#endif // End FLOAT_EQUALITY_H