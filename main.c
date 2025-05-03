#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 1000

int input[MAX];
int result[MAX];
int n;

typedef struct {
    int start;
    int end;
} Range;

void bubble(int* arr, int s, int e) {
    for (int i = s; i <= e; ++i) {
        for (int j = s; j < e - (i - s); ++j) {
            if (arr[j] > arr[j+1]) {
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

void* sort_part(void* arg) {
    Range* r = (Range*) arg;
    bubble(input, r->start, r->end);
    pthread_exit(0);
}

void* do_merge(void* arg) {
    int mid = (n + 1) / 2;
    int i = 0;
    int j = mid;
    int idx = 0;

    while (i < mid && j < n) {
        if (input[i] < input[j])
            result[idx++] = input[i++];
        else
            result[idx++] = input[j++];
    }

    while (i < mid)
        result[idx++] = input[i++];

    while (j < n)
        result[idx++] = input[j++];

    pthread_exit(0);
}

int main() {
    printf("How many numbers? (max %d): ", MAX);
    scanf("%d", &n);

    if (n < 2 || n > MAX) {
        printf("Invalid input size.\n");
        return 1;
    }

    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; ++i)
        scanf("%d", &input[i]);

    pthread_t tid1, tid2, tid_merge;
    Range* part1 = malloc(sizeof(Range));
    Range* part2 = malloc(sizeof(Range));

    int mid = (n + 1) / 2;

    part1->start = 0;
    part1->end = mid - 1;

    part2->start = mid;
    part2->end = n - 1;

    pthread_create(&tid1, NULL, sort_part, part1);
    pthread_create(&tid2, NULL, sort_part, part2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_create(&tid_merge, NULL, do_merge, NULL);
    pthread_join(tid_merge, NULL);

    printf("Sorted list:\n");
    for (int i = 0; i < n; ++i)
        printf("%d ", result[i]);
    printf("\n");

    free(part1);
    free(part2);

    return 0;
}



