#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/stat.h>	
#include <unistd.h>	
#include <string.h>
#include <errno.h>
#include <dirent.h>



int main(){
char *str = "String from file";
	char buffer;
	int fd = open("output.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	write(fd, str, strlen(str));
	off_t new_pos = lseek(fd, 0, SEEK_END);
	for(int i = new_pos - 1; i >= 0; i--){
		lseek(fd, i, SEEK_SET);
		read(fd, &buffer, 1);
		write(1, &buffer, 1);
	}
	close(fd);
}

