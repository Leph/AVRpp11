#ifndef BITS_H
#define BITS_H

#include "types.h"

namespace bits {

/**
 * Return the OR-ed typed value
 * of given individual variadic Bit Nums
 */
template <class T>
inline T value(typename Type<T>::bitNum num)
{
    return (1 << static_cast<uint8_t>(num));
}
template <class T, class ... BitNums>
inline T value(typename Type<T>::bitNum num, BitNums... bitNums)
{
    return (value<T>(num) | value<T>(bitNums...));
}

/**
 * Set the given memory register to zero and set to
 * True given variadic Bit Nums
 */
template <class T, class ... BitNums>
inline void set(volatile T& mem, BitNums... bitNums)
{
    mem = value<T>(bitNums...);
}

/**
 * Set the given variadic Bit Nums to True in given
 * memory register (no reset is done).
 */
template <class T, class ... BitNums>
inline void add(volatile T& mem, BitNums... bitNums)
{
    mem |= value<T>(bitNums...);
}

/**
 * Unset the given variadic Bit Nums to False in given
 * memory register (no reset is done).
 */
template <class T, class ... BitNums>
inline void clear(volatile T& mem, BitNums... bitNums)
{
    mem &= ~value<T>(bitNums...);
}

/**
 * Return the Logic value of given bit number
 * of given memory register
 */
template <class T>
inline logic get(volatile T& mem, typename Type<T>::bitNum num)
{
    return static_cast<logic>(mem & value<T>(num));
}

/**
 * Set the given bit number of given
 * memory register with given logic value
 */
template <class T>
inline void set(volatile T& mem, typename Type<T>::bitNum num, logic value)
{
    if (value == TRUE) {
        add(mem, num);
    } else {
        clear(mem, num);
    }
}

}

#endif

