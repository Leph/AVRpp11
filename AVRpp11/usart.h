#ifndef USART_H
#define USART_H

namespace usart {

/**
 * Usart specific interrupt handler
 */
struct UsartObject;
typedef isr::Handler<UsartObject> Handler;

/**
 * Usart transmiter and receiver mode
 */
enum UsartMode : uint8_t {
    Read,
    Write,
    ReadWrite,
    Disable,
};

/**
 * Usart number of stop bits
 */
enum UsartBitStop : uint8_t {
    BitStop1,
    BitStop2,
};

/**
 * Usart parity check mode
 */
enum UsartParity : uint8_t {
    ParityDisable,
    ParityEven,
    ParityOdd,
};

/**
 * Usart communication baud rate
 * (enum value are baudrate speed 
 * divide by 100)
 */
enum UsartBaudRate : uint16_t {
    BaudRate2400 = 24,
    BaudRate4800 = 48,
    BaudRate9600 = 96,
    BaudRate19200 = 192,
    BaudRate38400 = 384,
    BaudRate57600 = 576,
    BaudRate115200 = 1152,
};

/**
 * Compute the Usart UBRRn associated
 * with given CPU frequency and Baud rate
 */
constexpr inline word computeBaudRate(
    uint32_t cpuFrequency, UsartBaudRate baudrate)
{
    return (cpuFrequency/100)/(16*baudrate) - 1;
}

/**
 * Asynchronous serial port
 * transmiter and receiver
 */
struct UsartObject
{
    /**
     * Data, control and status, baud rate
     * usart registers
     */
    const bytePtr dataReg;
    const bytePtr AReg;
    const bytePtr BReg;
    const bytePtr CReg;
    const wordPtr baudRateReg;

    /**
     * User defined
     * interrupt routines
     */
    Handler::type onReadReadyFunc;
    Handler::type onDataSentFunc;
    Handler::type onWriteReadyFunc;

    /**
     * Configure given operating mode
     * and set transmited frame to 8 bits
     * and asynchronous mode
     * (Do not change interrupt callback)
     */
    inline void setMode(UsartMode mode) const
    {
        //Asynchronous mode
        bits::add(*CReg, ~bits::Bit6, ~bits::Bit7);
        //8 bits
        bits::add(*CReg, bits::Bit1, bits::Bit2);
        bits::add(*BReg, ~bits::Bit2);
        //Set mode
        if (mode == Read) {
            bits::add(*BReg, ~bits::Bit3, bits::Bit4);
        } else if (mode == Write) {
            bits::add(*BReg, bits::Bit3, ~bits::Bit4);
        } else if (mode == ReadWrite) {
            bits::add(*BReg, bits::Bit3, bits::Bit4);
        } else if (mode == Disable) {
            bits::add(*BReg, ~bits::Bit3, ~bits::Bit4);
        }
    }

    /**
     * Configure given stop bit
     */
    inline void setBitStop(UsartBitStop stop) const
    {
        if (stop == BitStop1) {
            bits::add(*CReg, ~bits::Bit3);
        } else if (stop == BitStop2) {
            bits::add(*CReg, bits::Bit3);
        }
    }

    /**
     * Configure given parity check
     */
    inline void setParity(UsartParity parity) const
    {
        if (parity == ParityDisable) {
            bits::add(*CReg, ~bits::Bit4, ~bits::Bit5);
        } else if (parity == ParityEven) {
            bits::add(*CReg, ~bits::Bit4, bits::Bit5);
        } else if (parity == ParityOdd) {
            bits::add(*CReg, bits::Bit4, bits::Bit5);
        }
    }

    /**
     * Configure given baudrate
     */
    inline void setBaudrate(UsartBaudRate baudrate) const
    {
        bits::add(*AReg, ~bits::Bit1);
        *baudRateReg = computeBaudRate(F_CPU, baudrate);
    }

    /**
     * Read or write a byte to or from
     * Usart data register
     */
    inline byte read() const
    {
        return *dataReg;
    }
    inline void write(byte value) const
    {
        *dataReg = value;
    }

    /**
     * Return true if new unread 
     * data is available in input buffer
     * (receive complete)
     */
    inline logic isReadReady() const
    {
        return bits::get(*AReg, bits::Bit7);
    }

