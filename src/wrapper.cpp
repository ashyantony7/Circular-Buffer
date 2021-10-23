#include <pybind11/pybind11.h>
#include "CircularBuffer.h"

namespace py = pybind11;

PYBIND11_MODULE(circularbuffer, module) {
  // doc for the module
  module.doc() = "Circular Buffer";

  // Add Circular Buffer class
  py::class_<CircularBuffer::CircularBuffer<double>, std::shared_ptr<CircularBuffer::CircularBuffer<double>>>(
    module, "CircularBuffer", "Circular buffer data structure")

    .def("__repr__",
         [](CircularBuffer::CircularBuffer<double>& buffer) {
           std::string display("CircularBuffer: [ ");
           const size_t window_size = buffer.get_window_size();
           if (window_size < 7) {
             for (auto it = buffer.begin(); it != buffer.end(); ++it) { display += std::to_string(*it) + ", "; }
           } else {
             for (size_t i = 0; i < 4; ++i) { display += std::to_string(buffer[i]) + ", "; }
             display += " .... " + std::to_string(buffer[window_size - 1]);
           }
           return display + " ] length: " + std::to_string(window_size);
         })

    .def(py::init([](const size_t window_size) { return CircularBuffer::CircularBuffer<double>(window_size); }),
         py::arg("window_size"))

    .def("__len__", &CircularBuffer::CircularBuffer<double>::get_window_size)

    .def("__getitem__", [](const CircularBuffer::CircularBuffer<double>& buffer, size_t index) { return buffer[index]; })

    .def("__setitem__",
         [](CircularBuffer::CircularBuffer<double>& buffer, size_t index, double value) { buffer[index] = value; })

    .def("__iter__",
         [](CircularBuffer::CircularBuffer<double>& buffer) { return py::make_iterator(buffer.begin(), buffer.end()); })

    .def_property_readonly("window_size", &CircularBuffer::CircularBuffer<double>::get_window_size, "window size")

    .def("clear", &CircularBuffer::CircularBuffer<double>::clear, "clear the buffer")

    .def("append", &CircularBuffer::CircularBuffer<double>::push_back, py::arg("value"), "add an element to the buffer");

}
