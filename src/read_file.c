#include <stdio.h>
#include <stdlib.h>

char *read_file(char const path[static 1])
{
	FILE *file = NULL;
	if (fopen_s(&file, path, "rb") != 0) return NULL;

	if (fseek(file, 0, SEEK_END) != 0) return NULL;
	const size_t length = ftell(file);
	if (fseek(file, 0, SEEK_SET) != 0) return NULL;

	size_t read_bytes = 0;
	char *buffer = malloc(length + 1);
	if (buffer)
	{
		buffer[length] = 0;
		read_bytes = fread(buffer, 1, length, file);

		if (read_bytes != length)
		{
			free(buffer);
			return NULL;
		}
	}

	fclose(file);

	return buffer;
}
