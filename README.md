Writes a string to a QR Code and saves the code to a PNG file.

Requires: `CMake` and `vcpkg` installed locally.

Usage:

```
Usage: ./build/qrgen -s <string> -o <filename.svg>
Options:
  -s, --string   The string to encode in the QR code (required).
  -o, --output   The output file name for the QR code SVG (required).
  -h, --help     Show this help message.
```

Configuring with CMake (i.e):

```bash
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" -D CMAKE_BUILD_TYPE=RelWithDebInfo -D CMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build build/
```
