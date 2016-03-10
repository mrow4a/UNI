#include "AT91SAM9263.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#define PMC_PCER (volatile unsigned int * const) 0xFFFFFC10 //PMC Peripheral Clock Enable Register

//functions
void initialization();
void readTerm();
int buttonsControl();
void runDiode(int diode1, int diode2);
void runDisp(unsigned int number);
void runDigit (int digit);
void clearSegments();
void dimSegment(int num, ...);
void lightSegment(int num, ...);
void dbgu_print_ascii(const char* string);
void m_putc(char c);
float input();
void output();
int check_string ();
float m_atof();
void dbgu_print_ascii_Str (float f);
void readData();
void whileLoop();

//variables and constants
char stack[100];
static int sign_read=0;
static int sign_write=0;
float temp = 30;
int piv=52487; //  
float temp_max = 100;
float temp_min = -1;

int main(void) {
	initialization();
	readData();		
	return 0;
}

void readData () { //take data from user
	clearSegments();
	dbgu_print_ascii("\x1b[2J");
	dbgu_print_ascii("------TERMOSTAT------\r\n\n------WELCOME------\r\nJustyna & Piotr\r\n");
	do {
		dbgu_print_ascii("\r\nInsert maximum temperature (0-100): ");
		temp_max = input();
	} while (temp_max >= 100 || temp_max < 0);

	do {
		dbgu_print_ascii("\r\n");
		dbgu_print_ascii("Insert minimum temperature (0-");
		dbgu_print_ascii_Str(temp_max);
		dbgu_print_ascii("): ");
		temp_min = input();
		dbgu_print_ascii("\r\n");
	} while (temp_min < 0 || temp_min >= temp_max);
	
	dbgu_print_ascii("Temperature should be between ");
	dbgu_print_ascii_Str(temp_min);
	dbgu_print_ascii(" and ");
	dbgu_print_ascii_Str(temp_max);
	dbgu_print_ascii("\r\n");
	
	whileLoop();
}

void whileLoop() {
	int button;
	volatile int tmp; //uses register PITC_PIVR to reset PITS
	while(1){
		readTerm();	
		//PIMR Periodic Interval Timer Mode Register 
		//PIV: Periodic Interval Value.  Defines the value compared with the primary 20-bit counter of the Periodic Interval Timer (CPIV).
		//PITEN: Period Interval Timer Enabled
		AT91C_BASE_PITC->PITC_PIMR=AT91C_PITC_PITEN |( AT91C_PITC_PIV & piv); // running the timer and setting time
		//Periodic Interval Timer Status Register
		//Periodic Interval Timer Status
		if (AT91C_BASE_PITC->PITC_PISR==AT91C_PITC_PITS) { // check if PIT is active
			tmp=AT91C_BASE_PITC->PITC_PIVR; // resets PITS
			button = buttonsControl();
			switch (button) {
				case 1:
					runDisp(temp_min);
				//	runDiode(1,0);
					break;
				case 2:
					runDisp(temp_max);
				//	runDiode(0,1);
					break;
				default:
					runDisp(temp);
					if (temp<temp_min) 
						runDiode(1,0);
					else if (temp>temp_max) 
						runDiode(0,1);
					else 
						runDiode(0,0);
					break;
			}
			if ((AT91C_BASE_PIOB->PIO_PDSR & (1 << 17)) == 0) // encoder
				readData();
		  }
	}
}

void readTerm() {
	static int counter=0;
	counter++; // counts occurence of PITS
	if(counter==100000) { //turning on
		AT91C_BASE_SPI1->SPI_TDR = 0xFFFF;	
		
		while ((AT91C_BASE_SPI1->SPI_SR & AT91C_SPI_RDRF) == 0);
		temp = (AT91C_BASE_SPI1->SPI_RDR & 0xFFFF) >> 3;
		temp = temp * 0.0625;
		dbgu_print_ascii_Str(temp);
		dbgu_print_ascii("\r\n");
		counter = 0;
	}
}



float input() {
	char c = 127;
	sign_write = 0;
	while (c!=13) { //13-enter
		while(!((AT91C_BASE_DBGU->DBGU_CSR) & (AT91C_US_RXRDY))); //CSR - Channel Status Register & RXRDY Interrupt
		c = AT91C_BASE_DBGU->DBGU_RHR;
		if ((c >= '0' && c <= '9') || c==46 || c==8) { //48-57-digits, 46-.
			if (c==8 && sign_write>0) { //8-backspace
				sign_write--;
				m_putc(c); //shift cursor
				m_putc(' '); //replace cursored sign with space
				m_putc(c); //shift cursor again to be again at the space 
			}
			else if(c!=13)
			{
				stack[sign_write] = c;
				m_putc(c);
				sign_write++;
			}
		}
	}
	stack[sign_write++]='\0';
	return m_atof();
}

float m_atof() {
	char str[10];
	int len = sign_write - 1;
	strcpy(str, stack);
	str[len]='\0';
	return atof(str);
}

