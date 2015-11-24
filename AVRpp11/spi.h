#ifndef SPI_HPP
#define SPI_HPP

namespace spi {

/**
 * Spi mode
 * On master mode, slave select (SS) has to 
 * be controled by user in ouput mode
 */
enum SpiMode : uint8_t {
    Master,
    Slave,
    Disable,
};

/**
 * Spi data transmiter order
 * (Most or Least significant bit first)
 */
enum SpiBitOrder : uint8_t {
    MSBFirst,
    LSBFirst,
};

/**
 * Spi clock idle logic level
 */
enum SpiClockIdle : uint8_t {
    ClockLow,
    ClockHigh,
};

/**
 * Spi data read and write on wire on 
 * leading (first) or trailing (last) clock edge
 */
enum SpiClockEdge : uint8_t {
    ClockLeading,
    ClockTrailing,
};

/**
 * Spi internal clock divider
 * (Set up transmission speed)
 */
enum SpiClockDivider : uint8_t {
    ClockDiv2,
    ClockDiv4,
    ClockDiv8,
    ClockDiv16,
    ClockDiv32,
    ClockDiv64,
    ClockDiv128,
};

/**
 * Hardware SPI Master/Slave
 */
struct SpiObject
{
    /**
     * Spi specific interrupt handler
     */
    typedef isr::Handler<SpiObject> Handler;

    /**
     * Control, status and data registers
     */
    const bytePtr controlReg;
    const bytePtr statusReg;
    const bytePtr dataReg;

    /**
     * User defined
     * interrupt routines
     */
    Handler::type onTransfertCompletFunc;
    
    /**
     * Configure and enable Spi mode or
     * disable it
     * Configure Spi Gpio io mode
     */
    inline void setMode(SpiMode mode) const
    {
        //Set mode
        if (mode == Master) {
            gpio::SCK.setMode(gpio::Output);
            gpio::MISO.setMode(gpio::Input);
            gpio::MOSI.setMode(gpio::Output);
            gpio::SS.setMode(gpio::Output);
            bits::add(*controlReg, bits::Bit6, bits::Bit4);
        } else if (mode == Slave) {
            gpio::SCK.setMode(gpio::Input);
            gpio::MISO.setMode(gpio::Output);
            gpio::MOSI.setMode(gpio::Input);
            gpio::SS.setMode(gpio::Input);
            bits::add(*controlReg, bits::Bit6, ~bits::Bit4);
        } else if (mode == Disable) {
            bits::add(*controlReg, ~bits::Bit6);
        }
    }

    /**
     * Configure given bit order
     */
    inline void setBitOrder(SpiBitOrder bitOrder) const
    {
        if (bitOrder == MSBFirst) {
            bits::add(*controlReg, ~bits::Bit5);
        } else if (bitOrder == LSBFirst) {
            bits::add(*controlReg, bits::Bit5);
        }
    }

    /**
     * Configure given clock idle logic level
     */
    inline void setClockIdle(SpiClockIdle idle) const
    {
        if (idle == ClockHigh) {
            bits::add(*controlReg, bits::Bit3);
        } else if (idle == ClockLow) {
            bits::add(*controlReg, ~bits::Bit3);
        }
    }

    /**
     * Configure given clock edge transmission
     */
    inline void setClockEdge(SpiClockEdge edge) const
    {
        if (edge == ClockLeading) {
            bits::add(*controlReg, bits::Bit2);
        } else if (edge == ClockTrailing) {
            bits::add(*controlReg, ~bits::Bit2);
        }
    }

    /**
     * Configure given clock divider
     */ 
    inline void setClockDivider(SpiClockDivider divider) const
    {
        if (divider == ClockDiv2) {
            bits::add(*controlReg, ~bits::Bit1, ~bits::Bit0);
            bits::add(*statusReg, bits::Bit0);
        } else if (divider == ClockDiv4) {
            bits::add(*controlReg, ~bits::Bit1, ~bits::Bit0);
            bits::add(*statusReg, ~bits::Bit0);
        } else if (divider == ClockDiv8) {
            bits::add(*controlReg, ~bits::Bit1, bits::Bit0);
            bits::add(*statusReg, bits::Bit0);
        } else if (divider == ClockDiv16) {
            bits::add(*controlReg, ~bits::Bit1, bits::Bit0);
            bits::add(*statusReg, ~bits::Bit0);
        } else if (divider == ClockDiv32) {
            bits::add(*controlReg, bits::Bit1, ~bits::Bit0);
            bits::add(*statusReg, bits::Bit0);
        } else if (divider == ClockDiv64) {
            bits::add(*controlReg, bits::Bit1, ~bits::Bit0);
            bits::add(*statusReg, ~bits::Bit0);
        } else if (divider == ClockDiv128) {
            bits::add(*controlReg, bits::Bit1, bits::Bit0);
            bits::add(*statusReg, ~bits::Bit0);
        }
    }

    /**
     * Read or write a byte to or from
     * Spi data register
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
     * Return true if Spi is in master mode 
     * and a byte is transmited and if Spi is in Slave mode
     * and a byte is received 
     */
    inline logic isTransfertComplet() const
    {
        return bits::get(*statusReg, bits::Bit7);
    }

    /**
     * Return true if data register is accessed during
     * a transfert
     */
    inline logic isCollision() const
    {
        return bits::get(*statusReg, bits::Bit6);
    }
    
    /**
     * Initialize and set up interrupt routine
     * with given callback fired when 
     * transfert is completed (read or write)
     * Or disable the interrupt
     */
    inline void onTransfertComplet
        (Handler::type handler = Handler::Disable)
    {
        onTransfertCompletFunc = handler;
        if (handler != Handler::Disable) {
            bits::add(*controlReg, bits::Bit7);
        } else {
            bits::add(*controlReg, ~bits::Bit7);
        }
    }
};

/**
 * Define const global object
 * as Spi instance
 */
SpiObject Spi = {
    &SPCR, &SPSR, &SPDR,
    SpiObject::Handler::Disable, 
};

/**
 * Define Spi interruptions handler
 */
ISR(SPI_STC_vect)
{
    if (Spi.onTransfertCompletFunc != SpiObject::Handler::Disable) {
        Spi.onTransfertCompletFunc(Spi);
    }
}

}

#endif

