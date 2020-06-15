#include <aio.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

extern "C" bool readFileFoo(const char* path, const char* buffer, unsigned int from, unsigned int to)
{
    aiocb structure;
    memset(&structure,0,sizeof(structure));
    structure.aio_fildes = open(path,O_RDWR, S_IRUSR | S_IWUSR);
    structure.aio_offset = from;
    structure.aio_buf = (void*)buffer;
    structure.aio_nbytes = BUFSIZ;
    structure.aio_sigevent.sigev_notify = SIGEV_NONE;
    //printf("Success: %d", aio_read(&structure));
    //aio_read(&structure);
    aio_read(&structure);
    while (aio_error(&structure))
    {
        puts("Wait for reading from file...");
	if(aio_error(&structure)==EINVAL)
	{
		puts("\noh no");
		getchar();
	}
        usleep(500000);
    }
    close(structure.aio_fildes);
    return true;
}

extern "C" bool writeFileFoo(const char* path, const char* buffer, unsigned int from, unsigned int howMany)
{
    aiocb structure;
    memset(&structure,0,sizeof(structure));
    puts("I am write dll function");
    structure.aio_fildes = open(path, O_RDWR, S_IRUSR | S_IWUSR);
    structure.aio_offset = from;
    structure.aio_buf = (void*)buffer;
    structure.aio_nbytes = howMany;
    structure.aio_sigevent.sigev_notify = SIGEV_NONE;
    aio_write(&structure);
    while (aio_error(&structure) == EINPROGRESS)
    {
        puts("Wait for writing to the result file...");
        usleep(500000);
	if(aio_error(&structure)==EINVAL)
	{
		puts("oh no");
	}
    }
    close(structure.aio_fildes);
    return true;
}
