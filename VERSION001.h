//
// Created by Manuel Muñoz on 2/9/16.
//

#ifndef REMOTICPROTO_VERSION001_H
#define REMOTICPROTO_VERSION001_H

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned long uint64;
namespace remotic
{


class VERSION001 {

public:
    static void writeNumber(char * buffer, int offset, int number, int size = 1);
    static void writeString(char * buffer, int offset, char * string, int size);

    static int readNumber(char * buffer, int offset, int size = 1);
    static char * readString(char * buffer, int offset, int leng);

};

}

#endif //REMOTICPROTO_VERSION001_H
