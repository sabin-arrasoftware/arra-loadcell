

#include <HX711.h>

namespace  arra {
    class Scale {
        // config
        static const float calibrationMass_ = 6.1;
        static const int numReadings_ = 10;

        public:
            Scale(const int& dout, const int& sck) {
                hx711_.begin(dout, sck);
                hx711_.tare();
            }

            void Calibrate() {
                float scale_weight = 0.0;
                for (int i = 0; i < numReadings_; ++i) {
                    scale_weight += hx711_.get_value();
                }
                scale_weight /= numReadings_;
                float calibrationFactor = scale_weight / calibrationMass_;
                hx711_.set_scale(calibrationFactor);
            }

            float GetValue() const {
                return hx711_.get_units();
            }

        private:
            HX711 hx711_;
    };
}