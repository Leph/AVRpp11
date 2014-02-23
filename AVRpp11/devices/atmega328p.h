#ifndef ATMEGA328P_H
#define ATMEGA328P_H

namespace gpio {

enum GpioPin : uint8_t
{
    Pin2 = 2,
    Pin3 = 3,
    Pin4 = 4,
    Pin5 = 5,
    Pin6 = 6,
    Pin9 = 9,
    Pin10 = 10,
    Pin11 = 11,
    Pin12 = 12,
    Pin13 = 13,
    Pin14 = 14,
    Pin15 = 15,
    Pin16 = 16,
    Pin17 = 17,
    Pin18 = 18,
    Pin19 = 19,
    Pin23 = 23,
    Pin24 = 24,
    Pin25 = 25,
    Pin26 = 26,
    Pin27 = 27,
    Pin28 = 28,
};

template <>
struct GpioPinReg<Pin2> {
    static constexpr bytePtr DirReg = &DDRD;
    static constexpr bytePtr OutReg = &PORTD;
    static constexpr bytePtr InReg = &PIND;
    static constexpr bits::BitNum DirBit = bits::Bit1;
    static constexpr bits::BitNum OutBit = bits::Bit1;
    static constexpr bits::BitNum InBit = bits::Bit1;
};
template <>
struct GpioPinReg<Pin3> {
    static constexpr bytePtr DirReg = &DDRD;
    static constexpr bytePtr OutReg = &PORTD;
    static constexpr bytePtr InReg = &PIND;
    static constexpr bits::BitNum DirBit = bits::Bit2;
    static constexpr bits::BitNum OutBit = bits::Bit2;
    static constexpr bits::BitNum InBit = bits::Bit2;
};
template <>
struct GpioPinReg<Pin4> {
    static constexpr bytePtr DirReg = &DDRD;
    static constexpr bytePtr OutReg = &PORTD;
    static constexpr bytePtr InReg = &PIND;
    static constexpr bits::BitNum DirBit = bits::Bit3;
    static constexpr bits::BitNum OutBit = bits::Bit3;
    static constexpr bits::BitNum InBit = bits::Bit3;
};
template <>
struct GpioPinReg<Pin5> {
    static constexpr bytePtr DirReg = &DDRD;
    static constexpr bytePtr OutReg = &PORTD;
    static constexpr bytePtr InReg = &PIND;
    static constexpr bits::BitNum DirBit = bits::Bit4;
    static constexpr bits::BitNum OutBit = bits::Bit4;
    static constexpr bits::BitNum InBit = bits::Bit4;
};
template <>
struct GpioPinReg<Pin6> {
    static constexpr bytePtr DirReg = &DDRD;
    static constexpr bytePtr OutReg = &PORTD;
    static constexpr bytePtr InReg = &PIND;
    static constexpr bits::BitNum DirBit = bits::Bit5;
    static constexpr bits::BitNum OutBit = bits::Bit5;
    static constexpr bits::BitNum InBit = bits::Bit5;
};
template <>
struct GpioPinReg<Pin9> {
    static constexpr bytePtr DirReg = &DDRB;
    static constexpr bytePtr OutReg = &PORTB;
    static constexpr bytePtr InReg = &PINB;
    static constexpr bits::BitNum DirBit = bits::Bit7;
    static constexpr bits::BitNum OutBit = bits::Bit7;
    static constexpr bits::BitNum InBit = bits::Bit7;
};
template <>
struct GpioPinReg<Pin10> {
    static constexpr bytePtr DirReg = &DDRB;
    static constexpr bytePtr OutReg = &PORTB;
    static constexpr bytePtr InReg = &PINB;
    static constexpr bits::BitNum DirBit = bits::Bit8;
    static constexpr bits::BitNum OutBit = bits::Bit8;
    static constexpr bits::BitNum InBit = bits::Bit8;
};
template <>
struct GpioPinReg<Pin11> {
    static constexpr bytePtr DirReg = &DDRD;
    static constexpr bytePtr OutReg = &PORTD;
    static constexpr bytePtr InReg = &PIND;
    static constexpr bits::BitNum DirBit = bits::Bit6;
    static constexpr bits::BitNum OutBit = bits::Bit6;
    static constexpr bits::BitNum InBit = bits::Bit6;
};
template <>
struct GpioPinReg<Pin12> {
    static constexpr bytePtr DirReg = &DDRD;
    static constexpr bytePtr OutReg = &PORTD;
    static constexpr bytePtr InReg = &PIND;
    static constexpr bits::BitNum DirBit = bits::Bit7;
    static constexpr bits::BitNum OutBit = bits::Bit7;
    static constexpr bits::BitNum InBit = bits::Bit7;
};
template <>
struct GpioPinReg<Pin13> {
    static constexpr bytePtr DirReg = &DDRD;
    static constexpr bytePtr OutReg = &PORTD;
    static constexpr bytePtr InReg = &PIND;
    static constexpr bits::BitNum DirBit = bits::Bit8;
    static constexpr bits::BitNum OutBit = bits::Bit8;
    static constexpr bits::BitNum InBit = bits::Bit8;
};
template <>
struct GpioPinReg<Pin14> {
    static constexpr bytePtr DirReg = &DDRB;
    static constexpr bytePtr OutReg = &PORTB;
    static constexpr bytePtr InReg = &PINB;
    static constexpr bits::BitNum DirBit = bits::Bit1;
    static constexpr bits::BitNum OutBit = bits::Bit1;
    static constexpr bits::BitNum InBit = bits::Bit1;
};
template <>
struct GpioPinReg<Pin15> {
    static constexpr bytePtr DirReg = &DDRB;
    static constexpr bytePtr OutReg = &PORTB;
    static constexpr bytePtr InReg = &PINB;
    static constexpr bits::BitNum DirBit = bits::Bit2;
    static constexpr bits::BitNum OutBit = bits::Bit2;
    static constexpr bits::BitNum InBit = bits::Bit2;
};
template <>
struct GpioPinReg<Pin16> {
    static constexpr bytePtr DirReg = &DDRB;
    static constexpr bytePtr OutReg = &PORTB;
    static constexpr bytePtr InReg = &PINB;
    static constexpr bits::BitNum DirBit = bits::Bit3;
    static constexpr bits::BitNum OutBit = bits::Bit3;
    static constexpr bits::BitNum InBit = bits::Bit3;
};
template <>
struct GpioPinReg<Pin17> {
    static constexpr bytePtr DirReg = &DDRB;
    static constexpr bytePtr OutReg = &PORTB;
    static constexpr bytePtr InReg = &PINB;
    static constexpr bits::BitNum DirBit = bits::Bit4;
    static constexpr bits::BitNum OutBit = bits::Bit4;
    static constexpr bits::BitNum InBit = bits::Bit4;
};
template <>
struct GpioPinReg<Pin18> {
    static constexpr bytePtr DirReg = &DDRB;
    static constexpr bytePtr OutReg = &PORTB;
    static constexpr bytePtr InReg = &PINB;
    static constexpr bits::BitNum DirBit = bits::Bit5;
    static constexpr bits::BitNum OutBit = bits::Bit5;
    static constexpr bits::BitNum InBit = bits::Bit5;
};
template <>
struct GpioPinReg<Pin19> {
    static constexpr bytePtr DirReg = &DDRB;
    static constexpr bytePtr OutReg = &PORTB;
    static constexpr bytePtr InReg = &PINB;
    static constexpr bits::BitNum DirBit = bits::Bit6;
    static constexpr bits::BitNum OutBit = bits::Bit6;
    static constexpr bits::BitNum InBit = bits::Bit6;
};
template <>
struct GpioPinReg<Pin23> {
    static constexpr bytePtr DirReg = &DDRC;
    static constexpr bytePtr OutReg = &PORTC;
    static constexpr bytePtr InReg = &PINC;
    static constexpr bits::BitNum DirBit = bits::Bit1;
    static constexpr bits::BitNum OutBit = bits::Bit1;
    static constexpr bits::BitNum InBit = bits::Bit1;
};
template <>
struct GpioPinReg<Pin24> {
    static constexpr bytePtr DirReg = &DDRC;
    static constexpr bytePtr OutReg = &PORTC;
    static constexpr bytePtr InReg = &PINC;
    static constexpr bits::BitNum DirBit = bits::Bit2;
    static constexpr bits::BitNum OutBit = bits::Bit2;
    static constexpr bits::BitNum InBit = bits::Bit2;
};
template <>
struct GpioPinReg<Pin25> {
    static constexpr bytePtr DirReg = &DDRC;
    static constexpr bytePtr OutReg = &PORTC;
    static constexpr bytePtr InReg = &PINC;
    static constexpr bits::BitNum DirBit = bits::Bit3;
    static constexpr bits::BitNum OutBit = bits::Bit3;
    static constexpr bits::BitNum InBit = bits::Bit3;
};
template <>
struct GpioPinReg<Pin26> {
    static constexpr bytePtr DirReg = &DDRC;
    static constexpr bytePtr OutReg = &PORTC;
    static constexpr bytePtr InReg = &PINC;
    static constexpr bits::BitNum DirBit = bits::Bit4;
    static constexpr bits::BitNum OutBit = bits::Bit4;
    static constexpr bits::BitNum InBit = bits::Bit4;
};
template <>
struct GpioPinReg<Pin27> {
    static constexpr bytePtr DirReg = &DDRC;
    static constexpr bytePtr OutReg = &PORTC;
    static constexpr bytePtr InReg = &PINC;
    static constexpr bits::BitNum DirBit = bits::Bit5;
    static constexpr bits::BitNum OutBit = bits::Bit5;
    static constexpr bits::BitNum InBit = bits::Bit5;
};
template <>
struct GpioPinReg<Pin28> {
    static constexpr bytePtr DirReg = &DDRC;
    static constexpr bytePtr OutReg = &PORTC;
    static constexpr bytePtr InReg = &PINC;
    static constexpr bits::BitNum DirBit = bits::Bit6;
    static constexpr bits::BitNum OutBit = bits::Bit6;
    static constexpr bits::BitNum InBit = bits::Bit6;
};

}

#endif

