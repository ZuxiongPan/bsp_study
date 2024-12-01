#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

const char* fileName = "/home/text.txt";
const char* strToWrite = "Hello Linux";

int main(int argc, const char** argv, const char** envp)
{
	int bytes = 0;
	int fd = open("/home/a.txt", O_CREAT | O_RDWR, 0664);
	
	if(fd == -1)
	{
		printf("open %s faied\n", fileName);
		return -1;
	}
	
	bytes = write(fd, strToWrite, strlen(strToWrite));
	if(bytes == -1)
	{
		printf("write to file %s failed\n", fileName);
		close(fd);
		return -1;
	}
	
	printf("write to file %s success\n", fileName);
	close(fd);

	return 0;
}
