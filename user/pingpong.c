#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int p1[2], p2[2];

    int rc = pipe(p1);
    if (rc != 0) {
        fprintf(2, "Pingpong: create pipe failed\n");
    }
    rc = pipe(p2);
    if (rc != 0) {
        fprintf(2, "Pingpong: create pipe failed\n");
    }

    if (fork() == 0) {
        close(p1[1]);
        close(p2[0]);
        // child
        char buf[1];
        int pid = getpid();
        int cnt = read(p1[0], buf, 1);
        if (cnt != 1) {
            fprintf(2, "%d: Read failed\n", pid);
            exit(1);
        }

        if (buf[0] == '*') {
            fprintf(2, "%d: received ping\n", pid);
        }

        char c = '#';
        write(p2[1], &c, 1);
        close(p1[0]);
        close(p2[1]);
        exit(0);
    } else {
        close(p1[0]);
        close(p2[1]);
        // parent
        char c = '*';
        write(p1[1], &c, 1);

        char buf[1];
        int pid = getpid();
        int cnt = read(p2[0], buf, 1);
        if (cnt != 1) {
            fprintf(2, "%d: Read failed\n", pid);
            exit(1);
        }

        if (buf[0] == '#') {
            fprintf(2, "%d: received pong\n", pid);
        }
        close(p1[1]);
        close(p2[0]);

        exit(0);
    }

    exit(0);
}
