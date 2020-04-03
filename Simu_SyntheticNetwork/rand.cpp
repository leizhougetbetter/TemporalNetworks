#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Return a double in range [0,1)
double rand_double1()
{
	return (double) rand()/((double)RAND_MAX+1);
}

//Return a double in range [0,1]
double rand_double2()
{
	double p = (double) (rand())/((double)RAND_MAX);
	if (p>1.0)
	{
		p = 1.0;
	}
	return p;
}

//Return a int in range [0,max)
int rand_int(int max)
{
	double a = rand_double1();
	return (int)(a*max)%max;
}