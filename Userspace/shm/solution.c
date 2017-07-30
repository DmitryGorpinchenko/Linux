#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1000
#define PERM_CRT 0666
#define PERM_ATT 0600

int main(int argc, char **argv)
{
    if (argc < 3) {
        return -1;
    }

    key_t k1, k2, k3;
    sscanf(argv[1], "%d", &k1);
    sscanf(argv[2], "%d", &k2);
    if ((k3 = ftok("./Makefile", 1)) < 0) {
        return -1;
    }
    printf("%d\n", k3);

    int i, shm1_id, shm2_id, shm3_id, *shm1, *shm2, *shm3;
    if ((shm3_id = shmget(k3, SHM_SIZE, IPC_CREAT | PERM_CRT)) < 0
    ||  (shm1_id = shmget(k1, SHM_SIZE, PERM_ATT)) < 0
    ||  (shm2_id = shmget(k2, SHM_SIZE, PERM_ATT)) < 0) {
        return -1;
    }
    if (((void *) (shm1 = shmat(shm1_id, NULL, 0))) == ((void *) -1)
    ||  ((void *) (shm2 = shmat(shm2_id, NULL, 0))) == ((void *) -1)
    ||  ((void *) (shm3 = shmat(shm3_id, NULL, 0))) == ((void *) -1)) {
        return -1;
    }

    for (i = 0; i < 100; i++) {
        shm3[i] = shm1[i] + shm2[i];
    }

    if (shmdt(shm1) < 0 || shmdt(shm2) < 0 || shmdt(shm3) < 0) {
        return -1;
    }
    return 0;
}

