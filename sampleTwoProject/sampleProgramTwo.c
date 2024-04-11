#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
	DIR *dirPtr;
	struct dirent *entryPtr;
	struct stat fileStat;

	dirPtr = opendir (".");

	while ((entryPtr = readdir (dirPtr)))
		if (stat(entryPtr->d_name, &fileStat) == 0) {
            printf("%-20s %ld bytes\n", entryPtr->d_name, fileStat.st_size);
        }

	closedir (dirPtr);
	return 0;
}