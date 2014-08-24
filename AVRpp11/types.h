#ifndef TYPES_H
#define TYPES_H

/**
 * Definition of 8 and 16 bits 
 * basic type and its pointer for
 * flash register
 */
typedef uint8_t byte;
typedef volatile uint8_t* bytePtr;
typedef volatile uint8_t& byteRef;
typedef uint16_t word;
typedef volatile uint16_t* wordPtr;
typedef volatile uint16_t& wordRef;

/**
 * Definition of signed 8 and 16 bits
 * basic type
 */
typedef int8_t sbyte;
typedef int16_t sword;

/**
 * Define both logic levels
 * low and high (false or true)
 */
enum logic : uint8_t
{
    Low = 0,
    High = 1,
    False = 0,
    True = 1,
};

/**
 * Definition of logic operator
 */
inline logic operator||(logic a, logic b)
{
    return (logic)(a | b);
}
inline logic operator&&(logic a, logic b)
{
    return (logic)(a & b);
}
inline logic operator!(logic a)
{
    return (logic)(!(uint8_t)a);
}

#endif

