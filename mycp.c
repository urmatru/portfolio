#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h> 
#include <sys/stat.h>

char* chbuf;
long ccounter = 0;

void bufallocator(char* filename) {
	struct stat sb;
	if (stat(filename, &sb) == -1) {
		perror("stat error");
		exit(EXIT_FAILURE);
	}	
	chbuf = (char*)malloc(sb.st_size);
	if (chbuf == nullptr) {
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	ccounter = sb.st_size; 
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Использование: %s <имя_файла> <имя_файла>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	(void)argc;
	bufallocator(argv[1]);	
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

	while ((str = read(fd1, chbuf, ccounter)) > 0) {
		stw = write(fd2, chbuf, str);
		if (stw == -1) {
			fprintf(stderr, "Ошибка записи: %s, CODE: %d", strerror(errno), errno);	
			exit(EXIT_FAILURE);
		}
	}  		
	close(fd1);
	close(fd2);
	free(chbuf);
}
