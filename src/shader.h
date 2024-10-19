#ifndef __SHADER__
#define __SHADER__

typedef unsigned int Shader;

Shader shader_new(char const *vertex_file, char const *fragment_file);
void shader_activate(Shader shader);
void shader_free(Shader shader);

#endif

