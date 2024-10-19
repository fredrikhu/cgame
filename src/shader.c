#include "read_file.h"
#include "shader.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int compile_shader(char const *path, GLenum shader_type);
void check_shader(int shader);
void check_shader_program(Shader shader_program);

Shader new_shader(char const *vertex_file, char const *fragment_file)
{
	unsigned int vertex_shader = compile_shader(vertex_file, GL_VERTEX_SHADER);
	unsigned int fragment_shader = compile_shader(fragment_file, GL_FRAGMENT_SHADER);

	Shader shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	check_shader_program(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
}

void activate_shader(Shader shader)
{
	glUseProgram(shader);
}

void free_shader(Shader shader)
{
	glDeleteProgram(shader);
}

unsigned int compile_shader(char const *path, GLenum shader_type)
{
	char *shader_source = read_file(path);
	unsigned int shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, (char const* const*)&shader_source, NULL);
	free(shader_source);
	glCompileShader(shader);

	check_shader(shader);

	return shader;
}

void check_shader(int shader)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Error: Vertex shader compilation failed %s\n", infoLog);
	}
}


void check_shader_program(Shader shader_program)
{
	int success;
	char infoLog[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		printf("Error: Linking shader program failed %s\n", infoLog);
	}
}
