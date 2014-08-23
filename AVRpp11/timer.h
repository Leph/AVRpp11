#ifndef TIMER_H
#define TIMER_H

namespace timer {

/**
 * Timer specific interrupt handler
 */
struct Timer0Object;
typedef isr::Handler<Timer0Object> HandlerTimer0;
struct Timer1Object;
typedef isr::Handler<Timer1Object> HandlerTimer1;

/**
 * Timer wave generation mode
 * WaveNormal and WavePwm
 * (See output pin mode)
 * and Timer Top counter mode
 * TopNormal : the counter is cleared when it overflow
 * TopCompareA : the counter is cleared when it matches
 * with A output compare register (therefor the output 
 * A Pin should not be used as output)
 */
enum TimerCounterMode : uint8_t {
    WaveNormalTopNormal,
    WaveNormalTopCompareA,
    WavePwmTopNormal,
    WavePwmTopCompareA,
};

/**
 * Timer output Pin mode
 * Disable : no specific output
 * Toggle : in normal wave mode, output pin value 
 * is toggle on compare match
 * Set : in normal wave mode, output pin value 
 * is set on compare match
 * Clear : in normal wave mode, output pin value 
 * is clear on compare match
 * Pwm : in pwm wave mode, output pin is set
 * at bottom and clear at compare match
 * PwmInv : in pwm wave mode, output pin is clear
 * at bottom and set at compare match
 * The Gpio Pin have to be in output mode
 */
enum TimerPinMode : uint8_t {
    PinDisable,
    PinToggle,
    PinSet,
    PinClear,
    PinPwm,
    PinPwmInv,
};

/**
 * Timer clock source
 * Stop : the timer is disabled
 * 1 : no prescaler
 * 8, 64, 256, 1024 : prescaler value for internal
 * clock source division
 * ExternalFalling : the counter is incremented when
 * input pin received falling edge
 * ExternalRising : the counter is incremented when
 * input pin received rising edge
 * (The external input pin can be either in 
 * input or output Gpio mode)
 */
enum TimerClock : uint8_t {
    ClockStop,
    ClockDiv1,
    ClockDiv8,
    ClockDiv64,
    ClockDiv256,
    ClockDiv1024,
    ClockExternalFalling,
    ClockExternalRising,
}; 


/**
 * Hardware Timer0 8 bits
 */
struct Timer0Object
{
    /**
     * A and B control, counter, A and B compare, 
     * mask and flag registers
     */
    const bytePtr controlAReg;
    const bytePtr controlBReg;
    const bytePtr counterReg;
    const bytePtr compareAReg;
    const bytePtr compareBReg;
    const bytePtr maskReg;
    const bytePtr flagReg;

    /**
     * User defined
     * interrupt routines
     */
    HandlerTimer0::type onMatchAFunc;
    HandlerTimer0::type onMatchBFunc;
    HandlerTimer0::type onOverflowFunc;

    /**
     * Configure given timer wave and top counter mode
     */
    inline void setCounterMode(TimerCounterMode mode) const
    {
        if (mode == WaveNormalTopNormal) {
            bits::add(*controlBReg, ~bits::Bit3);
            bits::add(*controlAReg, ~bits::Bit1, ~bits::Bit0);
        } else if (mode == WaveNormalTopCompareA) {
            bits::add(*controlBReg, ~bits::Bit3);
            bits::add(*controlAReg, bits::Bit1, ~bits::Bit0);
        } else if (mode == WavePwmTopNormal) {
            bits::add(*controlBReg, ~bits::Bit3);
            bits::add(*controlAReg, bits::Bit1, bits::Bit0);
        } else if (mode == WavePwmTopCompareA) {
            bits::add(*controlBReg, bits::Bit3);
            bits::add(*controlAReg, bits::Bit1, bits::Bit0);
        }
    }

    /**
     * Configure output pin mode for Pin A and B
     * with given mode
     */
    inline void setPinModeA(TimerPinMode mode) const
    {
        if (mode == PinDisable) {
            bits::add(*controlAReg, ~bits::Bit7, ~bits::Bit6);
        } else if (mode == PinToggle) {
            bits::add(*controlAReg, ~bits::Bit7, bits::Bit6);
        } else if (mode == PinSet) {
            bits::add(*controlAReg, bits::Bit7, bits::Bit6);
        } else if (mode == PinClear) {
            bits::add(*controlAReg, bits::Bit7, ~bits::Bit6);
        } else if (mode == PinPwm) {
            bits::add(*controlAReg, bits::Bit7, ~bits::Bit6);
        } else if (mode == PinPwmInv) {
            bits::add(*controlAReg, bits::Bit7, bits::Bit6);
        }
    }
    inline void setPinModeB(TimerPinMode mode) const
    {
        if (mode == PinDisable) {
            bits::add(*controlAReg, ~bits::Bit5, ~bits::Bit4);
        } else if (mode == PinToggle) {
            bits::add(*controlAReg, ~bits::Bit5, bits::Bit4);
        } else if (mode == PinSet) {
            bits::add(*controlAReg, bits::Bit5, bits::Bit4);
        } else if (mode == PinClear) {
            bits::add(*controlAReg, bits::Bit5, ~bits::Bit4);
        } else if (mode == PinPwm) {
            bits::add(*controlAReg, bits::Bit5, ~bits::Bit4);
        } else if (mode == PinPwmInv) {
            bits::add(*controlAReg, bits::Bit5, bits::Bit4);
        }
    }

