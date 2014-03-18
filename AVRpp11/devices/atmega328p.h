#ifndef ATMEGA328P_H
#define ATMEGA328P_H

#define GPIO_DEFINES \
    X(Pin2, D, 0) \
    X(Pin3, D, 1) \
    X(Pin4, D, 2) \
    X(Pin5, D, 3) \
    X(Pin6, D, 4) \
    X(Pin9, B, 6) \
    X(Pin10, B, 7) \
    X(Pin11, D, 5) \
    X(Pin12, D, 6) \
    X(Pin13, D, 7) \
    X(Pin14, B, 0) \
    X(Pin15, B, 1) \
    X(Pin16, B, 2) \
    X(Pin17, B, 3) \
    X(Pin18, B, 4) \
    X(Pin19, B, 5) \
    X(Pin23, C, 0) \
    X(Pin24, C, 1) \
    X(Pin25, C, 2) \
    X(Pin26, C, 3) \
    X(Pin27, C, 4) \
    X(Pin28, C, 5)

#define USART_DEFINES \
    X(Usart0, UDR0, UCSR0A, UCSR0B, UCSR0C, UBRR0)

#endif

