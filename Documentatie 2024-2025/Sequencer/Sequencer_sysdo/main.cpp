#define F_CPU           4000000UL   /* Main clock frequency */
#define START_TOKEN     0x03        /* Start Frame Token */
#define END_TOKEN       0xFC        /* End Frame Token */
/* Compute the baud rate */
#define USART1_BAUD_RATE(BAUD_RATE) (((float)F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

/* VREF start-up time */
#define VREF_STARTUP_TIME        (50)
/* Mask needed to get the 2 LSb for DAC Data Register */
#define LSB_MASK                 (0x03)

static void VREF_init(void);
static void DAC0_init(void);
static void DAC0_setVal(uint16_t value);


#include <avr/io.h>
#include <stdbool.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#define TIMER_PERIOD        0x2000

void CLKCTRL_init(void);
void PORT_init(void);
void VREF0_init(void);
void ADC0_init(void);
uint16_t ADC0_read(void);
void ADC0_start(void);
uint16_t per;

int maten = 1;
uint16_t toonhoogte[32];
int count = 0;
bool recording = true;
int state = 0;
bool aan = false;
bool buttonPressed = false;
float bpm = 60;
int listtotal = 32;
int timer = 0;
int ledCount = 0;
uint16_t  bem;
bool updatebpm = false;
bool interrrupt = false;
int gatedelay = 0;
bool firstplay = false;
uint16_t checkgate;

uint16_t currentToon;
/* This function initializes the CLKCTRL module */
void CLKCTRL_init(void)
{
	/* Unlock and set the clock frequency to 4 MHz */
	CCP = CCP_IOREG_gc;  // Unlock protected registers
	CLKCTRL.OSCHFCTRLA = CLKCTRL_FRQSEL_4M_gc;
}


/* This function initializes the PORT module */
void PORT_init(void)
{
	/* Configure PC0 as output for USART1 TX */
	PORTC.DIRSET = PIN0_bm;
	/* Disable interrupt and digital input buffer on PD3 */
	PORTD.PIN3CTRL &= ~PORT_ISC_gm;
	PORTD.PIN3CTRL |= PORT_ISC_INPUT_DISABLE_gc;

	/* Disable pull-up resistor */
	PORTD.PIN3CTRL &= ~PORT_PULLUPEN_bm;
	/* Configure PC0 as output for USART1 TX */
	PORTC.DIRSET = PIN0_bm;
	/* Disable interrupt and digital input buffer on PD3 */
	PORTD.PIN1CTRL &= ~PORT_ISC_gm;
	PORTD.PIN1CTRL |= PORT_ISC_INPUT_DISABLE_gc;

	/* Disable pull-up resistor */
	PORTD.PIN1CTRL &= ~PORT_PULLUPEN_bm;
}

/* This function initializes the VREF module */
void VREF0_init(void)
{
	VREF.DAC0REF = VREF_REFSEL_2V048_gc | VREF_ALWAYSON_bm; // Set VREF to 2.048V
	_delay_us(VREF_STARTUP_TIME);

	VREF.ADC0REF = VREF_REFSEL_2V048_gc;  /* Internal 2.048V reference */
}
static void DAC0_init(void)
{
	/* Disable digital input buffer on PA6 */
	PORTD.PIN6CTRL &= ~PORT_ISC_gm;
	PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;

	DAC0.CTRLA = DAC_ENABLE_bm | DAC_OUTEN_bm | DAC_RUNSTDBY_bm; // Enable DAC
}
static void DAC0_setVal(uint16_t value)
{
	DAC0.DATAL = (value & LSB_MASK) << 6;      // Store LSbs
	DAC0.DATAH = value >> 2;                   // Store MSbs
}


/* This function initializes the ADC module */
void ADC0_init(void)
{
	//ADC0.CTRLC = ADC_PRESC_DIV128_gc;        /* CLK_PER divided by 4 */
	ADC0.CTRLA = ADC_ENABLE_bm             /* ADC Enable: enabled */
	| ADC_RESSEL_10BIT_gc       /* 12-bit mode */
	| ADC_FREERUN_bm;           /* Enable Free-Run mode */
	ADC0.MUXPOS = ADC_MUXPOS_AIN1_gc;
	ADC0.MUXPOS = ADC_MUXPOS_AIN4_gc;     /* Select ADC channel AIN3 <-> PD3 */
}


/* This function returns the ADC conversion result */
uint16_t ADC0_read(int channel)
{
	if(channel == 2){
		ADC0.MUXPOS = ADC_MUXPOS_AIN5_gc;
	}
	else{
		ADC0.MUXPOS = ADC_MUXPOS_AIN1_gc;
	}
	if(channel == 0){
		ADC0.MUXPOS = ADC_MUXPOS_AIN1_gc;
	}
	for(int i; i > 50; i++){}
	/* Wait for ADC result to be ready */
	while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));
	/* Clear the interrupt flag by reading the result */
	return ADC0.RES;
}

