#include <iostream>

#include "libkiencoding.hpp"

using namespace libkiencoding;

int main(){
	std::cout << "Unescaping:\n";
	std::cout << "This\\nText\\nRight \\\\ here\n";
	std::cout << escaping::unescape("This\\nText\\nRight \\\\ here") << '\n';

	std::cout << '\n';

	std::cout << "Base64 decoding:\n";
	std::cout << "VGhpcyB0ZXh0IG92ZXIgaGVyZQ==\n";
	std::cout << base64::decode("VGhpcyB0ZXh0IG92ZXIgaGVyZQ==") << '\n';
}
