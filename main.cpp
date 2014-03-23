#include "AVRpp11/devices/arduinoUno.h"
//#include "AVRpp11/devices/atmega328p.h"
#include "AVRpp11/avrpp11.h"

int main()
{
    //volatile logic a = High || False;
    //volatile logic b = High && False;
    //volatile logic c = !True;
    //volatile uint8_t d = bits::value<byte>(bits::Bit7);
    //volatile uint8_t e = bits::value<byte>(bits::Bit7, bits::Bit1);
    //bits::set(DDRB, bits::Bit2);
    //bits::set(DDRB, bits::Bit2, bits::Bit3);
    //bits::add(DDRB, bits::Bit2);
    //bits::add(DDRB, bits::Bit2, bits::Bit3);
    //bits::clear(DDRB, bits::Bit2);
    //bits::clear(DDRB, bits::Bit2, bits::Bit3);
    //volatile logic f = bits::get(DDRB, bits::Bit1);
    //bits::set(DDRB, bits::Bit2, High);

    //isr::enable();
    //isr::disable();
    //logic g = isr::getState();
    //isr::setState(g);

    //gpio::Pin2.mode(gpio::Output);
    //gpio::Pin3.write(High);
    //volatile logic h = gpio::Pin3.read();
    //volatile logic i = gpio::Pin3.readOutput();

    //isr::disable();
    //usart::Usart0.disable();
    //usart::Usart0.init(
    //    usart::ReadWrite, 
    //    usart::BaudRate9600, 
    //    usart::Stop1, 
    //    usart::ParityDisable);
    //while (usart::Usart0.isWriteReady());
    //usart::Usart0.write('a');
    //while (usart::Usart0.isDataSent());
    //usart::Usart0.onReadReady([](const usart::UsartEntry& u){
    //    byte b = u.read();
    //    u.print(" Pouet:");
    //    u.print(42);
    //    u.print((char)(b+1));
    //    u.print(10001);
    //});
    //isr::enable();

    return 0;
}