void dbgu_print_ascii(const char* string) {
	int i = 0;
	while (string[i]) {
		m_putc(string[i]);
		i++;
	}
}

void dbgu_print_ascii_Str (float f) {
	char str[15];
	sprintf(str, "%.2f", f);
	dbgu_print_ascii(str);
}

void output() {
	sign_read = 0;
	while (sign_read<sign_write) {
		m_putc(stack[sign_read]);
		sign_read++;
	}
}

void m_putc(char c) {
    while(!((AT91C_BASE_DBGU->DBGU_CSR) & (AT91C_US_TXRDY)));//CSR - Channel Status Register & TXRDY Interrupt
    AT91C_BASE_DBGU->DBGU_THR = c; //THR -  Transmit Holding Register
}

void initialization() {
	//initDiodes
	//PB8 - PWM1 - USER'S LED1 CONTROL (DS1)
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB8;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB8;
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB8;
	//PC29 - PWM2 - USER'S LED2 CONTROL (DS2) 
	AT91C_BASE_PIOC->PIO_PER = AT91C_PIO_PC29;
	AT91C_BASE_PIOC->PIO_OER = AT91C_PIO_PC29;
	AT91C_BASE_PIOC->PIO_SODR = AT91C_PIO_PC29;

	//initDisplay
	//display - segment A
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB25;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB25;
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB25;
	//display - segment B
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB24;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB24;
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB24;
	//display - segment C
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB22;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB22;
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB22;
	//display - segment D
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB21;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB21;
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB21;
	//display - segment E
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB20;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB20;
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB20;
	//display - segment F
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB27;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB27;
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB27;
	//display - segment G
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB26;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB26;
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB26;
	//display - segment H
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB23;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB23;
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB23;
	//display - segment DIG1
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB30;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB30;
	AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB30;
	//display - segment DIG2
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB28;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB28;
	AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB28;
	
	//initButtons
	//PC4 - PWM2 - USER'S PUSH BUTTON (BP2) 
	//PC5 - PWM2 - USER'S PUSH BUTTON (BP1) 
	AT91C_BASE_PIOC->PIO_PER = (AT91C_PIO_PC4 | AT91C_PIO_PC5);
	AT91C_BASE_PIOC->PIO_ODR = (AT91C_PIO_PC4 | AT91C_PIO_PC5);
	
	//AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOCDE);
	*PMC_PCER = AT91C_ID_PIOB| AT91C_ID_PIOCDE; //clock 
	
#define SPI_CLOCK_DIVIDER (0x1F << 8)
	//initTerm
	*PMC_PCER = AT91C_ID_SPI1;
	AT91C_BASE_PIOB->PIO_PDR = (AT91C_PIO_PB15 | AT91C_PIO_PB12 | AT91C_PIO_PB14); // disabling SPI CLK, MISO and chips select for PIO, thus peripheral control
//	AT91C_BASE_PMC->PMC_PCER = (0x1 << AT91C_ID_SPI1); //clock for peripheral SPI 1 
	AT91C_BASE_PIOB->PIO_ASR = AT91C_PIO_PB15 | AT91C_PIO_PB12 | AT91C_PIO_PB14; //selects peripheral A from PIO Controller B Multiplexing
	AT91C_BASE_SPI1->SPI_MR = AT91C_SPI_MSTR | AT91C_SPI_PS_FIXED | AT91C_SPI_MODFDIS;// | (0xE << 16); // sets to master mode, for fixed peripheral select, disabled fauld detection and selects PCS = xxx0NPCS[3:0] = 1110
	AT91C_BASE_SPI1->SPI_CSR[0] = AT91C_SPI_BITS_16 | SPI_CLOCK_DIVIDER  | AT91C_SPI_NCPHA;//  ncpha ->1 mode -> 3
	AT91C_BASE_SPI1->SPI_CR = AT91C_SPI_SPIEN; //enabling SPI to receive/transmit data
	
		
	//initIO
	//DBGU_CR - Control Register, TXEN - (DBGU) Transmitter Enable, RXEN - (DBGU) Receiver Enable
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN | AT91C_US_RXEN;
	//DBGU_MR - Debug Unit Mode Register , AT91C_US_PAR_NONE - (DBGU) No Parity, AT91C_US_CHMODE_NORMAL - (DBGU) Normal Mode: The USART channel operates as an RX/TX USART.
	AT91C_BASE_DBGU->DBGU_MR = AT91C_US_PAR_NONE | AT91C_US_CHMODE_NORMAL;
//	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTTX; //to disable
	//DBGU_BRGR - (DBGU) Baud Rate Generator Register
	AT91C_BASE_DBGU->DBGU_BRGR = 54;//115200
	//AT91C_BASE_DBGU->DBGU_THR = 0;
	
	//intiDecoder
	AT91C_BASE_PIOB->PIO_PER = 1 << 17;
	AT91C_BASE_PIOB->PIO_ODR = 1 << 17;
	
}


