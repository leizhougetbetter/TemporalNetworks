#include <set>
#include <vector>
using namespace std;

//Return a double in range [0,1)
double rand_double1();

//Return a double in range [0,1]
double rand_double2();

//Return a int in range [0,max)
int rand_int(int max);

int bisection(vector<int> &pool,int index);

//Individual
class Ind
{
public:
	Ind()
	{
		degree_ = 0;
		str_ = 0;
		payoff_ = 0.0;
		group_.clear();
	}
public:
	int degree_;
	int str_;
	double payoff_;
	set <int> group_;
};

double is_linked(vector <Ind> & ind_arr);
