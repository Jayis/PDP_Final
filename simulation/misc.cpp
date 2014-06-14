#include "misc.h"

string num2string ( const int& num )
{
	ostringstream convert;

	convert << num;

	return convert.str();
}