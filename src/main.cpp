#include <png.h>

#include <iostream>
#include <qrcodegen.hpp>
#include <string>
#include <vector>

#include "flag_parse.hpp"

namespace {

static bool save_qr_to_png(const qrcodegen::QrCode& qr,
                           const std::string& filename, int scale = 12) {
  int size = qr.getSize();
  int width = size * scale;
  int height = size * scale;

  // 2 "modules" wide border around the actual QR code
  const int border = 2 * scale;
  // width + the border on both sides
  const int total_width = width + 2 * border;
  // as above
  const int total_height = height + 2 * border;

  FILE* fp = fopen(filename.c_str(), "wb");
  if (!fp) {
    std::cerr << "Error: Could not open file " << filename << " for writing"
              << std::endl;
    return false;
  }

  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) {
    fclose(fp);
    return false;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, nullptr);
    fclose(fp);
    return false;
  }

  // clean-up iff libpng encounters an error
  if (setjmp(png_jmpbuf(png))) {
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return false;
  }

  png_init_io(png, fp);

  png_set_IHDR(png, info, total_width, total_height, 8, PNG_COLOR_TYPE_GRAY,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png, info);

  std::vector<png_byte> row_buffer(total_width);

  constexpr const png_byte BLACK = 0;    // Black pixel value
  constexpr const png_byte WHITE = 255;  // White pixel value

  // write each row of the QR code to the buffer
  for (int y = 0; y < total_height; y++) {
    for (int x = 0; x < total_width; x++) {
      bool is_black = false;

      // Check if we're in the border area
      if (x >= border && x < total_width - border && y >= border &&
          y < total_height - border) {
        // We're in the QR code area
        int qr_x = (x - border) / scale;
        int qr_y = (y - border) / scale;

        // false for light, true for dark.
        is_black = qr.getModule(qr_x, qr_y);
      }

      row_buffer[x] = is_black ? BLACK : WHITE;
    }
    png_write_row(png, row_buffer.data());
  }

  // and write the end of the PNG file
  png_write_end(png, nullptr);
  png_destroy_write_struct(&png, &info);
  fclose(fp);

  return true;
}
}  // namespace

int main(int argc, char* argv[]) {
  flags::ProgramOptions opts = flags::parse_command_line(argc, argv);

  try {
    const qrcodegen::QrCode qr_code = qrcodegen::QrCode::encodeText(
        opts.qr_string.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);

    if (save_qr_to_png(qr_code, opts.file_name)) {
      std::cout << "QR code successfully saved to " << opts.file_name
                << std::endl;
    } else {
      std::cerr << "Error: Failed to save QR code to " << opts.file_name
                << std::endl;
      return 1;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
