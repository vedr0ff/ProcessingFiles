#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "utils_dump.h"

/*!
\brief Сохранение данных в файл
\param[in] file_name имя файла для сохранения
\param[in] sd данные для сохранения
\param[in] size размер данных для сохранения
\return
*/

void StoreDump(const char *file_name, const StatData *sd, int size)
{
    FILE *file = fopen(file_name, "wb");

    if (file == NULL)
    {
        fclose(file);
        return;
    }

    for (size_t i = 0; i < size; i++)
    {
        fwrite(&sd[i], sizeof(struct StatData), 1, file);
    }

    fclose(file);
    return;
}

/*!
\brief Чтение данных из файла
\param[in] file_name имя файла для чтения
\param[in] sd место для сохранения данных
\param[in] size размер прочтённых данных
\return
*/

int LoadDump(const char *file_name, StatData **res)
{
    int count = 0;

    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        printf("Don't open file - %s", file_name);
        return count;
    }

    int fsize = 0;
    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    *res = (StatData *)malloc(fsize + sizeof(StatData));

    while (fread((*res + count), sizeof(struct StatData), 1, file))
        count++;

    fclose(file);
    return count;
}

/*!
\brief Поиск элемента в массиве
\param[in] arr массив в котором осуществляем поиск
\param[in] n размер массива в котором осуществляем поиск
\param[in] key что ищем
\return
*/

int findElement(int arr[], int n, int key)
{
    int i;
    for (i = 0; i < n; i++)
        if (arr[i] == key)
            return 1;

    return 0;
}

/*!
\brief Объединение массивов данных
\param[in] sd1 первый массив данных
\param[in] size1 размер первого массива данных
\param[in] sd2 второй массив данных
\param[in] size2 размер второго массива данных
\param[in] size_res размер объединённых данных
\return объединённые данные
*/

StatData *JoinDump(StatData *sd1, int size1, StatData *sd2, int size2, int *size_res)
{
    int count_indexes = 0;
    int indexes[size2];

    for (size_t i = 0; i < size1; i++)
    {
        for (size_t t = 0; t < size2; t++)
        {
            if (sd1[i].id == sd2[t].id)
            {
                indexes[count_indexes] = t;
                count_indexes++;

                sd1[i].count = sd1[i].count + sd2[t].count;
                sd1[i].cost = sd1[i].cost + sd2[t].cost;
                if ((sd1[i].primary == 0) || (sd2[t].primary == 0))
                    sd1[i].primary = 0;
                sd1[i].mode = MAX(sd1[i].mode, sd2[t].mode);
            }
        }
    }

    StatData *res = (StatData *)malloc((size1 + (size2 - count_indexes)) * sizeof(StatData));

    for (size_t i = 0; i < size1; i++)
        res[i] = sd1[i];

    int count_sd2 = 0;
    for (size_t i = 0; i < size2; i++)
    {
        if (!findElement(indexes, count_indexes, i))
        {
            res[size1 + count_sd2] = sd2[i];
            count_sd2++;
        }
    }

    *size_res = size1 + count_sd2;
    return res;
}

/*!
\brief компаратор для сортировки по полю cost в структуре StatData
\param[in] a первая структура StatData для сравнения
\param[in] b вторая структура StatData для сравнения
\return результат сравнения
*/

int comp(const void *a, const void *b)
{
    return ((struct StatData *)a)->cost - ((struct StatData *)b)->cost;
}

/*!
\brief Сортировка массива структур StatData
\param[in] sd массив структур StatData
\param[in] size размер массива структур StatData
*/

void SortDump(StatData *sd, int size)
{
    qsort(sd, size, sizeof(struct StatData), comp);
}

/*!
\brief Сравнение двух массивов структур StatData
\param[in] sd1 первый массив структур StatData
\param[in] size1 размер первого массива структур StatData
\param[in] sd2 второй массив структур StatData
\param[in] size2 размер второго массива структур StatData
\return результат сравнения
*/

int compare(const StatData *sd1, int size1, const StatData *sd2, int size2)
{
    if (size1 != size2)
        return 0;

    for (size_t i = 0; i < size1; i++)
    {
        if (sd1[i].id != sd2[i].id)
            return 0;
        if (sd1[i].count != sd2[i].count)
            return 0;
        if (fabs(sd1[i].cost - sd2[i].cost) > 0.0001)
            return 0;
        if (sd1[i].primary != sd2[i].primary)
            return 0;
        if (sd1[i].mode != sd2[i].mode)
            return 0;
    }

    return 1;
}

/*!
\brief Вывод массива структур StatData
\param[in] sd массив структур StatData
\param[in] size размер массива структур StatData
*/
void printStatData(StatData *res, int size)
{
    printf("%-13s%-13s%-13s%-13s%-13s\n", "id", "count", "cost", "primary", "mode");
    for (size_t i = 0; i < size; i++)
    {
        printf("%#-10lx - %-10d - %-10.3e - %-10c - ", (res + i)->id, (res + i)->count, (res + i)->cost, ((res + i)->primary == 1) ? 'y' : 'n');
        int n = (res + i)->mode;
        if(n)
        {
            while (n) {
                if (n & 1)
                    printf("1");
                else
                    printf("0");

                n >>= 1;
            }
        }
        else
            printf("0");
        printf("\n");
    }
    printf("\n");
}
