/*
    RemoticProto binary protocol for Arduino
    Copyright (C) 2020.  Manuel Muñoz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


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
