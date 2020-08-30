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

#include "Message.h"


bool remotic::Message::write(remotic::Protocol *aProtocol) {

    aProtocol->writeNumber(0xA5);
    aProtocol->writeNumber(this->messageId, 2);
    aProtocol->writeNumber(this->messageType);
    aProtocol->writeNumber(payload.getBufferLength(), 2);
    aProtocol->add(payload.getBuffer(), payload.getBufferLength());
    aProtocol->writeNumber(getPayloadHash(), 2);
    return aProtocol->writeNumber(0xA5);
}
int remotic::Message::read(remotic::Protocol *aProtocol) {
    if(aProtocol->available(REMOTIC_MIM_PACKET_SIZE)) {
        if(aProtocol->readNumber() != 0xA5) return REMOTIC_READ_INVALID;
        this->messageId = aProtocol->readNumber(2);
        this->messageType = aProtocol->readNumber();
        int size = aProtocol->readNumber(2);
        if(aProtocol->available(size + 2 + 1)) {  // + 2 (hash) + 1 (0xA5)
            payload.erase();
            aProtocol->read(payload, size);
            int hash = aProtocol->readNumber(2);
#ifdef REMOTIC_DEBUG
            Serial.print("Hash = ");
            Serial.println( (unsigned short) hash, HEX );
            Serial.print("Calculated = ");
            Serial.println( (unsigned short) this->getPayloadHash(), HEX );
#endif
            if(hash == this->getPayloadHash()) {
                if(aProtocol->readNumber() != 0xA5) return REMOTIC_READ_INVALID;
                return REMOTIC_READ_SUCCESS;
            } else {
                return REMOTIC_READ_INVALID;
            }
        } else {
            return REMOTIC_READ_WAIT;
        }
    }
#ifdef REMOTIC_DEBUG
    Serial.print("Need data ");
    Serial.print(" Current ");
    Serial.print(aProtocol->getBufferLength());
    Serial.print(" Position ");
    Serial.print(aProtocol->getReadPosition());
    Serial.println("");
#endif
    return REMOTIC_READ_WAIT;
}

unsigned int remotic::Message::getPayloadHash() {
    crc.init();
    return crc.processBuffer(( char *) payload.getBuffer(), payload.getBufferLength());
}





