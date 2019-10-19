#include "mystat.h"

mystat_t initialize_mystat(char *file_name)
{
   mystat_t ms;
   struct stat filestat;
   lstat(file_name, &filestat);

   ms.filestat = &filestat;
   ms.file_name = malloc(sizeof (char) * BUFSIZE_LG);
   ms.file_type = malloc(sizeof (char) * BUFSIZE_LG);
   ms.mode = malloc(sizeof (char) * BUFSIZE_SM);
   ms.mode_octal = malloc(sizeof (char) * BUFSIZE_SM);
   ms.owner_id = malloc(sizeof(char) * BUFSIZE_SM);
   ms.group_id = malloc(sizeof(char) * BUFSIZE_SM);
   ms.last_file_access = malloc(sizeof(char) * BUFSIZE);
   ms.last_file_modification = malloc(sizeof(char) * BUFSIZE);
   ms.last_status_change = malloc(sizeof(char) * BUFSIZE);
   strcpy(ms.file_name, file_name);

   return ms;
}

void free_mystat(mystat_t *ms)
{
   free(ms->file_name);
   free(ms->file_type);
   free(ms->mode);
   free(ms->mode_octal);
   free(ms->owner_id);
   free(ms->group_id);
   free(ms->last_file_access);
   free(ms->last_file_modification);
   free(ms->last_status_change);
}

void get_data_from_stat(mystat_t *ms)
{
   ms->device_id = ms->filestat->st_dev;
   ms->inode_num = ms->filestat->st_ino;
   ms->mode_full = ms->filestat->st_mode;
   ms->link_count = ms->filestat->st_nlink;
   ms->uid = ms->filestat->st_uid;
   ms->gid = ms->filestat->st_gid;
   ms->block_size = ms->filestat->st_blksize;
   ms->file_size = ms->filestat->st_size;
   ms->blocks_allocated = ms->filestat->st_blocks;
   ms->last_file_access_raw = ms->filestat->st_atim;
   ms->last_status_change_raw = ms->filestat->st_mtim;
   ms->last_file_modification_raw = ms->filestat->st_ctim;
}

void get_linked_file_name(mystat_t *ms)
{
   if(readlink(ms->file_name, ms->file_type, BUFSIZE_LG) <= 0){
      strcpy(ms->file_type, "symbolic link - with dangling destination");
   }
   else{
      char *temp = strdup(ms->file_type);
      strcpy(ms->file_type, "symbolic link -> ");
      strcat(ms->file_type, temp);
      free(temp);
   }
}

char get_file_type(mystat_t *ms)
{
   if(S_ISREG(ms->mode_full)){
      strcpy(ms->file_type, "regular file");
      return '-';
   }
   else if(S_ISDIR(ms->mode_full)){
      strcpy(ms->file_type, "directory");
      return 'd';
   }
   else if(S_ISCHR(ms->mode_full)){
      strcpy(ms->file_type, "character device");
      return 'c';
   }
   else if(S_ISBLK(ms->mode_full)){
      strcpy(ms->file_type, "block device");
      return 'b';
   }
   else if(S_ISFIFO(ms->mode_full)){
      strcpy(ms->file_type, "FIFO/pipe");
      return 'p';
   }
   else if(S_ISSOCK(ms->mode_full)){
      strcpy(ms->file_type, "socket");
      return 's';
   }
   else if(S_ISLNK(ms->mode_full)){
      get_linked_file_name(ms);
      return 'l';
   }
   else{
      strcpy(ms->file_type, "unrecognized file type");
      return '-';
   }
}

void get_mode_octal(mystat_t *ms)
{
   size_t i;
   size_t perms[3] = {0, 0, 0};
   const size_t options[3] = {4, 2, 1};
   for(i = 0; i < 9; i++)
      perms[i / 3] += (ms->mode_full & (1 << (8 - i))) ? options[i % 3] : 0;
   for(i = 0; i < 3; i++)
      ms->mode_octal[i] = perms[i] + '0';
}

void get_mode(mystat_t *ms)
{
   size_t i;
   const char *options = "rwx";
   ms->mode[0] = get_file_type(ms);
   for(i = 0; i < 9; i++)
      ms->mode[i + 1] = (ms->mode_full & (1 << (8 - i))) ? options[i % 3] : '-';
}

void get_owner_id(mystat_t *ms)
{
   struct passwd *user_info = getpwuid(ms->uid);
   strcpy(ms->owner_id, user_info->pw_name);
}

void get_group_id(mystat_t *ms)
{
   struct group *group_info = getgrgid(ms->gid);
   strcpy(ms->group_id, group_info->gr_name);
}

void convert_time(struct timespec raw, char *result)
{
   const char *format = "%Y-%m-%d %X %z (%Z) %a (local)";
   struct tm *val = localtime(&raw.tv_sec);
   strftime(result, BUFSIZE, format, val);
}

void print_output(mystat_t *ms)
{
   printf("File: %s\n", ms->file_name);
   printf("  File type:\t\t    %s\n", ms->file_type);
   printf("  Device ID number:\t    %zu\n", ms->device_id);
   printf("  I-node number:\t    %zu\n", ms->inode_num);
   printf("  Mode:\t\t\t    %s\t\t(%s in octal)\n", ms->mode, ms->mode_octal);
   printf("  Link count:\t\t    %zu\n", ms->link_count);
   printf("  Owner Id:\t\t    %s\t\t(UID = %u)\n", ms->owner_id, ms->uid);
   printf("  Group Id:\t\t    %s\t\t(GID = %u)\n", ms->group_id, ms->gid);
   printf("  Preferred I/O block size: %zu bytes\n", ms->block_size);
   printf("  File size:\t\t    %zu bytes\n", ms->file_size);
   printf("  Blocks allocated:\t    %zu\n", ms->blocks_allocated);
   printf("  Last file access:\t    %s\n", ms->last_file_access);
   printf("  Last file modification:   %s\n", ms->last_file_modification);
   printf("  Last status change:\t    %s\n", ms->last_status_change);
}

void get_and_display_stats(char *file_name)
{
   mystat_t ms = initialize_mystat(file_name);
   get_data_from_stat(&ms);
   get_mode(&ms);
   get_mode_octal(&ms);
   get_owner_id(&ms);
   get_group_id(&ms);
   convert_time(ms.last_file_access_raw, ms.last_file_access);
   convert_time(ms.last_file_modification_raw, ms.last_file_modification);
   convert_time(ms.last_status_change_raw, ms.last_status_change);
   print_output(&ms);
   free_mystat(&ms);
}

int main(int argc, char *argv[])
{
   size_t i;
   for(i = 1; i < argc; i++)
      get_and_display_stats(argv[i]);
   return 0;
}