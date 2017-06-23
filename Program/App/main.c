#include "main.h"

extern int8u lcdptr;
					
static volatile int16u ultpulse;

int main(void)
{
	int8u i;
	float dist;
	
	init();
	
	while (TRUE) {
		
		if (Flag.Meas) {
			
			Flag.Meas = FALSE;
			
			for (i = 1; i <= 2; i++) {
				MeasDist(i, 0, &dist);
				if (dist < DIST_THRES) {
					lcdclrr(1);
					lcdws("OBJECT DETECTED");
					beep(1,150);
					APRVoice(i-1);
					
				}
			}
			
			MeasDist(3, 0, &dist);
			
			if (dist <= 0.25) {
				lcdclrr(1);
				lcdws("***** STOP *****");
				beep(3,150);
				APRVoice(3);
				
			} else if (dist <= 0.5) {
				lcdclrr(1);
				lcdws("*****DANGER*****");
				beep(2,150);
				APRVoice(4);
				
			} else if (dist <= 1.0) {
				lcdclrr(1);
				lcdws("FRONT OBJECT DET");
				beep(1,150);
				APRVoice(2);
				
			}
			lcdclrr(1);
		
		}
		
		sleep();
	}
	return 0;
}
static void init(void)
{
	buzinit();
	ledinit();
	beep(2,100);
	APRinit();
	lcdinit();
	tmr2init();
	Flagsinit();
	DispTitle();
	ultinit();
	sei();

}

static void Flagsinit(void)
{
	Flag.msg = FALSE;
	Flag.Meas = FALSE;
	Flag.Sw = FALSE;
	Flag.PIR = FALSE;
}
static void tmr2init(void)
{
	TCNT2   = 75;
	TIMSK   |= _BV(TOIE2);			//ENABLE OVERFLOW INTERRUPT
	TCCR2  |=  _BV(CS22) | _BV(CS21) | _BV(CS20); /* PRESCALAR BY 256 */
}

/* overflows at every 25msec */
ISR(TIMER2_OVF_vect)
{
	static int8u i,j,k;
	TCNT2 = 75;

	if (++i >= 200) i = 0;
	switch(i) {
		case 0: case 5: ledon(); break;
		case 1: case 6: ledoff(); break;
	}
	
	if (++j >= 40) {
		j = 0;
		Flag.Meas =  TRUE;
	}
}

void MeasDist(int8u sens,int8u disp, float *dist)
{
	
	int8u i;
	int32u cnt;
	char s[10];
	
	cnt = 0;
	
	for (i = 0; i < 8; i++) {
		calcdist(sens);
		cnt += ultpulse;
	}
	cnt >>= 3;

	*dist = (cnt * ULTRASONIC_CONST);

	if (disp != 0) {
		lcdptr = disp;
		lcdws("    ");
		ftoa(*dist,s,2);
		lcdptr = disp;
		lcdws(s);
	}
}
static void calcdist(int8u sens)
{
	switch (sens) {
		case 1:
				ULTSEN1_PORT		|= _BV(TRIG1_PIN);
				dlyus(10);
				ULTSEN1_PORT		&= ~_BV(TRIG1_PIN);
				
				ultpulse = 0;

				while ((ULTSEN1_PIN & _BV(ECHO1_PIN)) == 0);
				StartTmr();
				while (ULTSEN1_PIN & _BV(ECHO1_PIN));
				StopTmr();
				break;
		case 2:
				ULTSEN2_PORT		|= _BV(TRIG2_PIN);
				dlyus(10);
				ULTSEN2_PORT		&= ~_BV(TRIG2_PIN);
				
				ultpulse = 0;

				while ((ULTSEN2_PIN & _BV(ECHO2_PIN)) == 0);
				StartTmr();
				while (ULTSEN2_PIN & _BV(ECHO2_PIN));
				StopTmr();
				break;
		case 3:
				ULTSEN3_PORT		|= _BV(TRIG3_PIN);
				dlyus(10);
				ULTSEN3_PORT		&= ~_BV(TRIG3_PIN);
				
				ultpulse = 0;

				while ((ULTSEN3_PIN & _BV(ECHO3_PIN)) == 0);
				StartTmr();
				while (ULTSEN3_PIN & _BV(ECHO3_PIN));
				StopTmr();
				break;
	}
	
}
void ultinit(void)
{
	ULTSEN1_DDR 		|= _BV(TRIG1_PIN);
	ULTSEN1_DDR 		&= ~_BV(ECHO1_PIN);

	ULTSEN1_PORT		&= ~_BV(TRIG1_PIN);
	ULTSEN1_PORT		|= _BV(ECHO1_PIN);
	
	ULTSEN2_DDR 		|= _BV(TRIG2_PIN);
	ULTSEN2_DDR 		&= ~_BV(ECHO2_PIN);

	ULTSEN2_PORT		&= ~_BV(TRIG2_PIN);
	ULTSEN2_PORT		|= _BV(ECHO2_PIN);
	
	ULTSEN3_DDR 		|= _BV(TRIG3_PIN);
	ULTSEN3_DDR 		&= ~_BV(ECHO3_PIN);

	ULTSEN3_PORT		&= ~_BV(TRIG3_PIN);
	ULTSEN3_PORT		|= _BV(ECHO3_PIN);
	
	tmr0init();
}

static void tmr0init(void)
{
	TCNT0   =  170;
	TIMSK   |= _BV(TOIE0);			//ENABLE OVERFLOW INTERRUPT
	
}
/* OverFlows every 10us */
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 170;
	++ultpulse;
}
static void DispTitle(void)
{
	lcdclr();
	lcdws("ArtificialVision");
}