    /**
     * Configure given clock divider
     */
    inline void setClock(TimerClock clock) const
    {
        if (clock == ClockStop) {
            bits::add(*controlBReg, ~bits::Bit2, ~bits::Bit1, ~bits::Bit0);
        } else if (clock == ClockDiv1) {
            bits::add(*controlBReg, ~bits::Bit2, ~bits::Bit1, bits::Bit0);
        } else if (clock == ClockDiv8) {
            bits::add(*controlBReg, ~bits::Bit2, bits::Bit1, ~bits::Bit0);
        } else if (clock == ClockDiv64) {
            bits::add(*controlBReg, ~bits::Bit2, bits::Bit1, bits::Bit0);
        } else if (clock == ClockDiv256) {
            bits::add(*controlBReg, bits::Bit2, ~bits::Bit1, ~bits::Bit0);
        } else if (clock == ClockDiv1024) {
            bits::add(*controlBReg, bits::Bit2, ~bits::Bit1, bits::Bit0);
        } else if (clock == ClockExternalFalling) {
            bits::add(*controlBReg, bits::Bit2, bits::Bit1, ~bits::Bit0);
        } else if (clock == ClockExternalRising) {
            bits::add(*controlBReg, bits::Bit2, bits::Bit1, bits::Bit0);
        }
    }

    /**
     * Read and write to counter register
     */
    inline byte readCounter() const
    {
        return *counterReg;
    }
    inline void writeCounter(byte value) const
    {
        *counterReg = value;
    }
    
    /**
     * Read and write to counter compare 
     * register A and B
     */
    inline byte readCompareA() const
    {
        return *compareAReg;
    }
    inline void writeCompareA(byte value) const
    {
        *compareAReg = value;
    }
    inline byte readCompareB() const
    {
        return *compareBReg;
    }
    inline void writeCompareB(byte value) const
    {
        *compareBReg = value;
    }

    /**
     * Return true when a compare match A, B 
     * or overflow has occured
     * The flag is cleared manually or when associated 
     * interruption is executed
     */
    inline logic isMatchA() const
    {
        return bits::get(*flagReg, bits::Bit1);
    }
    inline logic isMatchB() const
    {
        return bits::get(*flagReg, bits::Bit2);
    }
    inline logic isOverflow() const
    {
        return bits::get(*flagReg, bits::Bit0);
    }

    /**
     * Manually clear compare match A, B 
     * and overflow flags
     */
    inline void clearMatchA() const
    {
        bits::add(*flagReg, bits::Bit1);
    }
    inline void clearMatchB() const
    {
        bits::add(*flagReg, bits::Bit2);
    }
    inline void clearOverflow() const
    {
        bits::add(*flagReg, bits::Bit0);
    }
    
    /**
     * Initialize and set up interrupt routine
     * with given callback fired when 
     * the A and B compare match or overflow occured
     * Or disable the interrupt
     *
     * For WaveNormal, overflow occurs at MAX value (0xFFFF)
     * and for WavePwn, overflow occurs at TOP value (before 
     * the counter loops to zero)
     */
    inline void onMatchA
        (HandlerTimer0::type handler = HandlerTimer0::Disable)
    {
        onMatchAFunc = handler;
        if (handler != HandlerTimer0::Disable) {
            bits::add(*maskReg, bits::Bit1);
        } else {
            bits::add(*maskReg, ~bits::Bit1);
        }
    }
    inline void onMatchB
        (HandlerTimer0::type handler = HandlerTimer0::Disable)
    {
        onMatchBFunc = handler;
        if (handler != HandlerTimer0::Disable) {
            bits::add(*maskReg, bits::Bit2);
        } else {
            bits::add(*maskReg, ~bits::Bit2);
        }
    }
    inline void onOverflow
        (HandlerTimer0::type handler = HandlerTimer0::Disable)
    {
        onOverflowFunc = handler;
        if (handler != HandlerTimer0::Disable) {
            bits::add(*maskReg, bits::Bit0);
        } else {
            bits::add(*maskReg, ~bits::Bit0);
        }
    }
};

/**
 * Hardware Timer1 16 bits
 */
struct Timer1Object
{
    /**
     * A, B and C control, counter, A and B compare, 
     * mask and flag registers
     */
    const bytePtr controlAReg;
    const bytePtr controlBReg;
    const bytePtr controlCReg;
    const wordPtr counterReg;
    const wordPtr compareAReg;
    const wordPtr compareBReg;
    const wordPtr captureReg;
    const bytePtr maskReg;
    const bytePtr flagReg;

