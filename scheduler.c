// clang -o scheduler.bin -Wall -Wpedantic scheduler.c
// ./scheduler.bin

#include <stdio.h>

typedef struct {
    int pid;            // プロセスID
    int priority;       // 優先度
    int remaining_time; // 実行に必要な残り時間
} Process;

// 戻り値は、次にスケジュールするタスクのpid
// スケジュールするタスクがない場合は0を返す
int schedule(Process *procs, int len_proc) {
    int pid = 0;

    int max_priority = -1;
    int min_rem_time;
    for (int i = 0; i < len_proc; i++) {
        if (procs[i].priority > max_priority && procs[i].remaining_time != 0) {
            pid = procs[i].pid;
            max_priority = procs[i].priority;
            min_rem_time = procs[i].remaining_time;
        } else if (procs[i].priority == max_priority &&
                   procs[i].remaining_time != 0) {
            if (procs[i].remaining_time < min_rem_time) {
                pid = procs[i].pid;
                min_rem_time = procs[i].remaining_time;
            }
        }
    }

    return pid;
}

struct Test {
    char *title;

    Process procs[64];
    int nprocs;

    int expected[64];
    int nexpected;
};

static int test(struct Test t) {
    int pid, time = 0;

    while (1) {
        pid = schedule(t.procs, t.nprocs);

        if (t.expected[time] != pid) {
            fprintf(stderr, "Expected pid is %d, but actually %d.\n",
                    t.expected[time], pid);
            return 1;
        }

        time++;

        if (pid == 0)
            break;

        for (int i = 0; i < t.nprocs; i++) {
            if (t.procs[i].pid == pid) {
                t.procs[i].remaining_time--;
                break;
            }
        }
    };

    return 0;
}

#define ARR_SIZE(a) (int)(sizeof(a) / sizeof(a[0]))

static void do_tests(void) {
    struct Test tests[] = {
        {
            .title = "Single process",
            .procs = {{1, 1, 2}},
            .nprocs = 1,
            .expected = {1, 1, 0},
            .nexpected = 2,
        },
        {
            .title = "Different priority",
            .procs = {{1, 1, 2}, {2, 2, 2}},
            .nprocs = 2,
            .expected = {2, 2, 1, 1, 0},
            .nexpected = 5,
        },
        {
            .title = "Same priority but different remaining time",
            .procs = {{1, 1, 2}, {2, 1, 3}},
            .nprocs = 2,
            .expected = {1, 1, 2, 2, 2, 0},
            .nexpected = 6,
        },
        {
            .title = "No remaining time",
            .procs = {{1, 2, 0}},
            .nprocs = 1,
            .expected = {0},
            .nexpected = 1,
        },
        {
            .title = "Iroiro",
            .procs = {{1, 2, 5}, {2, 1, 3}, {3, 3, 2}, {4, 2, 4}, {5, 1, 1}},
            .nprocs = 5,
            .expected = {3, 3, 4, 4, 4, 4, 1, 1, 1, 1, 1, 5, 2, 2, 2, 0},
            .nexpected = 16,
        },
    };

    for (int i = 0; i < ARR_SIZE(tests); i++) {
        struct Test t = tests[i];
        printf("%s... ", t.title);
        int res = test(t);
        printf("%s\n", res == 0 ? "OK" : "NG");
    }
}

int main(void) {
    do_tests();
    return 0;
}
