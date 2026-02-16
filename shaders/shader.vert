#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
    float angle;
} push;

void main() {
    float s = sin(push.angle);
    float c = cos(push.angle);
    mat2 rot = mat2(c, s, -s, c);

    gl_Position = vec4(rot * inPosition, 0.0, 1.0);
    fragColor = inColor;
}
