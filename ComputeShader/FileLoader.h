#pragma once

#include <fstream>
#include <string>

static const std::string RESOURCE_PATH = "../Resources/";

static std::string LoadFile(std::string filename)
{
	std::ifstream fin;
	int fileSize;
	char input;
	char* buffer;

	// Open the shader source file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return 0;
	}

	// Initialize the size of the file.
	fileSize = 0;

	// Read the first element of the file.
	fin.get(input);

	// Count the number of elements in the text file.
	while (!fin.eof())
	{
		fileSize++;
		fin.get(input);
	}

	// Close the file for now.
	fin.close();

	// Initialize the buffer to read the shader source file into.
	buffer = new char[fileSize + 1];
	if (!buffer)
	{
		return 0;
	}

	// Open the shader source file again.
	fin.open(filename);

	// Read the shader text file into the buffer as a block.
	fin.read(buffer, fileSize);

	// Close the file.
	fin.close();

	// Null terminate the buffer.
	buffer[fileSize] = '\0';

	std::string output(buffer);

	delete buffer;

	return output;
}