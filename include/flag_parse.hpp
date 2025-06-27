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
  std::string file_name;
  FileType file_type;
};

void add_extension_if_missing(std::string& filename, const FileType filetype);
ProgramOptions parse_command_line(int argc, char* argv[]);
}  // namespace flags

#endif