    /**
     * User defined
     * interrupt routines
     */
    HandlerTimer1::type onMatchAFunc;
    HandlerTimer1::type onMatchBFunc;
    HandlerTimer1::type onOverflowFunc;

    /**
     * Configure given timer wave and top counter mode
     */
    inline void setCounterMode(TimerCounterMode mode) const
    {
        if (mode == WaveNormalTopNormal) {
            bits::add(*controlBReg, ~bits::Bit4, ~bits::Bit3);
            bits::add(*controlAReg, ~bits::Bit1, ~bits::Bit0);
        } else if (mode == WaveNormalTopCompareA) {
            bits::add(*controlBReg, ~bits::Bit4, bits::Bit3);
            bits::add(*controlAReg, ~bits::Bit1, ~bits::Bit0);
        } else if (mode == WavePwmTopNormal) {
            *captureReg = 0xFFFF;
            bits::add(*controlBReg, bits::Bit4, bits::Bit3);
            bits::add(*controlAReg, bits::Bit1, ~bits::Bit0);
        } else if (mode == WavePwmTopCompareA) {
            bits::add(*controlBReg, bits::Bit4, bits::Bit3);
            bits::add(*controlAReg, bits::Bit1, bits::Bit0);
        }
    }

    /**
     * Configure output pin mode for Pin A and B
     * with given mode
     */
    inline void setPinModeA(TimerPinMode mode) const
    {
        if (mode == PinDisable) {
            bits::add(*controlAReg, ~bits::Bit7, ~bits::Bit6);
        } else if (mode == PinToggle) {
            bits::add(*controlAReg, ~bits::Bit7, bits::Bit6);
        } else if (mode == PinSet) {
            bits::add(*controlAReg, bits::Bit7, bits::Bit6);
        } else if (mode == PinClear) {
            bits::add(*controlAReg, bits::Bit7, ~bits::Bit6);
        } else if (mode == PinPwm) {
            bits::add(*controlAReg, bits::Bit7, ~bits::Bit6);
        } else if (mode == PinPwmInv) {
            bits::add(*controlAReg, bits::Bit7, bits::Bit6);
        }
    }
    inline void setPinModeB(TimerPinMode mode) const
    {
        if (mode == PinDisable) {
            bits::add(*controlAReg, ~bits::Bit5, ~bits::Bit4);
        } else if (mode == PinToggle) {
            bits::add(*controlAReg, ~bits::Bit5, bits::Bit4);
        } else if (mode == PinSet) {
            bits::add(*controlAReg, bits::Bit5, bits::Bit4);
        } else if (mode == PinClear) {
            bits::add(*controlAReg, bits::Bit5, ~bits::Bit4);
        } else if (mode == PinPwm) {
            bits::add(*controlAReg, bits::Bit5, ~bits::Bit4);
        } else if (mode == PinPwmInv) {
            bits::add(*controlAReg, bits::Bit5, bits::Bit4);
        }
    }

    /**
     * Configure given clock divider
     */
    inline void setClock(TimerClock clock) const
    {
        if (clock == ClockStop) {
            bits::add(*controlBReg, ~bits::Bit2, ~bits::Bit1, ~bits::Bit0);
        } else if (clock == ClockDiv1) {
            bits::add(*controlBReg, ~bits::Bit2, ~bits::Bit1, bits::Bit0);
        } else if (clock == ClockDiv8) {
            bits::add(*controlBReg, ~bits::Bit2, bits::Bit1, ~bits::Bit0);
        } else if (clock == ClockDiv64) {
            bits::add(*controlBReg, ~bits::Bit2, bits::Bit1, bits::Bit0);
        } else if (clock == ClockDiv256) {
            bits::add(*controlBReg, bits::Bit2, ~bits::Bit1, ~bits::Bit0);
        } else if (clock == ClockDiv1024) {
            bits::add(*controlBReg, bits::Bit2, ~bits::Bit1, bits::Bit0);
        } else if (clock == ClockExternalFalling) {
            bits::add(*controlBReg, bits::Bit2, bits::Bit1, ~bits::Bit0);
        } else if (clock == ClockExternalRising) {
            bits::add(*controlBReg, bits::Bit2, bits::Bit1, bits::Bit0);
        }
    }

