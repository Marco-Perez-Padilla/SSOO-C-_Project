#ifndef MANAGE_FILE_H
#define MANAGE_FILE_H

#include <string>
#include <expected>

#include "safeMap.h"

std::expected<SafeMap, int> read_all(const std::string& path, bool extended);
void send_response(std::string_view header, std::string_view body = {});

#endif