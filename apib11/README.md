MicMac v2 Python3 API
=====================


Dependencies
------------

As admin:

```sh
    apt install python3-pip doxygen clang-tools llvm-dev
```

Make sure to also install the [MMVII dependencies](../README.md#installation-from-sources).

In the Python virtual environment used for compilation:

```sh
pip3 install -r requirements.txt
```

Compilation
-----------

First, generate the source distribution in `apib11` directory:

```sh
python3 -m build --sdist
```

Then, in `apib11` directory and the correct Python virtual environment. This will build MMVII and the Python API:

```sh
# You can specify the number of threads to use when building using the CMAKE_BUILD_PARALLEL_LEVEL environment variable
pip3 wheel . --no-deps -w dist/
```

Installation
------------

```sh
pip3 install --force-reinstall dist/mmvii*.whl
```

Distribution
------------

The file dist/MMVII-*.whl, created at compilation, can be distributed to machines with same OS, architecture and python version.

This file contains all the necessary files to run the module:
MMVII does not have to be installed on the machine to use the python module.

It can be installed with:

```sh
pip3 install MMVII-*.whl
```

Upgrade pip if needed:

```sh
python3 -m pip install --upgrade pip
```
Alternatively you can distribute the mmvii-*.tar.gz archive that allow pip to directly build the package on install.

Usage
-----

```python
    import MMVII
```

The built-in Python help system can be used to have information about the API.

See 'examples' directory for use cases.


Binding Conventions
-------------------
  - implement python binding in a .cpp files that matches th C++ header.<p> (i.e. a binding for a C++ class that is declared in MMVII_MyClass.h we'll be implemented in py_MMVII_MyClass.cpp)
  - class names are the same as in C++ but without the leading 'c'
  - functions, methods, properties are the same as in C++ but with initial letter in lower case (and 'm' removed from class variables names)

