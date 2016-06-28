[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)

i3ipc++
=======
An implementation of i3 IPC in C++11.

## Requirements

* C++11 compiler
* sigc++ 2.0
* jsoncpp

## Using
Yet the only way of using is to add this repo as a submodule

```bash
git submodule add https://github.com/jaagr/i3ipcpp.git i3ipc++
```

Then just type this in your `CMakeLists.txt`:

```cmake
...
add_subdirectory(i3ipc++)
...
```

And then just link:

```cmake
...
target_link_libraries(someapp ${I3IPCPP_LIBRARIES})
...
```

## Version i3 support
It is written according to the *current* specification, so some of new features in IPC can be implemented. If there is some of them, please notice at issues page.

## Examples
There is few examples in `examples/` directory.

## Licensing
This library is licensed under under the MIT license, but it also uses [`github.com/drmgc/auss`](https://github.com/drmgc/auss).
