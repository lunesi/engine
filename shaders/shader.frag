#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;


layout(push_constant) uniform Push {
    float angle;
} push;

void main() {
    float r = fragColor.r * (sin(push.angle) * 0.5 + 0.5);
    float g = fragColor.g * (sin(push.angle + 2.0) * 0.5 + 0.5);
    float b = fragColor.b * (sin(push.angle + 4.0) * 0.5 + 0.5);
    
    outColor = vec4(r, g, b, 1.0);
}
