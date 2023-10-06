// serial.tpp

namespace arra {

template <class TCmdHandler>
Serial<TCmdHandler>::Serial(HardwareSerial& serial, TCmdHandler& h)
: serial_(serial)
, ch_(h) 
{
}

// Public

template <class TCmdHandler>
void Serial<TCmdHandler>::Start()
{
    serial_.begin(baud);
}

template <class TCmdHandler>
void Serial<TCmdHandler>::Write(const String& msg)
{
    serial_.println(msg);
}

template <class TCmdHandler>
void Serial<TCmdHandler>::ProcessSerialData()
{
    if (serial_.available()) 
    {
        ch_.Execute(getBuffer());
    }
}

// Private

template <class TCmdHandler>
Buffer Serial<TCmdHandler>::getBuffer() 
{
    Buffer buffer;
    buffer.size_ = serial_.readBytesUntil('\n', buffer.payload, sizeof(buffer.payload) - 1);
    return buffer;
}

} // namespace arra
