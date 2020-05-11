#include <aio.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

extern "C" bool readFileFoo(const char* path, const char* buffer, unsigned int from, unsigned int to)
{
    aiocb structure;
    puts("I am read dll function");
    structure.aio_fildes = open(path, O_RDONLY);
    structure.aio_offset = from;
    structure.aio_buf = (void*)buffer;
    structure.aio_nbytes = BUFSIZ;
    structure.aio_sigevent.sigev_notify = SIGEV_NONE;
    aio_read(&structure);
    while (aio_error(&structure))
    {
        puts("Wait for reading from file...");
        sleep(1);
    }
    close(structure.aio_fildes);
    return true;
}

extern "C" bool writeFileFoo(const char* path, const char* buffer, unsigned int from, unsigned int howMany)
{
    aiocb structure;
    puts("I am write dll function");
    structure.aio_fildes = open(path, O_WRONLY);
    structure.aio_offset = from;
    structure.aio_buf = (void*)buffer;
    structure.aio_nbytes = howMany;
    structure.aio_sigevent.sigev_notify = SIGEV_NONE;
    aio_write(&structure);
    while (aio_error(&structure))
    {
        puts("Wait for writing to the result file...");
        sleep(1);
    }
    close(structure.aio_fildes);
    return true;
}