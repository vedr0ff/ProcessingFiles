#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#include "utils_dump.h"

const StatData case_1_in_a[2] = {{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
                                 {.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode = 0}};

const StatData case_1_in_b[2] = {{.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode = 2},
                                 {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2}};

const StatData case_1_out[3] = {{.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2},
                                {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
                                {.id = 90089, .count = 14, .cost = 88.911, .primary = 0, .mode = 2}};

int system_safe(const char *cmd)
{
    int pid = 0;
    int stat = 0;
    if (cmd == NULL)
        return (1);

    if ((pid = vfork()) == 0) {
        int fdlimit = (int)sysconf(_SC_OPEN_MAX);
        for (int i = STDERR_FILENO + 1; i < fdlimit; i++)
            close(i);

        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
    }

    if (pid == -1){
        printf("errno comes from fork() %d", errno);
        stat = -1;
    }
    else{
        int status;
        if (waitpid(pid, &status, 0) == -1){
            printf("waitpid error");
            return 1;
        }

        if (WIFEXITED(status)){
            const int es = WEXITSTATUS(status);
            printf("exit status was %d\n", es);
            return es;
        }
    }
    return (stat);
}

int main(int argc, char **argv)
{   
    struct timespec ts;
    char buff[100];

    const char *file_in_1 = "./Processing_util/input_1";
    const char *file_in_2 = "./Processing_util/input_2";
    const char *file_out = "./Processing_util/output";
    
    timespec_get(&ts, TIME_UTC);
    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
    printf("Start dump store first file - time: %s.%09ld UTC\n\n", buff, ts.tv_nsec);
    StoreDump(file_in_1, case_1_in_a, (sizeof(case_1_in_a) / sizeof(case_1_in_a[0])));
    timespec_get(&ts, TIME_UTC);
    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
    printf("End dump store first file - time: %s.%09ld UTC\n\n", buff, ts.tv_nsec);

    timespec_get(&ts, TIME_UTC);
    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
    printf("Start dump store second file - time: %s.%09ld UTC\n\n", buff, ts.tv_nsec);
    StoreDump(file_in_2, case_1_in_b, (sizeof(case_1_in_b) / sizeof(case_1_in_b[0])));
    timespec_get(&ts, TIME_UTC);
    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
    printf("End dump store second file - time: %s.%09ld UTC\n\n", buff, ts.tv_nsec);

    timespec_get(&ts, TIME_UTC);
    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
    printf("Start processing util - time: %s.%09ld UTC\n\n", buff, ts.tv_nsec);
    system_safe("./Processing_util/Processing_util ./Processing_util/input_1 ./Processing_util/input_2 ./Processing_util/output");
    timespec_get(&ts, TIME_UTC);
    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
    printf("End processing util - time: %s.%09ld UTC\n\n", buff, ts.tv_nsec);

    StatData *res = (StatData *)malloc(1 * sizeof(StatData));
    int count = LoadDump(file_out, &res);
    printStatData(res, count);

    if (compare(res, count, case_1_out, (sizeof(case_1_out) / sizeof(case_1_out[0]))))
        printf("result processing util is the same as array - case_1_out \n");
    else
        fprintf(stderr, "%s", "result processing util is the not same as - case_1_out !\n");

    free(res);
}