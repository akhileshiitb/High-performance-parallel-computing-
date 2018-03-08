/*AUTHOR : PATIL AKHILESH SUBHASH 173079005 March-2018
ME 766 : High performance Computing , IIT Bombay 

Code for Comparative study of monte carlo neumerical method of integration
with serial and openmp parallel code implementations.
Refer Report Document for monte calro concept and problem statement  

*/
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>
#include<time.h>
#include<ctime>
//define number of random sample points to generate here 
#define SAMPLES 10000000
//mention number of parallel threads to use while implementing integration in openmp
#define TH_NUM 4

//function to generate random number 0 to num
unsigned int seed = time(NULL); //seed value for rand_r() function 
double random(double num){
	//use rand_r() istead of rand() in multithreading applications 
	double x ; 
	x = ((double)rand_r(&seed)/(double)(RAND_MAX)) * num;
	return x ; 
}


//function to check weather random point is inside or outside our curve y = sin(x)
bool is_inside_sine(double x , double y){
	double temp ; 
	temp = y - sin(x); 
	if(temp < 0){
		return true ; 
	}else{
		return false ; 
	}
}


int main(){ 
	omp_set_num_threads(TH_NUM);
	int i=0 ; 
	bool check ; 
	int inside_count = 0 ; //buffer to store number of random poins lying inside curve 
	int outside_count = 0 ; //buffer to store number of random points laying outside curve
	double x ; 
	double y ;  
	double area ; // variable to store calculated area from monte carlo method 
	//consider rectangle in monte carlo , integration from 0 to pi hence area = pi  
	double rectangle_area = 3.14 ; 
	//start and end variables to hold real time 
	double start ; 
	double end ; 
	int k ; 
	//serial and parallel implementation execution times 
	double ser_time,par_time; 
	 
//serial code implementation 
	start = omp_get_wtime(); //start timer 

//main for loop 
//generate random point inside rectangle and check how many of them laying inside 
//and hoe many lying outside the rectangle , 
//accordingly modify variables inside_count and outside_count 	
	for(i=1;i<=SAMPLES;i++){
		x = random(3.14); 
		y = random(1); //random position 
		check = is_inside_sine(x,y); 
		 
		if(check == true){
			inside_count = inside_count + 1 ; 
		}else {
			outside_count = outside_count + 1 ; 
		}
	}

		

end = omp_get_wtime();  //stop timer 

//calulating area by porportions (monte carlo method) 
area = rectangle_area * ((double)inside_count/(double)(outside_count + inside_count)); 
printf("Total points : %d ",inside_count + outside_count); 
printf("Inside : %d , Outside : %d \n",inside_count , outside_count); 
printf("value of integration : %f \n",area);
//calculating and printing execution time for serial code 
ser_time = end - start ; 
printf("Time taken serial: %f \n", ( (double) ( ser_time ) ));

//reset inside_count and outside_count variables for resuing it in parallel implementation
inside_count = 0 ; 
outside_count = 0 ;


//parallel code implementation 
start = omp_get_wtime(); //start timer 
	
	#pragma omp parallel private(i,x,y,check) reduction(+:inside_count,outside_count)
{	//each thread will process samples/th_num many of points and update count variables accordingly 
	//each thread will do same process but with less num of samples , but when combined 
	//they will have processed same number of samples as serial code , but much faster 
	for(i=1;i<=SAMPLES/TH_NUM;i++){
		x = random(3.14); 
		y = random(1); //random position 
		 
		check = is_inside_sine(x,y); 
		if(check == true){
			inside_count = inside_count + 1 ; 
		}else{
			outside_count = outside_count + 1 ; 
		}
	}
}
end = omp_get_wtime(); //stop time 

	//calulating area by porportions 
	area = rectangle_area * ((double)inside_count/(double)(inside_count + outside_count)); 
printf("Total points : %d ",inside_count + outside_count); 
printf("Inside : %d , Outside : %d \n",inside_count , outside_count); 
printf("value of integration : %f \n",area);
//execution time calculations 
par_time = end - start ; 
printf("Time taken parallel: %f \n", ( (double) ( par_time )  ));

//analysis section of serial vs parallel code 

printf("----------------------------\n"); 
//calculating and printing time difference between serial and parallel code 
printf("Differnce between serial and parallel (i.e Time saved) = %f\n ",(ser_time - par_time));  
//calculating percentage time saved with parallel code over serial code implementation 
printf("Percentage of time saved = %f\n ",((ser_time - par_time)*100/ser_time));  
//end of program 	
	return 0 ; 
}

