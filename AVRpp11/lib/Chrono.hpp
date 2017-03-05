#ifndef CHRONO_HPP
#define CHRONO_HPP

/**
 * Use 8 bit timer 0 to count
 * time between start and stop
 * call
 */
class Chrono
{
    public:

        /**
         * Configure the timer with given
         * clock prescaler and start to measure time
         */
        static inline void start(timer::TimerClock clock)
        {
            isr::disable();
            //Stop the timer
            timer::Timer0.setClock(timer::ClockStop);
            //Mode config
            timer::Timer0.setCounterMode(timer::WaveNormalTopNormal);
            timer::Timer0.setPinModeA(timer::PinDisable);
            timer::Timer0.setPinModeB(timer::PinDisable);
            //Reset state
            timer::Timer0.clearMatchA();
            timer::Timer0.clearMatchB();
            timer::Timer0.clearOverflow();
            timer::Timer0.writeCounter(0);
            //Set overflow interrupt
            timer::Timer0.onOverflow(
                [](HandlerArg(timer::Timer0) t) 
            {
                _countOverflow++;
            });
            //Reset counters
            _countCycle = 0;
            _countOverflow = 0;
            isr::enable();
            //Start the timer
            timer::Timer0.setClock(clock);
        }

        /**
         * Stop to measure time
         */
        static inline void stop()
        {
            isr::disable();
            //Stop the timer
            timer::Timer0.setClock(timer::ClockStop);
            //Read cycles
            _countCycle = timer::Timer0.readCounter();
            //Reset state
            timer::Timer0.clearMatchA();
            timer::Timer0.clearMatchB();
            timer::Timer0.clearOverflow();
            timer::Timer0.writeCounter(0);
            //Disable the interrupt
            timer::Timer0.onOverflow();
            isr::enable();
        }

        /**
         * Return the cycle and overflow counter
         */
        static inline word getCounterCycle()
        {
            return _countCycle;
        }
        static inline word getCounterOverflow()
        {
            return _countOverflow;
        }

    private:

        /**
         * Cycle counter
         */
        static volatile word _countCycle;
        static volatile word _countOverflow;
};

/**
 * Non const member definition
 */
volatile word Chrono::_countCycle = 0;
volatile word Chrono::_countOverflow = 0;

#endif

