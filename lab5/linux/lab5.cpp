#include <iostream>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>

#define TEXT_FILE_QUANTITY 5

typedef bool (*foo)(const char*, const char*, unsigned int, unsigned int);

void* handle;
pthread_mutex_t writeMut;
pthread_mutex_t delMut;

void* threadWrite(void *ptr) {
    char path[] = "/home/darya/labs/result.txt";
    int pos = 0;
    int quantity = 0;

    foo writeF = (foo)dlsym(handle, "writeFileFoo");

    while (pthread_mutex_trylock(&delMut)) {
        pthread_mutex_lock(&writeMut);
        for (int i = 0; i != BUFSIZ && ((char*)ptr)[i] != '\0'; i++)
            quantity++;
        writeF(path, (const char*)ptr, pos, quantity);
        pos += quantity;
        quantity = 0;
        pthread_mutex_unlock(&writeMut);
        usleep(70000);
    }
    puts("Exit from thread");
    pthread_mutex_unlock(&delMut);
    return 0;
}

int main() {
    char path[] = "/home/darya/labs/0.txt";
    unsigned int count = 1;
    pthread_t thread;
    char buffer[BUFSIZ] = {};

    handle = dlopen("/home/darya/lab5lib.so", RTLD_LAZY);
    foo readF = (foo)dlsym(handle, "readFileFoo");

    pthread_mutex_init(&writeMut, NULL);
    pthread_mutex_init(&delMut, NULL);

    pthread_mutex_lock(&writeMut);
    pthread_mutex_lock(&delMut);

    pthread_create(&thread, NULL, threadWrite, buffer);


    bool flagFirst = true;

    for (; count <= TEXT_FILE_QUANTITY; count++)
    {
        if (flagFirst) {
            path[17]--;
            count--;
            flagFirst = false;
        }
        path[17]++;
        readF(path, buffer, 0, 0);
        pthread_mutex_unlock(&writeMut);
        usleep(50000);
        pthread_mutex_lock(&writeMut);
        if (count + 1 > TEXT_FILE_QUANTITY)
            pthread_mutex_unlock(&delMut);
        for (int i = 0; i != BUFSIZ && buffer[i] != '\0'; i++)
            buffer[i] = {};
        usleep(100000);
    }

    pthread_mutex_lock(&delMut);

    pthread_mutex_destroy(&writeMut);
    pthread_mutex_destroy(&delMut);

    dlclose(handle);

    puts("Exit from program");
    return 0;
}