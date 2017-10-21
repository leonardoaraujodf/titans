
#include <msp430g2553.h>

#define ADC_CHANNELS 4
#define MOTOR_ESQUERDO BIT4 
#define MOTOR_DIREITO BIT5 
#define ARMA BIT6 
#define PARAMETRO_MEDIA 15

unsigned int media=0;
unsigned int adc[ADC_CHANNELS];
unsigned int SENSOR_FRONTAL = 0;
unsigned int SENSOR_TRASEIRO = 0;
unsigned int SENSOR_ESQUERDO = 0; 
unsigned int SENSOR_DIREITO = 0;

void Atraso(volatile unsigned int x)
{
    TA0CCR0 = 1000-1; 
    TA0CTL |= TACLR; //clear timer
    TA0CTL = TASSEL_2 + ID_0 + MC_1; 
    while(x>0)
    {
      x--;
      while((TA0CTL&TAIFG)==0);
              TA0CTL &= ~TAIFG;
    }
    TA0CTL = MC_0; //stop timer
}

void Selecao_Saida(char saida[])
{
  if(saida == "ARMA")
  {
    P1OUT &= ~(MOTOR_ESQUERDO + MOTOR_DIREITO); //desativa os motores
    P1OUT |= ARMA;
    Atraso(1000); //supondo que a arma fica acionada por 1 segundo
    P1OUT &= ~(ARMA);//desativa a arma
    Atraso(10000); //Atraso de 10s para a arma estabilizar 
  }
  else if(saida == "MOVER_FRENTE")
  {
    P1OUT |= MOTOR_ESQUERDO + MOTOR_DIREITO; //aciona os motores direito e esquerdo
    Atraso(100); //Espera 100 ms
  }
  else if(saida == "MOVER_ESQUERDA")
  {
    P1OUT &= ~(MOTOR_DIREITO); //desativa o motor direito
    P1OUT |= MOTOR_ESQUERDO; //aciona o motor esquerdo
    Atraso(100); //Espera 100 ms
  }
  else if(saida == "MOVER_DIREITA")
  {
    P1OUT &= ~(MOTOR_ESQUERDO);//desativa o motor esquerdo
    P1OUT |= MOTOR_DIREITO;//aciona o motor direito
    Atraso(100); //Espera 100 ms
  }
}

void Ler_Sensores()
{
  SENSOR_FRONTAL = adc[3];
  SENSOR_TRASEIRO = adc[2];
  SENSOR_ESQUERDO = adc[1];
  SENSOR_DIREITO = adc[0];
  if(media!=0)
  {
    if(media<20)
    {
      //media < 20 significa que existe muita incidência de ectoplasma
      //entao atire no fantasma!!!
      Selecao_Saida("ARMA");
    }
    else if(SENSOR_FRONTAL < (media + PARAMETRO_MEDIA))
    {
      Selecao_Saida("MOVER_FRENTE");
    }
    else if(SENSOR_TRASEIRO < (media + PARAMETRO_MEDIA) || SENSOR_DIREITO < (media + PARAMETRO_MEDIA))
    {
      //Se o sensor traseiro for acionado, se o robo virar a direita em 180º,
      //o sensor frontal sera acionado (e o traseiro desligado) 
      //e ele seguira o rastro
      Selecao_Saida("MOVER_DIREITA");
    }
    else if(SENSOR_ESQUERDO < (media + PARAMETRO_MEDIA))
    {
      Selecao_Saida("MOVER_ESQUERDA");
    }
    media = (adc[0] + adc[1] + adc[2] + adc[3])/4;
  }
  else
  {
    media = (adc[0] + adc[1] + adc[2] + adc[3])/4;
  }
}

void Setup_ADC()
{
   ADC10CTL1 |= INCH_3 + CONSEQ_1 + SHS_0 + ADC10SSEL_3;
   ADC10CTL0 |= SREF_0 + ADC10SHT_0 + MSC + ADC10ON + ADC10IE;
   ADC10AE0 |= BIT0+BIT1+BIT2+BIT3; // Analog Input in P1.0, P1.1, P1.2 and P1.3; 
   ADC10DTC1 = ADC_CHANNELS;
   ADC10CTL0 |= ENC + ADC10SC;
}

void Read_ADC()
{
  ADC10CTL0 &= ~ENC;
  while (ADC10CTL1 & BUSY);// Wait if ADC10 core is active
  ADC10SA = (unsigned int)adc;// Copies data in ADC10SA to unsigned int adc array
  ADC10CTL0 |= ENC + ADC10SC;
  __bis_SR_register(CPUOFF + GIE);
  Ler_Sensores();
}

void Setup_Outputs()
{
  P1OUT = 0;
  P1DIR |= MOTOR_ESQUERDO + MOTOR_DIREITO + ARMA;
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  Setup_Outputs();
  Setup_ADC();
  for(;;)
  {
    Read_ADC();
  }
  return 0;
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

