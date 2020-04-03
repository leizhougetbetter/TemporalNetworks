#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include <set>
#include <assert.h>
#include <math.h>
#include <utility>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;

//return a double in range [0,1)
double rand_double1()
{
    return (double) rand()/((double)RAND_MAX+1);
}

//return a double in range [0,1]
double rand_double2()
{
    double p = (double) (rand())/((double)RAND_MAX);
    if (p>1.0)
    {
        p = 1.0;
    }
    return p;
}

//return an int in range [0,max)
int rand_int(int max)
{
    double a = rand_double1();
    return (int)(a*max)%max;
}

//the definition of Ind class---each individual/node has four properties (1) degree_, the number of neighbors; (2) group_, the set of neighbors; (3) str_, strategy; (4) payoff_, the accumulated payoff
class Ind
{
public:
    Ind()
    {
        degree_ = 0;
        group_.clear();
        str_ = 0;
        payoff_ = 0.0;
    }
public:
    int degree_;
    set <int> group_;
    int str_;
    double payoff_;
};


// generate an ER network with average degree k
void er(vector<Ind> &ind_arr,int k);

// judge // judge if a network is connected. if not, make a slight modification to make it connected
double is_linked(vector <Ind> & ind_arr);

int bisection(vector<int> &pool,int index);

// generate M ER networks with average degree k
void er_original(int N, int k, int M);

// use M generated ER networks to form a temporal network with M snapshots. that is, in each ER network, a fraction p of edges are activated to be a snapshot of the temporal network
void er_temporal(int N, int k, int M, double p);

// evolution in the generated temporal network with M snapshots. in each snapshot, the system experiences g generations
void evo_temporal(int N, int k, int M, double p, int g);

// evolution in the static network
void evo_static(int N, int k,int M);


int main()
{
	srand((int)time(0));

	int N = 1000;    //network size
	int k = 10;      //average degree
	int M = 100;     //number of snapshots
	double p = 0.3;  //a fraction p of edges are activated when constructing the temporal networks
	int g = 100;
    
    
    er_original(N, k, M);

    er_temporal(N, k, M, p);

	evo_temporal(N, k, M, p, g);
    
    evo_static(N, k, M);

	return 0;
}


void er(vector<Ind> &ind_arr,int k)
{
    //Construct ER random network
    int N = int(ind_arr.size());
    int edge_num = int(N*k*0.5);
    int start = 0;
    int end = 0;
    int index = 0;
    int value = 0;
    int link_num = 0;
    int ind2 = N*N;
    vector <int> pool;
    for (int i = 0;i < ind2;i++)
    {
        pool.push_back(i);
    }
    while(edge_num-link_num)
    {
        index = rand_int(int(pool.size()));
        value = pool[index];
        start = value/N;
        end = value%N;
        if (start == end)
            continue;
        pool.erase(pool.begin()+index);
        value = N*end+start;
        index = bisection(pool,value);
        pool.erase(pool.begin()+index);
        ind_arr[start].group_.insert(end);
        ind_arr[end].group_.insert(start);
        link_num += 1;
    }
    for (int i = 0;i<N;i++)
    {
        ind_arr[i].degree_ = int(ind_arr[i].group_.size());
    }
    is_linked(ind_arr);
}



void er_original(int N, int k, int M)
{
    //construct M copies of a ER random network and output the list of edges
    ofstream outfile;
    string filename_out;
    stringstream s_out;
    s_out << "edgelist_original.txt";
    filename_out = s_out.str();
    outfile.open(filename_out.c_str());
        
    Ind tmp;
    vector <Ind> ind_arr(N,tmp);
    er(ind_arr,k);
    for (int m=0;m<M;m++)
    {
        int link_num = 0;
        for (int i = 0;i<N;i++)
        {
            for (set<int>::iterator it = ind_arr[i].group_.begin(); it != ind_arr[i].group_.end(); ++it)
            {
                if (i>*it)
                    continue;
                outfile<<m<<" ";                 //network id
                outfile<<link_num<<" ";          //edge id
                outfile<<i<<" ";                 //node id
                outfile<<*it<<" "<<endl;         //node id
                link_num++;
            }
        }
    }
}



