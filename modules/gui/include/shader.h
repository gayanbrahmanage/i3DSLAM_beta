#ifndef shader_H
#define shader_H

static const GLchar  *VERTEX_SOURCE =
"#version 330\n"
"in vec3 position;\n"
"in vec3 normal;\n"
"out vec3 transformedNormal;\n"
"out vec3 originalNormal;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
"void main(){\n"
"    gl_Position =  projection * view * model * vec4(position, 1.0);\n"
"    mat3 normalMatrix = transpose(inverse(mat3(view * model)));\n"
"    transformedNormal = normalMatrix * normal;\n"
"    originalNormal = abs(normal);\n"
"}\n";

static const GLchar *FRAGMENT_SOURCE =
"#version 330\n"
"in vec3 transformedNormal;\n"
"in vec3 originalNormal;\n"
"out vec4 outputColor;\n"
"void main() {\n"
"vec3 color = originalNormal;\n"
"float lighting = abs(dot(transformedNormal, vec3(0,0,-1)));\n"
"outputColor = vec4(color * lighting, 1.0f);\n" //constant white
"}";

#endif
