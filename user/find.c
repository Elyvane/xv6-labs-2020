#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

char* get_file_name(char* path) {
    char* p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    return p;
}

void find(char* path, char* target_name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_FILE:
            if (strcmp(get_file_name(path), target_name) == 0) {
                printf("%s\n", path);
            }

            break;

        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0)
                    continue;
                if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                    printf("find: path too long\n");
                    break;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;

                if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0)
                    continue;

                find(buf, target_name);
            }
            break;
    }
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: find <path> <target_name>\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}
