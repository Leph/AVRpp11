#ifndef ISR_H
#define ISR_H

namespace isr {

/**
 * Typedef for interrupt 
 * routine handler and
 * no handler global constant
 * for specific device
 *
 * Interruptions are by default
 * non nested
 */
template <class T>
struct Handler {
    typedef void(*type)(const T&);
    constexpr static type Disable = nullptr;
};
template <>
struct Handler<void> {
    typedef void(*type)();
    constexpr static type Disable = nullptr;
};

/**
 * Enable or disable (all) global interrupts
 */
inline void enable()
{
    sei();
}
inline void disable()
{
    cli();
}

/**
 * Get and set interrupt global flag
 */
inline logic getState()
{
    bits::get(SREG, bits::Bit7);
}
inline void setState(logic val)
{
    bits::set(SREG, bits::Bit7, val);
}

}

#endif

