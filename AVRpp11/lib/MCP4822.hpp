#ifndef MCP4822_HPP
#define MCP4822_HPP

/**
 * MCP4822 Digital to Analogic Converter
 * using SPI serial interface
 */
class MCP4822
{
    public:

        /**
         * DAC output channel
         */
        enum Channel : uint8_t {
            ChannelA,
            ChannelB,
        };

        /**
         * Initialize Spi for DAC transmission
         * The latch Gpio Pin is given 
         * (Pin must not be already used by Spi MOSI/MISO/SS/SCK)
         */
        static inline void init(const gpio::GpioObject& latchPin)
        {
            //Configure Spi
            isr::disable();
            _latchPin = &latchPin;
            spi::Spi.setMode(spi::Master);
            spi::Spi.setBitOrder(spi::MSBFirst);
            spi::Spi.setClockIdle(spi::ClockHigh);
            spi::Spi.setClockEdge(spi::ClockTrailing);
            spi::Spi.setClockDivider(spi::ClockDiv2);
            spi::Spi.onTransfertComplet();
            isr::enable();

            //Configure Slave Select and latch
            gpio::SS.write(High);
            _latchPin->setMode(gpio::Output);
            _latchPin->write(High);
        }

        /**
         * Start the write of given 12 bits value in DAC given 
         * output register (A or B)
         * The data transmission is stopped when completed
         * and DAC is latched
         * (Do not request a new write until last 
         * transmission isn't completed)
         */
        static inline void writeChannel(Channel channel, word value)
        {
            //Latch previous sent data
            _latchPin->write(Low);
            __asm__ __volatile__ ("nop\n\t");
            __asm__ __volatile__ ("nop\n\t");
            _latchPin->write(High);

            spi::Spi.onTransfertComplet();
            _frame1H = 0;
            _frame1L = 0;
            _state = 0;

            //Output channel
            if (channel == ChannelA) {
                bits::add(_frame1H, ~bits::Bit7);
            } else if (channel == ChannelB) {
                bits::add(_frame1H, bits::Bit7);
            }
            //Gain (x2)
            bits::add(_frame1H, ~bits::Bit5);
            //SHDN (true to enabled the channel)
            bits::add(_frame1H, bits::Bit4);
            //Value
            _frame1H |= (value >> 8) & 0b00001111;
            _frame1L = value & 0b11111111;

            //Start transmision
            spi::Spi.onTransfertComplet(MCP4822::isrHanderOneChannel);
            gpio::SS.write(Low);
            spi::Spi.write(_frame1H);
        }
        
        /**
         * Start the write of given 12 bits value in DAC 
         * both A and B output channel
         * The data transmission is stopped when completed
         * and DAC is latched
         * (Do not request a new write until last 
         * transmission isn't completed)
         */
        static inline void writeBoth(word channelA, word channelB)
        {
            //Latch previous sent data
            _latchPin->write(Low);
            __asm__ __volatile__ ("nop\n\t");
            __asm__ __volatile__ ("nop\n\t");
            _latchPin->write(High);

            spi::Spi.onTransfertComplet();
            _frame1H = 0;
            _frame1L = 0;
            _frame2H = 0;
            _frame2L = 0;
            _state = 0;

            //Output channel
            bits::add(_frame1H, ~bits::Bit7);
            bits::add(_frame2H, bits::Bit7);
            //Gain (x2)
            bits::add(_frame1H, ~bits::Bit5);
            bits::add(_frame2H, ~bits::Bit5);
            //SHDN (true to enabled the channel)
            bits::add(_frame1H, bits::Bit4);
            bits::add(_frame2H, bits::Bit4);
            //Value
            _frame1H |= (channelA >> 8) & 0b00001111;
            _frame1L = channelA & 0b11111111;
            _frame2H |= (channelB >> 8) & 0b00001111;
            _frame2L = channelB & 0b11111111;

            //Start transmision
            spi::Spi.onTransfertComplet(MCP4822::isrHanderBothChannel);
            gpio::SS.write(Low);
            spi::Spi.write(_frame1H);
        }

        /**
         * Spi on byte transmission completed interrupt handler
         * for one channel tranmission
         */
        static void isrHanderOneChannel(HandlerArg(spi::Spi) s)
        {
            if (_state == 0) {
                _state = 1;
                spi::Spi.write(_frame1L);
            } else if (_state == 1) {
                gpio::SS.write(High);
                spi::Spi.onTransfertComplet();
            }
        }

        /**
         * Spi on byte transmission completed interrupt handler
         * for both channel tranmission
         */
        static void isrHanderBothChannel(HandlerArg(spi::Spi) s)
        {
            if (_state == 0) {
                _state = 1;
                spi::Spi.write(_frame1L);
            } else if (_state == 1) {
                _state = 2;
                gpio::SS.write(High);
                gpio::SS.write(Low);
                spi::Spi.write(_frame2H);
            } else if (_state == 2) {
                _state = 3;
                spi::Spi.write(_frame2L);
            } else if (_state == 3) {
                gpio::SS.write(High);
                spi::Spi.onTransfertComplet();
            }
        }

    private:

        /**
         * Transmission state and data
         */
        static volatile byte _frame1H;
        static volatile byte _frame1L;
        static volatile byte _frame2H;
        static volatile byte _frame2L;
        static volatile byte _state;
        static const gpio::GpioObject* _latchPin;
};

/**
 * Non const member definition
 */
volatile byte MCP4822::_frame1H;
volatile byte MCP4822::_frame1L;
volatile byte MCP4822::_frame2H;
volatile byte MCP4822::_frame2L;
volatile byte MCP4822::_state;
const gpio::GpioObject* MCP4822::_latchPin = nullptr;

#endif

