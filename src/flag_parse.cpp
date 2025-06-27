#include "flag_parse.hpp"

void flags::add_extension_if_missing(std::string& filename,
                                     const FileType filetype) {
  const std::string& suffix = (filetype == FileType::PNG) ? ".png" : ".jpeg";

  const auto has_extension = [&suffix](const std::string& str) {
    return str.length() >= suffix.length() &&
           std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
  };

  if (!has_extension(filename)) {
    filename += suffix;
  }
}

void print_usage(const std::string& prog_name) {
  std::cerr
      << "Usage: " << prog_name << " -s <string> -o <filename> [options]\n"
      << "Options:\n"
      << "  -s, --string   The string to encode in the QR code (required).\n"
      << "  -o, --output   The output file name for the QR code (required).\n"
      << "  -j, --jpeg     Output a JPEG file instead of a PNG.\n"
      << "  -h, --help     Show this help message.\n";
}

flags::ProgramOptions flags::parse_command_line(int argc, char* argv[]) {
  flags::ProgramOptions opts;
  // skip prog name
  std::vector<std::string> args(argv + 1, argv + argc);

  for (size_t i = 0; i < args.size(); ++i) {
    const std::string& arg = args[i];

    if ((arg == "-s" || arg == "--string") && i + 1 < args.size()) {
      opts.qr_string = args[++i];
    } else if ((arg == "-o" || arg == "--output") && i + 1 < args.size()) {
      opts.file_name = args[++i];
    } else if (arg == "-j" || arg == "--jpeg") {
      opts.file_type = flags::FileType::JPEG;
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
  add_extension_if_missing(opts.file_name, opts.file_type);
  return opts;
}
