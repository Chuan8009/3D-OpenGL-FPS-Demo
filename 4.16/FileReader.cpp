#include "FileReader.h"

#include <iostream>

#define SECTION_CHAR '#'
#define COMMENT_CHAR '-'

// convert string to int
size_t str_val(const std::string& str) {
	size_t val = 0;

	for (auto c : str) {
		val += c;
	}

	return val;
}

// read num of sections and lines in each section
// sections[a] = b ; a = section, b = num of lines
const std::vector<size_t> query_file(std::fstream& file) {
	std::vector<size_t> sections{ 0 };

	char header;
	size_t section = 0;
	while (!file.eof()) {
		do {
			file.get(header);
		} while (header == '\n');

		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (header == SECTION_CHAR) {
			sections.push_back(0);
			++section;
		}
		else if (header != COMMENT_CHAR) {
			++sections[section];
		}
	}

	file.clear();
	file.seekg(0, std::ios::beg);

	return sections;
}

FileReader::FileReader(const char* file_path) :
	_num_lines(0)
{
	std::fstream file(file_path, std::ios::in);
	if (!file.is_open()) {
		return;
	}

	const std::vector<size_t> query = query_file(file);
	_data.resize(query.size());

	std::string key, val;
	size_t section = 0;

	// Default Table -- No Section Comment
	_section_hash_val = _new_section_hash_val(str_val(""));
	_data[_section_hash_val].key_val = str_val("");
	_data[_section_hash_val].table.resize(query[section]);
	_data[_section_hash_val].empty = false;

	while (std::getline(file, key, ' ')) {
		std::getline(file, val);

		while (*key.begin() == '\n') {
			key.erase(key.begin());
		}
		if (key[0] == SECTION_CHAR) {
			const int key_val = str_val(val);
			const int hash_val = _new_section_hash_val(key_val);
			if (hash_val == -1) {
				throw std::out_of_range("Table Sections are full");
			}
			++section;
			_data[hash_val].key_val = key_val;
			_data[hash_val].table.resize(query[section]);
			_data[hash_val].empty = false;
			_section_hash_val = hash_val;
		}
		else if (key[0] != COMMENT_CHAR) {
			const int key_val = str_val(key);
			const int hash_val = _new_key_hash_val(key_val, _section_hash_val);
			if (hash_val == -1) {
				throw std::out_of_range("Tables are full");
			}
			_data[_section_hash_val].table[hash_val] = { key_val, val };
		}
		++_num_lines;
	}

}

const bool FileReader::s_read_string(std::string* val, const std::string& key, const std::string& section) {
	const int section_hash_val = _get_section_hash_val(str_val(section));
	if (section_hash_val < 0) {
		return false;
	}
	const int key_hash_val = _get_key_hash_val(str_val(key), section_hash_val);
	if (key_hash_val < 0) {
		return false;
	}

	*val = _data[section_hash_val].table[key_hash_val].value;
	return true;
}

const bool FileReader::s_read_int(int* val, const std::string& key, const std::string& section) {
	const int section_hash_val = _get_section_hash_val(str_val(section));
	if (section_hash_val < 0) {
		return false;
	}
	const int key_hash_val = _get_key_hash_val(str_val(key), section_hash_val);
	if (key_hash_val < 0) {
		return false;
	}

	*val = std::stoi(_data[section_hash_val].table[key_hash_val].value);
	return true;
}

const bool FileReader::s_read_uint(unsigned int* val, const std::string& key, const std::string& section) {
	const int section_hash_val = _get_section_hash_val(str_val(section));
	if (section_hash_val < 0) {
		return false;
	}
	const int key_hash_val = _get_key_hash_val(str_val(key), section_hash_val);
	if (key_hash_val < 0) {
		return false;
	}

	*val = (unsigned int)std::stoi(_data[section_hash_val].table[key_hash_val].value);
	return true;
}

const bool FileReader::s_read_float(float* val, const std::string& key, const std::string& section) {
	const int section_hash_val = _get_section_hash_val(str_val(section));
	if (section_hash_val < 0) {
		return false;
	}
	const int key_hash_val = _get_key_hash_val(str_val(key), section_hash_val);
	if (key_hash_val < 0) {
		return false;
	}

	*val = std::stof(_data[section_hash_val].table[key_hash_val].value);
	return true;
}

