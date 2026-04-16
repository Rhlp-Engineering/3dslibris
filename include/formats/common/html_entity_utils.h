#pragma once

#include <cstdint>
#include <string>

namespace html_entity_utils {

bool DecodeHtmlEntityCodepoint(const std::string &entity, uint32_t *out);
bool DecodeHtmlEntityUtf8(const std::string &entity, std::string *out);
std::string NormalizeHtmlNamedEntitiesForXml(const std::string &in);

} // namespace html_entity_utils