void er_temporal(int N, int k, int M, double p)
{
    ofstream outfile;
    string filename_out;
    stringstream s_out;
    s_out << "edgelist_temporal.txt";
    filename_out = s_out.str();
    outfile.open(filename_out.c_str());

    stringstream s_in;
    s_in << "edgelist_original.txt";
    string filename_in;
    filename_in = s_in.str();
    ifstream infile(filename_in.c_str(), std::ios::in);
    char line[1024];
    
    
    int link_num_original = N*k*0.5;  //number of edges
    int link_num_proportional = N*k*0.5*p;  //number of edges to be activiated
    set <int> link_id_proportional;
    
    int line_cnt = 0;
    while (infile.getline(line, sizeof(line), '\n'))
    {
        //get the edge sequence of a ER network and activate a fraction p of edges to be a snapshot of the temporal network
        if (line_cnt == 0)
        {
            line_cnt = link_num_original;
            link_id_proportional.clear();
            int link_num_selected = link_num_proportional;
            int link_id_selected;
            set<int>::iterator it;
            while(link_num_selected)
            {
                link_id_selected = rand_int(link_num_original);
                it = link_id_proportional.find(link_id_selected);
                if (it == link_id_proportional.end())
                {
                    link_id_proportional.insert(link_id_selected);
                    link_num_selected --;
                }
            }
        }

        stringstream word(line);
        string xx;
        int info[4];
        for (int i=0;i<4;i++)
        {
            word >> xx;
            info[i] = stoi(xx);
        }
        set<int>::iterator it;
        it = link_id_proportional.find(info[1]);
        if (it != link_id_proportional.end())
        {
            outfile<<info[0]<<" "<<info[1]<<" "<<info[2]<<" "<<info[3]<<endl;
        }

        line_cnt--;
    }
    infile.close();
}




void evo_temporal(int N, int k, int M, double p, int g)
{
    ofstream outfile;
    string filename_out;
    stringstream s_out;
    s_out << "evo_temporal.txt";
    filename_out = s_out.str();
    outfile.open(filename_out.c_str());

    //input edge sequence of a termporal network
    stringstream s_in;
    s_in << "edgelist_temporal.txt";
    string filename_in;
    filename_in = s_in.str();
    ifstream infile(filename_in.c_str(), std::ios::in);
    char line[1024];
    
    int link_num_proportional = int(N*k*0.5*p); //number of edges in each snapshot of the temporal network
	vector <vector<int>> info(4);
	for (int i = 0; i < 4; i++)
		info[i].resize(M*link_num_proportional);

	int line_cnt = 0;
	while (infile.getline(line, sizeof(line), '\n'))
	{
		stringstream word(line);
		string xx;
		for (int i = 0; i < 4; i++)
		{
			word >> xx;
			info[i][line_cnt] = stoi(xx);
		}
		line_cnt++;
	}
	infile.close();

    //evolution on temporal networks
	double paymat[2][2] = { {0,1.001},{0,1} };
    int R = 100;
	int G = g * M * R;
	int C_num = 0;
	double fc = 0.0;
	vector <int> ind_str(N, 0);

	for (int b = 0; b < 21; b++)
	{
        std::cout<<b<<endl;
		paymat[0][1] = 1 + 0.05*b;
		outfile << paymat[0][1] << " ";
		for (int c = 0; c < 100; c++)
		{
			for (int i = 0; i < N; i++)
			{
				ind_str[i] = rand_int(2);
			}

			int times = 0;
			for (int n = 0; n < R; n++)
			{
				for (int m = 0; m < M; m++)
				{
                    //generate m_th snapshot from the edge list
					Ind tmp;
					vector <Ind> ind_arr(N, tmp);
					int link_start = link_num_proportional * m;
					int link_end = link_num_proportional * (m + 1);
					for (int j = link_start; j < link_end; j++)
					{
						ind_arr[info[2][j]].group_.insert(info[3][j]);
						ind_arr[info[3][j]].group_.insert(info[2][j]);
					}

                    //in each snapshot, the system experiences g generations
					for (int t = 0; t < g; t++)
					{
						for (int j = 0; j < N; j++)
						{
							ind_arr[j].degree_ = int(ind_arr[j].group_.size());
							ind_arr[j].payoff_ = 0;
							ind_arr[j].str_ = ind_str[j];
						}
						int obj1;
						int obj2;
                        
                        //play games
						for (int j = link_start; j < link_end; j++)
						{
							obj1 = info[2][j];
							obj2 = info[3][j];
							ind_arr[obj1].payoff_ += paymat[ind_arr[obj1].str_][ind_arr[obj2].str_];
							ind_arr[obj2].payoff_ += paymat[ind_arr[obj2].str_][ind_arr[obj1].str_];
						}
                        //strategy udpate
						for (int j = 0; j < N; j++)
						{
							if (ind_arr[j].degree_ == 0)
								continue;
							int imitatee_num = rand_int(ind_arr[j].degree_);
							int imitatee = 0;
							for (set<int>::iterator it = ind_arr[j].group_.begin(); it != ind_arr[j].group_.end(); ++it)
							{
								if (imitatee_num == 0)
								{
									imitatee = *it;
									break;
								}
								imitatee_num--;
							}
							if (ind_arr[j].payoff_ >= ind_arr[imitatee].payoff_)
								continue;
							double paydif = (paymat[0][1] - paymat[1][0])*max(ind_arr[j].degree_, ind_arr[imitatee].degree_);
							double toss = (ind_arr[imitatee].payoff_ - ind_arr[j].payoff_) / paydif;
							if (toss > rand_double1())
								ind_str[j] = ind_arr[imitatee].str_;
						}
                        
                        //record the trait frequency over last 2000 generations
						times++;
						if (G - times < 2000)
						{
							C_num = 0;
							for (int j = 0; j < N; j++)
								C_num += ind_str[j];
							fc += C_num;
						}
					}
				}
			}
			outfile << fc / (N * 2000) << " ";
			fc = 0;
		}
		outfile << endl;
	}
}