const bool FileReader::s_read_double(double* val, const std::string& key, const std::string& section) {
	const int section_hash_val = _get_section_hash_val(str_val(section));
	if (section_hash_val < 0) {
		return false;
	}
	const int key_hash_val = _get_key_hash_val(str_val(key), section_hash_val);
	if (key_hash_val < 0) {
		return false;
	}

	*val = std::stod(_data[section_hash_val].table[key_hash_val].value);
	return true;
}

const bool FileReader::read_string(std::string* val, const std::string& key) {
	const int key_hash_val = _get_key_hash_val(str_val(key), _section_hash_val);
	if (key_hash_val < 0) {
		return false;
	}

	*val = _data[_section_hash_val].table[key_hash_val].value;
	return true;
}

const bool FileReader::read_int(int* val, const std::string& key) {
	const int key_hash_val = _get_key_hash_val(str_val(key), _section_hash_val);
	if (key_hash_val < 0) {
		return false;
	}

	*val = std::stoi(_data[_section_hash_val].table[key_hash_val].value);
	return true;
}

const bool FileReader::read_uint(unsigned int* val, const std::string& key) {
	const int key_hash_val = _get_key_hash_val(str_val(key), _section_hash_val);
	if (key_hash_val < 0) {
		return false;
	}

	*val = (unsigned int)std::stoi(_data[_section_hash_val].table[key_hash_val].value);
	return true;
}

const bool FileReader::read_float(float* val, const std::string& key) {
	const int key_hash_val = _get_key_hash_val(str_val(key), _section_hash_val);
	if (key_hash_val < 0) {
		return false;
	}

	*val = std::stof(_data[_section_hash_val].table[key_hash_val].value);
	return true;
}

const bool FileReader::read_double(double* val, const std::string& key) {
	const int key_hash_val = _get_key_hash_val(str_val(key), _section_hash_val);
	if (key_hash_val < 0) {
		return false;
	}

	*val = std::stod(_data[_section_hash_val].table[key_hash_val].value);
	return true;
}


void FileReader::set_section(const std::string& section) {
	_section_hash_val = _get_section_hash_val(str_val(section));
	if (_section_hash_val == -1) {
		throw std::out_of_range("Invalid Section");
	}
}

const int FileReader::_get_section_hash_val(const int& section_val) {
	int hash_val = section_val % _data.capacity();
	const int start_val = hash_val;

	while (_data[hash_val].key_val != section_val) {
		hash_val = (hash_val + 1) % _data.capacity();

		if (hash_val == start_val) {
			return -1;
		}
	}

	return hash_val;
}

const size_t FileReader::get_num_lines() {
	return _num_lines;
}

const int FileReader::_new_section_hash_val(const int& section_val) {
	int hash_val = section_val % _data.capacity();
	const int start_val = hash_val;

	while (!_data[hash_val].empty) {
		hash_val = (hash_val + 1) % _data.capacity();

		if (hash_val == start_val) {
			return -1;
		}
	}

	return hash_val;
}

const int FileReader::_get_key_hash_val(const int& key_val, const int& section_hash_val) {
	if (_data[section_hash_val].empty) {
		return -1;
	}

	int hash_val = key_val % _data[section_hash_val].table.capacity();
	const int start_val = hash_val;

	while (_data[section_hash_val].table[hash_val].key_val != key_val) {
		hash_val = (hash_val + 1) % _data[section_hash_val].table.capacity();

		if (hash_val == start_val) {
			return -1;
		}
	}

	return hash_val;
}

const int FileReader::_new_key_hash_val(const int& key_val, const int& section_hash_val) {
	if (_data[section_hash_val].empty) {
		return -1;
	}

	int hash_val = key_val % _data[section_hash_val].table.capacity();
	const int start_val = hash_val;

	while (_data[section_hash_val].table[hash_val].value != "") {
		hash_val = (hash_val + 1) % _data[section_hash_val].table.capacity();

		if (hash_val == start_val) {
			return -1;
		}
	}

	return hash_val;
}