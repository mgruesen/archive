#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sys/time.h>
#include <omp.h>
using namespace std;
//number of dimensions
#define DIM 3
#define X 0
#define Y 1
#define Z 2
#define G 0.00000000006673
#define NUM_THREADS 16 
vector< vector<long double> > velocity;
vector< vector<long double> > position;
vector< vector<long double> > forces;
vector<long double> mass;
//number of objects
int n;
//starting time
long double t;


void printOutput(){
    cout.precision(16);
    cerr.precision(16);
    for(int i = 0; i < n; i++){
	cerr << position[i][X] << " " << position[i][Y] << endl;
    }
    cerr << endl << endl;
}

void writeOutput(){
    fstream fs ("out.dat", fstream::out);
    for (int i = 0; i < n; i++){
	fs << setprecision(12) << position[i][X] << " " << setprecision(12) << position[i][Y] << endl;
    }
    fs.close();
}
int main(int argc, char** argv){
    cout.precision(16);
    
    long double timestep;
    long double duration;
    long double output_time;
    long double output;
    if (argc < 4){
	cerr << "Invalid form. Usage is \'serial [timestep] [duration] [output_time]\'\n";
	return 1;
    }
    string arg = argv[1];
    timestep = std::stold(arg, nullptr);
    arg = argv[2];
    duration = std::stold(arg, nullptr);
    arg = argv[3];
    output_time = std::stold(arg, nullptr);

    //open file
    fstream fs ("data/tab512", fstream::in);
    if (fs.good()){//if it is good, proceed
	fs >> n;
	mass.resize(n, 0);
	velocity.resize(n, vector<long double>(DIM, 0));
	position.resize(n, vector<long double>(DIM, 0));
	fs >> t;
	for (int i = 0; i < n; i++){
	    //get mass of object i
	    fs >> mass[i];
	    //cout << "Mass: " << mass[i] << " X/Y Position: ";
	    for (int k = 0; k < DIM; k++){
		//get position in DIM dimensions for object i
		fs >> setprecision(16) >> position[i][k];
		//cout << setprecision(16) << position[i][k] << " ";
	    }
	    //cout << "X/Y Velocity: ";
	    for (int k = 0; k < DIM; k++){
		//get velocity in DIM dimensions for object i
		fs >> setprecision(16) >> velocity[i][k];
		//cout << setprecision(16) << velocity[i][k] << " ";
	    }
	    //cout << endl;
	}
    }
    else {
	fs.close();
	cerr << "Bad file data";
	return 1;
    }
    fs.close();
    /**
    for (int i = 0; i < n; i++){
	cout << "Object:" << i << " mass:" << mass[i];
	for (int j = 0; j < DIM; j++)
	    printf(" pos(%d):%Lf vel(%d):%Lf", j, position[i][j], j, velocity[i][j]);
	cout << endl;
    }
    **/
    //printf("Running simulation for %d objects, starting at time %Lf, stepping %Lf, until time %Lf. Output data every %d\n", n, t, timestep, duration, output_time);
    output = output_time;
    printOutput();
    struct timeval start;
    struct timeval finish;
    gettimeofday(&start, nullptr);
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel 
    {
    for (long double i = t; i < duration; i += timestep){
	if (fabs(i - output) < 0.000001){
	    #pragma omp single nowait
	    {
	    	printOutput();
	    	output += output_time;
	    }
	}
	//cout << "Time " << i << endl;
        #pragma omp single 
	forces.resize(n, vector<long double>(n, 0));
	#pragma omp for schedule(static, n / NUM_THREADS) 
	for (int q = 0; q < n; q++){
	    //compute force on object[q]
	    forces[q][X] = forces[q][Y] = 0;
	    for (int k = 0; k < n; k++){
		if (k != q){
		    //differences in x/y coordinates
	    	    long double x_diff = position[q][X] - position[k][X];
		    long double y_diff = position[q][Y] - position[k][Y];
		    //distance, distance cubed
		    long double dist = sqrt(x_diff*x_diff + y_diff*y_diff);
		    long double dist3 = dist * dist * dist;
		    //force
		    forces[q][X] -= ((G * mass[q] * mass[k]) / dist3) * x_diff;
		    forces[q][Y] -= ((G * mass[q] * mass[k]) / dist3) * y_diff;
		}
	   }
	}
	#pragma omp for schedule(static, n / NUM_THREADS)
	for (int q = 0; q < n; q++){
	    //compute position and velocity of object[k]
	    #pragma omp critical
	    {
	    	position[q][X] += timestep * velocity[q][X];
	    	position[q][Y] += timestep * velocity[q][Y];
	    	velocity[q][X] += timestep / mass[q] * forces[q][X];
	    	velocity[q][Y] += timestep / mass[q] * forces[q][Y];
	    }
	}
    }
    }
    gettimeofday(&finish, nullptr);
    printOutput();
    //writeOutput();
    int result = (finish.tv_sec * 1000000 + finish.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
    cerr << result << " usec\n";
    //cout << "done.\n";
    return 0;
}
