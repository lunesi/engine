#version 450

layout(push_constant) uniform Push {
    float angle;
} push;

layout(location = 0) out vec3 fragColor;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    float s = sin(push.angle);
    float c = cos(push.angle);
    mat2 rotationMatrix = mat2(c, s, -s, c);

    vec2 rotatedPos = rotationMatrix * positions[gl_VertexIndex];

    gl_Position = vec4(rotatedPos, 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}
