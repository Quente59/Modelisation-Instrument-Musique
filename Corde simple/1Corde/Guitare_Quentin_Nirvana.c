#include <stdio.h>


#define longueur 101

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
		
		if(( (t*dt) > t_min) && ((t*dt) < (t_min + 0.02f)) ){    	// on rajoute une impulsion en même temps qu'on change la note (deux mains en meme temps)
			
			uc[i0] += t*Fmax*dt*dt / (0.02f*freq*rho);
		}
	}

	else{

		if(( (t*dt) > t_min) && ((t*dt) < (t_min + 0.02f)) ){    
			
			uc[i0] += t*Fmax*dt*dt / (0.02f*freq*rho);
		}	


		if( ( (t*dt) > t_min) && ((t*dt) < t_max)){

	
			uc[indiceNote] += -0.1f * uc[indiceNote];
		}
	}	
}

	

int main(int argc, char *argv[]){
	
	float p = 0.6; //vitesse de la musique (aucune unité)


	int freq = 44100; // on prend des entiers car on a des tableaux qui utilisent ces variables
	int duration = 10; 

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
	

	int i0 = 20;	//zone ou on donne une impulsion avec la main droite

// valeur indiquant le placement des doigts
// les valeurs commentées sont les valeurs obtenues en mesurant sur une guitare
// les valeurs utilisées sont les valeurs obtenues en ajustant le son à l'oreille

	int i_la = 101;
        int i_si = 91;	//91
	int i_do = 81;	//80
	int i_do_d = 77;	//78
	int i_re = 73; //76
	int i_mi = 68;	//68
	int i_fa = 61;	//60
	int i_sol = 54;	//54


        float alpha;

	
	for(t=0; t<freq*duration; ++t){
		
		next(K1, K2, K3);	
	

// on renseigne les notes ici une par une, c'est compliqué, il existe une version avec partition mais la duree des notes est toujours la même (fixé au départ)
// cette version à l'avantage de permettre la modification de la duree des notes


		note(t, 1.0f*p, 1.5f*p, i_do, i0,  rho, dt, Fmax, freq);
		note(t, 1.5f*p, 2.0f*p, i_do, i0,  rho, dt, Fmax, freq);
		note(t, 2.0f*p, 2.5f*p, i_do_d, i0,  rho, dt, Fmax, freq);
		note(t, 2.5f*p, 3.5f*p, i_re, i0,  rho, dt, Fmax, freq);
		note(t, 3.5f*p, 4.0f*p, i_fa, i0,  rho, dt, Fmax, freq);
		note(t, 4.0f*p, 4.5f*p, i_re, i0,  rho, dt, Fmax, freq);
		note(t, 4.5f*p, 5.0f*p, i_fa, i0,  rho, dt, Fmax, freq);
		note(t, 5.0f*p, 5.5f*p, i_re, i0, rho, dt, Fmax, freq);
		note(t, 5.5f*p, 6.0f*p, i_re, i0,  rho, dt, Fmax, freq);
		note(t, 6.0f*p, 6.5f*p, i_do_d, i0,  rho, dt, Fmax, freq);
		note(t, 6.5f*p, 7.0f*p, i_do, i0,  rho, dt, Fmax, freq);
		note(t, 7.0f*p, 7.5f*p, i_sol, i0,  rho, dt, Fmax, freq);
		note(t, 7.5f*p, 8.0f*p, i_do, i0,  rho, dt, Fmax, freq);
		note(t, 8.0f*p, 9.0f*p, i_do, i0,  rho, dt, Fmax, freq);
		note(t, 9.0f*p, 9.5f*p, i_sol, i0,  rho, dt, Fmax, freq);
		note(t, 9.5f*p, 10.0f*p, i_do, i0,  rho, dt, Fmax, freq);
		note(t, 10.0f*p, 10.5f*p, i_do_d, i0,  rho, dt, Fmax, freq);
		note(t, 10.5f*p, 11.5f*p, i_re, i0,  rho, dt, Fmax, freq);
		note(t, 11.5f*p, 12.0f*p, i_fa, i0,  rho, dt, Fmax, freq);
		note(t, 12.0f*p, 12.5f*p, i_re, i0,  rho, dt, Fmax, freq);
		note(t, 12.5f*p, 13.0f*p, i_fa, i0,  rho, dt, Fmax, freq);
		note(t, 13.0f*p, 13.5f*p, i_re, i0, rho, dt, Fmax, freq);
		note(t, 13.5f*p, 14.0f*p, i_re, i0,  rho, dt, Fmax, freq);
		note(t, 14.0f*p, 14.5f*p, i_do_d, i0,  rho, dt, Fmax, freq);
		note(t, 14.5f*p, 15.0f*p, i_do, i0,  rho, dt, Fmax, freq);
		note(t, 15.0f*p, 15.5f*p, i_sol, i0,  rho, dt, Fmax, freq);
		note(t, 15.5f*p, 16.0f*p, i_do, i0,  rho, dt, Fmax, freq);
		note(t, 16.0f*p, 17.0f*p, i_do, i0,  rho, dt, Fmax, freq);
		note(t, 17.0f*p, 17.5f*p, i_sol, i0,  rho, dt, Fmax, freq);

		



		son[t] = uc[longueur / 3];  // "micro" placé en longueur/3
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
