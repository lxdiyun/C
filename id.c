#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc,char **argv) {
	pid_t my_pid,parent_pid;
	uid_t my_uid,my_euid;
	gid_t my_gid,my_egid;
	struct passwd *my_info;

	my_pid = getpid();
	parent_pid = getppid();
	my_uid = getuid();
	my_euid = geteuid();
	my_gid = getgid();
	my_egid = getegid();
	my_info = getpwuid(my_uid);

	printf("Process ID:%ld\n",my_pid);
	printf("Parent ID:%ld\n",parent_pid);
	printf("User ID:%ld\n",my_uid);		
	printf("Effective User ID:%ld\n",my_euid);
	printf("Group ID:%ld\n",my_gid);
	printf("Effective Group ID:%ld\n",my_egid);

	if(my_info) {
		printf("--------------------------------------\n");
		printf("My Login Name:%s\n",my_info->pw_name);
		printf("My Password:%s\n",my_info->pw_passwd );
		printf("My User ID:%ld\n",my_info->pw_uid);
		printf("My Group ID:%ld\n",my_info->pw_gid);
		printf("My Real Name:%s\n",my_info->pw_gecos);
		printf("My Home Dir:%s\n",my_info->pw_dir);
		printf("My Work Shell:%s\n",my_info->pw_shell);
	}

	return 0;
}
