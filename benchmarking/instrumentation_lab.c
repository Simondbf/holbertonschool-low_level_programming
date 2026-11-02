#include <stdio.h>
#include <time.h>

#define DATASET_SIZE 50000
#define SEED_VALUE 42u

static int dataset[DATASET_SIZE];

static unsigned int next_value(unsigned int *state)
{
    *state = (*state * 1103515245u) + 12345u;
    return *state;
}

static void build_dataset(void)
{
    unsigned int state;
    int i;

    state = SEED_VALUE;

    for (i = 0; i < DATASET_SIZE; i++)
        dataset[i] = (int)(next_value(&state) % 100000);
}

static void process_dataset(void)
{
    int i;
    int v;

    for (i = 0; i < DATASET_SIZE; i++)
    {
        v = dataset[i];
        v = (v * 3) + (v / 7) - (v % 11);
        if (v < 0)
            v = -v;
        dataset[i] = v;
    }
}

static unsigned long reduce_checksum(void)
{
    unsigned long sum;
    int i;

    sum = 0;
    for (i = 0; i < DATASET_SIZE; i++)
        sum = (sum * 131ul) + (unsigned long)dataset[i];

    return sum;
}

int main(void)
{
    unsigned long checksum;

	float start = clock();

	float start_build = clock();
	build_dataset();
	float end_build = clock();

	float start_process = clock();
    process_dataset();
	float end_process = clock();

	float start_reduce = clock();
	checksum = reduce_checksum();
	float end_reduce = clock();

	float end = clock();

    if (checksum == 0ul)
        printf("impossible\n");

	printf("TOTAL seconds: %.6f \n", (float)(end - start) / (float)CLOCKS_PER_SEC);
	printf("BUILD_DATA seconds: %.6f \n", (float)(end_build - start_build) / (float)CLOCKS_PER_SEC);
	printf("PROCESS seconds: %.6f \n", (float)(end_process - start_process) / (float)CLOCKS_PER_SEC);
	printf("REDUCE seconds: %.6f \n", (float)(end_reduce - start_reduce) / (float)CLOCKS_PER_SEC);

    return 0;
}