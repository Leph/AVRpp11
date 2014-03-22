#ifndef GPIO_H
#define GPIO_H

namespace gpio {

/**
 * Gpio pin mode
 */
enum GpioMode : uint8_t
{
    Input,
    Output,
    InputPullUp,
};

/**
 * General purpose
 * Inpur Output pin
 */
struct GpioEntry
{
    /**
     * Direction, output and input
     * register and bit number
     */
    const bytePtr dirReg;
    const bytePtr outReg;
    const bytePtr inReg;
    const bits::BitNum num;

    /**
     * Set the Pin to given mode
     */
    inline void mode(GpioMode mode) const
    {
        if (mode == Input) {
            bits::clear(*dirReg, num);
        } else if (mode == Output) {
            bits::add(*dirReg, num);
        } else if (mode == InputPullUp) {
            bits::clear(*dirReg, num);
            bits::add(*outReg, num);
        }
    }
    
    /**
     * Write given logical value to the
     * Gpio Pin (should be Output mode)
     */
    inline void write(logic value) const
    {
        bits::set(*outReg, num, value);
    }

    /**
     * Read and return the logical input value
     * of the Gpio Pin (should be in Input mode)
     */
    inline logic read() const
    {
        return bits::get(*inReg, num);
    }

    /**
     * Read and return the logical outup value
     * of current Gpio Pin
     */
    inline logic readOutput() const
    {
        return bits::get(*outReg, num);
    }
};

/**
 * Define const global object
 * as Gpio Pin
 */
#define X(Pin, Port, Num) \
    const GpioEntry Pin = { \
        &DDR##Port, \
        &PORT##Port, \
        &PIN##Port, \
        bits::Bit##Num \
    };
GPIO_DEFINES
#undef X

}

#endif

