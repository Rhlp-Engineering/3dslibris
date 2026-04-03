/*
    3dslibris - mobi_text_decode.h

    MOBI byte decoding and embedded-title extraction helpers.
*/

#pragma once

#include <3ds/types.h>

#include <string>

#include "formats/mobi/mobi_parser_core.h"

class Book;

namespace mobi_text_decode {

std::string DecodeBytesToUtf8(const std::string &in, u32 encoding,
                              bool *used_utf8_guess,
                              bool *used_legacy_guess);

void ApplyEmbeddedTitle(Book *book, const std::string &raw,
                        const mobi_parser_core::MobiHeaderInfo &header);

} // namespace mobi_text_decode
