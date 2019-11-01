
/****************************** HAMMAD Radhouane M2 IIEP ****************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define nThread 10
#define nLg_1 10
#define nCol_1 10
#define nLg_2 10
#define nCol_2 10
int A[nLg_1][nCol_1];
int B[nLg_2][nCol_2];
int ResultParallele[nLg_1][nCol_2];
int ResultSeq[nLg_1][nCol_2];
int Taille_Block;
Taille_Block=nLg_1/nThread;
void *produit(void *id);



int main(int argc,char *argv[]){
	int th,rc,i,j,m;
	
	pthread_t threads[nThread];
	double Temps_debut,Temps_Fin,Temps_Exe_Seq,Temps_Exe_parallele;
	
	srand(time(NULL));
	
	
	
	/***************** 	Initialisation des matrices	**************/
	for(i=0;i<nLg_1;i++){
		for(j=0;j<nCol_1;j++){
			A[i][j]=rand()%nLg_1;
		}
	}
	for(i=0;i<nLg_2;i++){
		for(j=0;j<nCol_2;j++){
			B[i][j]=rand()%nLg_2;
		}
	}
	for(i=0;i<nLg_1;i++){
		for(j=0;j<nCol_2;j++){
			
			ResultParallele[i][j]=0;
			ResultSeq[i][j]=0;
		}
	}
	
	
	
	
	/************* Affichage des matrices A et B *****************/
	printf("La Matrice A: \n");
	for(i=0;i<nLg_1;i++){
		for(j=0;j<nCol_1;j++){
			printf("%d  ",A[i][j]);
		}
		printf("\n");
	}
	
	printf("La Matrice B: \n");
	for(i=0;i<nLg_2;i++){
		for(j=0;j<nCol_2;j++){
			printf("%d  ",B[i][j]);
		}
		printf("\n");
	}
	
	
	
	
	/************** Calcule sequentiel ************/


	Temps_debut=clock();
	for(m=0;m<nLg_1;m++){
		for(i=0;i<nCol_2;i++){
			for( j=0;j<nLg_2;j++){
				ResultSeq[m][i]=ResultSeq[m][i]+A[m][j]*B[j][i];
			}
		}
	}
	Temps_Fin=clock();
	Temps_Exe_Seq=(double)(Temps_Fin-Temps_debut)/CLOCKS_PER_SEC;
	
	
	
	
	/********** Affichage du resultat sequentielle ***********/
	printf("le resutat sequantiel : \n");
	for(i=0;i<nLg_1;i++){
		for(j=0;j<nCol_2;j++){
			printf("%d  ",ResultSeq[i][j]);
		}
		printf("\n");
	}
	printf("le temps d execution =%f\n",Temps_Exe_Seq);
    
	
	
	
	
	/************** Calcule paralelle ******************/

	Temps_debut=clock();
	for(th=0;th<nThread;th++){
		rc=pthread_create(&threads[th],NULL,produit,(void *)th);
		if(rc){
			printf("erreur 0");
			exit(-1);
		}
	}

	for(th=0;th<nThread;th++){
		pthread_join(threads[th],NULL);
	}
	Temps_Fin=clock();
	Temps_Exe_parallele=(double)(Temps_Fin-Temps_debut)/CLOCKS_PER_SEC;
	
	
	
	
	
	/****************** Affichage du resultat paralelle ********************/

	printf("le resutat paralelle : \n");
	for(i=0;i<nLg_1;i++){
		for(j=0;j<nCol_2;j++){
			printf("%d  ",ResultParallele[i][j]);
		}
		printf("\n");
	}
	printf("le temps d execution =%f\n",Temps_Exe_parallele);

	
}



void *produit(void *id){
	long tid=*((long*)(&id));
	int i,j,z,start,end;
	

	start=tid*Taille_Block;
	if(tid!=nThread-1){
	    end=start+Taille_Block;
	}else{
	    end=nThread;
		
	}

    for(z=start;z<end;z++){
		for(i=0;i<nCol_2;i++){
			for( j=0;j<nLg_2;j++){
				ResultParallele[z][i]=ResultParallele[z][i]+A[z][j]*B[j][i];
			}
		}
	}
}

