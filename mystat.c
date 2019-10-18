#include "mystat.h"

void initialize_mystat(mystat_t *ms, struct stat *filestat)
{
   ms->filestat = filestat;
   ms->file_name = malloc(sizeof (char) * BUFSIZE);
   ms->file_type = malloc(sizeof (char) * BUFSIZE_LG);
   ms->mode = malloc(sizeof (char) * BUFSIZE_SM);
   ms->owner_id = malloc(sizeof(char) * BUFSIZE_SM);
   ms->group_id = malloc(sizeof(char) * BUFSIZE_SM);
}

void free_mystat(mystat_t *ms)
{
   free(ms->file_name);
   free(ms->file_type);
   free(ms->mode);
   free(ms->owner_id);
   free(ms->group_id);
}

void get_linked_file_name(mystat_t *ms)
{
   if(readlink(ms->file_name, ms->file_type, BUFSIZE_LG) == -1){
      strcpy(ms->file_type, " with dangling destination");
   }
   else{
      char *temp = strdup(ms->file_type);
      strcpy(ms->file_type, "symbolic link -> ");
      strcat(ms->file_type, temp);
      free(temp);
   }
}

void get_file_type(mystat_t *ms)
{
   if(S_ISREG(ms->filestat->st_mode))
      strcpy(ms->file_type, "regular file");
   else if(S_ISDIR(ms->filestat->st_mode))
      strcpy(ms->file_type, "directory");
   else if(S_ISCHR(ms->filestat->st_mode))
      strcpy(ms->file_type, "character device");
   else if(S_ISBLK(ms->filestat->st_mode))
      strcpy(ms->file_type, "block device");
   else if(S_ISFIFO(ms->filestat->st_mode))
      strcpy(ms->file_type, "FIFO/pipe");
   else if(S_ISSOCK(ms->filestat->st_mode))
      strcpy(ms->file_type, "socket");
   else if(S_ISLNK(ms->filestat->st_mode))
      get_linked_file_name(ms);
   else
      strcpy(ms->file_type, "unrecognized file type");
}

int main(int argc, char *argv[])
{
   
   return 0;
}