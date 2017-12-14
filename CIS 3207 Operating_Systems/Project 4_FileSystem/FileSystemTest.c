//Test program

#include "FileSystem.h"

int main(int argc, char **argv){
	int fd, size;
	char *str, str2[10];
	str = "Helloooooo World"; //string to be written to file
	size = strlen(str); //string size

	// Mapp drive
	map();

	// initialize root diretory
	initRoot();

	// Create directory
	my_create("dir1", "root", ".txt", 1);

	// Create File in directory
	my_create("file1", "dir1", ".txt", 0);

	// open file
	fd = my_open("file1");

	// Write to file
	my_write(fd, str, size);

	// close
	close(fd);

	// open file
	fd = my_open("file1");

	// Read file
	my_read(fd, str2, size);

	// Print read string
	printf("Read String: %s\n", str2);

	// Unmaps 2MB drive
	unmap();
}
