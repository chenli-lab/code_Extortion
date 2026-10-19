#include <iostream> 
#include <string>  
#include <sstream>      
#include <math.h>          
#include <fstream>          
#include <stdlib.h>        
#include <time.h>
#include <ctime>           
#include <queue>            
#include <stack>          
#include <iomanip> 
#include <algorithm>
#include <vector>    
#define PI 3.1415926
using namespace std;

void make1Darray(int*& array,int rows)
{
    array=new int [rows];
}

void delete1Darray(int *array)
{
    delete []array;
    array=0;
}

void make1Darray(double*& array,int rows)
{
    array=new double [rows];
}

void delete1Darray(double *array)
{
    delete []array;
    array=0;
}

void make2Darray(int **& array,int rows,int cols)
{
    array=new int *[rows];//make a point for every row
    for(int i=0;i<rows;i++)  //make a point for every col
        array[i]=new int [cols];
}

void delete2Darray(int **array,int rows)
{
    for(int i=0;i<rows;i++)
        delete []array[i];
    delete []array;
    array=0;
}

void make2Darray(double**& array,int rows,int cols)
{
    array=new double *[rows];//make a point for every row
    for(int i=0;i<rows;i++)  //make a point for every col
        array[i]=new double [cols];
}

void delete2Darray(double **array,int rows)
{
    for(int i=0;i<rows;i++)
        delete []array[i];
    delete []array;
    array=0;
}
void make3Darray(int *** &array,int high,int row,int col)
{
    int i,j;
    array=new int **[high];
    for(i=0;i<high;i++)
        array[i]=new int*[row];
    
    for(i=0;i<high;i++)
        for(j=0;j<row;j++)
            array[i][j]=new int[col];
}

void delete3Darray(int ***array,int hight, int row)
{
    int i,j;
    for(i=0;i<hight;i++)
        for(j=0;j<row;j++)
            delete []array[i][j] ;
    
    for(i=0;i<hight;i++)
        delete []array[i] ;
    delete []array;
    array=0 ;
}

void make3Darray(double ***&array,int high,int row,int col)
{
    int i,j;
    array=new double **[high];
    for(i=0;i<high;i++)
        array[i]=new double* [row];
    
    for(i=0;i<high;i++)
        for(j=0;j<row;j++)
            array[i][j]=new double[col];
}

void delete3Darray(double ***array,int hight, int row)
{
    int i,j;
    for(i=0;i<hight;i++)
        for(j=0;j<row;j++)
            delete []array[i][j] ;
    for(i=0;i<hight;i++)
        delete []array[i] ;
    delete []array;
    array=0 ;
}

#define N 2 
#define tmax 200000000

