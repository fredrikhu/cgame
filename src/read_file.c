#include <stdio.h>
#include <stdlib.h>

char *read_file(char *path)
{
	FILE *file = NULL;
	fopen_s(&file, path, "rb");

	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = malloc(length);
	if (buffer)
		fread(buffer, 1, length, file);

	fclose(file);

	return buffer;
}
