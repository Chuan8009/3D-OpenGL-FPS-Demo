#ifndef FILE_READER_H
#define FILE_READER_H

#include <vector>
#include <string>
#include <fstream>

/* Reads formatted data from a file
** example file:
** "file.txt"
** # People
** str_name Greg
** i_number 100

** Uses 2D vector of Key_Value pairs
** People = Section
** i_number = Value_Key
** Data[Section_Hash_Val] -> Table[Value_Key_Hash_Val] -> Value

** Only handles insertion not deletion - Only Reading data from files not changing it
** Hash arrays are static
*/

class FileReader {
public:
	struct Key_Value {
		int key_val;
		std::string value;
	};

	struct Key_Table {
		int key_val;
		std::vector<Key_Value> table;
		bool empty = true;
	};

	FileReader(const char* file_path);

	// s_read : reads from a defined section - default is no section
	// returns false if no key exists
	const bool s_read_string(std::string* val, const std::string& key, const std::string& section = "");
	const bool s_read_int   (int *val, const std::string& key, const std::string& section = "");
	const bool s_read_uint  (unsigned int *val, const std::string& key, const std::string& section = "");
	const bool s_read_float (float *val, const std::string& key, const std::string& section = "");
	const bool s_read_double(double *val, const std::string& key, const std::string& section = "");

	// read : reads from current section using set_section - default is no section
	// returns false if no key exists
	const bool read_string(std::string* val, const std::string& key);
	const bool read_int(int* val, const std::string& key);
	const bool read_uint(unsigned int* val, const std::string& key);
	const bool read_float(float* val, const std::string& key);
	const bool read_double(double* val, const std::string& key);

	// set section to read from using read_string, read_int ...
	void set_section(const std::string& section);

	const size_t get_num_lines();
private:
	std::vector<Key_Table> _data;
	size_t _num_lines;

	int _section_hash_val;

	const int _get_section_hash_val(const int& section_val);
	const int _new_section_hash_val(const int& section_val);
	const int _get_key_hash_val(const int& key_val, const int& section_hash_val = 0);
	const int _new_key_hash_val(const int& key_val, const int& section_hash_val = 0);
};

#endif