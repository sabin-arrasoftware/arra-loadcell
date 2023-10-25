#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "../arduino/proto.h"

namespace py = pybind11;

PYBIND11_MODULE(proto_module, m) {
    m.doc() = "Python module for proto.h";

    py::enum_<arra::OperationType>(m, "OperationType")
        .value("CALIBRATE", arra::OperationType::CALIBRATE)
        .value("WEIGHT", arra::OperationType::WEIGHT)
        .value("ADD_SCALE", arra::OperationType::ADD_SCALE)
        .value("ERROR", arra::OperationType::ERROR)
        .export_values();

    py::enum_<arra::MessageType>(m, "MessageType")
        .value("REQUEST", arra::MessageType::REQUEST)
        .value("RESPONSE", arra::MessageType::RESPONSE)
        .export_values();

    py::enum_<arra::ErrorCode>(m, "ErrorCode")
        .value("ERR_UNKNOWN_REQUEST", arra::ErrorCode::ERR_UNKNOWN_REQUEST)
        .value("ERR_HEADER_TRUNCATED", arra::ErrorCode::ERR_HEADER_TRUNCATED)
        .value("ERR_PAYLOAD_TRUNCATED", arra::ErrorCode::ERR_PAYLOAD_TRUNCATED)
        .value("ERR_INVALID_PAYLOAD_SIZE", arra::ErrorCode::ERR_INVALID_PAYLOAD_SIZE)
        .value("ERR_INVALID_SCALE_INDEX", arra::ErrorCode::ERR_INVALID_SCALE_INDEX)
        .export_values();

    py::class_<arra::Message>(m, "Message")
        .def(py::init<>())
        .def("ToString", &arra::Message::ToString)
        .def_readwrite("operationType_", &arra::Message::operationType_)
        .def_readwrite("messageType_", &arra::Message::messageType_)
        .def_readwrite("payloadSize_", &arra::Message::payloadSize_)
        .def_property("payload_", 
                      [](const arra::Message& m) -> py::array_t<uint8_t> {
                          return py::array_t<uint8_t>(arra::PAYLOAD_SIZE, m.payload_);
                      },
                      [](arra::Message& m, const py::array_t<uint8_t>& arr) {
                          std::memcpy(m.payload_, arr.data(), arra::PAYLOAD_SIZE);
                      });

    py::class_<arra::CalibrateRequest>(m, "CalibrateRequest")
        .def(py::init<>())
        .def("FromMessage", &arra::CalibrateRequest::FromMessage)
        .def("ToMessage", &arra::CalibrateRequest::ToMessage)
        .def_readwrite("scaleIndex_", &arra::CalibrateRequest::scaleIndex_)
        .def_readwrite("calibrationMass_", &arra::CalibrateRequest::calibrationMass_);

    py::class_<arra::CalibrateResponse>(m, "CalibrateResponse")
        .def(py::init<>())
        .def("FromMessage", &arra::CalibrateResponse::FromMessage)
        .def("ToMessage", &arra::CalibrateResponse::ToMessage)
        .def_readwrite("success_", &arra::CalibrateResponse::success_);

    py::class_<arra::WeightRequest>(m, "WeightRequest")
        .def(py::init<>())
        .def("FromMessage", &arra::WeightRequest::FromMessage)
        .def("ToMessage", &arra::WeightRequest::ToMessage);

    py::class_<arra::WeightResponse>(m, "WeightResponse")
        .def(py::init<>())
        .def("FromMessage", &arra::WeightResponse::FromMessage)
        .def("ToMessage", &arra::WeightResponse::ToMessage)
        .def_readwrite("numberOfScales_", &arra::WeightResponse::numberOfScales_)
        .def_property("floatWeight_", 
                      [](const arra::WeightResponse& wr) -> py::array_t<float> {
                          return py::array_t<float>(arra::MAX_NR_SCALES, wr.floatWeight_);
                      },
                      [](arra::WeightResponse& wr, const py::array_t<float>& arr) {
                          std::memcpy(wr.floatWeight_, arr.data(), sizeof(float) * arra::MAX_NR_SCALES);
                      });
    
    py::class_<arra::AddScaleRequest>(m, "AddScaleRequest")
        .def(py::init<>())
        .def("FromMessage", &arra::AddScaleRequest::FromMessage)
        .def("ToMessage", &arra::AddScaleRequest::ToMessage)
        .def_readwrite("scaleIndex_", &arra::AddScaleRequest::scaleIndex_)
        .def_readwrite("dt1_", &arra::AddScaleRequest::dt1_)
        .def_readwrite("sck1_", &arra::AddScaleRequest::sck1_)
        .def_readwrite("dt2_", &arra::AddScaleRequest::dt2_)
        .def_readwrite("sck2_", &arra::AddScaleRequest::sck2_);

    py::class_<arra::AddScaleResponse>(m, "AddScaleResponse")
        .def(py::init<>())
        .def("FromMessage", &arra::AddScaleResponse::FromMessage)
        .def("ToMessage", &arra::AddScaleResponse::ToMessage)
        .def_readwrite("success_", &arra::AddScaleResponse::success_);

    py::class_<arra::ErrorResponse>(m, "ErrorResponse")
        .def(py::init<>())
        .def("FromMessage", &arra::ErrorResponse::FromMessage)
        .def("ToMessage", &arra::ErrorResponse::ToMessage)
        .def_readwrite("errorCode_", &arra::ErrorResponse::errorCode_);

    m.def("createErrorResponse", &arra::createErrorResponse);
}
