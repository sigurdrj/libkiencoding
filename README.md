# encoding

# Documentation
All functions are under the `libkiencoding::` namespace

## Base64
`libkiencoding::base64::decode(const string &input)` \
Decodes a base64 input 

## Escaping https://en.cppreference.com/w/cpp/language/escape (https://archive.is/aRQrB)
`libkiencoding::escaping::escape(const string &input)` \
Escapes a string following the C++ standard 

`libkiencoding::escaping::unescape(const string &input)` \
Unescapes a string following the C++ standard
