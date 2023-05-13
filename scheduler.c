// clang -o scheduler.bin -Wall -Wpedantic scheduler.c
// ./scheduler.bin

#include <stdio.h>

#define NUM_PROCESSES 5
#define NUM_SEQ 16

typedef struct {
    int pid;            // プロセスID
    int priority;       // 優先度
    int remaining_time; // 実行に必要な残り時間
} Process;

// 戻り値は、次にスケジュールするタスクのpid
// スケジュールするタスクがない場合は0を返す
int schedule(Process *proc, int len_proc) {
    // ここに実装を書いてください
    return 0;
}

int test(Process *proc, int len_proc, int *expected, int len_expected) {
    int pid, time = 0;

    while (1) {
        pid = schedule(proc, len_proc);

        if (expected[time] != pid) {
            fprintf(stderr, "Expected pid is %d, but actually %d.\n",
                    expected[time], pid);
            return 1;
        }

        time++;

        if (pid == 0)
            break;

        for (int i = 0; i < len_proc; i++) {
            if (proc[i].pid == pid) {
                proc[i].remaining_time--;
                printf("pid = %d, rem = %d\n", pid, proc[i].remaining_time);
                break;
            }
        }
    };

    puts("PASS");

    return 0;
}

int main(void) {
    Process processes[NUM_PROCESSES] = {// pid, priority, remaining_time
                                        {1, 2, 5},
                                        {2, 1, 3},
                                        {3, 3, 2},
                                        {4, 2, 4},
                                        {5, 1, 1}};

    int expected_seq[NUM_SEQ] = {3, 3, 4, 4, 4, 4, 1, 1,
                                 1, 1, 1, 5, 2, 2, 2, 0};

    return test(processes, NUM_PROCESSES, expected_seq, NUM_SEQ);
}
