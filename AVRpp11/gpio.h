#ifndef GPIO_H
#define GPIO_H

namespace gpio {

/**
 * Forward declaration of Gpio pin
 * (to be declared in devices implementation)
 */
enum GpioPin : uint8_t;

/**
 * Declare trait class for 
 * Gpio pin registers definition
 * (to be specialize in devices implementation)
 */
template <GpioPin T>
struct GpioPinReg {
};

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
 * Set mode implementation
 * (no function template partial specialization)
 */
template <GpioPin pin, GpioMode mode>
struct GpioModeImpl 
{
};
template <GpioPin pin>
struct GpioModeImpl<pin, Input>
{
    static void exec()
    {
        bits::set(
            *GpioPinReg<pin>::DirReg,
            GpioPinReg<pin>::DirBit,
            LOW);
    }
};
template <GpioPin pin>
struct GpioModeImpl<pin, Output>
{
    static void exec()
    {
        bits::set(
            *GpioPinReg<pin>::DirReg,
            GpioPinReg<pin>::DirBit,
            HIGH);
    }
};
template <GpioPin pin>
struct GpioModeImpl<pin, InputPullUp>
{
    static void exec()
    {
        bits::set(
            *GpioPinReg<pin>::DirReg,
            GpioPinReg<pin>::DirBit,
            LOW);
        bits::set(
            *GpioPinReg<pin>::OutReg,
            GpioPinReg<pin>::OutBit,
            HIGH);
    }
};

/**
 * Set for the given Pin the given Mode
 */
template <GpioPin pin, GpioMode m>
inline void mode()
{
    GpioModeImpl<pin, m>::exec();
}

/**
 * Set given output logic value for
 * given Pin
 */
template <GpioPin pin>
inline void write(logic value)
{
    bits::set(
        *GpioPinReg<pin>::OutReg,
        GpioPinReg<pin>::OutBit,
        value);
}

/**
 * Retrive from given Pin input logic value
 */
template <GpioPin pin>
inline logic read()
{
    return bits::get(
        *GpioPinReg<pin>::InReg,
        GpioPinReg<pin>::InBit);
}

}

#endif

