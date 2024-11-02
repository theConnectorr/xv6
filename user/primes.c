#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    const int N = 280;
    int i, j, prime = -1, pid = -1;
    int primes[N];
    int p[2];

    i = 0;
    while (i < N) {
        primes[i] = i + 2;
        i++;
    }

    for (i = 2; i < N; i++) {
        for (j = i; j < N; j++) {
            if (primes[j - 1] != 0) {
                if (j == i) {
                    prime = j;
                    primes[j - 1] = prime;
                    pid = fork();
                    pipe(p);
                }
                else if (j > i && (j % i) == 0) {
                    primes[j - 1] = 0;
                }
                else if (j > i && (j % i) != 0) {
                    primes[j - 1] = j + 1;
                }
            }
        }

        if (p[0] > 0 && p[1] > 0 && pid >= 0) {
            if (pid > 0) {
                write(p[0], primes, sizeof primes);
                pid = wait(0);
            }
            else if (pid == 0) {
                read(p[1], primes, sizeof primes);
                printf("prime %d\n", prime);
                exit(0);
            }
            close(p[0]);
            close(p[1]);
        }
    }

    exit(0);
    return 0;
}