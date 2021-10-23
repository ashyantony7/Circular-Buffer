# Circular-Buffer
Simple Circular Buffer implementation in C++ with a python extension using [pybind11](https://github.com/pybind/pybind11)

Adapts the ```__iter__```, ```__getitem__```, and ```__setitem__``` special methods in Python; therefore can be used like any other container.

## Requirements
1. C++ compiler with C++17 standard or higher
2. Pybind11 2.6 or higher for python extension

## Installation using pipenv
1. Install required packages
```pipenv install```

2. Build extension
```pipenv run python3 setup.py install```

## Example
```python
from circularbuffer import CircularBuffer

buffer = CircularBuffer(window_size=10)

for i in range(15):
    buffer.append(i)

for val in buffer:
    print(val)
```
Output

```
5.0
6.0
7.0
8.0
9.0
10.0
11.0
12.0
13.0
14.0
```
