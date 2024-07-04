#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

unsigned char second_unit=0;
unsigned char second_tens=0;
unsigned char minute_unit=0;
unsigned char minute_tens=0;
unsigned char hour_unit=0;
unsigned char hour_tens=0;

void setTimer1(void)
{
	TCCR1A|=(1<<FOC1A)|(1<<WGM12);
	TCCR1B|=(1<<CS11);
	TIMSK|=(1<<OCIE1A);
	TCNT1=0;
	OCR1A=62499;
}

void setInt0(void)
{
	GICR|=(1<<6);
	MCUCR|=(1<<ISC01);
	PORTD|=(1<<2);
}

void setInt1(void)
{
	GICR|=(1<<7);
	MCUCR|=(1<<ISC11)|(1<<ISC10);
}

void setInt2(void)
{
	GICR|=(1<<5);
	MCUCSR&=~(1<<ISC2);
	PORTB|=(1<<2);
}

ISR(TIMER1_COMPA_vect)
{
	if(second_unit==9)
	{
		if(second_tens==5)
		{
			second_unit=0;
			second_tens=0;
			if(minute_unit==9)
			{
				if(minute_tens==5)
				{
					minute_unit=0;
					minute_tens=0;
					if(hour_unit==9)
					{
						hour_unit=0;
						hour_tens++;
					}
					else{
						hour_unit++;
					}
				}
				else{
					minute_unit=0;
					minute_tens++;
				}
			}
			else{
				minute_unit++;
			}
		}
		else{
			second_unit=0;
			second_tens++;
		}

	}
	else{
		second_unit++;
	}
}

ISR(INT0_vect)
{
	second_unit=0;
	second_tens=0;
	minute_unit=0;
	minute_tens=0;
	hour_unit=0;
	hour_tens=0;
}

ISR(INT1_vect)
{
	TCCR1B&=~(1<<CS10)&~(1<<CS11)&~(1<<CS12);
}

ISR(INT2_vect)
{
	TCCR1B|=(1<<CS11);
}

int main (void)
{
	DDRC=0x0F;
	DDRA=0x3F;
	PORTC=0x0F;
	PORTA=0;
	SREG|=(1<<7);
	setInt0();
	setInt1();
	setInt2();
	setTimer1();

	while(1)
	{
		PORTA=1;
		PORTC=second_unit;
		_delay_ms(2);
		PORTA=2;
		PORTC=second_tens;
		_delay_ms(2);

		PORTA=4;
		PORTC=minute_unit;
		_delay_ms(2);
		PORTA=8;
		PORTC=minute_tens;
		_delay_ms(2);

		PORTA=0x10;
		PORTC=hour_unit;
		_delay_ms(2);
		PORTA=0x20;
		PORTC=hour_tens;
		_delay_ms(2);
	}
}
