#ifndef __SHADER__
#define __SHADER__

typedef unsigned int Shader;

Shader new_shader(char const *vertex_file, char const *fragment_file);
void activate_shader(Shader shader);
void free_shader(Shader shader);

#endif

