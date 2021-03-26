#include "readinfile.h"

void read_stdin(Readbuf read_buf) {
	int bytes;
	char buf[BUFSIZ];

	while ((bytes = read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
		read_buf(buf);	
	}

	if (bytes < 0) {
		fprintf(stderr, "Failed to read from stdin: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void read_exec(char *file_name, Readbuf read_buf) {
	mmapd_file_t mmapd_file;
	
	read_file(file_name, &mmapd_file);
	read_buf(mmapd_file.text);
	free_file(&mmapd_file);
}

void read_file(char *file_name, mmapd_file_t *mmapd_file) {
	int fd;
	char *text;
	struct stat sb;

	if ((fd = open(file_name, O_RDONLY)) == -1) {
		fprintf(stderr, "Failed to open %s: %s \n", file_name, strerror(errno));
		exit(EXIT_FAILURE);	
	}

	if (fstat(fd, &sb) == -1) {
		fprintf(stderr, "Failed to fstat %s: %s\n", file_name, strerror(errno));
		(void)close(fd);
		exit(EXIT_FAILURE);
	}

	if ((mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == NULL) {
		fprintf(stderr, "Failed to mmap %s: %s\n", file_name, strerror(errno));
		(void)close(fd);
		exit(EXIT_FAILURE);
	}

	mmapd_file->fd = fd;
	mmapd_file->text = text;
	mmapd_file->size = sb.st_size;
}

void free_file(mmapd_file_t *mmapd_file) {
	(void)munmap(mmapd_file->text, mmapd_file->size);
	(void)close(mmapd_file->fd);
}
