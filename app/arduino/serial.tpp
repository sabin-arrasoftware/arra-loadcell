// serial.tpp

namespace arra {

static const int HEADER_SIZE = 3;


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
void Serial<TCmdHandler>::ProcessSerialData()
{
    if (serial_.available()) 
    {
        Message request = read();
        Message response = request.operationType_ != ERROR ?
            ch_.Execute(request) : request;
        write(response);
    }
}

// Private

template <class TCmdHandler>
Message Serial<TCmdHandler>::read() 
{
    Message msg;

    // 1. Read the header
    byte header[HEADER_SIZE];
    size_t bytesRead = serial_.readBytes(header, HEADER_SIZE);
    if (bytesRead != HEADER_SIZE) 
    {
        return createErrorResponse(ERR_HEADER_TRUNCATED);
    }

    msg.operationType_ = header[0];
    msg.messageType_ = header[1];
    msg.payloadSize_ = header[2];

    // 2. Read the payload based on the size from the header
    if (msg.payloadSize_ > PAYLOAD_SIZE) 
    {
        return createErrorResponse(ERR_INVALID_PAYLOAD_SIZE);
    }

    bytesRead = serial_.readBytes(msg.payload_, msg.payloadSize_);
    if (bytesRead != msg.payloadSize_)
    {
        return createErrorResponse(ERR_PAYLOAD_TRUNCATED);
    }

    return msg;
}

template <class TCmdHandler>
void Serial<TCmdHandler>::write(const Message& msg)
{
    const size_t payload_size = msg.payloadSize_;
    serial_.write(reinterpret_cast<const byte*>(&msg), HEADER_SIZE + payload_size);
}

} // namespace arra
