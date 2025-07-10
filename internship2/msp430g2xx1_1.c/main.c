/*
 * Making a motor controller program. The motor is controlled by push-buttons
 * ~2.4s (full step) 360 degree rotation time
 * ~1.9s (half step) 360 degree rotation time
 * -------Give motor time to warm up before taking it redline---------
 */

#include <msp430.h>

#define ON 1
#define OFF 0
#define CW 1	// clockwise
#define CCW 0	// counterclockwise
#define FULL 1
#define HALF 0

void port1_init(void);

void spin(void);
void timer(int n);



int Delay = 25;
int Rotation = FULL;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;

	port1_init();
	
	while (1)
	{
		
			spin();
	}
}

void port1_init(void)
{
	P1OUT = 0x00;				// port 1 initializaion for motor control signals
	P1SEL = 0x00;
	P1DIR = 0xFF;
	P1REN = 0x00;

	P1IES = 0x00;
	P1IFG = 0x00;
	P1IE = 0x00;
}

void timer(int n)	// n = 10 is 1ms
{
	while (n-- != 0)
		_delay_cycles(100);
}

void spin(void)
{
	const int half_cw[8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};
	const int half_ccw[8] = {0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09};
	unsigned int step = 0;


	

			
		
			for (step = 0; step < 8; step++)
			{
					P1OUT = half_cw[step];

				timer(Delay);
			}

	P1OUT = 0x00;
}