int main()
{
	int t,t_log,T;
	int FracC,FracD,FracE;
	int tNum;
	int NumC[501],NumD[501],NumE[501],NumCupdate[500],NumDupdate[500],NumEupdate[500];
	int i,j,k;
	int Cn,Dn,En;
	int *Action,*state,*state_new;
	int num_state,num_action;
	double ***Qtable,*Reward,Q_max,Q_max_next,payoff;
	double epsilon,alpha,gamma;
	double r;
	double t_log_exp;
	double phi,b,c;
	
	phi=1.5;   //extortion factor 
	b=2.0;
	c=b-1;   
	
	epsilon = 0.01;
	alpha = 0.1;       
	gamma = 0.9;
	
	num_state = 9;  //combin action of own and the neighbors
	num_action = 3;  
	
	t_log = int(pow(10,3));
	t_log_exp = 3;
	
	ofstream outfile1("Timeseries_phi1.5_b2.0_gamma0.9.txt");
    if(!outfile1)
    {
    	cout<<"You can't open the pointed file!"<<endl;
    	exit(1);
	}
	
	make1Darray(state,N);
	make1Darray(state_new,N);
	make1Darray(Action,N);
	make1Darray(Reward,N);
	make3Darray(Qtable,N,num_state,num_action);
	
	srand((unsigned)time(NULL));
	srand48((long)time(NULL));
	//--------------------Set initial condition -------------//	
		for(i=0;i<N;i++)
	    {
	        for(j=0;j<num_state;j++)
	        {
	            for(k=0;k<num_action;k++)
	            {
	                Qtable[i][j][k] = drand48();                  //all iterms in Q-table are set to be 0 (default)
				}
	        }
	        Action[i] = rand()%num_action;                         //set to be 0(T,T) or 1(T,N) 2(N,T) 3(N,N) randomly
	            
	        Reward[i] = 0;                                //No reward at the beginning       
	    }
	    //----------------Statistical state-----------------//
		for(i=0;i<N;i++)
		{
			j = abs(i-1);
			if(Action[i] == 0)  //C
			{
				if(Action[j] == 0)  //C
				{
					state[i] = 0;   //CC
				}
				else if(Action[j] == 1)  //D
				{
					state[i] = 1;  //CD
				}
				else          //E
				{
					state[i] = 2;   //CE
				}
			}
			else if(Action[i] == 1)  //D
			{
				if(Action[j] == 0) 
				{
					state[i] = 3;  //DC
				}
				else if(Action[j] == 1)  
				{
					state[i] = 4;  //DD
				}
				else
				{
					state[i] = 5; //DE
				}
			}
			else  //E
			{
				if(Action[j] == 0)  //EC
				{
					state[i] = 6;
				}
				else if(Action[j] == 1)  //ED
				{
					state[i] = 7;  
				}
				else   //EE
				{
					state[i] = 8; 
				}
			}
		}
			
		T = 0;
		for(t=1;t<tmax;t++)
		{
			//--------Choose an action------------//
			Cn = 0;
			Dn = 0;
			En = 0;
		    for(i=0;i<N;i++)
		    {
		        r = drand48(); 
		        if(r<epsilon)                         //Exploration with greedy algorithm
		        {
		            Action[i] = rand()%num_action;        
		        }
		        else                                  //Exploitation for the optimal action
		        {
		            Q_max = -1000;                     //starting with a very small value
		            for(k=0;k<num_action;k++)                                //go through all neighbors
		            {
		                if(Qtable[i][state[i]][k] > Q_max)                    //Search for the iterm with the largest Q value within the row with given state State[i]
		                {
		                    Q_max = Qtable[i][state[i]][k];
		                    Action[i] = k;                                     
		                }      
		            }
		        }
		        if(Action[i] == 0)
		        {
		            Cn++; 
				}
				else if(Action[i] == 1)
				{
					Dn++;
				}
				else
				{
					En++;	
				}
					
				Reward[i] = 0; 
		    }
		    
		    if(T<500)
		   	{
		   		NumC[T] = Cn;  //T is in [0,499] 
				NumD[T] = Dn;   
			    NumE[T] = En;   		
				
				T++;
			}
		
			else
			{
				NumC[T] = Cn;  // In here T is 500
				NumD[T] = Dn;
				NumE[T] = En;   
				 
				for(k=0;k<T;k++)  //elemate the element which T is 0 Add the new T=500
				{
					NumCupdate[k] = NumC[k+1]; 
					NumDupdate[k] = NumD[k+1];
					NumEupdate[k] = NumE[k+1];
				}
				for(k=0;k<T;k++)
				{
					NumC[k] = NumCupdate[k];
					NumD[k] = NumDupdate[k];
					NumE[k] = NumEupdate[k];
				}
				
				FracC = 0;
				FracD = 0;
				FracE = 0;
				for(k=0;k<T;k++)
				{
					FracC += NumC[k];
					FracD += NumD[k];
					FracE += NumE[k];		
				}
				
				if(t<1000&&t%10==0)
				{
					outfile1<<t<<"  "<<double(FracC)/(T*N)<<"  "<<double(FracD)/(T*N)<<"  "<<double(FracE)/(T*N)<<endl;
				}
				else if(t==t_log)
				{
					outfile1<<t<<"  "<<double(FracC)/(T*N)<<"  "<<double(FracD)/(T*N)<<"  "<<double(FracE)/(T*N)<<endl;
						
					t_log_exp += 0.0025;
		            t_log=int(pow(10,t_log_exp)); 
				}	
			
			}
			
			  //--------Perform action and measure the reward------------//
			for(i=0;i<N;i++)
		    {
		    	j = abs(i-1);
		    	
				payoff = 0;
		    	if(Action[i] == 0)   // i choose C
		    	{
					if(Action[j] == 0)  //CC
					{
						payoff =  b-c;  // R=1
					}
					else if(Action[j] == 1)  //CD
					{
						payoff = -c;   //b=0.1  S 	
					}	
					else     //CE
					{
						payoff = (pow(b,2)-pow(c,2))/(b*phi+c);	
					}	
				}
				else if(Action[i] == 1)   // i choose D
				{
					if(Action[j] == 0)    //DC
					{
						payoff = b;   // T
					}
					else if(Action[j] == 1)    //DD
					{
						payoff = 0; 
					}
					else        //DE
					{
						payoff = 0; 
					}
				}
				else     // i choose E
				{
					if(Action[j] == 0)    //EC
					{
						payoff = (pow(b,2)-pow(c,2))*phi/(b*phi+c);   
					}
					else if(Action[j] == 1)    //eD
					{
						payoff = 0; 
					}
					else        //EE
					{
						payoff = 0; 
					}
				}
				
				Reward[i] = payoff;
			}
			
			//--------------Statistics state-----------//
			for(i=0;i<N;i++)
			{
				j = abs(i-1);
				if(Action[i] == 0)
				{
					if(Action[j] == 0)  //CC
					{
						state_new[i] = 0;
					}
					else if(Action[j] == 1) 
					{
						state_new[i] = 1;  //CD
					}
					else
					{
						state_new[i] = 2;  //CE
					}
				}
				else if(Action[i] == 1)
				{
					if(Action[j] == 0)  //DC
					{
						state_new[i] = 3;
					}
					else if(Action[j] == 1) 
					{
						state_new[i] = 4;  //DD
					}
					else
					{
						state_new[i] = 5;  //DE
					}
				}
				else
				{
					if(Action[j] == 0)  //EC
					{
						state_new[i] = 6;
					}
					else if(Action[j] == 1) 
					{
						state_new[i] = 7;  //ED
					}
					else
					{
						state_new[i] = 8;  //EE
					}
				}
			}
			for(i=0;i<N;i++)
			{
			    Q_max_next = -1000;
			        		 
				for(k=0;k<num_action;k++)                                   //go through all actions
				{
				    if(Qtable[i][state_new[i]][k] > Q_max_next)                //Search for the iterm with the largest Q value
				    {
				        Q_max_next = Qtable[i][state_new[i]][k];
				    }
				}
				Qtable[i][state[i]][Action[i]] = (1-alpha)*Qtable[i][state[i]][Action[i]] + alpha*(Reward[i] + gamma*Q_max_next); 
			}
			for(i=0;i<N;i++)
			{
				state[i] = state_new[i];
			} 
			
		}
	    
	return 0;	
} 
