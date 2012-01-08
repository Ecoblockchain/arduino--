#include "arduino++.h"
#include "serial.h"

volatile uint16_t delta = 0;

ISR(TIMER0_COMPA_vect)
    {
    ILock il;
    delta = Timer1::TCNT;
    Arduino::D13::set();
    }

int main(void)
    {
    // Bare bones init
    Arduino::interrupts();
    Timer0::prescaler256();
    Timer1::prescaler1();

    Serial.begin(9600);

    for(;;)
        {
        {
        ILock il;
        delta = 0;
        Timer1::reset();
        Timer0::reset();
        Timer0::enableCompareInterruptA(128);
        }

        // wait for interrupt
        while (!delta)
            ;
        Timer0::disableCompareInterruptA();
        Arduino::D13::clear();
        
        Serial.write("0x");
        Serial.writeHex(delta);
        Serial.write("\r\n");
        _delay_ms(2000);
        }
    }
