#include <stdio.h>

#include "utils_dump.h"

void main(int argc, char **argv)
{
    StatData *res1 = (StatData *)malloc(1 * sizeof(StatData));
    int count1 = LoadDump(argv[1], &res1);
    printStatData(res1, count1);

    StatData *res2 = (StatData *)malloc(1 * sizeof(StatData));
    int count2 = LoadDump(argv[2], &res2);
    printStatData(res2, count2);

    int size;
    StatData *out = JoinDump(res1, count1, res2, count2, &size);
    free(res1);
    free(res2);
    SortDump(out, size);

    printStatData(out, size);

    StoreDump(argv[3], out, size);
    free(out);

    return;
}