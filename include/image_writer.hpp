#ifndef IMAGE_WRITER_HPP
#define IMAGE_WRITER_HPP

#include <cstdlib>
#include <qrcodegen.hpp>

namespace image_writer {
class ImageWriter {
 public:
  ImageWriter(const std::string& filename, const qrcodegen::QrCode& qr)
      : filename_(filename), qr_(qr) {
      }

  ~ImageWriter() {
    if (file_ptr_) {
      fclose(file_ptr_);
    }
  }

  std::string_view get_filename() const { return filename_; }
  bool save_qr_to_png(int scale = 12);
  bool save_qr_to_jpeg(int scale = 12, int quality = 95);

 private:
  struct QrImageDimensions {
    int scale;
    int width;
    int height;
    int border;
    // derived values
    int total_width;
    int total_height;
  };

  void open_file() {
    file_ptr_ = fopen(filename_.c_str(), "wb");
    if (!file_ptr_) {
      throw std::runtime_error("Could not open file " + filename_ +
                               " for writing");
    }
  }

  inline void set_dimensions(int scale) {
    const int size = qr_.getSize();
    dimensions_.scale = scale;
    dimensions_.width = size * scale;
    dimensions_.height = size * scale;
    // 2 "modules" wide border around the actual QR code
    dimensions_.border = 2 * scale;
    // width + the border on both sides
    dimensions_.total_width = dimensions_.width + 2 * dimensions_.border;
    // as above
    dimensions_.total_height = dimensions_.height + 2 * dimensions_.border;
  }

  std::string filename_;
  qrcodegen::QrCode qr_;
  FILE* file_ptr_ = nullptr;
  QrImageDimensions dimensions_;
  static constexpr const unsigned char BLACK = 0;    // Black pixel value
  static constexpr const unsigned char WHITE = 255;  // White pixel value
};
}  // namespace image_writer

#endif  // IMAGE_WRITER_HPP