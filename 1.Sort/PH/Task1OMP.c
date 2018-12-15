#include <omp.h>
#include <iostream>
#include <cstring>
#include <climits>
#include <fstream>

#define ARRAYSIZE 262144
#define THREADS 8

int main(int argc, char** argv){
	std::ofstream logfile;

	char filename[] = "Task1OMP.logs";
	remove( filename );
	logfile.open(filename, std::fstream::out);

	if (!logfile ) 
  	{
        // std::cout << "Cannot open file, file does not exist. Creating new file..";
        logfile.open(filename,  std::fstream::in | std::fstream::out);
        logfile <<"\n";
        logfile.close();
   	} 
   	if(argc > 1)
   		srand(std::stoi(argv[0]));
   	else
		srand(0);
	int UnSortedArray[ARRAYSIZE] = {};
	for(int i=0;i<=ARRAYSIZE;i++){
		UnSortedArray[i] = rand();
	}

	logfile << "UnSortedArray" << std::endl << std::endl;

	int MergeSortArray[THREADS][ARRAYSIZE/THREADS] = {};

	#pragma omp parallel for
	for(int i=0;i<=THREADS;i++){
		std::memcpy(&MergeSortArray[i],&UnSortedArray[i*(ARRAYSIZE/THREADS)],sizeof(int)*(ARRAYSIZE/THREADS));
	}

	int done = 0;
	#pragma omp parallel for
	for(int THREADNUM=0;THREADNUM<=THREADS;THREADNUM++){
		while(done == 0){

			for(int i=1;i<(ARRAYSIZE/THREADS);i++){
				if(MergeSortArray[THREADNUM][i-1]>MergeSortArray[THREADNUM][i]){
					int temp = MergeSortArray[THREADNUM][i];
					MergeSortArray[THREADNUM][i] = MergeSortArray[THREADNUM][i-1];
					MergeSortArray[THREADNUM][i-1]=temp;
				}
			}
			for(int i=1;i<ARRAYSIZE/THREADS;i++){
				if(MergeSortArray[THREADNUM][i-1]>MergeSortArray[THREADNUM][i]){
					done=0;
					break;
				}
				done=1;
			}
		}
		done=0;
	}

	for(int THREADNUM=0;THREADNUM<THREADS;THREADNUM++){
		for(int i = 0;i < ARRAYSIZE/THREADS;i++){
			logfile << MergeSortArray[THREADNUM][i] <<  " " ;
		}

		// std::cout << std::endl << std::endl;
	}

	logfile << std::endl << std::endl << "MergeSortArray" << std::endl << std::endl;
	
	int iterator[THREADS];
	int SortedArray[ARRAYSIZE];
	int threadChosen = 0;
	int smallest = MergeSortArray[0][0];
	
	// #pragma omp parallel for
	for(int i=0;i<ARRAYSIZE;i++){
		for(int j = 0;j<THREADS;j++){
			if (iterator[j] <=(ARRAYSIZE/THREADS)-1)
				if(smallest > MergeSortArray[j][iterator[j]]){
					smallest = MergeSortArray[j][iterator[j]];
					threadChosen = j;
				}
		}
		SortedArray[i] = smallest;
		iterator[threadChosen] = iterator[threadChosen] + 1;
		smallest = INT_MAX;
	}

	for(int i=0;i<ARRAYSIZE;i++){
		 logfile << SortedArray[i] << " ";
	}
	logfile.close();
	return 0;
}

