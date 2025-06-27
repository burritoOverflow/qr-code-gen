#include <cstdlib>
#include <iostream>
#include <qrcodegen.hpp>
#include <string>
#include <vector>

#include "flag_parse.hpp"
#include "image_writer.hpp"

int main(int argc, char* argv[]) {
  flags::ProgramOptions opts = flags::parse_command_line(argc, argv);

  try {
    const qrcodegen::QrCode qr_code = qrcodegen::QrCode::encodeText(
        opts.qr_string.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);

    image_writer::ImageWriter writer(opts, qr_code);

    bool write_success = true;

    if (opts.filetype == flags::FileType::PNG) {
      if (!writer.save_qr_to_png()) {
        write_success = false;
      }
    } else /* opts.file_type == flags::FileType::JPEG */ {
      if (!writer.save_qr_to_jpeg()) {
        write_success = false;
      }
    }

    if (write_success) {
      std::cout << "QR code successfully saved to " << writer.get_filename()
                << std::endl;
    } else {
      std::cerr << "Error: Failed to save QR code to " << writer.get_filename()
                << std::endl;
      std::exit(EXIT_FAILURE);
    }

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return 0;
}
