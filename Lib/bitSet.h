#ifndef BITSET_H
#define BITSET_H

#include "types.h"

/**
 * Return the OR-ed typed value
 * of given individual variadic Bit Nums
 */
template <class T>
inline T bitValue(typename Type<T>::bitNum num)
{
    return (1 << static_cast<uint8_t>(num));
}

template <class T, class ... BitNums>
inline T bitValue(typename Type<T>::bitNum num, BitNums... bitNums)
{
    return (bitValue<T>(num) | bitValue<T>(bitNums...));
}

/**
 * Set the given memory register to zero and set to
 * True given variadic Bit Nums
 */
template <class T, class ... BitNums>
inline void setBits(volatile T& mem, BitNums... bitNums)
{
    mem = bitValue<T>(bitNums...);
}

/**
 * Set the given variadic Bit Nums to True in given
 * memory register (no reset is done).
 */
template <class T, class ... BitNums>
inline void addBits(volatile T& mem, BitNums... bitNums)
{
    mem |= bitValue<T>(bitNums...);
}

/**
 * Unset the given variadic Bit Nums to False in given
 * memory register (no reset is done).
 */
template <class T, class ... BitNums>
inline void clearBits(volatile T& mem, BitNums... bitNums)
{
    mem &= ~bitValue<T>(bitNums...);
}

#endif

