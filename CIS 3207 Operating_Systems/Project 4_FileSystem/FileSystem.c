//actual program
#include "FileSystem.h"

// Maps drive
void map(){
	int i;

	// Creates file descriptor for 2MB drive
	if((fd=open("Drive2MB", O_RDWR)) == -1){
		fprintf(stderr, "error open: %s\n", strerror(errno));
		exit(0);
	}

  	// Maps 2MB drive for FAT Table
	if((fatTableMap=mmap(NULL, FAT_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "error fat mapping input: %s\n", strerror(errno));
		exit(0);
	}

	// Maps 2MB drive for data sector
	if((dataSectorMap=mmap(NULL, DATA_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 4096)) == MAP_FAILED) {
		fprintf(stderr, "error data mapping input: %s\n", strerror(errno));
		exit(0);
	}

	// Initialize FAT entries to -2
	// Unused = -2, EOF = -1, Next Cluster Exists >= 0
	for(i=0; i<FAT_SIZE; i++){
		fatTableMap[i] = -2;
	}

	// initialize data map
	for(i=0; i<DATA_SIZE; i++){
		dataSectorMap[i] = 0;
	}
}

// Unmaps drive
void unmap(){
	// Unmap FAT
    if (munmap(fatTableMap, FAT_SIZE) == -1) {
        close(fd);
        printf("Error mmap: %s\n", strerror(errno)); //error handling
		exit(0);
    }

    // Unmap data sector
	if (munmap(dataSectorMap, DATA_SIZE) == -1) {
        close(fd);
        printf("Error mmap: %s\n", strerror(errno)); //error handling
		exit(0);
    }

    // closes file descriptor
    close(fd);
}

// initialize a directory
void initDir(int index){
	int i, entrySize, start, end;
	struct dirEntry emptyEntry;

	// Initialize empty struct directory entries
	entrySize = sizeof(struct dirEntry);
	emptyEntry.name = "";
	emptyEntry.ext = "";
	emptyEntry.fatIndex = -1;
	emptyEntry.isDir = -1;
	emptyEntry.time = 0;

	// Fills a cluster with empty directory structs
	start = index * 512;
	end = start+CLUSTER_LENGTH;
	for(i=start; i<end; i=i+DIR_ENTRY_SIZE){
		memcpy(&dataSectorMap[i], &emptyEntry, entrySize);
	}
}

// initializes root
void initRoot(){
	int fat = 0;

	// Save First 2 FAT Entries for Root
	fatTableMap[0] = 1;
	fatTableMap[1] = -1;

	// Initialize first 2 clusters
	initDir(0);
	initDir(1);
}

// Finds first empty FAT entry
int findFreeFat(){
	int i;
	for(i=DATA_START; i<FAT_SIZE; i++){
		if(fatTableMap[i] == -2){
			return i;
		}
	}

	//if free block not found
	return -2;
}

// Finds free directory entry in a directory
int findFreeDir(int cluster){
	int i, end;
	struct dirEntry file;
	end = cluster + CLUSTER_LENGTH;

	for(i=cluster; i<end; i=i+DIR_ENTRY_SIZE){
		memcpy(&file, &dataSectorMap[i], sizeof(struct dirEntry));

		if(file.fatIndex == -1){
			return i;
		}
	}
	return -1;
}

// Finds FAT location of a specific directory
int searchDir(char *dirName, int start){
	int i, end, index, temp;
	struct dirEntry file;
	char *name;

	end = start + CLUSTER_LENGTH;
	if (start == 0){
		end = start + ROOT_BYTE_SIZE;
	}

	// Checks each directory entry
	for(i=start; i<end; i=i+DIR_ENTRY_SIZE){
		memcpy(&file, &dataSectorMap[i], sizeof(struct dirEntry));
		index = file.fatIndex;

		// For empty entries
		if(index <= 0){
			continue;
		}

		// If file is a directory
		if(file.isDir==1){
			name = file.name;

			// Returns FAT index if found
			if (strcmp(dirName, name)==0){
				return index;
			}

			// Searchs for clusters in FAT linked list
			temp=fatTableMap[index];
			while(temp!= -1){
				index = searchDir(dirName, temp*CLUSTER_LENGTH);
				if(index >= 0){
					return index;
				}
				temp = fatTableMap[temp];
			}

			// Recursively checks for subdirectory
			index = searchDir(dirName, index*CLUSTER_LENGTH);
			if(index >= 0){
				return index;
			}
		}
	}

	return -2;
}

// returns fat location for a file by name
int searchFile(char *fileName, int start){
	int i, end, index, temp;
	struct dirEntry file;
	char *name;

	end = start + CLUSTER_LENGTH;
	if (start == 0){
		end = start + ROOT_BYTE_SIZE;
	}

	// Checks each directory entry
	for(i=start; i<end; i=i+DIR_ENTRY_SIZE){

		memcpy(&file, &dataSectorMap[i], sizeof(struct dirEntry));
		index = file.fatIndex;
		name = file.name;

		// For empty entries
		if(index <= 0){
			continue;
		}

		// Returns FAT index if found
		if (strcmp(fileName, name)==0){
			if(file.isDir==0){
				return index;
			}else if (file.isDir==1){
				return -3;
			}
		}

		if(file.isDir != 1){
			continue;
		}

		// Searchs for clusters in FAT linked list
		temp=fatTableMap[index];
		while(temp!= -1){
			index = searchFile(fileName, temp*CLUSTER_LENGTH);
			if(index >= 0){
				return index;
			}
			temp = fatTableMap[temp];
		}

		// Recursively checks in subdirectory
		index = searchFile(fileName, index*CLUSTER_LENGTH);
		if(index >= 0){
			return index;
		}
	}

	return -2;
}

// returns file entry location given a its fat index
int searchFileEntry(int fatIndex, int start){
	int i, end, index, temp;
	struct dirEntry file;
	char *name;

	end = start + CLUSTER_LENGTH;
	if (start == 0){
		end = start + ROOT_BYTE_SIZE;
	}

	// Checks each directory entry
	for(i=start; i<end; i=i+DIR_ENTRY_SIZE){
		memcpy(&file, &dataSectorMap[i], sizeof(struct dirEntry));
		index = file.fatIndex;
		name = file.name;

		// For empty entries
		if(index <= 0){
			continue;
		}

		// Returns FAT index if found
		if (index ==fatIndex){
			if(file.isDir==0){
				return i;
			}else if (file.isDir==1){
				return -1;
			}
		}

		if(file.isDir != 1){
			continue;
		}

		// Searchs for clusters in FAT linked list
		temp=fatTableMap[index];
		while(temp!= -1){
			i = searchFileEntry(fatIndex, temp*CLUSTER_LENGTH);
			if(i >= 0){
				return i;
			}
			temp = fatTableMap[temp];
		}

		// Recursively checks in subdirectory
		i = searchFileEntry(fatIndex, index*CLUSTER_LENGTH);
		if(index >= 0){
			return index;
		}
	}

	return -1;
}

// Creates file in correct directory
void my_create(char *fileName, char *dir, char *ext, int isDir){
	int i, freeFat, dirIndex, fatIndex, dataIndex, cluster;
	struct dirEntry file;

	// Finds FAT Index of correct directory, default directory is root
	if((dirIndex=searchDir(fileName, ROOT_INDEX))== FAT_EOF){
		dirIndex = 0;
	}
	cluster = dirIndex * 512;

	// Find empty dir entry
	if((dataIndex=findFreeDir(cluster)) == -1) {

		if((freeFat=findFreeFat()) == -2){
			printf("Error: Out of FAT SPACE\n");
		}

		//Allocate more space for directory
		fatTableMap[dirIndex] = freeFat;
		fatTableMap[freeFat] = -1;

		initDir(freeFat);
		cluster = freeFat * 512;
		dataIndex=findFreeDir(cluster);
	}

	// find free fat index
	if((fatIndex=findFreeFat()) == -2){
		printf("Error: Out of FAT SPACE\n");
		exit(0);
	}

	// Initialize file entry
	file.name = fileName;
	file.ext = ext;
	file.fatIndex = fatIndex;
	file.isDir = isDir;
	file.time = time(0);
	fatTableMap[fatIndex] = -1;

	// add file to directory
	memcpy(&dataSectorMap[dataIndex], &file, sizeof(struct dirEntry));

	// if directory, initialize directory
	if(isDir == 1){
		initDir(fatIndex);
	}
}

// Finds file & creates file descriptor to first cluster
int my_open(char *fileName){
	int i, fat, index;

	// find fat index of file
	fat = searchFile(fileName, 0);
	if(fat == -2){
		printf("Error: no file found or is directory\n");
		exit(0);
	}

	// return int starting at cluster location
	return fat*CLUSTER_LENGTH;
}

// Closes file pointer to first cluster
void my_close(int* fd){
	// Closes pointer by setting int to -1
	*fd = -1;
}

// Remove a file's directory entry and intialize data in cluster and FAT
int my_delete(char *fileName){
	int i, fat, index, start, end, entrySize, temp, loc;
	struct dirEntry emptyEntry, file;

	// Initialize empty struct directory entries
	entrySize = sizeof(struct dirEntry);
	emptyEntry.name = "";
	emptyEntry.ext = "";
	emptyEntry.fatIndex = -1;
	emptyEntry.isDir = -1;
	emptyEntry.time = 0;

	fat = searchFile(fileName, 0);
	if(fat == -2){
		printf("Error: file not found\n");
		exit(0);
	}else if(fat == -3){
		printf("Error: can't delete directory unless it is empty\n");
	}


	// Remove directory entry
	loc = searchFileEntry(fat, 0);
	memcpy(&dataSectorMap[loc], &emptyEntry, entrySize);

	// Iterate over file in FAT table
	index = fatTableMap[fat];
	fatTableMap[fat] = -2;
	while(index != -1){

		// Zero out one cluster
		start = index*512;
		end = start + CLUSTER_LENGTH;
		for(i=start; i< end; i++){
			dataSectorMap[i] = 0;
		}

		temp = fatTableMap[index];
		fatTableMap[temp] = -2;
		index = temp;
	}
}

/* Read File
	Search for file, Go to FAT Table, read from data cluster, when at end, check if there
	is another data cluster in FAT Table
*/
void my_read(int fd, char *buffer, int count){
	int i, fat, start, end, data, dataIndex, flag;
	fat = fd/CLUSTER_LENGTH;

	// Decides if data fits into greater than one cluster
	if(count <= CLUSTER_LENGTH){
		end = fd+count;
		flag = 0;
	}else{
		end = fd+CLUSTER_LENGTH;
		flag = 1;
	}

	// Stores up to size n bytes into buffer
	for(i=fd; i<end; i++){
		*buffer = dataSectorMap[i];
		buffer++;
	}

	// If size is bigger than 1 cluster, recursively write to next cluster
	if(flag){
		dataIndex = dataSectorMap[fat*CLUSTER_LENGTH];

		// recursively writes to next cluster
		count = count - CLUSTER_LENGTH;
		my_read(dataIndex, buffer, count);
	}
}

/* Write File
	Search for file, Go to FAT Table, write to data cluster, when at end, find free
	another data cluster in FAT Table, write to new data cluster, update FAT
*/
void my_write(int fd, char *buffer, int size){
	int i, fat, freeFat, end, flag, dataIndex;
	fat = fd/CLUSTER_LENGTH;

	// Decides if data fits into greater than one cluster
	if(size <= CLUSTER_LENGTH){
		end = fd+size;
		flag = 0;
	}else{
		end = fd+CLUSTER_LENGTH;
		flag = 1;
	}
	// writes data to 1 cluster
	for(i=fd; i<end; i++){
		dataSectorMap[i] = *buffer;
		buffer++;
	}

	// If size is bigger than 1 cluster, recursively write to next cluster
	if(flag){
		// Determines if another cluster is already allocateds
		if(fatTableMap[fat] >= -1){
			dataIndex = dataSectorMap[fat*CLUSTER_LENGTH];
		}else{
			// allocates another cluster to file
			freeFat = findFreeFat();
			fatTableMap[fat] = freeFat;
			fatTableMap[freeFat] = -1;
			dataIndex = dataSectorMap[freeFat*CLUSTER_LENGTH];
		}

		// recursively writes to next cluster
		size = size - CLUSTER_LENGTH;
		my_write(dataIndex, buffer, size);
	}
}
