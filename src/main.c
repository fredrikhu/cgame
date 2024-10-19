#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "read_file.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow* window);
void render_scene();
void prepare_scene();
void compile_shaders();
void check_shader(int shader);
void check_shader_program();
unsigned int compile_shader(char *path, GLenum shader_type);

unsigned int vbo;
unsigned int vao;
unsigned int ebo;
unsigned int shader_program;

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    glfwTerminate();
	printf("Failed to create GLFW window\n");
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
	  printf("Failed to initialize GLAD\n");
	  return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  prepare_scene();

  while (!glfwWindowShouldClose(window))
  {
	  process_input(window);

	  render_scene();

	  glfwSwapBuffers(window);
	  glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(__attribute__ ((unused)) GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void prepare_scene()
{
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	compile_shaders();
}

void compile_shaders()
{
	unsigned int vertex_shader = compile_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	unsigned int fragment_shader = compile_shader("shaders/shader.frag", GL_FRAGMENT_SHADER);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	check_shader_program();

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

unsigned int compile_shader(char *path, GLenum shader_type)
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

void check_shader_program()
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

void render_scene()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//float time = glfwGetTime();
	//float green = (sin(time) / 2.0f) + 0.5f;
	//int vertex_color_location = glGetUniformLocation(shader_program, "ourColor");

	glUseProgram(shader_program);

	//glUniform4f(vertex_color_location, 0.0f, green, 0.0f, 1.0f);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
