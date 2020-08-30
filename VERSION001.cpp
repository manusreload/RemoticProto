//
// Created by Manuel Muñoz on 2/9/16.
//

#include "VERSION001.h"

void remotic::VERSION001::writeNumber(char *buffer, int offset, int number, int size) {
    for (int i = 0; i < size; ++i) {
        buffer[i + offset] = (number << (i * 8)) & 0xFF;
    }
}

void remotic::VERSION001::writeString(char *buffer, int offset, char *string, int size) {
    writeNumber(buffer, offset, size, 2);
    for (int i = 0; i < size; ++i) {
        buffer[i + offset + 2] = string[i];
    }
}

int remotic::VERSION001::readNumber(char *buffer, int offset, int size) {
    int number = 0;
    for (int i = 0; i < size; ++i) {
        number += buffer[i + offset] >> (i * 8);
    }
    return number;
}

char* remotic::VERSION001::readString(char *buffer, int offset, int leng) {
    char * string = new char[leng];
    for (int i = 0; i < leng; ++i) {
        string[i] = buffer[i + offset];
    }
    return string;

}
