#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <cglm/cglm.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow* window);
void render_scene();
void prepare_scene();
unsigned int load_texture(char const* path, const GLenum format);

unsigned int vbo;
unsigned int vao;
unsigned int ebo;
unsigned int wall_texture;
unsigned int face_texture;
Shader shader_program;

mat4 transform = GLM_MAT4_IDENTITY_INIT;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	vec4 vec = {1.0f, 0.0f, 0.0f, 1.0f};
	//vec4 destvec = GLM_VEC4_ZERO_INIT;
	glm_rotate(transform, glm_rad(90.0f), (vec3){0.0f, 0.0f, 1.0f});
	glm_scale(transform, (vec3){ 0.5f, 0.5f, 0.5f });
	//glm_translate(mat, (vec3){1.0f, 1.0f, 0.0f});
	//glm_mat4_mulv(mat, vec, vec);

	printf("%f %f %f %f", vec[0], vec[1], vec[2], vec[3]);

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		printf("Failed to create GLFW window\n");
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

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

	shader_free(shader_program);
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
	stbi_set_flip_vertically_on_load(true);
	float vertices[] = {
		// positions         // colors          // textures
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,   // top left
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,   // top right
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	wall_texture = load_texture("textures/wall.jpg", GL_RGB);
	face_texture = load_texture("textures/awesomeface.png", GL_RGBA);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader_program = shader_new("shaders/shader.vert", "shaders/shader.frag");
	shader_use(shader_program);
	shader_set_int(shader_program, "texture1", 0);
	shader_set_int(shader_program, "texture2", 1);
	shader_set_m4fv(shader_program, "transform", (const float *)transform);
}

unsigned int load_texture(char const* path, const GLenum format)
{
	unsigned int texture;
	int width, height, channels;
	unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

	if (!data)
		return 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

//double previous_time = 0;
//unsigned int frame_count = 0;
void render_scene()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

//	if ((++frame_count)%100 == 0)
//	{
//		double time = glfwGetTime();
//		double time_diff = time - previous_time;
//		previous_time = time;
//		printf("fps: %d, frame time: %f\n", (int)(1.0/time_diff), time_diff);
//	}

	//float green = (sin(time) / 2.0f) + 0.5f;
	//int vertex_color_location = glGetUniformLocation(shader_program, "ourColor");

	shader_use(shader_program);

	//glUniform4f(vertex_color_location, 0.0f, green, 0.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wall_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, face_texture);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
