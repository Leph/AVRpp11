#ifndef PRINTER_HPP
#define PRINTER_HPP

/**
 * Macros for number to string conversion
 */
#define TEST(num, pow) \
    if (val >= num ## pow) { \
        str[index] = static_cast<const char(&)[2]>(#num)[0]; \
        index++; \
        val -= num ## pow; \
        isStart = true; \
    }
#define TESTALL(pow) \
    TEST(9, pow) \
    TEST(8, pow) \
    TEST(7, pow) \
    TEST(6, pow) \
    TEST(5, pow) \
    TEST(4, pow) \
    TEST(3, pow) \
    TEST(2, pow) \
    TEST(1, pow)

/**
 * Initialize Usart0 and write to the serial connection 
 * characters, numbers and strings using a ring buffer
 */
class Printer
{
    public:

        /**
         * Define Printer circular buffer size
         */
        static constexpr byte bufferSize = 50;
        
        /**
         * Initialize the serial connection
         * with one bit stop, no parity check and
         * given baudrate
         */
        static inline void init(usart::UsartBaudRate baudrate)
        {
            isr::disable();
            usart::Usart0.setMode(usart::Write);
            usart::Usart0.setBitStop(usart::BitStop1);
            usart::Usart0.setParity(usart::ParityDisable);
            usart::Usart0.setBaudrate(baudrate);
            usart::Usart0.onWriteReady();
            isr::enable();
        }

        /**
         * Print given character or string
         */
        static inline void write(char c)
        {
            queueByte(c);
        }
        static inline void write(const char* str)
        {
            for (byte i=0;str[i]!='\0';i++) {
                queueByte(str[i]);
            }
        }

        /**
         * Print given 8 and 16 unsigned
         * bit number
         */
        static inline void write(byte val)
        {
            char str[4];
            byte index = 0;
            bool isStart = false;

            if (val >= 100) {
                TEST(2, 00)
                TEST(1, 00)
            }
            if (val >= 10) {
                TESTALL(0)
            } else if (isStart) {
                str[index] = '0';
                index++;
            }
            if (val >= 1) {
                TESTALL()
            } else {
                str[index] = '0';
                index++;
            }
            str[index] = '\0';

            write(str);
        }
        static inline void write(word val)
        {
            char str[6];
            byte index = 0;
            bool isStart = false;

            if (val >= 10000) {
                TEST(6, 0000)
                TEST(5, 0000)
                TEST(4, 0000)
                TEST(3, 0000)
                TEST(2, 0000)
                TEST(1, 0000)
            }
            if (val >= 1000) {
                TESTALL(000)
            } else if (isStart) {
                str[index] = '0';
                index++;
            }
            if (val >= 100) {
                TESTALL(00)
            } else if (isStart) {
                str[index] = '0';
                index++;
            }
            if (val >= 10) {
                TESTALL(0)
            } else if (isStart) {
                str[index] = '0';
                index++;
            }
            if (val >= 1) {
                TESTALL()
            } else {
                str[index] = '0';
                index++;
            }
            str[index] = '\0';

            write(str);
        }

        /**
         * Print given logical value
         */
        static inline void write(logic l)
        {
            if (l == True) {
                write("True");
            } else {
                write("False");
            }
        }

        /**
         * Print given signed number
         */
        static inline void write(sbyte val)
        {
            if (val < 0) {
                queueByte('-');
                write((byte)-val);
            } else {
                write((byte)val);
            }
        }
        static inline void write(sword val)
        {
            if (val < 0) {
                queueByte('-');
                write((word)-val);
            } else {
                write((word)val);
            }
        }

        /**
         * Compilator time alias
         */
        static inline void write(long val)
        {
            write((sbyte)val);
        }

        /**
         * Print a new line
         */
        static inline void endl()
        {
            write("\r\n");
        }

        /**
         * Wait that all writen characters 
         * are sent on the bus
         */
        static inline void waitFlush()
        {
            while(_begin != _end);
        }

    private:

        /**
         * Print ring buffer and begin/end indexes
         */
        static volatile byte _buffer[bufferSize];
        static volatile byte _begin;
        static volatile byte _end;

        /**
         * Add a byte into the buffer ring or wait if full
         */
        static inline void queueByte(byte c)
        {
            //Wait for space in circular buffer
            while (
                _end+1 == _begin || 
                (_end == bufferSize-1 && _begin == 0)
            );

            isr::disable();
            //Enable if necessary
            if (_begin == _end) {
                usart::Usart0.onWriteReady(Printer::isrHandler);
            }
            //Append buffer
            _buffer[_end] = c;
            if (_end == bufferSize-1) {
                _end = 0;
            } else {
                _end++;
            }
            isr::enable();
        }

        /**
         * Interrupt handler on write ready
         */
        static void isrHandler(HandlerArg(usart::Usart0) u)
        {
            if (_begin != _end) {
                //Write next character
                //if available
                u.write(_buffer[_begin]);
                //Pop it
                if (_begin == bufferSize-1) {
                    _begin = 0;
                } else {
                    _begin++;
                }
            }
            //Disable the interrupt when 
            //the buffer is empty
            if (_begin == _end) {
                u.onWriteReady();
            }
        }
};

/**
 * Non const member definition
 */
volatile byte Printer::_buffer[Printer::bufferSize];
volatile byte Printer::_begin = 0;
volatile byte Printer::_end = 0;

#undef TEST
#undef TESTALL

#endif

