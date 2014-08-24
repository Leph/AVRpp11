#include "AVRpp11/mapping/arduinoUno.h"
//#include "AVRpp11/mapping/atmega328p.h"
#include "AVRpp11/avrpp11.h"

int main()
{
    volatile logic l1 = High || False;
    volatile logic l2 = High && False;
    volatile logic l3 = !True;
    
    volatile byte v1 = bits::value<byte>(bits::Bit2);
    volatile byte v2 = bits::value<byte>(bits::Bit1, bits::Bit2);
    volatile byte v3 = bits::value<byte>(bits::Bit1, ~bits::Bit2);
    volatile byte iv1 = bits::valueInv<byte>(bits::Bit2);
    volatile byte iv2 = bits::valueInv<byte>(~bits::Bit1, ~bits::Bit2);
    volatile byte iv3 = bits::valueInv<byte>(bits::Bit1, ~bits::Bit2);
    
    bits::assign(DDRB, bits::Bit2);
    bits::assign(DDRB, bits::Bit2, bits::Bit3);
    bits::add(DDRB, bits::Bit2);
    bits::add(DDRB, ~bits::Bit2);
    bits::add(DDRB, ~bits::Bit2, bits::Bit1, bits::Bit3, ~bits::Bit4);
    volatile logic l4 = bits::get(DDRB, bits::Bit1);
    bits::set(DDRB, bits::Bit2, High);
    bits::toggle(DDRB, bits::Bit2);

    isr::enable();
    isr::disable();
    volatile logic l5 = isr::getState();
    isr::setState(l5);

    gpio::D3.setMode(gpio::Output);
    gpio::D3.write(High);
    gpio::D2.setMode(gpio::Input);
    volatile logic l6 = gpio::D2.read();
    volatile logic l7 = gpio::D3.readOutput();
    gpio::D3.toggle();

    isr::disable();
    usart::Usart0.setMode(usart::Write);
    usart::Usart0.setBitStop(usart::BitStop1);
    usart::Usart0.setParity(usart::ParityDisable);
    usart::Usart0.setBaudrate(usart::BaudRate9600);
    while (!usart::Usart0.isWriteReady());
    usart::Usart0.write('a');
    while (!usart::Usart0.isDataSent());
    usart::Usart0.onWriteReady([](HandlerArg(usart::Usart0) u){
        u.write('b');
    });
    isr::enable();
    
    isr::disable();
    spi::Spi.setMode(spi::Master);
    spi::Spi.setBitOrder(spi::MSBFirst);
    spi::Spi.setClockIdle(spi::ClockHigh);
    spi::Spi.setClockEdge(spi::ClockLeading);
    spi::Spi.setClockDivider(spi::ClockDiv2);
    isr::enable();
    
    isr::disable();
    gpio::D13.setMode(gpio::Output);
    timer::Timer0.setCounterMode(timer::WaveNormalTopNormal);
    timer::Timer0.setPinModeA(timer::PinDisable);
    timer::Timer0.setPinModeB(timer::PinDisable);
    timer::Timer0.setClock(timer::ClockDiv1024);
    timer::Timer0.onOverflow([](HandlerArg(timer::Timer0) t) {
        gpio::D13.toggle();
    });
    isr::enable();
    
    isr::disable();
    gpio::D13.setMode(gpio::Output);
    timer::Timer1.setCounterMode(timer::WaveNormalTopCompareA);
    timer::Timer1.setPinModeA(timer::PinDisable);
    timer::Timer1.setPinModeB(timer::PinDisable);
    timer::Timer1.setClock(timer::ClockDiv1024);
    timer::Timer1.writeCompareA(15625);
    timer::Timer1.onMatchA([](HandlerArg(timer::Timer1) t) {
        gpio::D13.toggle();
    });
    isr::enable();
    
    while (1);

    return 0;
}

