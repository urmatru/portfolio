#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h> 
#include <sys/stat.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
	char temp_buf[BUF_SIZE];
	if (argc != 3) {
		fprintf(stderr, "Использование: %s <имя_файла> <имя_файла>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int fd1, fd2;
	ssize_t str, stw;
	fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) {
		perror("1 open error");
		exit(EXIT_FAILURE);
	}
	fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd2 == -1) {
		perror("2 open error");
		exit(EXIT_FAILURE);
	}

	while ((str = read(fd1, temp_buf, BUF_SIZE)) > 0) {
		ssize_t total_written = 0;
		while (total_written < str) {
			stw = write(fd2, temp_buf + total_written, str - total_written);
			if (stw == -1) {
				if (errno == EINTR) {
					continue;
				}
				fprintf(stderr, "Ошибка записи: %s, CODE %d\n", strerror(errno), errno);
				close(fd1);
				close(fd2);
				exit(EXIT_FAILURE);
			}
			total_written += stw;
		}
		
	}  		
}
