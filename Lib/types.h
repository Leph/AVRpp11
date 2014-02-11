#ifndef TYPES_H
#define TYPES_H

/**
 * Definition of 8 and 16 bits 
 * basic type and its pointer for
 * flash register
 */
typedef uint8_t byte;
typedef volatile uint8_t* bytePtr;
typedef uint16_t word;
typedef volatile uint16_t* wordPtr;

/**
 * Definition of byte and word
 * bits numbers
 */
enum class ByteBitNum : uint8_t 
{
    BIT1 = 0,
    BIT2 = 1,
    BIT3 = 2,
    BIT4 = 3,
    BIT5 = 4,
    BIT6 = 5,
    BIT7 = 6,
    BIT8 = 7,
};
enum class WordBitNum : uint8_t
{
    BIT1 = 0,
    BIT2 = 1,
    BIT3 = 2,
    BIT4 = 3,
    BIT5 = 4,
    BIT6 = 5,
    BIT7 = 6,
    BIT8 = 7,
    BIT9 = 8,
    BIT10 = 9,
    BIT11 = 10,
    BIT12 = 11,
    BIT13 = 12,
    BIT14 = 13,
    BIT15 = 14,
    BIT16 = 15,
};

/**
 * Trait class for getting
 * Bits Nums from type
 */
template <class T>
struct Type {
};

/**
 * Trait specialization for byte
 */
template <>
struct Type <byte> {
    typedef byte type;
    typedef bytePtr ptr;
    typedef ByteBitNum bitNum;
};

/**
 * Trait specialization for word
 */
template <>
struct Type <word> {
    typedef word type;
    typedef wordPtr ptr;
    typedef WordBitNum bitNum;
};

#endif