int buttonsControl() { //0 - both buttons released or both pressed, 1 - left pressed, 2 - right one pressed
	if ((!(AT91C_BASE_PIOC->PIO_PDSR & (AT91C_PIO_PC4))) && (!(AT91C_BASE_PIOC->PIO_PDSR & (AT91C_PIO_PC5))))
		return 0;
	else if (!(AT91C_BASE_PIOC->PIO_PDSR & (AT91C_PIO_PC4))) {// if 0 at 4 bit, then button is pressed 
		return 2;
	} else if (!(AT91C_BASE_PIOC->PIO_PDSR & (AT91C_PIO_PC5))) {// if 0 at 4 bit, then button is pressed 
		return 1;
	}
	return 0;
}

void runDiode(int diode1, int diode2) {
	static int counter=0;
	counter++; // counts occurence of PITS
	if(counter==3) { //turning on
		if(diode1)
			AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB8;
		if(diode2)
			AT91C_BASE_PIOC->PIO_CODR = AT91C_PIO_PC29;
	}
	else if(counter==10) { //turning off
		if(diode1)
			AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB8;
		if(diode2)
			AT91C_BASE_PIOC->PIO_SODR = AT91C_PIO_PC29;
		counter=0;
	}
	else {
		AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB8;
		AT91C_BASE_PIOC->PIO_SODR = AT91C_PIO_PC29;
	}
}


void runDisp(unsigned int number) {
	static int counter=0;
	counter++; // counts occurence of PITS
	if(counter==1 && number >= 10) { //tens
		clearSegments();
		AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB28;
		AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB30;
		runDigit((number/10));  //TBD

	}
	if(counter==2) { //turning off
		clearSegments();
		AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB30;
		AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB28;
		runDigit(number-(number/10)*10);  //TBD
		counter=0;
	}
}

void runDigit (int digit) {
	switch (digit) {
		case 1:
			lightSegment(2, AT91C_PIO_PB24, AT91C_PIO_PB22);
			break;
		case 2:
			lightSegment(5, AT91C_PIO_PB25, AT91C_PIO_PB24, AT91C_PIO_PB26, AT91C_PIO_PB20, AT91C_PIO_PB21);
			break;
		case 3:
			lightSegment(5, AT91C_PIO_PB25, AT91C_PIO_PB24, AT91C_PIO_PB26, AT91C_PIO_PB22, AT91C_PIO_PB21);
			break;
		case 4:
			lightSegment(4, AT91C_PIO_PB27, AT91C_PIO_PB26, AT91C_PIO_PB24, AT91C_PIO_PB22);
			break;
		case 5:
			lightSegment(5, AT91C_PIO_PB25, AT91C_PIO_PB27, AT91C_PIO_PB26, AT91C_PIO_PB22, AT91C_PIO_PB21);
			break;
		case 6:
			lightSegment(6, AT91C_PIO_PB25, AT91C_PIO_PB27, AT91C_PIO_PB20, AT91C_PIO_PB21, AT91C_PIO_PB22, AT91C_PIO_PB26);
			break;
		case 7:
			lightSegment(3, AT91C_PIO_PB25, AT91C_PIO_PB24, AT91C_PIO_PB22);
			break;
		case 8:
			lightSegment(7, AT91C_PIO_PB25, AT91C_PIO_PB24, AT91C_PIO_PB22, AT91C_PIO_PB21, AT91C_PIO_PB20, AT91C_PIO_PB27, AT91C_PIO_PB26);
			break;
		case 9:
			lightSegment(6, AT91C_PIO_PB25, AT91C_PIO_PB24, AT91C_PIO_PB22, AT91C_PIO_PB21, AT91C_PIO_PB27, AT91C_PIO_PB26);
			break;
		case 0:
			lightSegment(6, AT91C_PIO_PB25, AT91C_PIO_PB24, AT91C_PIO_PB22, AT91C_PIO_PB21, AT91C_PIO_PB20, AT91C_PIO_PB27);
			break;
	  default:
	    break;
	}

		
}

void clearSegments() {
	dimSegment(8,AT91C_PIO_PB25, AT91C_PIO_PB24, AT91C_PIO_PB22, AT91C_PIO_PB21, AT91C_PIO_PB20, AT91C_PIO_PB27, AT91C_PIO_PB26, AT91C_PIO_PB23);
}

void lightSegment(int num, ...) {
	int i;
	va_list valist;
	va_start(valist, num);
	for (i = 0; i < num; i++)
		AT91C_BASE_PIOB->PIO_SODR = va_arg(valist, int);
	va_end(valist);
}

void dimSegment(int num, ...) {
	int i;
	va_list valist;
	va_start(valist, num);
	for (i = 0; i < num; i++)
		AT91C_BASE_PIOB->PIO_CODR = va_arg(valist, int);
	va_end(valist);
}
