#include <stdio.h>
#include <Windows.h>

#define TEXT_FILE_QUANTITY 5

typedef bool (*func)(const char*, const char*, unsigned int, unsigned int);	//тип функции для чтения/записи информации из файла

HMODULE dll;				//дескриптор для подключаемой библиотеки
CRITICAL_SECTION writeSec;  //критические секции
CRITICAL_SECTION delSec;

DWORD WINAPI threadWrite(LPVOID ptr)
{
	char path[] = "D:/Учеба/4 сем/сповм/lab5/lab5/store/result.txt";
	int pos = 0;
	int quantity = 0;

	func writeF = (func)GetProcAddress(dll, "writeFileFoo");  //получаем функцию из динамической библиотеки для дальнейшего вызова

	while (!TryEnterCriticalSection(&delSec))  //пока критическую секцию занимает другой поток
	{
		EnterCriticalSection(&writeSec);      //заходим в критическую секцию записи
		for (int i = 0; i != BUFSIZ && ((char*)ptr)[i] != '\0'; i++)
			quantity++;													//подсчет количества символов, которые нужно записать
		writeF(path, (const char*)ptr, pos, quantity);					//запись в файл
		pos += quantity;				//запоминаем позицию для записи след. файла
		quantity = 0;					//обнуляем количество
		LeaveCriticalSection(&writeSec);	//покидаем критическую секцию записи
		Sleep(70);
	}
	puts("Exit from thread");
	LeaveCriticalSection(&delSec);
	return 0;
}

int main()
{
	char path[] = "D:/Учеба/4 сем/сповм/lab5/lab5/store/0.txt";
	unsigned int count = 1;
	char buffer[BUFSIZ] = {};

	dll = LoadLibraryA("LibraryDLL.dll");				  // подключение динамической библиотеки
	func readF = (func)GetProcAddress(dll, "readFileFoo");  //получаем из нее функцию чтения файла

	InitializeCriticalSection(&writeSec);  //инициализация критических секций
	InitializeCriticalSection(&delSec);

	EnterCriticalSection(&writeSec);
	EnterCriticalSection(&delSec);

	CreateThread(NULL, NULL, threadWrite, buffer, NULL, NULL); //создание потока записи в файл

	bool flagFirst = true;

	for (; count <= TEXT_FILE_QUANTITY; count++)  //цикл прохода по всем файлам для чтения
	{
		if (flagFirst)  //чтение информации из первого файла
		{
			path[37]--;
			count--;
			flagFirst = false;
		}
		path[37]++;						//переход к следующему файлу
		readF(path, buffer, 0, 0);      //чтение информации из файла
		LeaveCriticalSection(&writeSec);
		Sleep(50);
		EnterCriticalSection(&writeSec);
		if (count + 1 > TEXT_FILE_QUANTITY)
			LeaveCriticalSection(&delSec);
		for (int i = 0; i != BUFSIZ && buffer[i] != '\0'; i++)
			buffer[i] = {};
		Sleep(100);
	}

	EnterCriticalSection(&delSec);

	DeleteCriticalSection(&writeSec);
	DeleteCriticalSection(&delSec);

	FreeLibrary(dll);   // îòêëþ÷àåì äëë

	puts("Exit from program");
	return 0;
}