#ifndef ENCODING_HPP
#define ENCODING_HPP

#include <istream>
#include <string>
#include <vector>
#include <map>

#define ADD_ESCAPE_CHARACTER_WITH_VALUE_AND_INCREMENT(ESCAPECHARACTER, VALUE) \
	case ESCAPECHARACTER: \
	ret += VALUE; \
	++i; \
	break;

#define ADD_ESCAPE_CHARACTER_WITH_VALUE(ESCAPECHARACTER, VALUE) \
	case ESCAPECHARACTER: \
	ret += VALUE; \
	break;

using std::string;
using std::vector;

namespace encoding{
	// TODO Maybe make a generic radix64 encoding/decoding where you can supply your own lookup/alphabet
	// Then have built in alphabets for common radix64 encodings
	// See: https://en.wikipedia.org/wiki/Base64#Radix-64_applications_not_compatible_with_Base64

	// TODO Have some weird memoization optimization function aswell?

	// TODO Verify the padding check is proper, also try fuzzing this or something and compare with other base64 libs
	namespace base64{
		string decode(const string &input, const bool &requirePadding = true){
			string out="";

			const string b64LookupTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
			unsigned char bitBuf = 0;
			unsigned char nBitsUntilChar = 0;

			for (char c : input){
				// TODO Find an efficient formula for converting without a lookup table for the specific base64 lookup table
				const size_t digitIndex = b64LookupTable.find(c);
				if (digitIndex == string::npos)
					break;

				for (int bitIndex=5; bitIndex>=0; bitIndex--){
//					bitBuf = bitBuf << 1 | ((digitIndex >> bitIndex) & 1);
					bitBuf <<= 1;
					bitBuf |= (digitIndex >> bitIndex) & 1;

					if (nBitsUntilChar++ == 7){
						out += bitBuf;
						nBitsUntilChar = 0;
					}
				}
			}

			return out;
		}

		string decode_old(const string &input){
			string out="";

			const string b64LookupTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
			vector <bool> bitBuf = {};

			for (char c : input){
				const size_t digitIndex = b64LookupTable.find(c);
				if (digitIndex == string::npos)
					break;

				for (int bitIndex=5; bitIndex>=0; bitIndex--){
					bitBuf.push_back((digitIndex >> bitIndex) & 1);

					if (bitBuf.size() == 8){
						char charToAdd = 0;

						for (bool bit : bitBuf)
							charToAdd = (charToAdd << 1) | bit;

						out += charToAdd;
						bitBuf.clear();
						bitBuf.shrink_to_fit();
					}
				}
			}

			return out;
		}
	}

	namespace escaping{
		// https://en.cppreference.com/w/cpp/language/escape
		string unescape(const string &input){
			if (input.size() <= 1)
				return input;

			string ret = "";

			for (unsigned long long i = 1; i <= input.size(); i++){
				const char lastChar = input[i - 1];
				const char currChar = input[i];

				if (lastChar != '\\'){
					ret += lastChar;
					continue;
				}

				switch (currChar){
					ADD_ESCAPE_CHARACTER_WITH_VALUE_AND_INCREMENT('\\', '\\')
					ADD_ESCAPE_CHARACTER_WITH_VALUE_AND_INCREMENT('a', 0x07) // Bell
					ADD_ESCAPE_CHARACTER_WITH_VALUE_AND_INCREMENT('b', 0x08) // Backspace
					ADD_ESCAPE_CHARACTER_WITH_VALUE_AND_INCREMENT('f', 0x0c) // Form feed - new page
					ADD_ESCAPE_CHARACTER_WITH_VALUE_AND_INCREMENT('n', 0x0a) // Newline
					ADD_ESCAPE_CHARACTER_WITH_VALUE_AND_INCREMENT('r', 0x0d) // Carriage return
					ADD_ESCAPE_CHARACTER_WITH_VALUE_AND_INCREMENT('t', 0x09) // Horizontal tab
					ADD_ESCAPE_CHARACTER_WITH_VALUE_AND_INCREMENT('v', 0x0b) // Vertical tab

					default:
						ret += currChar;
					break;
				}
			}

			return ret;
		}

