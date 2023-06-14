
//#include <ArduinoSTL.h>
#include "HashMap.h"

namespace arra {
    typedef void (*fn_event)();

    class Serial {

    static const int baud = 9600;
    public:
        Serial(HardwareSerial& serial) : serial_(serial), count_(0){}

        // Add callback for given event.
        void add_callback(const String& cmd, fn_event cb){
          shm_.insertNode(cmd, cb);
          // cmds_[count_] = cmd;
          // callbacks_[count_] = cb;
          // ++count_;
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
                //fn_event cb = static_cast<fn_event>(shm_.getValue(cmd));

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

        // Registered callbacks for triggered event.
        // Key = cmd, value = callback.
        String cmds_[10];
        fn_event callbacks_[10];

        int count_;

        SHashMap<String, fn_event> shm_;
    };
}