    /**
     * Return true if writen data have been
     * shift to the wire (transmit complet)
     */
    inline logic isDataSent() const
    {
        return bits::get(*AReg, bits::Bit6);
    }

    /**
     * Return true if output buffer is available
     * for new write (data register empty)
     */
    inline logic isWriteReady() const
    {
        return bits::get(*AReg, bits::Bit5);
    }

    /**
     * Return true if no valid stop bit
     * have been received
     */
    inline logic isFrameError() const
    {
        return bits::get(*AReg, bits::Bit4);
    }

    /**
     * Return true if unread input buffer
     * have been override by new data
     */
    inline logic isOverRunError() const
    {
        return bits::get(*AReg, bits::Bit3);
    }

    /**
     * Return true the parity check is
     * enable and the check is failed
     */
    inline logic isParityError() const
    {
        return bits::get(*AReg, bits::Bit2);
    }

    /**
     * Return true if either frame,
     * overrun or parity error is set
     */
    inline logic isError() const
    {
        return isFrameError() || 
            isOverRunError() ||
            isParityError();
    }

    /**
     * Initialize and set up interrupt routine
     * with given callback fired when 
     * a byte is received
     * Or disable the interrupt
     */
    inline void onReadReady
        (Handler::type handler = Handler::Disable)
    {
        onReadReadyFunc = handler;
        if (handler != Handler::Disable) {
            bits::add(*BReg, bits::Bit7);
        } else {
            bits::add(*BReg, ~bits::Bit7);
        }
    }

    /**
     * Initialize and set up interrupt routine
     * with given callback fired when 
     * last outputed data is write to the wire
     * Or disable the interrupt
     */
    inline void onDataSent
        (Handler::type handler = Handler::Disable)
    {
        onDataSentFunc = handler;
        if (handler != Handler::Disable) {
            bits::add(*BReg, bits::Bit6);
        } else {
            bits::add(*BReg, ~bits::Bit6);
        }
    }

    /**
     * Initialize and set up interrupt routine
     * with given callback fired when 
     * output buffer is ready for a new write
     * Or disable the interrupt
     */
    inline void onWriteReady
        (Handler::type handler = Handler::Disable)
    {
        onWriteReadyFunc = handler;
        if (handler != Handler::Disable) {
            bits::add(*BReg, bits::Bit5);
        } else {
            bits::add(*BReg, ~bits::Bit5);
        }
    }

    /**
     * Print the given character or string to
     * initialized Usart
     */
    /*
    void print(char c) const
    {
        logic state = isr::getState();
        isr::disable();
        while (!isWriteReady());
        write(c);
        while (!isDataSent());
        isr::setState(state);
    }
    void print(const char* str) const
    {
        logic state = isr::getState();
        isr::disable();
        for (int i=0;str[i]!='\0';i++) {
            while (!isWriteReady());
            write(str[i]);
            while (!isDataSent());
        }
        isr::setState(state);
    }
    void print(int val) const
    {
        char str[6];
        uint8_t i = 0;
        int count = 10000;
        bool isPrint = false;

        while (count > 0) {
            byte digit = val/count;
            if (isPrint || count == 1 || digit != 0) {
                str[i] = digit + 48;
                isPrint = true;
                i++;
            }
            val = val - count*digit;
            count = count/10;
        }
        str[i] = '\0';
        
        print(str);
    }
    */
};

/**
 * Define const global object
 * as Usart instance
 */
UsartObject Usart0 = {
    &UDR0, &UCSR0A, &UCSR0B, &UCSR0C, &UBRR0,
    Handler::Disable, Handler::Disable, Handler::Disable 
};

/**
 * Define Usart interruptions handler
 */
ISR(USART_RX_vect)
{
    if (Usart0.onReadReadyFunc != Handler::Disable) {
        Usart0.onReadReadyFunc(Usart0);
    }
}
ISR(USART_UDRE_vect)
{
    if (Usart0.onWriteReadyFunc != Handler::Disable) {
        Usart0.onWriteReadyFunc(Usart0);
    }
}
ISR(USART_TX_vect)
{
    if (Usart0.onDataSentFunc != Handler::Disable) {
        Usart0.onDataSentFunc(Usart0);
    }
}

}

#endif

