// pch.cpp: файл исходного кода, соответствующий предварительно скомпилированному заголовочному файлу

#include "pch.h"

//EXPORT DWORD WINAPI ReadFileDLL(void* _parameters)
//{
//	CritSec _section = *(CritSec*)_parameters;
//
//	filesystem::path _filesPath(filesystem::current_path() / "TextFiles");
//	filesystem::create_directories(_filesPath);
//
//	char _buf[SIZEBUF] = { 0 };
//	int _readBytes = 0;
//
//	OVERLAPPED _overlapped;
//	_overlapped.hEvent = 0;
//	_overlapped.Offset = 0;
//	_overlapped.OffsetHigh = 0;
//
//	for (filesystem::directory_entry i : filesystem::directory_iterator(_filesPath)) {
//	
//		LeaveCriticalSection(_section.addSec);
//		Sleep(50);
//		EnterCriticalSection(_section.addSec);
//
//		HANDLE _curFile = CreateFileW(i.path().c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
//		ReadFile(_curFile, _buf, SIZEBUF, (LPDWORD)&_readBytes, &_overlapped);
//		//WaitForSingleObject(_curFile, INFINITE);
//		CloseHandle(_curFile);
//		_buf[_overlapped.InternalHigh] = 0;
//		WriteFile(*_section.buf, _buf, strlen(_buf), NULL, NULL);
//
//		//Sleep(100);
//	}
//	CloseHandle(_overlapped.hEvent);
//	return 0;
//}
//
//EXPORT DWORD WINAPI WriteFileDLL(void* _parameters)
//{
//	CritSec _section = *(CritSec*)_parameters;
//	HANDLE _outputFile = CreateFileA("output.txt", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
//	char _buf[SIZEBUF] = { 0 };
//	int _readBytes = 0;
//
//	OVERLAPPED _overlapped;
//	_overlapped.hEvent = 0;
//	_overlapped.Offset = 0;
//	_overlapped.OffsetHigh = 0;
//
//	while (!TryEnterCriticalSection(_section.delSec)) {
//
//		EnterCriticalSection(_section.addSec);
//		ReadFile(*_section.buf, _buf, SIZEBUF, (LPDWORD)&_readBytes, NULL);
//		_buf[_readBytes] = 0;
//		WriteFile(_outputFile, _buf, strlen(_buf), NULL, &_overlapped);
//		_overlapped.Offset += strlen(_buf);
//		//WaitForSingleObject(_outputFile, INFINITE);
//		LeaveCriticalSection(_section.addSec);
//		//Sleep(100);
//	}
//	LeaveCriticalSection(_section.delSec);
//	CloseHandle(_outputFile);
//	return 0;
//}
// При использовании предварительно скомпилированных заголовочных файлов необходим следующий файл исходного кода для выполнения сборки.