void evo_static(int N, int k, int M)
{
    ofstream outfile;
    string filename_out;
    stringstream s_out;
    s_out << "evo_static.txt";
    filename_out = s_out.str();
    outfile.open(filename_out.c_str());

    stringstream s_in;
    s_in << "edgelist_temporal.txt";
    string filename_in;
    filename_in = s_in.str();
    ifstream infile(filename_in.c_str(), std::ios::in);
    char line[1024];

    //input the edge sequence of the temporal network and generate the static network
    Ind tmp;
    vector <Ind> ind_arr(N, tmp);
    int info[4] = { 0 };
    while (infile.getline(line, sizeof(line), '\n'))
    {
        stringstream word(line);
        string xx;
        for (int i = 0; i < 4; i++)
        {
            word >> xx;
            info[i] = stoi(xx);
        }
        ind_arr[info[2]].group_.insert(info[3]);
        ind_arr[info[3]].group_.insert(info[2]);
    }
    infile.close();

    
    //evolution on the static network
    double paymat[2][2] = {{0,1.001},{0,1}};
    int G = 1000000;
    double fc = 0.0;
    vector <int> ind_str(N,0);

    for (int b=0;b<21;b++)
    {
        paymat[0][1] = 1+0.05*b;
        outfile<<paymat[0][1]<<" ";
        for (int c=0;c<100;c++)
        {
            for (int i=0;i<N;i++)
            {
                ind_str[i] = rand_int(2);
            }

            for (int t=0;t<1000000;t++)
            {
                for (int j=0;j<N;j++)
                {
                    ind_arr[j].degree_ = int(ind_arr[j].group_.size());
                    ind_arr[j].payoff_ = 0;
                    ind_arr[j].str_ = ind_str[j];
                }

                //play games
                for (int j=0;j<N;j++)
                {
                    for (set<int>::iterator it = ind_arr[j].group_.begin(); it != ind_arr[j].group_.end(); ++it)
                        ind_arr[j].payoff_ += paymat[ind_arr[j].str_][ind_arr[*it].str_];
                }

                //strategy udpate
                for (int j=0;j<N;j++)
                {
                    if(int(ind_arr[j].degree_) == 0)
                        continue;
                    int imitatee_num = rand_int(int(ind_arr[j].degree_));
                    int imitatee = 0;
                    for (set<int>::iterator it = ind_arr[j].group_.begin(); it != ind_arr[j].group_.end(); ++it)
                    {
                        if (imitatee_num==0)
                        {
                            imitatee = *it;
                            break;
                        }
                        imitatee_num --;
                    }
                    if (ind_arr[j].payoff_ >= ind_arr[imitatee].payoff_)
                        continue;
                    double paydif = (paymat[0][1]-paymat[1][0])*max(ind_arr[j].degree_,ind_arr[imitatee].degree_);
                    double toss = (ind_arr[imitatee].payoff_-ind_arr[j].payoff_)/paydif;
                    if (toss > rand_double1())
                        ind_str[j] = ind_arr[imitatee].str_;
                }
                if (G-t<=2000)
                {
                    for (int j=0;j<N;j++)
                        fc += ind_str[j];
                }
            }
            outfile<<fc/(N*2000)<<" ";
            fc = 0;
        }
        outfile<<endl;
    }
}


int bisection(vector<int> &pool,int index)
{
    int a = 0;
    int b = int(pool.size());
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

