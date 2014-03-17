#ifndef BITS_H
#define BITS_H

namespace bits {

/**
 * Definition of bit numbers
 * up to 16 bits (word)
 * zero indexed
 */
enum BitNum : uint8_t
{
    Bit0 = 0,
    Bit1 = 1,
    Bit2 = 2,
    Bit3 = 3,
    Bit4 = 4,
    Bit5 = 5,
    Bit6 = 6,
    Bit7 = 7,
    Bit8 = 8,
    Bit9 = 9,
    Bit10 = 10,
    Bit11 = 11,
    Bit12 = 12,
    Bit13 = 13,
    Bit14 = 14,
    Bit15 = 15,
};

/**
 * Return the OR-ed typed value
 * of given individual variadic Bit Nums
 */
template <class T>
inline T value(BitNum num)
{
    return (1 << static_cast<uint8_t>(num));
}
template <class T, class ... BitNums>
inline T value(BitNum num, BitNums... bitNums)
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
inline logic get(volatile T& mem, BitNum num)
{
    return static_cast<logic>(mem & value<T>(num));
}

/**
 * Set the given bit number of given
 * memory register with given logic value
 */
template <class T>
inline void set(volatile T& mem, BitNum num, logic value)
{
    if (value == True) {
        add(mem, num);
    } else {
        clear(mem, num);
    }
}

}

#endif