//EXPORT DWORD WINAPI ReadFileDLL(void* _parameters)
//{
//	tempSection _section = *(tempSection*)_parameters;
//
//	filesystem::path _filesPath(filesystem::current_path() / "TextFiles");
//	filesystem::create_directories(_filesPath);
//
//	char _buf[SIZEBUF] = { 0 };
//	int _readBytes = 0;
//
//	OVERLAPPED _overlapped;
//	_overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, (LPWSTR)"_event");
//	_overlapped.Offset = 0;
//	_overlapped.OffsetHigh = 0;
//
//	cout << "I am read dll function" << endl;
//
//	for (filesystem::directory_entry i : filesystem::directory_iterator(_filesPath))
//	{
//		HANDLE _curFile = CreateFileW(i.path().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
//		ReadFile(_curFile, _buf, SIZEBUF, (LPDWORD)&_readBytes, &_overlapped);
//		WaitForSingleObject(_overlapped.hEvent, INFINITE);
//		CloseHandle(_curFile);
//		_buf[_overlapped.InternalHigh] = 0;
//		WriteFile(*_section.pipe, _buf, strlen(_buf), NULL, NULL);
//		Sleep(100);
//		PulseEvent(*_section.event);
//		WaitForSingleObject(*_section.event, INFINITE);
//	}
//	CloseHandle(_overlapped.hEvent);
//	cout << "Wait for reading from file..." << endl;
//	return 0;
//}
//
//EXPORT DWORD WINAPI WriteFileDLL(void* _parameters)
//{
//	tempSection _section = *(tempSection*)_parameters;
//	HANDLE _outputFile = CreateFileA("output.txt", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
//	char _buf[SIZEBUF] = { 0 };
//	int _readBytes = 0;
//	OVERLAPPED _overlapped;
//	_overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, (LPWSTR)"_event");
//	_overlapped.Offset = 0;
//	_overlapped.OffsetHigh = 0;
//	while (true) {
//		WaitForSingleObject(*_section.event, INFINITE);
//		ReadFile(*_section.pipe, _buf, SIZEBUF, (LPDWORD)&_readBytes, NULL);
//		_buf[_readBytes] = 0;
//		WriteFile(_outputFile, _buf, strlen(_buf), NULL, &_overlapped);
//		_overlapped.Offset += strlen(_buf);
//		WaitForSingleObject(_overlapped.hEvent, INFINITE);
//		Sleep(100);
//		PulseEvent(*_section.event);
//	}
//	CloseHandle(_outputFile);
//	return 0;
//}

//EXPORT DWORD WINAPI ReaderFileFunc(void* _parameters)
//{
//	tempPipe _pipe = *(tempPipe*)_parameters;
//
//	filesystem::path _filesPath(filesystem::current_path() / "TextFiles");
//	filesystem::create_directories(_filesPath);
//
//	char _buf[SIZEBUF] = { 0 };
//	int _readBytes = 0;
//
//	OVERLAPPED _overlapped;
//	_overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, "_event");
//	_overlapped.Offset = 0;
//	_overlapped.OffsetHigh = 0;
//
//	for (filesystem::directory_entry i : filesystem::directory_iterator(_filesPath)) {
//		HANDLE _curFile = CreateFileW(i.path().c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
//		ReadFile(_curFile, _buf, SIZEBUF, (LPDWORD)&_readBytes, &_overlapped);
//
//		WaitForSingleObject(_overlapped.hEvent, INFINITE);
//		CloseHandle(_curFile);
//		_buf[_overlapped.InternalHigh] = 0;
//		WriteFile(*_pipe.pipe, _buf, strlen(_buf), NULL, NULL);
//
//		Sleep(100);
//		PulseEvent(*_pipe.event);
//		WaitForSingleObject(*_pipe.event, INFINITE);
//	}
//	CloseHandle(_overlapped.hEvent);
//	return 0;
//}
//
//EXPORT DWORD WINAPI WriterFileFunc(void* _parameters)
//{
//	tempPipe _pipe = *(tempPipe*)_parameters;
//	HANDLE _outputFile = CreateFileA("output.txt", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
//	char _buf[SIZEBUF] = { 0 };
//	int _readBytes = 0;
//	OVERLAPPED _overlapped;
//	_overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, "_event");
//	_overlapped.Offset = 0;
//	_overlapped.OffsetHigh = 0;
//
//	while (true) {
//		WaitForSingleObject(*_pipe.event, INFINITE);
//		ReadFile(*_pipe.pipe, _buf, SIZEBUF, (LPDWORD)&_readBytes, NULL);
//		_buf[_readBytes] = 0;
//		WriteFile(_outputFile, _buf, strlen(_buf), NULL, &_overlapped);
//		_overlapped.Offset += strlen(_buf);
//		WaitForSingleObject(_overlapped.hEvent, INFINITE);
//		Sleep(100);
//		PulseEvent(*_pipe.event);
//	}
//	CloseHandle(_outputFile);
//	return 0;
//}