		// https://en.cppreference.com/w/cpp/language/escape
		string escape(const string &input){
			string ret = "";

			for (char currChar : input){
				switch (currChar){
					ADD_ESCAPE_CHARACTER_WITH_VALUE('\\', '\\')
					ADD_ESCAPE_CHARACTER_WITH_VALUE('a', 0x07) // Bell
					ADD_ESCAPE_CHARACTER_WITH_VALUE('b', 0x08) // Backspace
					ADD_ESCAPE_CHARACTER_WITH_VALUE('f', 0x0c) // Form feed - new page
					ADD_ESCAPE_CHARACTER_WITH_VALUE('n', 0x0a) // Newline
					ADD_ESCAPE_CHARACTER_WITH_VALUE('r', 0x0d) // Carriage return
					ADD_ESCAPE_CHARACTER_WITH_VALUE('t', 0x09) // Horizontal tab
					ADD_ESCAPE_CHARACTER_WITH_VALUE('v', 0x0b) // Vertical tab

					default:
						ret += currChar;
					break;
				}
			}

			return ret;
		}
	}

	// FIXME: Unimplemented
/*	namespace csv{
		// TODO Add bool header something and also with different possible characters for delims etc

		// Strict RFC4180 compliance
		std::map <string, std::vector <string>> decode_rfc4180(std::basic_istream &stream){
			
		}
	}*/

	namespace escape_for_html{
		string escape(const string &input, bool escapeEverything = true){
			string ret = "";

			for (char c : input){
				ret += "&#" + std::to_string(int(c)) + ";";
			}

			return ret;
		}

		string unescape(const string &input){
			string ret = "";

//			std::cout << "DEBUG -----\n\n";
//			std::cout << "input: " << input << '\n';

			for (unsigned long long i = 3; i < input.size(); i++){
				if (input[i] == ';'){
//					std::cout << "semicolon match at i=" << i << '\n';

					string numBuf = "";

					unsigned j;
					for (j = 0; j < 3; j++){
						if ((input[i - j - 1] > '9') || (input[i - j - 1] < '0')){
//							std::cout << "input[" << i << " - " << j << " - 1] = " << input[i - j] << " (not a digit, breaking)\n";
							break;
						}

//						std::cout << "inserting " << input[i - j - 1] << " at beginning of numBuf\n";
//						std::cout << "\tprev: " << numBuf << '\n';
						numBuf.insert(numBuf.begin(), input[i - j - 1]);
//						std::cout << "\tpost: " << numBuf << '\n';
					}

//					std::cout << "Num digits aka j = " << j << '\n';

//					std::cout << "input[i - j - 1] = " << input[i - j - 1] << " (hash expected), input[i - j - 2] = " << input[i - j - 2] << " (ampersand expected)\n";

					if ((input[i - j - 1] != '#') || (input[i - j - 2] != '&')){
//						std::cout << "DEBUG: " << ret << '\n';
						return ret;
					}

					try{
						ret += std::stoi(numBuf);
					} catch(std::invalid_argument &){
//						std::cout << "DEBUG: " << ret << '\n';
						return ret;
					}
				}
			}

//			std::cout << "DEBUG: " << ret << '\n';
			return ret;
		}
	}

	namespace percentencoding{
		struct InvalidPercentEncoding{
			unsigned long long index;

			InvalidPercentEncoding(const unsigned long long newIndex){
				index = newIndex;
			}
		};

		string get_parsed_percent_encoding(const string &input){
			string ret = "";

			for (unsigned long long i=0; i<input.size(); i++){
				if (input[i] == '%'){
					++i;

					char escapedChar = 0;

					for (unsigned char escapeIndexDelta = 0; escapeIndexDelta < 2; escapeIndexDelta++){
						if (i + escapeIndexDelta >= input.size()){
							InvalidPercentEncoding error(i);
							throw error;
						}

						// TODO shorten the .find() part? some ascii trick minus an offset to get hex to decimal size_t digitIndex = string("0123456789ABCDEF").find(input[i + escapeIndexDelta]); 
						size_t digitIndex = string("0123456789ABCDEF").find(input[i + escapeIndexDelta]);

						if (digitIndex == string::npos){
							InvalidPercentEncoding error(i);
							throw error;
						}
							
						escapedChar |= (int(digitIndex) << ((escapeIndexDelta ^ 1) << 2));
					}

					++i;

					ret += escapedChar;
				} else{
					ret += input[i];
				}
			}

			return ret;
		}
	}
}

#endif // ENCODING_HPP
