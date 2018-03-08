/*AUTHOR : PATIL AKHILESH SUBHASH 173079005 March-2018
ME 766 : High performance Computing , IIT Bombay 

Code for Comparative study of trapezoidal neumerical method of integration
with serial and openmp parallel code implementations.
Refer Report Document for trapezoidal concept and problem statement  

*/

#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>
#include<time.h>
#include<ctime>

//define number of trapezoids in the area of integration 
#define STEPS 20000000
//define number of parallel threds to use (1 to 8)
#define TH_NUM 8 

#define _USE_MATH_DEFINES
// pi is accesed via M_PI 
int main(){
	//define range of integration starting from 0 
	//here steps are number of trapezoids we are using for integration 
	
	int steps =STEPS;  
	//for measuring execution times 
	double start_time ; 
	double end_time ; 
	//set number of parallel threads 
	omp_set_num_threads(TH_NUM); 
	
	//lower limit of intergration 0 upper limit is Pi here 
	double start = 0 ; 
	//variable sum to contain sum of area in each iteration 
	double sum = 0 ;
	//height of each trapezoid calculated here  
	double step_size = (M_PI - start)/steps; 
	double x = 0 ; 
	double temp_sum = 0 ; 
	int i ; //iteration variable 
	double ser_time,par_time ; //for time calculations 


	start_time = omp_get_wtime(); //start real measuring time 

	//serial code
	//in each iteration area of trapezoid is calculated and added to sum 
	//total steps's number of trapezoids area is calculated and added 
	for(i=1;i <= steps;i++){
		temp_sum = (0.5)*(step_size)*( sin(x) + sin(x + step_size) ); 
		sum = sum + temp_sum ; 
		x = x + step_size ; 
		//printf("Upto Trapezoid %i Area = %.20f \n",i,sum);
	}
	end_time = omp_get_wtime(); //stop measuring real time 
	ser_time = end_time - start_time ; //time calculations 
	printf("Area of Integration using Trapezoidal rule is = %.64f \n", sum); 
	printf("Time taken serial: %f \n", ( (double) ( ser_time )));

//parallel code implementation 
	sum = 0 ; 
	x = 0 ;
	//to pass each thred amount of work(iterations) it needs to do 
	int step_lim = steps/TH_NUM ; 
	//variable to store thread id 
	int my_id ; 
		start_time = omp_get_wtime(); 
		//here reduction is used in openmp pragma because we are accesing shared 
		//varialbe sum 
#pragma omp parallel private(x,temp_sum,my_id,i) reduction(+:sum)
		{	//get thread ID 
			my_id = omp_get_thread_num();

			//depending on thread ID choose lower limit of integration i.e x 
			//here we devide integration area to number of threads and calculate 
			//area parallelly with TH_NUM of threads  
			switch(my_id){
				//for thread 0 
				case 0 : {
					x = 0 ; 
					break ; 
				}
				//for thread 1 
				case 1 : {
					x = step_size*(1*steps/TH_NUM);
					break ; 
				}
				//for thread 2 
				case 2 : {
					x = step_size*(2*steps/TH_NUM);
					break ; 
				}
				//for thread 3 
				case 3 : {
					x = step_size*(3*steps/TH_NUM);
					break; 
				}
				//for thread 4 
				case 4 :{
					x = step_size*(4*steps/TH_NUM);
					break; 
				}
				//for thread 5 
				case 5 :{
					x = step_size*(5*steps/TH_NUM);
					break; 
				}
				//for thread 6 
				case 6 :{
					x = step_size*(6*steps/TH_NUM);
					break; 
				}
				//for thread 7 
				case 7 :{
					x = step_size*(7*steps/TH_NUM);
					break; 
				}
			}
				//calculate sum of area of all trapezoids coming in each threads limits
			    // and add it  to global sum 
				for(i=1;i <= step_lim;i++){
						temp_sum = (0.5)*(step_size)*( sin(x) + sin(x + step_size) ); 
						sum = sum + temp_sum ; 
						x = x + step_size ; 
							}

		}



	end_time = omp_get_wtime();// stop real time measurement  
	par_time = end_time - start_time ; //time calculations 
	printf("Area of Integration using Trapezoidal rule is = %.64f \n", sum); 
	printf("Time taken parallel: %f \n", ( (double) ( par_time ) ));

//timing analysis 
printf("----------------------------\n"); 
//calculating and printing time difference between serial and parallel code 
printf("Differnce between serial and parallel (i.e Time saved) = %f\n ",(ser_time - par_time));  
//calculating percentage time saved with parallel code over serial code implementation 
printf("Percentage of time saved = %f\n ",((ser_time - par_time)*100/ser_time));  
//end of program 	

	return 0 ; 

}
