#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>

#ifndef BUFSIZE_SM
   #define BUFSIZE_SM 32
#endif   // BUFSIZE_SM

#ifndef BUFSIZE
   #define BUFSIZE 256
#endif   // BUFSIZE

#ifndef BUFSIZE_LG
   #define BUFSIZE_LG 512
#endif   // BUFSIZE_LG

const int column1 = 27;


typedef struct mystat {
   struct stat *filestat;

   // Variables that CANNOT be copied directly from stat (need functions to get)
   char *file_name;
   char *file_type;
   char *mode;
   char *mode_octal;
   char *owner_id;
   char *group_id;

   // Variables that CAN be copied directly from stat
   size_t device_id;
   size_t inode_num;
   mode_t mode_full;
   size_t link_count;
   uid_t uid;
   gid_t gid;
   blksize_t block_size;
   off_t file_size;
   blkcnt_t blocks_allocated;

   // Time stuff
   struct timespec last_file_access_raw;
   struct timespec last_file_modification_raw;
   struct timespec last_status_change_raw;
   char *last_file_access;
   char *last_file_modification;
   char *last_status_change;
} mystat_t;

mystat_t initialize_mystat(char *);
void free_mystat(mystat_t *);
void get_data_from_stat(mystat_t *);
void get_linked_file_name(mystat_t *);
char get_file_type(mystat_t *);
void get_mode_octal(mystat_t *);
void get_mode(mystat_t *);
void get_owner_id(mystat_t *);
void get_group_id(mystat_t *);
void convert_time(struct timespec, char *);
void print_output(mystat_t *);
void get_and_display_stats(char *);