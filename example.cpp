#include <iostream>

#include "encoding.hpp"

int main(){
	std::cout << "Unescaping:\n";
	std::cout << "This\\nText\\nRight \\\\ here\n";
	std::cout << encoding::escaping::unescape("This\\nText\\nRight \\\\ here") << '\n';

	std::cout << '\n';

	std::cout << "Base64 decoding:\n";
	std::cout << "VGhpcyB0ZXh0IG92ZXIgaGVyZQ==\n";
	std::cout << encoding::base64::decode("VGhpcyB0ZXh0IG92ZXIgaGVyZQ==") << '\n';
}
