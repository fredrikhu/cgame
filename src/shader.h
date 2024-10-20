#ifndef __SHADER__
#define __SHADER__

#include <stdbool.h>
typedef unsigned int Shader;

Shader shader_new(char const *vertex_file, char const *fragment_file);
void shader_use(const Shader shader);
void shader_free(const Shader shader);

void shader_set_bool(const Shader shader, char const *name, const bool value);
void shader_set_int(const Shader shader, char const *name, const int value);
void shader_set_float(const Shader shader, char const *name, const float value);
void shader_set_m4fv(const Shader shader, char const *name, const float *value);

#endif

