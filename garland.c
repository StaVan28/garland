#define F_CTL 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
 
//-----------------------------------------------

// regs and PORT* init defines
#define ALL_OUTPUT		0b00000000
#define ALL_INPUT		0b11111111

// operation with bits
#define   SET_BIT(bit_place)	 (1 << bit_place)
#define CLEAR_BIT(bit_place)	~(1 << bit_place)

// timer1 bits
#define CS10_BIT	0
#define CS11_BIT	1
#define CS12_BIT	2

// timer1 time defines
#define          ZERO_TIME	0
#define       QUARTER_TIME	16384
#define          HALF_TIME	32768
#define THREE_FOURTHS_TIME	49152

// extetn interapt 0 bits
#define ISC00_BIT	0
#define ISC01_BIT	1
#define  INT0_BIT	6
#define  INT0_PIN	2	// on ATmega8535

// SREG bits
#define I_BIT 7 // enable global interapt

//-----------------------------------------------

void init_registers        (void);
void init_button           (void);
void init_timer1           (void);
void init_extern_interrups (void);

void  first_mode (void);
void second_mode (void);

// GLOBAL VARS
char   CUR_MODE_FUNC_COUNTER = 0;
void (*CUR_MODE_FUNC) (void) = first_mode;
//------

ISR(INT0_vect)
{
	if (CUR_MODE_FUNC_COUNTER == 0)
	{
		CUR_MODE_FUNC = second_mode;
		CUR_MODE_FUNC_COUNTER++;
	}
	else
	{
		CUR_MODE_FUNC = first_mode;
		CUR_MODE_FUNC_COUNTER = 0;
	}
	_delay_ms (200);
}

//-----------------------------------------------

int main ()
{
	init_registers ();

	init_button ();

	init_timer1 ();

	init_extern_interrups ();

	while (1)
	{
		CUR_MODE_FUNC ();
	}

	return 0;
}

//-----------------------------------------------

void init_registers (void)
{
	DDRC  = ALL_INPUT;
	PORTC = ALL_OUTPUT;
}

//------

void init_button (void)
{
	DDRD  &= CLEAR_BIT(INT0_PIN);
	PORTD |=   SET_BIT(INT0_PIN);
}

//------

void init_timer1 (void)
{
	TCCR1B = SET_BIT(CS10_BIT) | SET_BIT(CS11_BIT);
	TCNT1  = 0;	// decimal init
}

//------

void init_extern_interrups (void)
{
	GICR   = SET_BIT(INT0_BIT);
	MCUCR  = SET_BIT(ISC00_BIT) | SET_BIT(ISC01_BIT);
	SREG  |= SET_BIT(I_BIT);
}

//------

void first_mode (void)
{
	if (TCNT1 == ZERO_TIME)
	{
		PORTC |=   SET_BIT(0);
		PORTC &= CLEAR_BIT(1); 
	}

	if (TCNT1 == HALF_TIME)
	{
		PORTC |=   SET_BIT(1);
		PORTC &= CLEAR_BIT(0); 
	}
}

//------

void second_mode(void)
{
	if (TCNT1 == ZERO_TIME)
	{
		PORTC |=   SET_BIT(0);
		PORTC &= CLEAR_BIT(1); 
	}

	if (TCNT1 == QUARTER_TIME)
	{
		PORTC |=   SET_BIT(1);
		PORTC &= CLEAR_BIT(0); 
	}

	if (TCNT1 == HALF_TIME)
	{
		PORTC |=   SET_BIT(0);
		PORTC &= CLEAR_BIT(1); 
	}

	if (TCNT1 == THREE_FOURTHS_TIME)
	{
		PORTC |=   SET_BIT(1);
		PORTC &= CLEAR_BIT(0); 
	}
}

//------