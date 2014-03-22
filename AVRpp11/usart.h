#ifndef USART_H
#define USART_H

namespace usart {

/**
 * Usart specific interrupt handler
 */
struct UsartEntry;
typedef isr::Handler<UsartEntry> Handler;

/**
 * Define global Usart interrupts
 * handler function pointer
 */
#define X(Usart, Data, A, B, C, BaudRate, VectRx, VectUDRE, VectTx) \
    Handler::type Usart##_onReadReady = Handler::Disable; \
    Handler::type Usart##_onDataSent = Handler::Disable; \
    Handler::type Usart##_onWriteReady = Handler::Disable;
USART_DEFINES
#undef X

/**
 * Usart transmiter and receiver mode
 */
enum UsartMode : uint8_t {
    Read,
    Write,
    ReadWrite,
};

/**
 * Usart number of stop bits
 */
enum UsartStop : uint8_t {
    Stop1,
    Stop2,
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
    BaudRate9600 = 96,
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
struct UsartEntry
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
     * Pointers to user defined
     * interrupt routines
     */
    Handler::type* const _onReadReady;
    Handler::type* const _onDataSent;
    Handler::type* const _onWriteReady;

    /**
     * Initialize and enable the Usart with
     * given operating mode, the baurate, stop bit
     * and parity check mode
     * The Usart is asynchronous, 8 bits
     */
    inline void init(
        UsartMode mode, 
        UsartBaudRate baudrate,
        UsartStop stop, 
        UsartParity parity) const
    {
        //Asynchronous mode
        bits::clear(*CReg, bits::Bit6, bits::Bit7);
        //Operating mode
        if (mode == Read) {
            bits::clear(*BReg, bits::Bit3);
            bits::add(*BReg, bits::Bit4);
        } else if (mode == Write) {
            bits::add(*BReg, bits::Bit3);
            bits::clear(*BReg, bits::Bit4);
        } else if (mode == ReadWrite) {
            bits::add(*BReg, bits::Bit3, bits::Bit4);
        }
        //Stop bit
        if (stop == Stop1) {
            bits::clear(*CReg, bits::Bit3);
        } else if (stop == Stop2) {
            bits::add(*CReg, bits::Bit3);
        }
        //Parity check
        if (parity == ParityDisable) {
            bits::clear(*CReg, bits::Bit4, bits::Bit5);
        } else if (parity == ParityEven) {
            bits::clear(*CReg, bits::Bit4);
            bits::add(*CReg, bits::Bit5);
        } else if (parity == ParityOdd) {
            bits::add(*CReg, bits::Bit4, bits::Bit5);
        }
        //8 bits
        bits::add(*CReg, bits::Bit1, bits::Bit2);
        bits::clear(*BReg, bits::Bit2);
        //Baud rate
        bits::clear(*AReg, bits::Bit1);
        *baudRateReg = computeBaudRate(F_CPU, baudrate);
    }

    /**
     * Disable both read and write
     * Usart operation
     * (Free associated Gpio)
     */
    inline void disable() const
    {
        bits::clear(*BReg, bits::Bit3, bits::Bit4);
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
        (Handler::type handler = Handler::Disable) const
    {
        if (handler != Handler::Disable) {
            bits::add(*BReg, bits::Bit7);
            *_onReadReady = handler;
        } else {
            bits::clear(*BReg, bits::Bit7);
        }
    }

    /**
     * Initialize and set up interrupt routine
     * with given callback fired when 
     * last outputed data is write to the wire
     * Or disable the interrupt
     */
    inline void onDataSent
        (Handler::type handler = Handler::Disable) const
    {
        if (handler != Handler::Disable) {
            bits::add(*BReg, bits::Bit6);
            *_onDataSent = handler;
        } else {
            bits::clear(*BReg, bits::Bit6);
        }
    }

    /**
     * Initialize and set up interrupt routine
     * with given callback fired when 
     * output buffer is ready for a new write
     * Or disable the interrupt
     */
    inline void onWriteReady
        (Handler::type handler = Handler::Disable) const
    {
        if (handler != Handler::Disable) {
            bits::add(*BReg, bits::Bit5);
            *_onWriteReady = handler;
        } else {
            bits::clear(*BReg, bits::Bit5);
        }
    }

    /**
     * Print the given character or string to
     * initialized Usart
     */
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
};

/**
 * Define const global object
 * as Usart instance
 */
#define X(Usart, Data, A, B, C, BaudRate, VectRx, VectUDRE, VectTx) \
    const UsartEntry Usart = { \
        &Data, \
        &A, \
        &B, \
        &C, \
        &BaudRate, \
        &Usart##_onReadReady, \
        &Usart##_onDataSent, \
        &Usart##_onWriteReady \
    };
USART_DEFINES
#undef X

/**
 * Define Usart interruptions handler
 */
#define X(Usart, Data, A, B, C, BaudRate, VectRx, VectUDRE, VectTx) \
    ISR(_VECTOR(VectRx)) \
    { \
        if (Usart##_onReadReady != Handler::Disable) { \
            Usart##_onReadReady(Usart); \
        } \
    } \
    ISR(_VECTOR(VectUDRE)) \
    { \
        if (Usart##_onWriteReady != Handler::Disable) { \
            Usart##_onWriteReady(Usart); \
        } \
    } \
    ISR(_VECTOR(VectTx)) \
    { \
        if (Usart##_onDataSent != Handler::Disable) { \
            Usart##_onDataSent(Usart); \
        } \
    }
USART_DEFINES
#undef X

}

#endif

