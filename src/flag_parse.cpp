#include "flag_parse.hpp"

#include <cstdlib>

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

namespace {
void print_usage(const std::string& prog_name) {
  std::cerr
      << "Usage: " << prog_name << " -s <string> -o <filename> [options]\n"
      << "Options:\n"
      << "  -s, --string   The string to encode in the QR code (required).\n"
      << "  -o, --output   The output file name for the QR code (required).\n"
      << "  -j, --jpeg     Output a JPEG file instead of a PNG.\n"
      << "  -s --scale <scale>  Scale factor for the QR code (default: 12).\n"
      << "  -h, --help     Show this help message.\n";
}

int parse_scale_value(const std::string& scale_str) {
  int scale;
  try {
    scale = std::stoi(scale_str);
  } catch (const std::invalid_argument&) {
    std::cerr << "error: invalid scale value '" << scale_str << "'."
              << std::endl;
    std::exit(EXIT_FAILURE);
  } catch (const std::out_of_range&) {
    std::cerr << "error: scale value '" << scale_str << "' is out of range."
              << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // check if the value is in an acceptable and valid range
  if (scale < 1 || scale > 50) {
    std::cerr << "error: scale value must be between 1 and 50." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  return scale;
}
}  // namespace

flags::ProgramOptions flags::parse_command_line(int argc, char* argv[]) {
  flags::ProgramOptions opts;
  // skip prog name
  std::vector<std::string> args(argv + 1, argv + argc);

  for (size_t i = 0; i < args.size(); ++i) {
    const std::string& arg = args[i];

    if ((arg == "-s" || arg == "--string") && i + 1 < args.size()) {
      opts.qr_string = args[++i];
    } else if ((arg == "-o" || arg == "--output") && i + 1 < args.size()) {
      opts.filename = args[++i];
    } else if (arg == "-j" || arg == "--jpeg") {
      opts.filetype = flags::FileType::JPEG;
    } else if (arg == "--scale" && i + 1 < args.size()) {
      opts.scale = parse_scale_value(args[++i]);
    } else if (arg == "-h" || arg == "--help") {
      print_usage(argv[0]);
      std::exit(EXIT_SUCCESS);
    }
  }

  // verify that both required arguments are provided
  if (opts.qr_string.empty() || opts.filename.empty()) {
    print_usage(argv[0]);
    std::cerr << "\nError: Both --string and --output arguments are required."
              << std::endl;
    exit(1);
  }

  // We accept an arbitrary string for this arg, so add png extension if not
  // present
  add_extension_if_missing(opts.filename, opts.filetype);
  return opts;
}
