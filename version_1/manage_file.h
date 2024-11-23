#include <string>
#include <expected>

#include "safeMap.h"

std::expected<std::string_view, int> read_all(const std::string& path, bool extended);
void send_response(std::string_view header, std::string_view body = {});