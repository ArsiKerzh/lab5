#include <iostream>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>

#define TEXT_FILE_QUANTITY 5

typedef bool (*foo)(const char*, const char*, int , int);

void* handle;
pthread_mutex_t writeMut;
pthread_mutex_t delMut;

void* threadWrite(void *ptr) {
    char path[] = "/home/antosa2972/lab5/files/result";
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
    char path[] = "/home/antosa2972/lab5/files/a";
    unsigned int count = 1;
    pthread_t thread;
    char buffer[BUFSIZ] = {};

    handle = dlopen("/home/antosa2972/lab5/libfsdyn.so", RTLD_LAZY);
    if(!handle){
	puts("ERROR So");
	getchar();
	return 1;
    }
    foo readF = (foo)dlsym(handle, "readFileFoo");
    if(!readF){
	puts("ERROR Func");
	getchar();
	return 1;
    }


    pthread_mutex_init(&writeMut, NULL);
    pthread_mutex_init(&delMut, NULL);
    pthread_mutex_lock(&writeMut);
    pthread_mutex_lock(&delMut);
    pthread_create(&thread, NULL, threadWrite, buffer);

    for (; count <= TEXT_FILE_QUANTITY; count++)
    {      
        readF(path, buffer,0,0);
        pthread_mutex_unlock(&writeMut);
        usleep(50000);
        pthread_mutex_lock(&writeMut);
        if (count + 1 > TEXT_FILE_QUANTITY)
            pthread_mutex_unlock(&delMut);
        for (int i = 0; i != BUFSIZ && buffer[i] != '\0'; i++)
            buffer[i] = {};
	path[28]++;
        usleep(100000);
    }

    pthread_mutex_lock(&delMut);

    pthread_mutex_destroy(&writeMut);
    pthread_mutex_destroy(&delMut);

    dlclose(handle);

    puts("Exit from program");
    return 0;
}
