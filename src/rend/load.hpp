

std::string get_file_contents(const char* filename)
{
	try
	{
		std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return (contents);
		}
		throw(errno);
	}
	catch (int e)
	{
		std::cout << "Error: " << e << std::endl;
		return "Error in get_file_contents";
	}
}

// file e.g. /res/obj_list.txt
// file content: 1 monkey res/obj/suzanne.obj res/textures/monkey.png
//               2 cube res/obj/scube.obj res/textures/cube.png
//               ...



int read_list_of_3(std::string file, std::map<int, std::string>& NameDict, std::map<int, std::string>& PathDict)
{
	try
	{
		std::string content = get_file_contents(file.c_str());
		std::istringstream iss(content);
		std::string line;
		int index = 0;
		while (std::getline(iss, line))
		{
			std::istringstream iss2(line);
			std::string word;
			int i = 0;
			while (std::getline(iss2, word, ' '))
			{
				if (i == 0)
				{
					index = std::stoi(word);
				}
				else if (i == 1)
				{
					NameDict[index] = word;
				}
				else if (i == 2)
				{
					PathDict[index] = word;
				}
				i++;
			}
		}
		return index;
	}
	catch (...)
	{
		std::cout << "Error: reading object list from " << file << " failed!" << std::endl;
		return -1;
	}

}




// write to i dictionary the key-value pair from the line with help of recursion
template <typename... Args>
void write_to_dict(unsigned target_index, unsigned current_index,
	int int_to_write, std::string string_to_write,
	std::map<int, std::string>& dict, Args&... args)
{
	if (target_index == current_index)
	{
		dict[int_to_write] = string_to_write;
	}
	else
	{
		write_to_dict(target_index, current_index + 1, int_to_write, string_to_write, args...);
	}
}


// for last dictonary cut last symbol from strng
template <>
void write_to_dict(unsigned target_index, unsigned current_index,
	int int_to_write, std::string string_to_write,
	std::map<int, std::string>& dict)
{
	if (target_index == current_index)
	{
		// check if it end on a new line
		if (string_to_write[string_to_write.size() - 1] == '\n' ||
			string_to_write[string_to_write.size() - 1] == '\r')
		{
			string_to_write = string_to_write.substr(0, string_to_write.size() - 1);
		}
		dict[int_to_write] = string_to_write;
	}
	else
	{
		// cout error
		// throw
		throw;
	}
}

template <typename... Args>
int read_list_of_n(std::string file, Args&... args)
{
	std::string content = get_file_contents(file.c_str());
	std::istringstream iss(content);

	// check if file is opened throw exception
	if (iss.fail())
	{
		throw;
	}

	std::string line;
	int index = 0;
	while (std::getline(iss, line))
	{
		std::istringstream iss2(line);
		std::string word;
		int i = 0;
		while (std::getline(iss2, word, ' '))
		{
			if (i == 0)
			{
				index = std::stoi(word);
			}
			else
			{
				write_to_dict(i - 1, 0, index, word, args...);
			}
			i++;
		}
	}
	return index;
}

// test function to load res/test.txt
void test_loadhpp()
{
	std::map<int, std::string> dict1;
	std::map<int, std::string> dict2;
	std::map<int, std::string> dict3;
	std::map<int, std::string> dict4;

	int index = read_list_of_n("res/test.txt", dict1, dict2, dict3, dict4);

	std::map<int, std::string> res_dict1 = {
		{1, "1"},
		{2, "2"},
		{3, "3"}
	};
	std::map<int, std::string> res_dict2 = {
		{1, "11"},
		{2, "21"},
		{3, "31"}
	};
	std::map<int, std::string> res_dict3 = {
		{1, "12"},
		{2, "22"},
		{3, "32"}
	};
	std::map<int, std::string> res_dict4 = {
		{1, "13"},
		{2, "23"},
		{3, "33"}
	};
	

	// check if all dictionaries are equal
	if (dict1 == res_dict1 && dict2 == res_dict2 && dict3 == res_dict3 && dict4 == res_dict4)
	{
		std::cout << "test_loadhpp passed" << std::endl;
	}
	else
	{
		std::cout << "test_loadhpp failed" << std::endl;
	}
	

}

