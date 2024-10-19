#include "read_file.h"
#include "shader.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

const size_t info_log_size = 512;

unsigned int compile_shader(char const *path, GLenum shader_type);
void check_shader(const int shader);
void check_shader_program(const Shader shader_program);

Shader shader_new(char const *vertex_file, char const *fragment_file)
{
	const unsigned int vertex_shader = compile_shader(vertex_file, GL_VERTEX_SHADER);
	const unsigned int fragment_shader = compile_shader(fragment_file, GL_FRAGMENT_SHADER);

	const Shader shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	check_shader_program(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
}

void shader_use(const Shader shader)
{
	glUseProgram(shader);
}

void shader_free(const Shader shader)
{
	glDeleteProgram(shader);
}

void shader_set_bool(const Shader shader, char const *name, const bool value)
{
	const int uniform = glGetUniformLocation(shader, name);
	glUniform1i(uniform, (int)value);
}

void shader_set_int(const Shader shader, char const *name, const int value)
{
	const int uniform = glGetUniformLocation(shader, name);
	glUniform1i(uniform, value);
}

void shader_set_float(const Shader shader, char const *name, const float value)
{
	const int uniform = glGetUniformLocation(shader, name);
	glUniform1i(uniform, value);
}


unsigned int compile_shader(char const *path, GLenum shader_type)
{
	const unsigned int shader = glCreateShader(shader_type);
	char *shader_source = read_file(path);
	glShaderSource(shader, 1, (char const* const*)&shader_source, NULL);
	free(shader_source);
	glCompileShader(shader);

	check_shader(shader);

	return shader;
}

void check_shader(const int shader)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[info_log_size];
		glGetShaderInfoLog(shader, info_log_size, NULL, infoLog);
		printf("Error: Vertex shader compilation failed: %s\n", infoLog);
	}
}


void check_shader_program(const Shader shader_program)
{
	int success;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		char infoLog[info_log_size];
		glGetProgramInfoLog(shader_program, info_log_size, NULL, infoLog);
		printf("Error: Linking shader program failed: %s\n", infoLog);
	}
}
