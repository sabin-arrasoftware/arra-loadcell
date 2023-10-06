#pragma once
#include "proto.h"

namespace arra {

typedef void (*fn_event)();

template <class TCmdHandler>
class Serial {

static const int baud = 9600;
static const int BUFFER_SIZE = 16;

public:
    Serial(HardwareSerial& serial, TCmdHandler& h) : serial_(serial), ch_(h) {} 
   
    void start() {
        serial_.begin(baud);
    }

    void write(const String& msg) {
        serial_.println(msg);
    }
    
    Buffer get_buffer() {
        Buffer buffer;
        buffer.size_ = serial_.readBytesUntil('\n', buffer.payload, sizeof(buffer.payload) - 1); // Read command bytes
        return buffer;
    }
    
    void processSerialData(){
        if (serial_.available()){
            ch_.execute(get_buffer());
        }
    }     

private:
    HardwareSerial& serial_; 
    TCmdHandler& ch_;    
};

}
