//***********************************************************************
//************* CDA 6316 Weekend Class August 11/18, 201 ****************
//************* Dr. Bassem Alhalabi and TA Pablo Pastran ****************
//************* A midterm code for students to use and modify ***********
#include <msp430.h>

int value=0, i=0 ;
int light = 0, lightroom = 0;
int temp = 0, temproom = 0;
int touch =0, touchroom =0;
int ADCReading [3];
int firstReadings = 1;
int touching;
int toggleFW = 1;

void fadeLED(int valuePWM);
void ConfigureAdc(void);

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 	// Stop WDT
	P1DIR |= ( BIT4 | BIT5 | BIT6);				// set bits 4, 5, 6 as outputs
	P1DIR |= (~BIT0 |~BIT1 |~BIT2); 			// set bits 0, 1, 2	as inputs
	P2DIR |= BIT0;								// set bit	0		as outputs
	ConfigureAdc();

	__delay_cycles(100000);
for (;;)
{
		i = 0; temp = 0; light = 0; touch =0;	// set all analog values to zero
		for(i=1; i<=5 ; i++)					// read all three analog values 5 times each and average
		{
			ADC10CTL0 &= ~ENC;					//ADC10 Enable Conversion
			while (ADC10CTL1 & BUSY);			//Wait while ADC is busy
			ADC10SA = (unsigned)&ADCReading[0]; //RAM Address of ADC Data, must be reset every conversion
			ADC10CTL0 |= (ENC | ADC10SC); 		//Start ADC Conversion
			while (ADC10CTL1 & BUSY); 			//Wait while ADC is busy

			light += ADCReading[0]; touch += ADCReading[1]; temp += ADCReading[2];  // sum     all 5 reading for the three variables
		} 	light = light/5;        touch = touch/5;        temp = temp/5;          // Average the 5 reading for the three variables

		if(firstReadings > 0){
			lightroom = light;
			temproom = temp;
			firstReadings--;
		}

		//light controlling Motor Speed
//		light = ((light- 50)*100)/(1000- 50); if(light <= 5)light = 0; else if (light >=99)light = 100; // range  50-900 to 0-100, good for absolute LED dimming
/*
		if(light < lightroom+2 && light > lightroom-2){light = 17;}
		else if (light >= lightroom*1.1){ light = 0;}
		else if (light >=900){light = 100;} // range 600-900 to 0-100, good for motor with hand shade
		else{light = ((light-750)*100)/(1000-750);}
		fadeLED(light);

*/
		//if light is within +- 20% of room's initial light reading - run motor 90% duty cycle !! 80% causes motor to fail
				if(light < (lightroom + (lightroom * 0.20)) && light >= (lightroom - (lightroom * 0.2))){
					fadeLED(10);
				}
				//if light is 20% brighter than initial room light, run motor 100%
				else if(light < (lightroom - (lightroom * 0.2))){
					fadeLED(0);
				}
				//if light is being blocked by finger - turn motor off
				else if(light >= 550){
					fadeLED(100);
				}

				//MOTOR ONLY speed if function of light
				else if(light > (lightroom + (lightroom * 2.3))){
			 		fadeLED(18);
				}
				else if(light > (lightroom + (lightroom * 1.8))){
					fadeLED(17);
				}
				else if(light > (lightroom + (lightroom * 1.5))){
					fadeLED(16);
				}
				else if(light > (lightroom + (lightroom * 1.2))){
					fadeLED(15);
				}
				else if(light > (lightroom + (lightroom * 0.9))){
					fadeLED(14);
				}
				else if(light > (lightroom + (lightroom * 0.7))){
					fadeLED(13);
				}
				else if(light > (lightroom + (lightroom * 0.4))){
					fadeLED(12);
				}

		//Touch Controlling Motor Direction

		if(touch > 800) {
			touching = 0;
		}
		if(touch > 500 && touch < 800){}
		if(touch <= 500) {
			if(toggleFW && !touching){
				P1OUT = (~BIT4 |  BIT5); // set motor FW direction
				 __delay_cycles(200);
				touching = 1;
				toggleFW = 0;
			}
			else if(!toggleFW && !touching){
				P1OUT = ( BIT4 | ~BIT5); // set motor RV direction
				__delay_cycles(200);
				touching = 1;
				toggleFW = 1;
			}
		}



		//Temperature Controlling SSR
		if(temp > temproom && temp < temproom*1.03) {}
		else
		{	if(temp >= temproom+temproom*0.03) {P2OUT =  BIT0; __delay_cycles(200);}    // turn SSR on
			if(temp <= temproom)	{P2OUT = ~BIT0; __delay_cycles(200);}    // turn SSR off
		}

}
}

void ConfigureAdc(void)
{
   ADC10CTL1 = INCH_2 | CONSEQ_1; 				// A2 + A1 + A0, single sequence
   ADC10CTL0 = ADC10SHT_2 | MSC | ADC10ON;
   while (ADC10CTL1 & BUSY);
   ADC10DTC1 = 0x03; 							// 3 conversions
   ADC10AE0 |= (BIT0 | BIT1 | BIT2); 			// ADC10 option select
}

void fadeLED(int valuePWM)
{
	P1SEL |= (BIT0 | BIT6);                    	// P1.0 and P1.6 TA1/2 options
	CCR0 = 100 - 0;                            	// PWM Period
	CCTL1 = OUTMOD_3;                           // CCR1 reset/set
	CCR1 = valuePWM;                            // CCR1 PWM duty cycle
	TACTL = TASSEL_2 + MC_1; // SMCLK, up mode
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	__bic_SR_register_on_exit(CPUOFF);
}
