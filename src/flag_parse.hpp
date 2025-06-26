#ifndef COMMAND_LINE_FLAG_PARSE_HPP
#define COMMAND_LINE_FLAG_PARSE_HPP

#include <iostream>
#include <string>
#include <vector>

namespace flags {

inline static const std::string SUFFIX = ".png";

struct ProgramOptions {
  std::string qr_string;
  std::string file_name;
};

inline void print_usage(const std::string& prog_name) {
  std::cerr
      << "Usage: " << prog_name << " -s <string> -o <filename.png>\n"
      << "Options:\n"
      << "  -s, --string   The string to encode in the QR code (required).\n"
      << "  -o, --output   The output file name for the QR code PNG "
         "(required).\n"
      << "  -h, --help     Show this help message.\n";
}

inline void add_png_extension_if_missing(std::string& filename) {
  const auto has_png_extension = [](const std::string& str) {
    return str.length() >= SUFFIX.length() &&
           std::equal(SUFFIX.rbegin(), SUFFIX.rend(), str.rbegin());
  };

  if (!has_png_extension(filename)) {
    filename += SUFFIX;
  }
}

inline ProgramOptions parse_command_line(int argc, char* argv[]) {
  ProgramOptions opts;
  std::vector<std::string> args(argv + 1, argv + argc);

  for (size_t i = 0; i < args.size(); ++i) {
    const std::string& arg = args[i];

    if ((arg == "-s" || arg == "--string") && i + 1 < args.size()) {
      opts.qr_string = args[++i];
    } else if ((arg == "-o" || arg == "--output") && i + 1 < args.size()) {
      opts.file_name = args[++i];
    } else if (arg == "-h" || arg == "--help") {
      print_usage(argv[0]);
      exit(0);
    }
  }

  // verify that both required arguments are provided
  if (opts.qr_string.empty() || opts.file_name.empty()) {
    print_usage(argv[0]);
    std::cerr << "\nError: Both --string and --output arguments are required."
              << std::endl;
    exit(1);
  }

  // We accept an arbitrary string for this arg, so add png extension if not
  // present
  add_png_extension_if_missing(opts.file_name);
  return opts;
}

}  // namespace flags

#endif