#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <utmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void track_runtime_duration(int pid) {
    char path[40], line[100];
    FILE *fp;
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    fp = fopen(path, "r");

    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    long int starttime;
    for (int i = 0; i < 22; i++) {
        fscanf(fp, "%s", line);
    }
    starttime = atol(line);
    fclose(fp);

    long int uptime;
    fp = fopen("/proc/uptime", "r");
    fscanf(fp, "%ld", &uptime);
    fclose(fp);

    long int hertz = sysconf(_SC_CLK_TCK);
    long int runtime = uptime - (starttime / hertz);

    printf("Application has been running for %ld seconds.\n", runtime);
}

void check_memory_usage(int pid) {
    char path[40], line[100];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *fp = fopen(path, "r");

    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    while (fgets(line, 100, fp)) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            printf("Memory usage: %s", line + 6);
            break;
        }
    }
    fclose(fp);
}

void increment_open_count(const char *app_name) {
    FILE *fp = fopen("app_count.log", "a+");
    if (fp == NULL) {
        perror("Error opening log file");
        return;
    }
    fprintf(fp, "%s\n", app_name);
    fclose(fp);
    printf("Logged opening of %s\n", app_name);
}

void track_user_sessions() {
    struct utmp *user_entry;
    setutent(); // Open utmp file

    while ((user_entry = getutent())) {
        if (user_entry->ut_type == USER_PROCESS) {
            printf("User: %s\n", user_entry->ut_user);
            printf("Terminal: %s\n", user_entry->ut_line);
            printf("Host: %s\n", user_entry->ut_host);
            printf("Login Time: %ld\n\n", user_entry->ut_tv.tv_sec);
        }
    }
    endutent();
}

void show_memory_statistics() {
    FILE *fp = fopen("/proc/meminfo", "r");
    char line[100];

    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    while (fgets(line, 100, fp)) {
        if (strncmp(line, "MemTotal:", 9) == 0 ||
            strncmp(line, "MemFree:", 8) == 0 ||
            strncmp(line, "Buffers:", 8) == 0 ||
            strncmp(line, "Cached:", 7) == 0 ||
            strncmp(line, "SwapTotal:", 10) == 0 ||
            strncmp(line, "SwapFree:", 9) == 0) {
            printf("%s", line);
        }
    }
    fclose(fp);
}

int main() {
    int choice, pid;

    while (1) {
        printf("\nMonitoring Application System\n");
        printf("1. Track Runtime Duration\n");
        printf("2. Check Memory Usage\n");
        printf("3. Increment Open Count\n");
        printf("4. Track User Sessions\n");
        printf("5. Show Memory Statistics\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter PID of the application: ");
                scanf("%d", &pid);
                track_runtime_duration(pid);
                break;
            case 2:
                printf("Enter PID of the application: ");
                scanf("%d", &pid);
                check_memory_usage(pid);
                break;
            case 3:
                increment_open_count("YourApp");
                break;
            case 4:
                track_user_sessions();
                break;
            case 5:
                show_memory_statistics();
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
