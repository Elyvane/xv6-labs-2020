#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

int pipeline(int id) {
    int fd[2];
    int rc = pipe(fd);
    if (rc != 0) {
        printf("prime guard %d, create pipe failed\n", id);
    }

    if (fork() == 0) {
        close(fd[1]);
        int current = 0;
        int fd_child = -1;
        while (read(fd[0], &current, 4) > 0) {
            if (current == id) {
                printf("prime %d\n", id);
            }
            if (current % id != 0) {
                if (fd_child < 0) {
                    fd_child = pipeline(current);
                }
                write(fd_child, &current, 4);
            }
        }
        close(fd[0]);
        if (fd_child > 0) {
            close(fd_child);
            wait(0);
        }

        exit(0);
    }

    close(fd[0]);
    return fd[1];
}

int main(int argc, char* argv[]) {
    int fd = pipeline(2);
    for (int n = 2; n <= 35; n++) {
        write(fd, &n, 4);
    }
    close(fd);

    wait(0);
    exit(0);
}
