# PYAF
A toy example on how to use py_AFF3CT and integrate your own C++ modules.

## Compile the project
First, compile py_AFF3CT se documentation [here](py_aff3ct/README.md)

Compile the custom library:

	$ mkdir build && cd build
	$ ../configure.py --verbose
	$ cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -march=native -fvisibility=hidden -fvisibility-inlines-hidden -faligned-new"
	$ make -j4

## Organization of this repository
The `src/cpp` folder contains all that is required to build your own C++ extension of py_AFF3CT.

### Sources of custom modules
The subfolder `src/cpp/module` contains the code of custom modules (here some filters are developped).
Note that the mother `Filter` class inherits from the AFF3CT class `aff3ct::module::Module`.
```cpp
class Filter : public Module, public tools::Interface_reset
{
    ...
}
```

### Sources the wrapper
The "glue" between C++ code and Python is contained in `src/cpp/Wrapper/wrapper.cpp`. For example, the `Filter` class is wrapped here
```cpp
py::class_<aff3ct::module::Filter<float>>(m_filter,"Filter", py_aff3ct_module)		.def(py::init<const int, const int>(), "N"_a, "N_fil"_a);
```
the line
```cpp
.def(py::init<const int, const int>(), "N"_a, "N_fil"_a);
```
wraps the constructor for the `Filter` class.

## Use your custom library
In python 
