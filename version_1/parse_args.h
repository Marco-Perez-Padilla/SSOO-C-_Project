#include <string>
#include <expected>

enum class parse_args_errors {
 missing_argument,
 unknown_option,
 // ...
};

struct program_options {
 bool show_help = false;
 bool extended_mode = false;
 std::string output_filename;
 // ...
 //std::vector<std::string> additional_args; 
};

std::expected<program_options, parse_args_errors> parse_args(int argc, char* argv[]);
void print_usage ();