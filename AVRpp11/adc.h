#ifndef ADC_H
#define ADC_H

namespace adc {

/**
 * Adc input channel
 *
 * Either pins or internal
 * temperature sensor, 1.1V or
 * ground input channel
 */
enum AdcInput : byte {
    PinAdc0,
    PinAdc1,
    PinAdc2,
    PinAdc3,
    PinAdc4,
    PinAdc5,
    Temperature,
    Internal,
    Ground,
};

/**
 * Adc reference voltage
 *
 * Supply is AVcc.
 * External is ARef.
 * Internal is 1.1V on atmega328p.
 * (External capacitor on ARef is advise).
 */
enum AdcReference : byte {
    ReferenceSupply,
    ReferenceExternal,
    ReferenceInternal,
};

/**
 * Analog to Digital Converter
 * 
 * Current implementation is limited.
 * Only 128 prescaler is supported (full accuraty).
 * Only 6 input pins are supported.
 * Only single mode is supported.
 */
struct AdcObject
{
    /**
     * Adc specific interrupt handler
     */
    typedef isr::Handler<AdcObject> Handler;

    /**
     * Multiplexer selection, 
     * control and status A/D, 
     * data low/high and digital input
     * disable registers
     */
    const bytePtr multiplexerReg;
    const bytePtr controlStatusAReg;
    const bytePtr controlStatusBReg;
    const bytePtr dataLowReg;
    const bytePtr dataHighReg;
    const bytePtr inputDisableReg;
    
    /**
     * User defined
     * interrupt routines
     */
    Handler::type onConversionCompleteFunc;

    /**
     * Set Adc input channel
     */
    inline void setInput(AdcInput input) const
    {
        if (input == PinAdc0) {
            bits::add(*multiplexerReg, 
                ~bits::Bit3, ~bits::Bit2, ~bits::Bit1, ~bits::Bit0);
        } else if (input == PinAdc1) {
            bits::add(*multiplexerReg, 
                ~bits::Bit3, ~bits::Bit2, ~bits::Bit1, bits::Bit0);
        } else if (input == PinAdc2) {
            bits::add(*multiplexerReg, 
                ~bits::Bit3, ~bits::Bit2, bits::Bit1, ~bits::Bit0);
        } else if (input == PinAdc3) {
            bits::add(*multiplexerReg, 
                ~bits::Bit3, ~bits::Bit2, bits::Bit1, bits::Bit0);
        } else if (input == PinAdc4) {
            bits::add(*multiplexerReg, 
                ~bits::Bit3, bits::Bit2, ~bits::Bit1, ~bits::Bit0);
        } else if (input == PinAdc5) {
            bits::add(*multiplexerReg, 
                ~bits::Bit3, bits::Bit2, ~bits::Bit1, bits::Bit0);
        } else if (input == Temperature) {
            bits::add(*multiplexerReg, 
                bits::Bit3, ~bits::Bit2, ~bits::Bit1, ~bits::Bit0);
        } else if (input == Internal) {
            bits::add(*multiplexerReg, 
                bits::Bit3, bits::Bit2, bits::Bit1, ~bits::Bit0);
        } else if (input == Ground) {
            bits::add(*multiplexerReg, 
                bits::Bit3, bits::Bit2, bits::Bit1, bits::Bit0);
        }
    }

    /**
     * Set Adc reference voltage
     */
    inline void setReference(AdcReference ref) const
    {
        if (ref == ReferenceExternal) {
            bits::add(*multiplexerReg, 
                ~bits::Bit7, ~bits::Bit6);
        } else if (ref == ReferenceSupply) {
            bits::add(*multiplexerReg, 
                ~bits::Bit7, bits::Bit6);
        } else if (ref == ReferenceInternal) {
            bits::add(*multiplexerReg, 
                bits::Bit7, bits::Bit6);
        }
    }

    /**
     * Enable and configure the Adc
     */
    inline void enable() const
    {
        //Set result right ajusted
        bits::add(*multiplexerReg, ~bits::Bit5);
        //Set prescaler to 128
        //and enable the Adc
        bits::add(*controlStatusAReg, 
            bits::Bit7, bits::Bit2, bits::Bit1, bits::Bit0);
    }

    /**
     * Disable the Adc znd stop current conversion
     */
    inline void disable() const
    {
        //Disable the Adc
        bits::add(*controlStatusAReg, ~bits::Bit7);
    }

    /**
     * Start a new conversion
     */
    inline void startConversion() const 
    {
        bits::add(*controlStatusAReg, bits::Bit6);
    }

    /**
     * Return true if the current 
     * convertion is not yet finished
     */
    inline logic isConverting() const
    {
        return bits::get(*controlStatusAReg, bits::Bit6);
    }

    /**
     * Read the 2 bytes value in data register
     */
    inline word readValue() const
    {
        return ADC;

        //Read low byte first and then high part
        word value;
        value = *dataLowReg & 0xFF;
        value |= *dataHighReg << 8;
        return value;
    }
    
    /**
     * Initialize and set up interrupt routine
     * with given callback fired when 
     * a analog conversion is completed.
     * Or disable the interrupt.
     */
    inline void onConversionComplete
        (Handler::type handler = Handler::Disable)
    {
        onConversionCompleteFunc = handler;
        if (handler != Handler::Disable) {
            bits::add(*controlStatusAReg, bits::Bit3);
        } else {
            bits::add(*controlStatusAReg, ~bits::Bit3);
        }
    }
};

/**
 * Define const global object
 * as Adc instance
 */
AdcObject Adc = {
    &ADMUX, &ADCSRA, &ADCSRB, &ADCL, &ADCH, &DIDR0,
    AdcObject::Handler::Disable
};

/**
 * Define Adc interruptions handler
 */
ISR(ADC_vect)
{
    if (Adc.onConversionCompleteFunc != AdcObject::Handler::Disable) {
        Adc.onConversionCompleteFunc(Adc);
    }
}

}

#endif

