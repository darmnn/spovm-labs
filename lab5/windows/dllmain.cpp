#include "pch.h"
#include <iostream>
#include <Windows.h>

bool readFileFoo(const char* path, const char* buffer, unsigned int from, unsigned int to)
{
	OVERLAPPED structure;
	structure.Offset = NULL;
	structure.OffsetHigh = NULL;
	structure.hEvent = NULL;
	std::cout << "I am read dll function" << std::endl;
	HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	ReadFile(file, (LPVOID)buffer, BUFSIZ, NULL, &structure);
	std::cout << "Wait for reading from file..." << std::endl;
	WaitForSingleObject(file, INFINITE);			
	CloseHandle(file);
	return true;
}

bool writeFileFoo(const char* path, const char* buffer, unsigned int from, unsigned int howMany)
{
	OVERLAPPED structure;
	structure.Offset = from;
	structure.OffsetHigh = NULL;
	structure.hEvent = NULL;
	std::cout << "I am write dll function" << std::endl;
	HANDLE file = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	WriteFile(file, buffer, howMany, NULL, &structure);
	std::cout << "Wait for writing to the result file..." << std::endl;
	WaitForSingleObject(file, INFINITE);
	CloseHandle(file);
	return true;
}
