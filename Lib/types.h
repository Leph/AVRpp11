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
 * and associated types
 */
template <>
struct Type <byte> {
    typedef byte type;
    typedef bytePtr ptr;
    typedef byteRef ref;
    typedef ByteBitNum bitNum;
};
template <>
struct Type <bytePtr> {
    typedef byte type;
    typedef bytePtr ptr;
    typedef byteRef ref;
    typedef ByteBitNum bitNum;
};
template <>
struct Type <byteRef> {
    typedef byte type;
    typedef bytePtr ptr;
    typedef byteRef ref;
    typedef ByteBitNum bitNum;
};

/**
 * Trait specialization for word
 * and associated types
 */
template <>
struct Type <word> {
    typedef word type;
    typedef wordPtr ptr;
    typedef wordRef ref;
    typedef WordBitNum bitNum;
};
template <>
struct Type <wordPtr> {
    typedef word type;
    typedef wordPtr ptr;
    typedef wordRef ref;
    typedef WordBitNum bitNum;
};
template <>
struct Type <wordRef> {
    typedef word type;
    typedef wordPtr ptr;
    typedef wordRef ref;
    typedef WordBitNum bitNum;
};

/**
 * Define both logic levels
 * low and high (false or true)
 */
enum logic : uint8_t
{
    LOW = 0,
    HIGH = 1,
    FALSE = 0,
    TRUE = 1,
};

/**
 * Define logic operator
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

