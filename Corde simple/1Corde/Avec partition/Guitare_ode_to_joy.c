#include <stdio.h>


#define longueur 101
#define max_partition 1000

float *uc;
float *up;
float *us; // variables globales

void next(float K1, float K2, float K3){

	int i;
	for(i=1; i< (longueur - 1); ++i){
		us[i] = uc[i+1] * (K1 + K3) 
			+ uc[i]*(2 - 2*K1 -K2 -2*K3)
			+ uc[i-1] * (K1 + K3)
			+ up[i+1] * (-K3)
			+ up[i] * (-1 + K2 + 2*K3)
			+ up[i-1] * (-K3);


	}
		


	float *up_backup = up;
	up = uc;
	uc = us;
	us = up_backup;

	K1 = 0.0f;	

}


void note(int t, float t_min, float t_max, int indiceNote, int i0, float rho, float dt, float Fmax, int freq){
	



	if (indiceNote == 101){
		
		if((t*dt) < (t_min + 0.02f)){    	// on rajoute une impulsion en même temps qu'on change la note (deux mains en meme temps)
			
			uc[i0] += t*Fmax*dt*dt / (0.02f*freq*rho);
		}
	}

	else{

		if((t*dt) < (t_min + 0.02f)){    
			
			uc[i0] += t*Fmax*dt*dt / (0.02f*freq*rho);
		}	

				

	
		uc[indiceNote] += -0.1f * uc[indiceNote];
		
	}	
}




	

int main(int argc, char *argv[]){
	



	int freq = 44100; // on prend des entiers car on a des tableaux qui utilisent ces variables
	int duration = 40; 


	float E = 1.0e8;
	float epsilon = 1.4f;
	float dt = 1/44100.0f;
	float dx = 0.65f / (longueur - 1);
	float rho = 8000.0f;	
	float mu_air = 1.0e-3;
	float delta_air = 2.0e-3;
	float mu_visc = 89.0f/2; //89

	float K1 = epsilon*E*dt*dt / (rho*dx*dx);
	float K2 = mu_air*dt / (3.14f*delta_air*rho);
	float K3 = mu_visc*dt / (dx*dx*rho);
	
	float s1[longueur];
	float s3[longueur];

	float Fmax = 1.0e9f*2;

	float s2[longueur];
	us = s2;
	
	up = s1;
	uc = s3;

	FILE* fichier_son;

	float son[freq*duration]; // pour enregistrer le son d'un point en 1 seconde

	
	int t;
        for(t=0; t<freq*duration; ++t){
		son[t] = 0.0f;
	}

	
	int i;
	for(i=0; i<longueur; ++i){
		up[i] = 0.0f;
		uc[i] = 0.0f;
	}
	
	

	us[0] = 0.0f;
	us[longueur-1] = 0.0f;
	

	int i0 = 20; //zone ou on donne une impulsion avec la main droite

// valeur indiquant le placement des doigts
// les valeurs commentées sont les valeurs obtenues en mesurant sur une guitare
// les valeurs utilisées sont les valeurs obtenues en ajustant le son à l'oreille


	int i_la = 101;
        int i_si = 91;	//91
	int i_do = 81;	//80
	int i_do_d = 77;	//78		//represente par un 'x' dans la partition de Nirvana
	int i_re = 73; //76
	int i_mi = 68;	//68
	int i_fa = 61;	//60
	int i_sol = 54;	//54





//////////////////////////////////////////////////////////////////////////
/////////////////                                           //////////////
/////////////////         LECTURE DE LA PARTITION            //////////////
/////////////////                                            /////////////
////////////////////////////////////////////////////////////////////////////

	FILE* partition = NULL;
	char caractereActuel = 0;
	int nb_notes = 0;

	partition = fopen("partition_caro.txt","r");

	char *notes[max_partition];
	i = 0;
	if(partition != NULL){
		do{
			caractereActuel = fgetc(partition);
			if((caractereActuel != EOF)){
			
				notes[i] = caractereActuel;

				nb_notes += 1;
			        //printf("%c\n",notes[i]);
			}
			i = i+1;
		} while(caractereActuel != EOF);
		
		
		fclose(partition);
	}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
	
	
	i = 0;
	
	int indice_note = i_la;
	float duree_note = 0.3f;


// ici, on traduit la partition en placement de doigts

	for(i=0;i<nb_notes-1;++i){

		if(notes[i] == 'C'){
				notes[i] = i_do;
		}			
		if(notes[i] == 'D'){
				notes[i] = i_re;
		}			
		if(notes[i] == 'E'){
				notes[i] = i_mi;
		}			
		if(notes[i] == 'F'){
				notes[i] = i_fa;
		}			
		if(notes[i] == 'G'){
				notes[i] = i_sol;
		}			
		if(notes[i] == 'A'){
				notes[i] = i_la;
		}			
		if(notes[i] == 'B'){
				notes[i] = i_si;
		}			

		
	}

	i = 0;

	for(t=0;t<freq*duration;++t){
		
		next(K1, K2, K3);
		
		if(((t*dt) >= 1.0f + duree_note*(i+1))&&(i<(nb_notes-2))){	// teste si on change de note ou non
			i += 1;
			//printf("%d\n",notes[i]);

		}
		
		indice_note = notes[i];

		note(t, 1.0f + duree_note*i, 1.0f + duree_note*(i+1), indice_note, i0,  rho, dt, Fmax, freq);	//note à jouer
		
		son[t] = uc[longueur / 3];		// "micro" placé en longueur/3
	
		
		
		

	}	
// enregistrement des données

	fichier_son = fopen("sortie.dat", "w");
	for (t=0; t<freq*duration; t++){
		fprintf(fichier_son, "%e, ", son[t]);
	}
	fclose(fichier_son);
	printf("\n");
	return 0;

	
	

}
