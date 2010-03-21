#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <time.h>

#define DEVFILE "/proc/net/dev"
#define MAX_IF 256              /* max # of interfaces to read */
#define MAX_IF_NAME 8           /* max length of an interface name */
#define MAX_LINE 256
#define MAX_COLS 24
#define DEF_INTERVAL 5
#define DEF_COUNT 0
#define DEF_SHOW_HEADER 1
#define DEF_SHOW_TIME 0
#define DEF_SHOW_DATE 0
#define DEF_SHOW_MB 0
#define DEF_REPEAT_HEADER 25
#define DATA_COLS 16            /* # of data columns in DEVFILE */
#define VERSION "20100318"

typedef struct
{
    char iface[MAX_IF_NAME];
    long long in_bytes;
    long long in_pkts;
    unsigned long in_errs;
    long long out_bytes;
    long long out_pkts;
    unsigned long out_errs;
    unsigned long coll;
} stat_struct;

int split(const char *line, char **cols, int cols_max, char split_char);

