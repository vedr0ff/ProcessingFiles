#ifndef UTILS_DUMP
#define UTILS_DUMP

#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? a : b)

typedef struct StatData
{
    long id;
    int count;
    float cost;
    unsigned int primary : 1;
    unsigned int mode : 3;
} StatData;

int system_safe(const char *cmd);
void StoreDump(const char *file_name, const StatData *sd, int size);
int LoadDump(const char *file_name, StatData **res);
int findElement(int arr[], int n, int key);
StatData *JoinDump(StatData *sd1, int size1, StatData *sd2, int size2, int *size_res);
void SortDump(StatData *sd, int size);
int compare(const StatData *sd1, int size1, const StatData *sd2, int size2);
void printStatData(StatData *res, int size);

#endif //UTILS_DUMP