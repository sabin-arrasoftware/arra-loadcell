
//#include <ArduinoSTL.h>
#include "HashMap.h"

namespace arra {
    typedef void (*fn_event)();

    class Serial {

    static const int baud = 9600;
    public:
        Serial(HardwareSerial& serial) : serial_(serial){}

        // Add callback for given event.
        void add_callback(const String& cmd, fn_event cb){
          shm_.insertNode(cmd, cb);
        }

        void start() {
            serial_.begin(baud);
        }

        void read() {
            if (serial_.available())
            {
                String cmd = serial_.readStringUntil('\n');
                cmd.trim();
                
                fn_event cb = shm_.getValue(cmd);

                if (cb) 
                {
                    cb();
                } 
                else 
                {
                    serial_.println("Key not found");
                }                
                
            }
        }

        void write(const String& msg) {
            serial_.println(msg);
        }

    private:
        HardwareSerial& serial_;

        int count_;

        SHashMap<String, fn_event> shm_;
    };
}