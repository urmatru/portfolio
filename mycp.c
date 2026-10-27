#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h> 
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <limits.h>

#define BUF_SIZE 1024

void def_file_copy(int* fd1, int* fd2, char* temp_buf) {
	ssize_t str, stw;
	ssize_t total_written = 0; 
	while ((str = read(*fd1, temp_buf, BUF_SIZE)) > 0) {
		while (total_written < str) {
			stw = write(*fd2, temp_buf + total_written, str - total_written);
			if (stw == -1) {
				if (errno == EINTR) {
					continue;
				}
				fprintf(stderr, "Ошибка записи: %s, CODE %d\n", strerror(errno), errno);
				close(*fd1);
				close(*fd2);
				exit(EXIT_FAILURE);
			}
			total_written += stw;
		}
	}
}

int main(int argc, char *argv[]) {
	int opt;
	bool recursive_flag = false;
	while ((opt = getopt(argc, argv, "rR")) != -1) {
		switch(opt) {
			case 'r':
			case 'R':
			recursive_flag = 1;
			break;
		default:
			fprintf(stderr, "Использование %s -r <источник> <цель>\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	} 
	if (argc - optind < 2) {
		fprintf(stderr, "Ошибка: Не указан источник или цель\n");
		exit(EXIT_FAILURE);
	}
	char *rf = argv[optind];
	char *wf = argv[optind + 1];

	char temp_buf[BUF_SIZE]; 
									//CP start
	
	int fd_1, fd_2;
	int* fd1 = &fd_1;
	int* fd2 = &fd_2;

	if (recursive_flag) {
		struct stat sb;
		struct dirent *rd; 	
		if (lstat(rf, &sb) == -1) {
			fprintf(stderr, "Ошибка lstat для начального пути\n");
			exit(EXIT_FAILURE);
		};
		
		if (S_ISDIR(sb.st_mode)) {
			DIR* folder = opendir(rf);
			if (folder == NULL) {
				fprintf(stderr, "Ошибка opendir для целевого каталога\n");
				exit(EXIT_FAILURE);
			}
			char file_name[PATH_MAX];
			while ((rd = readdir(folder)) != NULL) {
				if (strcmp(rd->d_name, ".") == 0 || strcmp(rd->d_name, "..") == 0) {
					continue;
				}
				snprintf(file_name, sizeof(file_name), "%s/%s", rf, rd->d_name);
				fd_1 = open(file_name, O_RDONLY);
				if (fd_1 == -1) {
					fprintf(stderr, "Ошибка open для файла из начального каталога\n");
					exit(EXIT_FAILURE);
				}
				snprintf(file_name, sizeof(file_name), "%s/%s", wf, rd->d_name);
				fd_2 = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
				if (fd_2 == -1) {
					fprintf(stderr, "Ошибка open для файла из конечного каталога\n");
					exit(EXIT_FAILURE);
				}/* else if (ERRNO == EISIDR) {
					
				} */
				def_file_copy(fd1, fd2, temp_buf);
			}
		closedir(folder);	
		}
	} else {
		if (argc != 3) {
			fprintf(stderr, "Использование: %s <имя_файла> <имя_файла>\n", argv[0]);
			exit(EXIT_FAILURE);
		}
		fd_1 = open(rf, O_RDONLY);
		if (*fd1 == -1) {
			perror("1 open error");
			exit(EXIT_FAILURE);
		}

		fd_2 = open(wf, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (*fd2 == -1) {
			perror("2 open error");
			exit(EXIT_FAILURE);
		}
		def_file_copy(fd1, fd2, temp_buf);
	}
	
	close(*fd1);
	close(*fd2);
		
}


















		/*	while ((str = read(fd1, temp_buf, BUF_SIZE)) > 0) {
			ssize_t str, stw;
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
			}  */
