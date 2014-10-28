#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define BOP_FILELOAD_ERR std::cerr << "(bop file loading) Error:"

namespace bop {
	namespace util {

		std::string loadIntoString(std::string filename) {
			std::fstream file_stream(filename.c_str(), std::ios::in);
			if (file_stream.is_open()) {
				std::string file_string;
				std::string temp_str;
				while (std::getline(file_stream, temp_str, '\n')) {
					file_string += temp_str + '\n';
				}
				file_stream.close();
				return file_string;
			}
			else {
				BOP_FILELOAD_ERR << "Could not load \"" << filename << "\", does the file exist?" << std::endl;
				file_stream.close();
				return "";
			}
		}

		std::vector<std::string> loadIntoVector(std::string filename, char delim = '\n') {
			std::fstream file_stream(filename);
			if (file_stream.is_open()) {
				std::vector<std::string> strvec;
				std::string temp_str;
				while(std::getline(file_stream, temp_str, delim)) {
					strvec.push_back(temp_str);
				}
				return strvec;
			}
			else {
				BOP_FILELOAD_ERR << "Could not load \"" << filename << "\", does the file exist?" << std::endl;
				file_stream.close();
				return std::vector<std::string>();
			}
		}
	};
};
