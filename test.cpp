#ifndef ARDUINO

#include <stdlib.h>
#include <iostream>
#include "Protocol.h"
#include "Message.h"

using namespace std;
using namespace remotic;

int main() {

    int i = 1;
    while(i-- != 0)
    {
        remotic::Message message;
        message.setMessageType(1);
        message.setMessageId(2);
        char * data = (char *) "\1\2\3\4\5\6";
        message.getPayload().add((unsigned char *) data, 6);
        message.getPayload().writeNumber(0xFFffFFff, 4);
        message.getPayload().writeFloat(100.0);
        message.getPayload().writeString("This is a test");

        Protocol aProtocol;
        message.write(&aProtocol);
        aProtocol.display();

        Message message1;
        message1.read(&aProtocol);

        printf("%d %d %d\n", message1.getMessageId(), message1.getMessageType(), message1.getPayloadHash());
        message1.getPayload().display();

        message.getPayload().skip(6);
        printf("%x\n", message.getPayload().readNumber(4));
        printf("%f\n", message.getPayload().readFloat());
        printf("%s\n", message.getPayload().readString());
//        for(int j = 0; j < 10; j++) {
//            aProtocol.reset();
//        }
//        while( aProtocol.available(6)) {
//            printf("%d %d %d\n", aProtocol.readNumber(), aProtocol.readNumber(4), aProtocol.readNumber());
//            aProtocol.clear();
//            aProtocol.display();
//
//        }


//        remotic::Message message;
//        message.setMessageType(1);
//        for(int j = 0; j < 1; j++)
//        {
//            message.getPayload().writeNumber(0x00000001, 4);
//            message.getPayload().writeNumber(0x0000FF00, 4);
//            message.getPayload().writeNumber(0x00FF0000, 4);
//            message.getPayload().writeNumber(0xFF000000, 4);
//            message.getPayload().writeNumber(0xFF0000FF, 4);
//            message.getPayload().writeNumber(0xFF00FFFF, 4);
//            message.getPayload().writeNumber(0xFFFFFFFF, 4);
//        }
//        message.write(&aProtocol);
//        aProtocol.readNumber(6);
//
//        for(int j = 0; j < 1; j++)
//        {
//            printf("[%d] %#010x\n",j, aProtocol.readNumber(4));
//            printf("[%d] %#010x\n",j, aProtocol.readNumber(4));
//            printf("[%d] %#010x\n",j, aProtocol.readNumber(4));
//            printf("[%d] %#010x\n",j, aProtocol.readNumber(4));
//            printf("[%d] %#010x\n",j, aProtocol.readNumber(4));
//            printf("[%d] %#010x\n",j, aProtocol.readNumber(4));
//            printf("[%d] %#010x\n",j, aProtocol.readNumber(4));
//        }
        //aProtocol.display();

        aProtocol.clear();
    }

    return 0;
}

#endif