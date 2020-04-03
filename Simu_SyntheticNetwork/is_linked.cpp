# include <vector>
# include <assert.h>
# include <iostream>
# include "Header.h"
using namespace std;

double is_linked(vector <Ind> & ind_arr)
{
    // judge if a network is connected. if not, make a slight modification to make it connected
	int N = int(ind_arr.size());
	int A = 0;
	int I = 0;
	vector <int> Q0;
	vector <int> Q1;
	vector <int> Q2;
	vector <int> Q3;
	vector <int> Q4;

	while(1)
	{
		Q0.clear();
		Q1.clear();
		Q2.clear();
		Q3.clear();
		Q4.clear();
		for (int i = 0;i<N;i++)
		{
			Q0.push_back(i);
		}
		Q0.erase(Q0.begin());
		Q1.push_back(0);
		Q4.push_back(0);
		while (Q1.size())
		{
			A = Q1[0];
 			Q1.erase(Q1.begin());
			Q2.push_back(A);
			for (set<int>::iterator it = ind_arr[A].group_.begin(); it != ind_arr[A].group_.end(); ++it)
			{
				for (int i = 0;i < Q0.size();i++)
				{
					if (Q0[i] > *it)
						break;
					if (Q0[i] == *it)
					{
						Q4.push_back(*it);
						Q0.erase(Q0.begin()+i);
						break;
					}
				}
				for (int i = 0;i < Q2.size();i++)
				{
					if (*it == Q2[i])
					{
						I = 1;
						break;
					}
				}
				if(I == 1)
				{
					I = 0;
					continue;
				}
				Q1.push_back(*it);
			}
		}
 
		if (Q0.size() > N/2)
		{
			Q0 = Q4;
		}
		if (Q0.size() > 0)
		{
			int index = 0;
			int delete_node = 0;
			int seperate_node = 0;
			for (int i = 0;i<N;i++)
			{
				if (ind_arr[i].degree_ > 1)
					Q3.push_back(i);
			}
			for (int i = 0;i<Q0.size();i++)
			{
				while(1)
				{
					if (Q3.size() == 0)
					cout<<"Q3 is wrong!"<<endl;
					index = rand_int(int(Q3.size()));
					seperate_node = Q3[index]; 
					if (ind_arr[seperate_node].degree_>1)
						break;
				}
				Q3.erase(Q3.begin()+index);	
				if (ind_arr[seperate_node].degree_ == 0)
					cout<<"seperate_node is wrong!"<<endl;
				index = rand_int(ind_arr[seperate_node].degree_);
                set<int>::iterator it = ind_arr[seperate_node].group_.begin();
				for (int j = 0;j<index;j++)
				{
					it++;
				}
				delete_node = *it;
				ind_arr[seperate_node].group_.erase(it);
				ind_arr[seperate_node].degree_ --;
			
				for (set<int>::iterator it1 = ind_arr[delete_node].group_.begin(); it1 != ind_arr[delete_node].group_.end(); ++it1)
				{
					if (*it1 == seperate_node)
					{
						ind_arr[delete_node].group_.erase(it1);
						break;
					}
				}
				ind_arr[Q0[i]].group_.insert(delete_node);
				ind_arr[Q0[i]].degree_ += 1;
				ind_arr[delete_node].group_.insert(Q0[i]);
			}
		}
		if (Q0.size() == 0)
			break;
	}
    return 1;
}