/* This function starts the ADC conversions*/
void ADC0_start(void)
{
	/* Start ADC conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
}

void newBPM(float bpm){
	
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	per = (uint16_t) (TIMER_PERIOD / bpm);
	TCA0.SINGLE.PER = per;
	TCA0.SINGLE.CTRLA = (TCA_SINGLE_CLKSEL1_bm | TCA_SINGLE_CLKSEL2_bm);
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
	
	sei();
	return;
}

void checklist(){
	
	listtotal = 32;
	

}

void gateInterrupt(){
	uint16_t adc = ADC0_read(1);
	//DAC0_setVal(adc);
	
	PORTA.OUTCLR = PIN3_bm;
	PORTA.OUTCLR = PIN4_bm;
	PORTA.OUTCLR = PIN5_bm;
	PORTA.OUTCLR = PIN6_bm;
	if(state != 0){
		if(timer % 4 == 0){
			
			
			if(ledCount == 0){
				PORTA.OUTSET = PIN3_bm;
				}else if(ledCount == 1){
				PORTA.OUTSET = PIN4_bm;
			}
			else if(ledCount == 2){
				PORTA.OUTSET = PIN5_bm;
			}
			else if(ledCount == 3){
				PORTA.OUTSET = PIN6_bm;
			}
			ledCount++;
			if (ledCount >= 4){
				ledCount = 0;
			}
		}
		timer++;
	}
	
	
	
	
	switch (state)
	{
		case 1:
		//record
		
		toonhoogte[count] = adc;
		DAC0_setVal(0);
		break;
		case 2:
		DAC0_setVal(toonhoogte[count]);
		
		//replace
		
		
		
		if (adc > 5){
			toonhoogte[count] = adc;
		}
		break;
		case 3:
		if(1){
			
			DAC0_setVal(toonhoogte[count]);
		}
		
		
	}
	count++;
	//DAC0_setVal(adc);
	currentToon = toonhoogte[count];
}

void tick(){
	
	checklist();
	if (count >= listtotal) {
		count = 0;
	}
	if(updatebpm){
		gateInterrupt();
		
		//foei niet in de ISR ^^
		//DAC0_setVal(ADC0_read(2));
	}
	else
	{
		bem = ADC0_read(2);
		
		newBPM(bem / 100 + 10);

		
	}
	
	updatebpm = !updatebpm;
}


ISR(TCA0_OVF_vect) {
	
	interrrupt = true;
	tick();
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;

	//DAC0_setVal(ADC0_read(1));
}


void output(uint8_t toon, bool gate){
	//DAC0_setVal(toon);
	PORTA.OUTCLR = PIN1_bm;
}

void deletes(){
	for(int i = 0; i < listtotal; i++){
		toonhoogte[i] = 0;
	}
	
}



int main(void)
{

	checkgate = toonhoogte[count];
	uint16_t adcVal;
	/* Initialize all peripherals */
	CLKCTRL_init();
	PORT_init();
	VREF0_init();
	ADC0_init();
	DAC0_init();
	/* Start the ADC conversions */
	ADC0_start();
	newBPM(10);
	//gate out                                               PORT A1
	PORTD.DIR = PORTD.DIR | PIN7_bm;
	// Initialize time variablE
	
	PORTA.DIRCLR = PIN7_bm;
	PORTC.DIRCLR = PIN0_bm;
	PORTC.DIRCLR = PIN1_bm;

	PORTA.DIR = PORTA.DIR | PIN3_bm;
	
	PORTA.DIR = PORTA.DIR | PIN4_bm;
	
	PORTA.DIR = PORTA.DIR | PIN5_bm;
	
	PORTA.DIR = PORTA.DIR | PIN6_bm;
	

	PORTC.DIR = PORTC.DIR | PIN2_bm;
	

	
	
	

	while (1)
	{
		state = 0;
		if ((PORTC.IN & (PIN1_bm)))
		{
			PORTA.OUTSET = PIN6_bm;
			/* LED0 on */
			state = 2;
			
		}
		else if((PORTC.IN & (PIN0_bm))){
			state = 1;
		}
		else if(PORTA.IN & (PIN7_bm)){
			state = 3;
			if(!firstplay){
				count = 0;
				firstplay = true;
			}
		}
		/* 1: Released */
		if(PORTC.IN & (PIN2_bm)){
			deletes();
		}
		//DAC0_setVal(bpm);
		
		
		if(state == 0){
			DAC0_setVal(0);
		}

		if((currentToon == checkgate) && (state >= 2)){
			PORTD.OUTSET = PIN7_bm;
			gatedelay = 1;
		}
		else if(toonhoogte[count] <= 10 && (state >= 2)){
			PORTD.OUTCLR = PIN7_bm;
		} else
		{
			PORTD.OUTCLR = PIN7_bm;
			//PORTC.OUTSET = PIN6_bm;
		}
		if(state <= 1)
		{
			PORTD.OUTCLR = PIN7_bm;
		}
		
		if(gatedelay > 50){
			//PORTC.OUTCLR = PIN6_bm;
			checkgate = currentToon;
			gatedelay = 0;
			
			}else{
			gatedelay++;
		}
		
		if(interrrupt){
			
			//PORTB.OUTTGL = PIN3_bm;
			
		}
		
	}
}