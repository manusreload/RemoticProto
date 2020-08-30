# RemoticProto

Binary Protocol for Arduino.


## Message

The `Message` class represents the main communication structure. 

`Protocol &getPayload()`

Returns the Protocol object for the payload to be sent.

`void setMessageType(uint32 messageType)`

Set the message type for the payload specification.

`void setMessageId(uint32 messageId)`

Set the message identification. It's can be used for acknowledge if the message was received.

`unsigned int getPayloadHash();`

Computes the CRC16 sum for the payload.

`bool write(Protocol * aProtocol);`

Writes the raw message to a `Protocol` object. This can be user later for sending the message. Returns true if the message fits in the Buffer and false otherwise.

`int read(Protocol * aProtocol);`

Fill the current `Message` object whit the data in the Protocol object.
Returns `REMOTIC_READ_WAIT` if the buffer isn't enough long, and need more data.
Returns `REMOTIC_READ_SUCCESS` if the message is read successfully.
Returns `REMOTIC_READ_INVALID` if the message has errors. You need to discard all information in the buffer.

### Binary Codification

The message structure follows the next implementation:

| Byte   | Value                  | Description               |
|--------|------------------------|---------------------------|
| 0x01   | 0xA5                   | Start Byte always is 0xA5 |
| 0x02   | Message ID LSB         | Message ID (LSB)          |
| 0x03   | Message ID MSB         | Message ID (MSB)          |
| 0x04   | Message Type (LSB)     | Message Type              |
| 0x05   | Message Type (MSB)     | Message Type              |
| 0x06   | Payload Length (LSB)   | Payload Length (LSB)      |
| 0x07   | Payload Length (MSB)   | Payload Length            |
| 0x08   | Payload byte 1         | First payload byte        |
| 0x09   | Payload byte 2         | ...                       |
| 0x0n   | Payload byte n         | Last payload byte         |
| 0x0n+1 | Payload hash CRC (LSB) |                           |
| 0x0n+2 | Payload hash CRC (MSB) |                           |
| 0x0n+3 | 0xA5                   | Last byte always is 0xA5  |

## Protocol

`bool writeNumber(uint64 number, int size = 1)`

Writes an integer number to the buffer. Returns true if it fits in the buffer and false otherwise. 

`bool writeString(char * string);`

Writes a character string to the buffer. The string must be `\0` terminated. Returns true if it fits in the buffer and false otherwise.

`bool writeFloat(float number);`

Writes a number in IEEE representation to the buffer. Returns true if it fits in the buffer and false otherwise.

### Dynamic buffer re-allocation

This library comes with a feature for small size chips that not have enought RAM. You can specify a small size for the buffer that grows dynamically when is necessary.
You can enable the feature setting the header:

`#define REMOTIC_DYNAMIC_ALLOC 1`

Enable the dynamic buffer re-allocation

`#define REMOTIC_DYNAMIC_ALLOC_MIN_SIZE  16`

Set the initial buffer size.

`#define REMOTIC_DYNAMIC_ALLOC_MAX_SIZE  256`

Set the maximum buffer size.

# Example

## Using read/write methods
```cpp
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

Serial.println(message1.getPayload().read(6));
Serial.println(message1.getPayload().readNumber(4));
Serial.println(message1.getPayload().readFloat());
Serial.println(message1.getPayload().readString());
```

## Read from Stream
```cpp

#define client Serial
unsigned long timeout = 0;

remotic::Protocol rxproto;
remotic::Message message;


void loop() {

  while (client.available() && millis() - timeout < 10000L) {
    char c = client.read();
    if(!rxproto.add(c)) {
      rxproto.erase();
    }
    timeout = millis();
  }
  while(rxproto.available(REMOTIC_MIM_PACKET_SIZE))
  {
    int readResult = message.read(&rxproto);
    if(readResult == REMOTIC_READ_SUCCESS) {
      processMessage(message);
      rxproto.clear();
    } else if (readResult == REMOTIC_READ_WAIT) {
      break;
    } else {
      rxproto.erase();
      break;
    }
  }
}

void processMessage(remotic::Message &message) {
  ...
}
```

# Attributions


CRC16: https://github.com/jpralves/crc16
