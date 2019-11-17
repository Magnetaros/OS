#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/dir.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>


void print_file_info(struct stat*,struct passwd*,struct group*,struct dirent*);
void mask_to_str(__mode_t,char*);

int main()
{
    DIR *curr_dir;
    struct dirent *dir;
    curr_dir = opendir(".");

    if (curr_dir) {
        struct stat stat_buf;
        struct passwd pswd;
        struct group usr_gr;
        
        while ((dir = readdir(curr_dir)) != NULL) 
            print_file_info(&stat_buf,&pswd,&usr_gr,dir);

        closedir(curr_dir);
    }
    return 0;
}

void print_file_info(struct stat *stat_buf, struct passwd *pswd, struct group *usr_gr, struct dirent *dir)
{
    struct tm *f_time;
    char time_str[40];
    char *mask = (char*)malloc(10 * sizeof(char));
    strcpy(mask, "----------");

    stat(dir->d_name,stat_buf);
    pswd = getpwuid(stat_buf->st_uid);
    usr_gr = getgrgid(stat_buf->st_gid);
    mask_to_str(stat_buf->st_mode,mask);
    f_time = localtime(&stat_buf->st_mtime);
    strftime(time_str, 80, "%b %d %H:%M", f_time);

    printf("%s %lu %s %s %4ld %s %s\n", mask, stat_buf->st_nlink, pswd->pw_name, usr_gr->gr_name, stat_buf->st_size, time_str, dir->d_name);
    free(mask);
}

void mask_to_str(__mode_t mask,char *result)
{
    if((mask & S_IFDIR) != 0) *(result + 0) = 'd';
    if((mask & S_IRUSR) != 0) *(result + 1) = 'r';
    if((mask & S_IWUSR) != 0) *(result + 2) = 'w';
    if((mask & S_IXUSR) != 0) *(result + 3) = 'x';
    if((mask & S_IRGRP) != 0) *(result + 4) = 'r';
    if((mask & S_IWGRP) != 0) *(result + 5) = 'w';
    if((mask & S_IXGRP) != 0) *(result + 6) = 'x';
    if((mask & S_IROTH) != 0) *(result + 7) = 'r';
    if((mask & S_IWOTH) != 0) *(result + 8) = 'w';
    if((mask & S_IXOTH) != 0) *(result + 9) = 'x';
}
