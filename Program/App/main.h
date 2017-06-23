#ifndef MAIN_H
#define MAIN_H

#include "includes.h"


#define DIST_THRES				0.5
#define ULTRASONIC_CONST		0.01724

/******* ULTRASNOIC SENSOR CONFIGURATION    *******/
#define	ULTSEN1_DDR			DDRB
#define	ULTSEN1_PORT		PORTB
#define	ULTSEN1_PIN			PINB
#define ECHO1_PIN			PB3
#define TRIG1_PIN			PB2

#define	ULTSEN2_DDR			DDRB
#define	ULTSEN2_PORT		PORTB
#define	ULTSEN2_PIN			PINB
#define ECHO2_PIN			PB7
#define TRIG2_PIN			PB6

#define	ULTSEN3_DDR			DDRD
#define	ULTSEN3_PORT		PORTD
#define	ULTSEN3_PIN			PIND
#define ECHO3_PIN			PD0
#define TRIG3_PIN			PD1
/******* ULTRASNOIC SENSOR CONFIGURATION  ENDS *******/

struct  {
	volatile int8u msg:1;
	volatile int8u Meas:1;
	volatile int8u Sw:1;
	volatile int8u PIR:1;

}Flag;

//DEFINE MACROS
#define StartTmr()					TCCR0  	|= _BV(CS01)
#define StopTmr()					TCCR0  	&= ~_BV(CS01)


//FUNCTION PROTOTYPES
static	void	init		(void);
static	void	dispMsg		(void);
static	void 	tmr2init	(void);
static	void	Flagsinit	(void);
static	void	EXTinit		(void);
		void	MeasDist	(int8u sens,int8u disp, float *dist);
static	void	tmr0init	(void);
static	void	calcdist	(int8u sens);
		void	ultinit		(void);
static	void	DispTitle	(void);

#endif
