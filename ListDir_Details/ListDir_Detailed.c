#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

void printFileType(mode_t mode);

int main(int argc, char *argv[]) {
    DIR *dirPtr;
    struct dirent *entryPtr;
    struct stat statBuf;
    char *path;

    path = (argc > 1) ? argv[1] : ".";
    dirPtr = opendir(path);
    if (dirPtr == NULL) {
        perror("Failed to open directory");
        return EXIT_FAILURE;
    }

    printf("Name----------------Inode--------------Type---UID---GID---User---------Group\n");
    printf("\n");

    while ((entryPtr = readdir(dirPtr)) != NULL) {
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entryPtr->d_name);

        if (stat(fullPath, &statBuf) == -1) {
            perror("Failed to get file stats");
            continue;
        }

        struct passwd *pw = getpwuid(statBuf.st_uid);
        struct group *gr = getgrgid(statBuf.st_gid);
        
        printf("%-20s %-10lu ", entryPtr->d_name, statBuf.st_ino);
        
        if (S_ISDIR(statBuf.st_mode)) 
            printf("Dir   ");
        else if (S_ISREG(statBuf.st_mode)) 
            printf("File  ");
        else 
            printf("Other ");

        printf("%-5d %-5d %-10s %-10s\n", 
               statBuf.st_uid, statBuf.st_gid, 
               pw ? pw->pw_name : "N/A", 
               gr ? gr->gr_name : "N/A");
    }

    closedir(dirPtr);
    return EXIT_SUCCESS;
}

