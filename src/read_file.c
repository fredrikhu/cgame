#include <stdio.h>
#include <stdlib.h>

char *read_file(char const *path)
{
	FILE *file = NULL;
	fopen_s(&file, path, "rb");

	fseek(file, 0, SEEK_END);
	size_t length = ftell(file);
	fseek(file, 0, SEEK_SET);

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
