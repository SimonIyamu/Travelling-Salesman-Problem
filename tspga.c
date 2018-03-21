#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int P=100;
int G=10000;
double PC=0.5;
double PM=0.1;

double dist(int x1,int y1,int x2,int y2){
	return sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
}

int main(int argc,char* argv[]){
	int towns,t,*times,i,z,w,j,length,loc,*temp,*best,**A,**B,**C,gens=0,cut1,cut2,r1,r2,x,k,*check;
	double f[P],sum,r,F[P],sf,bf,bf_all,**d;
	char flag,ied=0;
	srand(time(NULL));
	if(argc>1){
		for(k=1 ; k<argc ; k++){
			if(!strcmp(argv[k],"-p"))
				P=atoi(argv[k+1]);
			if(!strcmp(argv[k],"-g"))
				G=atoi(argv[k+1]);
			if(!strcmp(argv[k],"-pc"))
				PC=atof(argv[k+1]);
			if(!strcmp(argv[k],"-pm"))
				PM=atof(argv[k+1]);
	
			if(!strcmp(argv[k],"-i")){
				ied=1;
				FILE *fp;
				char dim[]="DIMENSION: ",data[]="NODE_COORD_SECTION",c,val[10],boo=1;
				fp=fopen(argv[k+1],"rb");
				if(fp==NULL){
					printf("The file does not exist in this directory.\n");
					return 1;
				}


				i=0;
				while(!feof(fp)){		/* Look for DIMENSION:  or DIMENSION : in the file */
					c=fgetc(fp);
					if(c==dim[i])
						i++;
					else
						i=0;
					if(i==strlen(dim)){
						boo=0;
						break;
					}
				}
				if(boo){
					char dim2[]="DIMENSION : ";
					fp=fopen(argv[k+1],"rb");	
					i=0;
					while(!feof(fp)){
						c=fgetc(fp);
						if(c==dim2[i])
							i++;
						else
							i=0;
						if(i==strlen(dim2)){
							boo=0;
							break;
						}
					}
				}

				if(boo){
					printf("Wrong file\n");
					return 1;
				}
				i=0;
				c=fgetc(fp);
				while(c!=10){
					val[i]=c;
					c=fgetc(fp);
					i++;
				}
				towns=atoi(val);



				fp=fopen(argv[k+1],"rb");
				i=0;
				while(!feof(fp)){		/* Look for NODE_CORD_SECTION in the file */
					c=fgetc(fp);
					if(c==data[i])
						i++;
					else
						i=0;
					if(i==strlen(data)){
						boo=0;
						break;
					}
				}

				if(boo){
					printf("Wrong file\n");
					return 1;
				}

				double cord[towns][2];

				
				t=0;
				c=fgetc(fp);	
				while(c!='E'){
					c=fgetc(fp);
					while(c==' ')			/*skips all spaces till the town number*/
						c=fgetc(fp);
					while(c!=' ')			/*skips the town number */
						c=fgetc(fp);	
					c=fgetc(fp);
					while(c==' ')			/*skips all spaces till x*/
						c=fgetc(fp);

					for(i=0;i<10;i++)
						val[i]=0;
					i=0;
					while(c!=' '){			/*saves x */
						val[i]=c;
						c=fgetc(fp);
						i++;
					}
					cord[t][0]=atof(val);

					c=fgetc(fp);
					while(c==' ')			/*skips all spaces till y*/
						c=fgetc(fp);

					for(i=0;i<10;i++)
						val[i]=0;
					i=0;
					while(c!='\n'){			/*saves y*/
						val[i]=c;
						c=fgetc(fp);
						i++;
					}
					cord[t][1]=atoi(val);
					t++;

					c=fgetc(fp);
				}

				d=malloc(towns*sizeof(int *));
				if(d==NULL){
					printf("Memory Error\n");
					return -1;
				}
				for(i=0 ; i<towns ; i++){
					d[i]=malloc(towns*sizeof(int));
					if(d[i]==NULL){
						printf("Memory Error\n");
						return -1;
					}
				}

				for(i=0 ; i<towns ; i++)
					for(j=i ; j<towns ; j++)
						if(i==j)
							d[i][j]=0;
						else{
							d[i][j]=dist(cord[i][0],cord[i][1],cord[j][0],cord[j][1]);
							d[j][i]=d[i][j];
						}
				fclose(fp);
			}
		}
	}




	if(!ied)
		scanf("%d",&towns);

	A=malloc(P*sizeof(int *));
	if(A==NULL){
		printf("Memory Error\n");
		return -1;
	}
	for(i=0 ; i<P ; i++){
		A[i]=malloc(towns*sizeof(int));
		if(A[i]==NULL){
			printf("Memory Error\n");
			return -1;
		}
	}
	B=malloc(P*sizeof(int *));
	if(B==NULL){
		printf("Memory Error\n");
		return -1;
	}
	for(i=0 ; i<P ; i++){
		B[i]=malloc(towns*sizeof(int));
		if(B[i]==NULL){
			printf("Memory Error\n");
			return -1;
		}
	}
	C=malloc(P*sizeof(int *));
	if(C==NULL){
		printf("Memory Error\n");
		return -1;
	}
	for(i=0 ; i<P ; i++){
		C[i]=malloc(towns*sizeof(int));
		if(C[i]==NULL){
			printf("Memory Error\n");
			return -1;
		}
	}
	best=malloc(towns*sizeof(int));
	temp=malloc(towns*sizeof(int));
	
	if(!ied){
	/*		Read the distances		*/
		d=malloc(towns*sizeof(double *));
		if(d==NULL){
			printf("Memory Error\n");
			return -1;
		}
		for(i=0 ; i<towns ; i++){
			d[i]=malloc(towns*sizeof(double));
			if(d[i]==NULL){
				printf("Memory Error\n");
				return -1;
			}
		}

		for(i=0 ; i<towns ; i++)
			for(j=i ; j<towns ; j++)
				if(i==j)
					d[i][j]=0;
				else{
					scanf("%lf",&d[i][j]);
					d[j][i]=d[i][j];
				}
	}

	/*		Validation		*/

	if(argc>1){
		for(k=1 ; k<argc ; k++){
			
			if(!strcmp(argv[k],"-c")){
				check=malloc(towns*sizeof(int));
				times=malloc(towns*sizeof(int));
				int path=0;
				for(i=0 ; i<towns ; i++){
					scanf("%lf",&check[i]);
					times[i]=0;
				}
				for(i=0 ; i<towns ; i++)
					for(j=0 ; j<towns ; j++)
						if(check[j]==i+1)	/* i+1 because counting starts from 0*/
							times[i]++;
				flag=1;
				for(i=0 ; i<towns ; i++){
					if(times[i]!=1)
						flag=0;
				}
				if(flag){
					for(i=0 ; i<towns-1 ; i++)
						path+=d[check[i]-1][check[i+1]-1];
					path+=d[check[towns-1]-1][check[0]-1];


					printf("Path length: %d\n",path);
				}else
					printf("Wrong path\n");	
				return 0;
			}
		}
	}



	/*		Initialization		*/

	for(i=0 ; i<P ; i++){
		for(z=0 ; z<towns ; z++){
			temp[z]=z;			/*	Temp is used to save the values that are not used	*/
		}
		length=towns;
		for(j=0 ; j<towns ; j++){
			w=rand()%(towns-j);
			A[i][j]=temp[w];
			z=w;
			while(z<length-1){
				temp[z]=temp[z+1];
				z++;
			}
			length--;	
		}
	}


	bf_all=-1;
	while(gens<G){
		/*		Selection		*/

		sf=0;
		for(i=0 ; i<P ; i++){
			F[i]=0;
			for(j=0 ; j<towns-1 ; j++)
				F[i]+=d[A[i][j]][A[i][j+1]];
			F[i]+=d[A[i][towns-1]][A[i][0]];
			F[i]=1.0/(double)F[i];

			sf+=F[i];
		}

		bf=F[0];
		for(i=0 ; i<P ; i++){			/*i=0 ,because the 0th element must be compared with bf_all too*/
			if(F[i]>bf){
				bf=F[i];
			}
			if(F[i]>bf_all){
				bf_all=F[i];
				loc=gens;
				for(z=0 ; z<towns ; z++){
					best[z]=A[i][z];
				}
			}
			
		}

		for(i=0 ; i<P ; i++){
			f[i]=(double)F[i]/(double)sf;
		}


		sum=0;
		for(j=0 ; j<P ; j++){
			r=(double) rand() / (double)((unsigned)RAND_MAX +1);
			i=0;
			while(1){
				sum+=f[i];
				if(r<sum)
					break;
				i++;
			}

			for(z=0 ; z<towns ; z++)
				B[j][z]=A[i][z];
			sum=0;
		}

		/*		Crossover 		*/

		for(i=0 ; i<P ; i++)
			for(j=0 ; j<towns ; j++)
				C[i][j]=B[i][j];

		for(i=0 ; i<P ; i+=2){
			r=(double) rand() / (double)((unsigned)RAND_MAX +1);
			if(r<PC){		/* If r<PC then Paths i and i+1 will be crossed over*/

				for(z=0 ; z<towns-1 ; z++){
					temp[z]=z;
				}				
				
				
				cut1=rand()%(towns-1);
				z=cut1;
				while(z<towns-1){
					temp[z]=temp[z+1];
					z++;
				}
				w=rand()%(towns-2);
				cut2=temp[w];

				if(cut1>cut2){
					int t=cut1;
					cut1=cut2;
					cut2=t;
				}
					

				int movelen=towns-abs(cut1-cut2);			
				int move1[movelen],move2[movelen];


				j=0;
				z=cut2+1;
				while(j<movelen){
					flag=0;
					for(x=cut1+1 ; x<cut2+1 ;x++)
						if(C[i+1][z]==C[i][x]){
							flag=1;
						}
					if(!flag){
						move1[j]=C[i+1][z];
						j++;
					}
					if(z==towns-1)
						z=0;
					else
						z++;
					
				}

				
				
				j=0;
				z=cut2+1;
				while(j<movelen){
					flag=0;
					for(x=cut1+1 ; x<cut2+1 ;x++)
						if(C[i][z]==C[i+1][x]){
							flag=1;
						}
					if(!flag){
						move2[j]=C[i][z];
						j++;
					}
					if(z==towns-1)
						z=0;
					else
						z++;	
				}

				z=cut2+1;
				for(j=0 ; j<movelen ; j++){
					C[i][z]=move1[j];
					C[i+1][z]=move2[j];
					if(z==towns-1)
						z=0;
					else
						z++;
				}
			}
		}

		/*		Mutation	 		*/

		for(i=0 ; i<P ; i++){
			r=(double) rand() / (double)((unsigned)RAND_MAX +1);
			if(r<PM){
				for(z=0 ; z<towns ; z++){
					temp[z]=z;
				}
				
				r1=rand()%(towns);
				z=r1;
				while(z<towns-1){
					temp[z]=temp[z+1];
					z++;
				}
				w=rand()%(towns-1);
				r2=temp[w];
				int t=C[i][r1];
				C[i][r1]=C[i][r2];
				C[i][r2]=t;


			}
		}

		for(j=0 ; j<P ; j++)
			for(i=0 ; i<towns ; i++)
				A[j][i]=C[j][i];

		printf("Generation %d:\tBFit = %f\tAvFit = %f\tBLength = %.0f\n",gens,bf,(double)sf/P,1.0/bf);
		gens++;
	}

	printf("Best chromosome is: ");
	for(i=0 ; i<towns ; i++)
		printf("%d ",best[i]+1);
	printf("\nPath length = %.0f\n\n",1.0/bf_all);
}
