#include <iostream>
#include <vector>
#include <string>
#include <fstream>

void AES128_ECB_decrypt(uint8_t* input, const uint8_t* key, uint8_t *output);

std::vector<uint8_t> file_read(std::ifstream& file)
{
    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<uint8_t> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<uint8_t>(file),
               std::istream_iterator<uint8_t>());

    return vec;
}

int main (int argc, const char* argv[])
{
	//std::vector<uint8_t> input;
	//std::vector<uint8_t> key;
	
	if (argc < 3)
	{
		fprintf(stderr,"usage: ./dec 'input' 'key'\n");
		return -1;
	}
	std::ifstream inp_file(argv[1]);
	std::ifstream key_file(argv[2]);

	if (!inp_file.is_open() || !key_file.is_open())
		return -1;

	std::vector<uint8_t> input = file_read(inp_file);
	input.resize((input.size() + 15) & ~0xF);
	std::vector<uint8_t> key = file_read(key_file);
	std::vector<uint8_t> output(16);

	for (int i = 0; i < input.size()/16; ++i)
	{
		AES128_ECB_decrypt(input.data() + i*16, key.data(), output.data());
		std::cout<<std::string(output.begin(),output.end())<<std::endl;
	}
	return 0;
}
