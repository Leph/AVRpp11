#ifndef GPIO_H
#define GPIO_H

namespace gpio {

/**
 * Gpio pin mode
 */
enum GpioMode : uint8_t
{
    Input = 0,
    Output = 1,
    InputPullUp = 2,
};

/**
 * Gpio data structure for
 * runtime mapping
 */
struct GpioMappingEntry
{
    const bytePtr dirReg;
    const bytePtr outReg;
    const bytePtr inReg;
    const bits::BitNum num;
};

/**
 * Gpio pin declaration
 * for device specific layout
 */
enum GpioPin : uint8_t
{
    #define X(Pin, Port, Num) Pin,
    GPIO_DEFINES
    #undef X
};

/**
 * Gpio runtime global array
 * with device specific mapping
 */
const GpioMappingEntry GpioMapping[] = {
    #define X(Pin, Port, Num) \
    { \
        &DDR##Port, \
        &PORT##Port, \
        &PIN##Port, \
        bits::Bit##Num \
    },
    GPIO_DEFINES
    #undef X
};

/**
 * Set the given Gpio Pin to given mode
 */
inline void mode(GpioPin pin, GpioMode mode)
{
    if (mode == Input) {
        bits::clear(
            *(GpioMapping[pin].dirReg),
            GpioMapping[pin].num);
    } else if (mode == Output) {
        bits::add(
            *(GpioMapping[pin].dirReg),
            GpioMapping[pin].num);
    } else if (mode == InputPullUp) {
        bits::clear(
            *(GpioMapping[pin].dirReg),
            GpioMapping[pin].num);
        bits::add(
            *(GpioMapping[pin].outReg),
            GpioMapping[pin].num);
    }
}

/**
 * Write given logical value to given
 * Gpio Pin (should be Output mode)
 */
inline void write(GpioPin pin, logic value)
{
    bits::set(
        *(GpioMapping[pin].outReg),
        GpioMapping[pin].num, 
        value);
}

/**
 * Read and return the logical value
 * of given Gpio Pin (should be in Input mode)
 */
inline logic read(GpioPin pin)
{
    return bits::get(
        *(GpioMapping[pin].inReg),
        GpioMapping[pin].num);
}

}

#endif

