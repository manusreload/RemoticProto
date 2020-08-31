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

#include "Protocol.h"
//#define SerialDebug Serial

remotic::Protocol::Protocol(){
#if REMOTIC_DYNAMIC_ALLOC
    resizeBuffer(REMOTIC_DYNAMIC_ALLOC_MIN_SIZE*2);
#else
    buffer = new unsigned char[MAX_BUFFER];
#endif
    erase();
}
remotic::Protocol::~Protocol(){
    delete[] buffer;
}

bool remotic::Protocol::add(unsigned char *buffer, int size) {
    if(free(size))
    {
        for (int i = 0; i < size; i++) {
            this->buffer[length++] = buffer[i];
        }
        return true;
    }
    return false;
}

bool remotic::Protocol::add(unsigned char number) {
    if(free(1))
    {
        this->buffer[length++] = number;
        return true;
    }
    return false;
}



bool remotic::Protocol::available(int size) {
    return position + size <= length;
}

void remotic::Protocol::skip(int count) {
    position += count;
}

uint64 remotic::Protocol::readNumber(int size) {
    if(!this->available(size)) return 0;
    uint64 number = 0;
    for (unsigned int i = 0; i < size; i++) {
#ifdef REMOTIC_LITTLE_ENDIAN
        number += (uint64) ((buffer[position++] & 0xFF)) << (i * 8);
#else
        number += (uint64) ((buffer[position++] & 0xFF)) << ((size - i - 1) * 8);
#endif
    }
    return number;
}

char * remotic::Protocol::readString(int &leng) {
    if(available(2))
    {
        leng = readNumber(2);
        if(available(leng))
        {
            char * string = new char[leng + 1];
            for (int i = 0; i < leng; i++) {
                string[i] = (char) buffer[position++];
            }
            string[leng] = '\0';
            return string;
        }
    }
    return 0;
}

void remotic::Protocol::reset() {
    this->position = 0;
}

void remotic::Protocol::clear() {

    unsigned char * tmp = new unsigned char[getBufferSize()];
    int leng = length - position;
    for (int i = 0; i < leng; i++) {
        tmp[i] = buffer[position++];
    }
    delete[] buffer;
    buffer = tmp;
    length = leng;
    position = 0;
}

bool remotic::Protocol::writeNumber(uint64 number, int size) {
    if(free(size))
    {
        for (int i = 0; i < size; i++) {

#ifdef REMOTIC_LITTLE_ENDIAN
            buffer[length++] = (uint8) ((number >> (i * 8)) & 0xFF);
#else
            buffer[length++] = (uint8) ((number >> ((size - i - 1) * 8)) & 0xFF);
#endif
        }
        return true;
    }
    return false;
}
bool remotic::Protocol::writeFloat(float f) {

    if(!free(4)) return false;
    float normalized;
    int16_t shift;
    int32_t sign, exponent, significand;

    if (f == 0.0)
        return 0; //handle this special case
    //check sign and begin normalization
    if (f < 0) {
        sign = 1;
        normalized = -f;
    } else {
        sign = 0;
        normalized = f;
    }
    //get normalized form of f and track the exponent
    shift = 0;
    while (normalized >= 2.0) {
        normalized /= 2.0;
        shift++;
    }
    while (normalized < 1.0) {
        normalized *= 2.0;
        shift--;
    }
    normalized = normalized - 1.0;
    //calculate binary form (non-float) of significand
    significand = normalized*(0x800000 + 0.5f);
    //get biased exponent
    exponent = shift + 0x7f; //shift + bias
    //combine and return
    uint64 value = (sign<<31) | (exponent<<23) | significand;
    this->buffer[this->length++] = (value >> 8 * 0) & 0xFF;
    this->buffer[this->length++] = (value >> 8 * 1) & 0xFF;
    this->buffer[this->length++] = (value >> 8 * 2) & 0xFF;
    this->buffer[this->length++] = (value >> 8 * 3) & 0xFF;
    return true;
}

bool remotic::Protocol::writeString(char *string, int size) {
    if(free(2 + size))
    {
        writeNumber(size, 2);
        for (int i = 0; i < size; i++) {
            buffer[length++] = string[i];
        }
        return true;
    }
    return false;
}

bool remotic::Protocol::free(int size) {
#if REMOTIC_DYNAMIC_ALLOC
    if(length + size <= MAX_BUFFER) {
        return checkBuffer(length + size);
    }
    return false;
#else
    return length + size <= MAX_BUFFER;
#endif
}

void remotic::Protocol::erase() {

    length = 0;
    position = 0;

    for(int i = 0; i < getBufferSize(); i++) {
        buffer[i] = 0;
    }
}

bool remotic::Protocol::writeString(char *string) {
    int size = strlen(string);
//    while (string[++size] != '\0');
    return writeString(string, size);
}

char *remotic::Protocol::readString() {
    int size;
    return readString(size);
}

void remotic::Protocol::display() {
#ifndef ARDUINO
    printf("{bufferSize=%d} ", this->getBufferSize());
    for (int i = 0; i < length; i++) {
        printf("%02X ", (unsigned const char) buffer[i]);
    }
    printf("\n");
#endif
#ifdef SerialDebug

    SerialDebug.println("SerialDebug");

#endif
}

bool remotic::Protocol::read(unsigned char *buff, int leng) {
    if(this->available(leng)) {
        ::memcpy(buff, buffer+position, leng);
        position+=leng;
        return true;
    }
    return false;
}

bool remotic::Protocol::read(remotic::Protocol &dest, int &leng) {
    if(this->available(leng)) {
        dest.add(buffer+position, leng);
        position+=leng;
        return true;
    }
    return false;
}

float remotic::Protocol::readFloat() {
    if(this->available(4)) {

        int b32 = buffer[position++];
        b32 += buffer[position++] << 8;
        b32 += buffer[position++] << (8 * 2);
        b32 += buffer[position++] << (8 * 3);
        float result;
        int32_t shift;
        uint16_t bias;

        if (b32 == 0)
            return 0.0;
        //pull significand
        result = (b32&0x7fffff); //mask significand
        result /= (0x800000);    //convert back to float
        result += 1.0f;          //add one back
        //deal with the exponent
        bias = 0x7f;
        shift = ((b32>>23)&0xff) - bias;
        while (shift > 0) {
            result *= 2.0;
            shift--;
        }
        while (shift < 0) {
            result /= 2.0;
            shift++;
        }
        //sign
        result *= (b32>>31)&1 ? -1.0 : 1.0;
        return result;
    }
    return 0;
}


#if REMOTIC_DYNAMIC_ALLOC
bool remotic::Protocol::checkBuffer(int size) {
    if(bufferSize < size) {
        return resizeBuffer(size);
    }
    return true;
}
bool remotic::Protocol::resizeBuffer(int minLength) {
    int newSize = bufferSize;
    while(newSize < minLength) {
        newSize += REMOTIC_DYNAMIC_ALLOC_MIN_SIZE;
    }
    if(newSize > REMOTIC_DYNAMIC_ALLOC_MAX_SIZE) return false;
    bufferSize = newSize;
    unsigned char * tmp = new unsigned char[bufferSize];

    for (int i = 0; i < length; i++) {
        tmp[i] = buffer[i];
    }
    for (int i = length; i < bufferSize; i++) {
        tmp[i] = 0;
    }
    buffer = NULL;
    delete[] buffer;
    buffer = tmp;
    return true;
}
#endif
