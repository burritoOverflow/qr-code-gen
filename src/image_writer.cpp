#include "image_writer.hpp"

#include <iostream>
#include <string>

extern "C" {
#include <jpeglib.h>
}
#include <png.h>

#include "image_writer.hpp"

bool image_writer::ImageWriter::save_qr_to_png(int scale) {
  set_dimensions(scale);
  open_file();

  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) {
    fclose(file_ptr_);
    return false;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, nullptr);
    fclose(file_ptr_);
    return false;
  }

  // clean-up iff libpng encounters an error
  if (setjmp(png_jmpbuf(png))) {
    png_destroy_write_struct(&png, &info);
    fclose(file_ptr_);
    return false;
  }

  png_init_io(png, file_ptr_);

  png_set_IHDR(png, info, dimensions_.total_width, dimensions_.total_height, 8,
               PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png, info);

  std::vector<unsigned char> row_buffer(dimensions_.total_width);

  // write each row of the QR code to the buffer
  for (int y = 0; y < dimensions_.total_height; y++) {
    for (int x = 0; x < dimensions_.total_width; x++) {
      bool is_black = false;

      // Check if we're in the border area
      if (x >= dimensions_.border &&
          x < dimensions_.total_width - dimensions_.border &&
          y >= dimensions_.border &&
          y < dimensions_.total_height - dimensions_.border) {
        // We're in the QR code area
        int qr_x = (x - dimensions_.border) / scale;
        int qr_y = (y - dimensions_.border) / scale;

        // false for light, true for dark.
        is_black = qr_.getModule(qr_x, qr_y);
      }

      row_buffer[x] = is_black ? BLACK : WHITE;
    }
    png_write_row(png, row_buffer.data());
  }

  // and write the end of the PNG file
  png_write_end(png, nullptr);
  png_destroy_write_struct(&png, &info);
  return true;
}

bool image_writer::ImageWriter::save_qr_to_jpeg(int scale, int quality) {
  set_dimensions(scale);
  open_file();

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, file_ptr_);

  cinfo.image_width = dimensions_.total_width;
  cinfo.image_height = dimensions_.total_height;
  cinfo.input_components = 1;
  cinfo.in_color_space = JCS_GRAYSCALE;  // monochrome

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);
  jpeg_start_compress(&cinfo, TRUE);

  std::vector<unsigned char> row_buffer(dimensions_.total_width);
  JSAMPROW row_pointer[1];
  row_pointer[0] = row_buffer.data();

  for (int y = 0; y < dimensions_.total_height; y++) {
    for (int x = 0; x < dimensions_.total_width; x++) {
      bool is_black = false;

      if (x >= dimensions_.border &&
          x < dimensions_.total_width - dimensions_.border &&
          y >= dimensions_.border &&
          y < dimensions_.total_height - dimensions_.border) {
        const int qr_x = (x - dimensions_.border) / scale;
        const int qr_y = (y - dimensions_.border) / scale;

        is_black = qr_.getModule(qr_x, qr_y);
      }

      row_buffer[x] = is_black ? BLACK : WHITE;
    }

    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  return true;
}