# include<iostream>
# include<vector>
using namespace std;

int bisection(vector<int> &pool,int index)
{
	int a = 0;
	int b = pool.size();
	int result = 0;
	while (1)
	{
		int c = (a+b)/2;
		if (pool[c] < index)
			a = c;
		else
			b = c;
		if (b - a == 1 && pool[b] == index)
		{	
			result = b;
			break;
		}
		if (b - a == 1 && pool[a] == index)
		{
			result = a;
			break;
		}
		if (b == a)
		{
			result = a;
			break;
		}

	}
	return result;
}

int bisection_double(vector<double> &vdInd_fraction,double dResearch)
{
	int a = 0;
	int b = vdInd_fraction.size()-1;
	int result = 0;
	if (vdInd_fraction[0] > dResearch)
		return 0;
	while (1)
	{
		int c = (a+b)/2;
		if (vdInd_fraction[c] <= dResearch)
			a = c;
		else
			b = c;
		if (b - a == 1 || b == a)
		{	
			result = b;
			break;
		}
	}
	return result;	
}