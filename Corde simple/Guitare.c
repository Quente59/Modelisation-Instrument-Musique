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
		
	/*for(int i=1; i< (longueur - 1); ++i){
			up[i] = uc[i];
			uc[i] = us[i];
	}*/

	float *up_backup = up;
	up = uc;
	uc = us;
	us = up_backup;

	K1 = 0.0f;	

}

int main(int argc, char *argv[]){
	
	int freq = 44100; // on prend des entiers car on a des tableaux qui utilisent ces variables
	int duration = 5; // on peut utiliser un define --> #define duration 1 (pas de point-virgule)


	float E = 1.0e8;
	float epsilon = 1.4f;
	float dt = 1/44100.0f;
	float dx = 0.65f / (longueur - 1);
	float rho = 8000.0f;	
	float mu_air = 1.0e-3;
	float delta_air = 2.0e-3;
	float mu_visc = 89.0f;

	float K1 = epsilon*E*dt*dt / (rho*dx*dx);
	float K2 = mu_air*dt / (3.14f*delta_air*rho);
	float K3 = mu_visc*dt / (dx*dx*rho);
	
	float s1[longueur];
	float s3[longueur];

	float Fmax = 1.0e9f;

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
	

	int i0 = 20;
        int i_note1 = 60;
	int i_note2 = 50;
        float alpha;

	
	for(t=0; t<freq*duration; ++t){
		
		next(K1, K2, K3);	
	
		if( (t*dt) < 0.02f){
			uc[i0] += t*Fmax*dt*dt / (0.02f*freq*rho);
		}

		if(( (t*dt) > 0.5f) && ((t*dt) <= 1.0f) ){    // on peut rajouter une impulsion en même temps qu'on change la note (deux mains en meme temps)
			
			alpha = rho/(dt*dt) * (dt/0.01f);	
			
			//uc[i_note] += (dt*dt/rho) * (-alpha * uc[i_note]);
			uc[i_note1] += -0.05f * uc[i_note1];
		}

		if( (t*dt) > 1.0f){
			
			alpha = rho/(dt*dt) * (dt/0.01f);	
			
			
			uc[i_note2] += -0.05f * uc[i_note2];
		}

		son[t] = uc[longueur / 3];
	}

	/*fichier_son = fopen("sortie.dat", "w");
	for (int i=0; i<(longueur - 1); i++){
		fprintf(fichier_son, "%f, ", uc[i]);
	}
	fclose(fichier_son);
	printf("\n");
	return 0;
	
	// ici, on a enregistré les positions des points de la corde au bout d'une seconde

	*/

	fichier_son = fopen("sortie.dat", "w");
	for (t=0; t<freq*duration; t++){
		fprintf(fichier_son, "%e, ", son[t]);
	}
	fclose(fichier_son);
	printf("\n");
	return 0;

	// ici, on a enregistré le son au point position 26
	
	

}
