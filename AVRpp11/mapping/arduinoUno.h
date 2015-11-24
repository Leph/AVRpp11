#ifndef ARDUINOUNO_H
#define ARDUINOUNO_H

#define GPIO_DEFINES \
    X(D0, D, 0) \
    X(D1, D, 1) \
    X(D2, D, 2) \
    X(D3, D, 3) \
    X(D4, D, 4) \
    X(D5, D, 5) \
    X(D6, D, 6) \
    X(D7, D, 7) \
    X(D8, B, 0) \
    X(D9, B, 1) \
    X(D10, B, 2) \
    X(D11, B, 3) \
    X(D12, B, 4) \
    X(D13, B, 5) \
    X(A0, C, 0) \
    X(A1, C, 1) \
    X(A2, C, 2) \
    X(A3, C, 3) \
    X(A4, C, 4) \
    X(A5, C, 5)

#define PIN_ALIAS \
    X(D0, RX) \
    X(D1, TX) \
    X(D13, Led) \
    X(D13, SCK) \
    X(D12, MISO) \
    X(D11, MOSI) \
    X(D10, SS)

#endif