    /**
     * Read and write to counter register
     */
    inline word readCounter() const
    {
        return *counterReg;
    }
    inline void writeCounter(word value) const
    {
        *counterReg = value;
    }
    
    /**
     * Read and write to counter compare 
     * register A and B
     */
    inline word readCompareA() const
    {
        return *compareAReg;
    }
    inline void writeCompareA(word value) const
    {
        *compareAReg = value;
    }
    inline word readCompareB() const
    {
        return *compareBReg;
    }
    inline void writeCompareB(word value) const
    {
        *compareBReg = value;
    }

    /**
     * Return true when a compare match A, B 
     * or overflow has occured
     * The flag is cleared manually or when associated 
     * interruption is executed
     */
    inline logic isMatchA() const
    {
        return bits::get(*flagReg, bits::Bit1);
    }
    inline logic isMatchB() const
    {
        return bits::get(*flagReg, bits::Bit2);
    }
    inline logic isOverflow() const
    {
        return bits::get(*flagReg, bits::Bit0);
    }

    /**
     * Manually clear compare match A, B 
     * and overflow flags
     */
    inline void clearMatchA() const
    {
        bits::add(*flagReg, bits::Bit1);
    }
    inline void clearMatchB() const
    {
        bits::add(*flagReg, bits::Bit2);
    }
    inline void clearOverflow() const
    {
        bits::add(*flagReg, bits::Bit0);
    }
    
    /**
     * Initialize and set up interrupt routine
     * with given callback fired when 
     * the A and B compare match or overflow occured
     * Or disable the interrupt
     *
     * For WaveNormal, overflow occurs at MAX value (0xFFFF)
     * and for WavePwn, overflow occurs at TOP value (before 
     * the counter loops to zero)
     */
    inline void onMatchA
        (HandlerTimer1::type handler = HandlerTimer1::Disable)
    {
        onMatchAFunc = handler;
        if (handler != HandlerTimer1::Disable) {
            bits::add(*maskReg, bits::Bit1);
        } else {
            bits::add(*maskReg, ~bits::Bit1);
        }
    }
    inline void onMatchB
        (HandlerTimer1::type handler = HandlerTimer1::Disable)
    {
        onMatchBFunc = handler;
        if (handler != HandlerTimer1::Disable) {
            bits::add(*maskReg, bits::Bit2);
        } else {
            bits::add(*maskReg, ~bits::Bit2);
        }
    }
    inline void onOverflow
        (HandlerTimer1::type handler = HandlerTimer1::Disable)
    {
        onOverflowFunc = handler;
        if (handler != HandlerTimer1::Disable) {
            bits::add(*maskReg, bits::Bit0);
        } else {
            bits::add(*maskReg, ~bits::Bit0);
        }
    }
};

/**
 * Define const global object
 * as Timer instance
 */
Timer0Object Timer0 = {
    &TCCR0A, &TCCR0B, &TCNT0, &OCR0A, &OCR0B, &TIMSK0, &TIFR0,
    HandlerTimer0::Disable, HandlerTimer0::Disable, HandlerTimer0::Disable 
};
Timer1Object Timer1 = {
    &TCCR1A, &TCCR1B, &TCCR1C, &TCNT1, &OCR1A, &OCR1B, &ICR1, &TIMSK1, &TIFR1,
    HandlerTimer1::Disable, HandlerTimer1::Disable, HandlerTimer1::Disable 
};

/**
 * Define Timer interruptions handler
 */
ISR(TIMER0_COMPA_vect)
{
    if (Timer0.onMatchAFunc != HandlerTimer0::Disable) {
        Timer0.onMatchAFunc(Timer0);
    }
}
ISR(TIMER0_COMPB_vect)
{
    if (Timer0.onMatchBFunc != HandlerTimer0::Disable) {
        Timer0.onMatchBFunc(Timer0);
    }
}
ISR(TIMER0_OVF_vect)
{
    if (Timer0.onOverflowFunc != HandlerTimer0::Disable) {
        Timer0.onOverflowFunc(Timer0);
    }
}
ISR(TIMER1_COMPA_vect)
{
    if (Timer1.onMatchAFunc != HandlerTimer1::Disable) {
        Timer1.onMatchAFunc(Timer1);
    }
}
ISR(TIMER1_COMPB_vect)
{
    if (Timer1.onMatchBFunc != HandlerTimer1::Disable) {
        Timer1.onMatchBFunc(Timer1);
    }
}
ISR(TIMER1_OVF_vect)
{
    if (Timer1.onOverflowFunc != HandlerTimer1::Disable) {
        Timer1.onOverflowFunc(Timer1);
    }
}

}

#endif

