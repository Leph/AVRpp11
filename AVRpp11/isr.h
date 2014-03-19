#ifndef ISR_H
#define ISR_H

namespace isr {

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

}

#endif

