//[TITANS] Processo Seletivo 01/2017
//Projeto Fantasma - Eletrônica Digital 
//Código Auxiliador v1.0

#include <stdlib.h>
#include <stdio.h>


int SF=0,ST=0,SD=0,SE=0; 
int MD=0, ME=0, AR=0;
int estado_atual = 0;

/*		
	Sinais de entrada:
		SF: 	Sensor da Frente
		ST: 	Sensor de Trás
		SD: 	Sensor da Direita
		SE:	 	Sensor da Esquerda
	
	Sinais de Controle:
		MD:  	Motor Direito
		ME: 	Motor Esquerdo
		AR: 	Arma


	Estados:
		Parado:	 		estado_atual = 0
		Para_Frente: 	estado_atual = 1
		Virar_direita:  estado_atual = 2
		Virar_esquerda: estado_atual = 3
		Atacar: 		estado_atual = 4	 
*/

void Sinais_de_Entrada(){

	printf("Sinais de Entrada:");
	scanf("%d %d %d %d", &SF, &ST, &SD, &SE);


}

void Sinais_de_Controle(int estado){

	switch(estado){
		case 0:
			MD = 0;
			ME = 0;
			AR = 0;
			estado_atual = 0;
			printf("Parado\n");
			printf("MD = %d\nME = %d\nAR = %d\n", MD, ME, AR);
			
		break;
			
		case 1:
			MD = 1;
			ME = 1;
			AR = 0;
			estado_atual = 1;
			printf("Para Frente\n");
			printf("MD = %d\nME = %d\nAR = %d\n", MD, ME, AR);
		break;

		case 2:
			MD = 0;
			ME = 1;
			AR = 0;
			estado_atual = 2;
			printf("Virar a Direita\n");
			printf("MD = %d\nME = %d\nAR = %d\n", MD, ME, AR);
		break;

		case 3:
			MD = 1;
			ME = 0;
			AR = 0;
			estado_atual = 3;
			printf("Virar a Esquerda\n");
			printf("MD = %d\nME = %d\nAR = %d\n", MD, ME, AR);
		break;

		case 4:
			MD = 0;
			ME = 0;
			AR = 1;
			estado_atual = 4;
			printf("Atacar\n");
			printf("MD = %d\nME = %d\nAR = %d\n", MD, ME, AR);
		break;

		default:
			printf("Estado Impossivel\n");

	}
}

int main(){

	while(1){

			Sinais_de_Entrada();
		
			// Monte sua Máquina de Estados Aqui //
	}

return 0;
}
