/*
    RemoticProto binary protocol for Arduino
    Copyright (C) $today.year.  Manuel Muñoz

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
