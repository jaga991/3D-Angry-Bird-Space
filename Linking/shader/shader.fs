#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

// cube type
uniform int cubeType;

uniform vec3 color;
void main()
{
    vec4 texColor;
    if (cubeType == 1) {
        texColor = texture(texture1, TexCoord);
    } else if (cubeType == 2) {
        texColor = texture(texture2, TexCoord);
    } else if (cubeType == 3) {
        texColor = texture(texture3, TexCoord);
    } else {
        texColor = texture(texture4, TexCoord);
    }
    FragColor = vec4(color, 1.0) * texColor;
}