#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BUFSIZE_SM
   #define BUFSIZE_SM 32
#endif   // BUFSIZE_SM

#ifndef BUFSIZE
   #define BUFSIZE 256
#endif   // BUFSIZE

#ifndef BUFSIZE_LG
   #define BUFSIZE_LG 512
#endif   // BUFSIZE_LG

typedef struct mystat {
   struct stat *filestat;
   char *file_name;
   char *file_type;
   size_t device_id;
   size_t inode_num;
   char *mode;
   size_t mode_octal;
   size_t link_count;
   char *owner_id;
   char *group_id;
   size_t uid;
   size_t gid;
   size_t block_size;
   size_t file_size;
   size_t blocks_allocated;
   time_t last_file_access_raw;
   time_t last_status_change_raw;
   time_t last_file_modification_raw;
   struct tm *last_file_access;
   struct tm *last_file_modification;
   struct tm *last_status_change;
} mystat_t;

void initialize_mystat(mystat_t *, struct stat *);
void free_mystat(mystat_t *);
void get_linked_file_name(mystat_t *);
void get_file_type(mystat_t *);