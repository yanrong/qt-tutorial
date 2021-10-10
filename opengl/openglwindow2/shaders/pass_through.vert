#version 330 core
//vertext shader

//input: attribute name 'position' with 3 floats per vertex
layout (location = 0) in vec3 position;

void main() {
    gl_Position = vec4(position, 1.0);
}
