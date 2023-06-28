
//#include <ArduinoSTL.h>
#include "HashMap.h"
#include "Stream.h"

namespace arra {
    typedef void (*fn_event)();

    // Define message structure
    struct Message {
      byte id;
      byte length;
      char content[32];  // Adjust the buffer size based on maximum message length
    };


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

        void handle_serial_data() { 
            if (serial_.available())
            {
                String cmd = get_command();
                write("cmd: " + cmd);
                fn_event cb = get_callback(cmd);
                execute_callback(cb);                
                print_result(cb);
                // Message message;
                // read_message(message);

                // write("Received message: ");
                // write_message(message);

                // String cmd = String(message.content);
                // write("cmd: " + cmd);
                // fn_event cb = get_callback(cmd);
                // execute_callback(cb);
                // print_result(cb);
            }
        } 

        void write(const String& msg) {
            serial_.println(msg);
        }

        String get_command() {
            String cmd = serial_.readStringUntil('\n');
            cmd.trim();
            return cmd;
        }

        fn_event get_callback(const String& cmd) {
            return shm_.getValue(cmd); 
        } 

        void execute_callback(fn_event cb) {
            if (cb) 
            {
                cb();
            }
        }

        void print_result(fn_event cb) {
            if (!cb) 
            {
                write("Key not found");
                return;
            }

            write("Calling callback");
        }

    private:
        HardwareSerial& serial_;

        SHashMap<String, fn_event> shm_;

    //   void read_message(Message& message) {
    //     byte id = serial_.read();
    //     byte length = serial_.read();
    //     serial_.readBytes(message.content, length);
    //     message.id = id;
    //     message.length = length;
    //   }

    //   void write_message(const Message& message) {
    //     serial_.print("ID: ");
    //     serial_.println(message.id);
    //     serial_.print("Length: ");
    //     serial_.println(message.length);
    //     serial_.print("Content: ");
    //     serial_.println(message.content);
    //   }
    };
}