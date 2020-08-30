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

#ifndef REMOTICPROTO_MESSAGE_H
#define REMOTICPROTO_MESSAGE_H

#include "VERSION001.h"
#include "Protocol.h"
#include "CRC16.h"
#ifdef ARDUINO
#include <Arduino.h> //needed for Serial.println
#endif

#define REMOTIC_READ_SUCCESS 0
#define REMOTIC_READ_WAIT 1
#define REMOTIC_READ_INVALID 2

#define REMOTIC_MIM_PACKET_SIZE 9

#define REMOTIC_MESSAGE_EMPTY 0x00
#define REMOTIC_MESSAGE_HELLO 0x01
#define REMOTIC_MESSAGE_STATUS 0x02
#define REMOTIC_MESSAGE_SET 0x03
#define REMOTIC_MESSAGE_READY 0x04
#define REMOTIC_MESSAGE_ERROR 0x05
#define REMOTIC_MESSAGE_SERVER_READY 0x06
#define REMOTIC_MESSAGE_ACK 0x07
#define REMOTIC_MESSAGE_INPUT_MAP 0x08
#define REMOTIC_MESSAGE_INPUT_MAP_LOW 0x09
namespace remotic
{

    /**
     * Use this class to generate new messages to server.
     */
class Message {
public:
    Message() : payload() {};
    bool write(Protocol * aProtocol);
    int read(Protocol * aProtocol);

    unsigned int getPayloadHash();


    Protocol &getPayload() {
        return payload;
    }

    Protocol * getPayloadRef() {
        return &payload;
    }


    uint32 getMessageId() const {
        return messageId;
    }

    void setMessageId(uint32 messageId) {
        Message::messageId = messageId;
    }

    uint32 getMessageType() const {
        return messageType;
    }

    void setMessageType(uint32 messageType) {
        Message::messageType = messageType;
    }

    void clear() {
        payload.erase();
        messageType = 0;
    }

private:
//    uint32 version = 1;
    uint32 messageId = 0;
    uint32 hash = 0;
    uint32 messageType = 0;
    Protocol payload;
    CRC16 crc;
};
}


#endif //REMOTICPROTO_MESSAGE_H
