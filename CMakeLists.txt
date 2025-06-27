cmake_minimum_required(VERSION 3.25)
project(QRCodeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(unofficial-nayuki-qr-code-generator REQUIRED)

add_executable(qrgen
    src/image_writer.cpp
    src/flag_parse.cpp
    src/main.cpp)

target_include_directories(qrgen PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(qrgen PRIVATE
    PNG::PNG
    JPEG::JPEG
    unofficial::nayuki-qr-code-generator::nayuki-qr-code-generator
)

target_compile_options(qrgen PRIVATE -Wall -Wextra -pedantic)