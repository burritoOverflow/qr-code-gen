#ifndef COMMAND_LINE_FLAG_PARSE_HPP
#define COMMAND_LINE_FLAG_PARSE_HPP

#include <iostream>
#include <string>
#include <vector>

namespace flags {
enum class FileType {
  PNG,
  JPEG,
};

struct ProgramOptions {
  std::string qr_string;
  std::string filename;
  FileType filetype = FileType::PNG;  // default file type
  unsigned int scale = 12;            // default scale factor
};

void add_extension_if_missing(std::string& filename, const FileType filetype);
ProgramOptions parse_command_line(int argc, char* argv[]);
}  // namespace flags

#endif