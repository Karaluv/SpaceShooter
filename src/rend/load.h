std::string get_file_contents(const char *filename)
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

int read_obj_list(std::string file, std::map<int, std::string> &NameDict, std::map<int, std::string> &PathObjDict,
                  std::map<int, std::string> &PathTextureDict)
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
                    PathObjDict[index] = word;
                }
                else if (i == 3)
                {
                    PathTextureDict[index] = word;
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