#include <cstdlib>
#include <iostream>
#include <random>
#include <omp.h>
using namespace std;

int main(int argc,char **argv)
{
    vector<double> vecx;
    vector<double> vecy;
    #pragma omp parallel private(vecx,vecy)
    {
	int size= omp_get_num_threads();
	
    #pragma omp master
       	cout << argv[1] << endl;

    	int N=atoi(argv[1])/size;
	int rank= omp_get_thread_num();
	uniform_real_distribution<double> uni(0, 1);
        mt19937 eng(rank); //GIVE EACH THREAD ITS OWN ENGINE

        for(int i=0; i<N; i++)
        {
            vecx.push_back(uni(eng));
            vecy.push_back(uni(eng));
        }

        #pragma omp critical
	for(int j=0;j<N;j++) cout << vecx[j] <<" "<<vecy[j] << endl;
    }
	return 0;
}
