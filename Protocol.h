//
// Created by Manuel Muñoz on 2/9/16.
//

#ifndef REMOTICPROTO_PROTOCOL_H
#define REMOTICPROTO_PROTOCOL_H

#include <stdio.h>
#include <string.h>

#include "VERSION001.h"

#ifndef ARDUINO
typedef unsigned short uint16_t;
typedef short int16_t;
typedef int int32_t;
#endif
#ifndef MAX_BUFFER
#define MAX_BUFFER 128
#endif
#define REMOTIC_DYNAMIC_ALLOC 1
#define REMOTIC_LITTLE_ENDIAN 1

#if REMOTIC_DYNAMIC_ALLOC
#define REMOTIC_DYNAMIC_ALLOC_MIN_SIZE  16
#define REMOTIC_DYNAMIC_ALLOC_MAX_SIZE  256
#endif
namespace remotic
{

    class Protocol {
    public:
        Protocol();
        ~Protocol();
        bool add(unsigned char * buffer, int size);
        bool add(unsigned char number);
        bool available(int size = 1);
        uint64 readNumber(int size = 1);
        float readFloat();
        bool read(unsigned char * buff, int leng);
        bool read(Protocol &dest, int & leng);
        char * readString(int & leng);
        char * readString();

        bool writeFloat(float number);
        bool writeNumber(uint64 number, int size = 1);
        bool writeString(char * string, int size);
        bool writeString(char * string);
        bool free(int size = 1);

        void reset();
        void clear();
        void erase();
        void skip(int count);

        unsigned char * getBuffer() { return buffer; };
        int getBufferLength() { return  length; };
        int getReadPosition() { return  position; };


        void display();

#if REMOTIC_DYNAMIC_ALLOC
        bool checkBuffer(int size);
        bool resizeBuffer(int minLength);
        int getBufferSize() { return  bufferSize; };
#else
        int getBufferSize() { return  MAX_BUFFER; };
#endif
    protected:
        unsigned char * buffer;
        int length = 0;
        int position = 0; // read position

#if REMOTIC_DYNAMIC_ALLOC
        int bufferSize = 0; // buffer internal size

#endif
    };
}
#endif //REMOTICPROTO_PROTOCOL_H
