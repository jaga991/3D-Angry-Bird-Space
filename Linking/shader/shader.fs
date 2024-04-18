#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;

uniform sampler2D bird1;
uniform sampler2D bird2;
uniform sampler2D bird3;
uniform sampler2D bird4;



// cube type
uniform int cubeType;

uniform vec3 color;
void main()
{
    vec4 texColor;
    if (cubeType == 1) {
        texColor = texture(texture1, TexCoord);
    } else if (cubeType == 3) {
        texColor = texture(texture3, TexCoord);
    } else if (cubeType == 4) {
        texColor = texture(texture4, TexCoord);
    } else if (cubeType == 5) {
		texColor = texture(texture5, TexCoord);
	}
else if (cubeType == 11) {
        texColor = texture(bird1, TexCoord);
    } 
else if (cubeType == 12) {
        texColor = texture(bird2, TexCoord);
    } 
else if (cubeType == 13) {
        texColor = texture(bird3, TexCoord);
    } 
else if (cubeType == 14) {
        texColor = texture(bird4, TexCoord);
    } 

    FragColor = vec4(color, 0.5) * texColor;
}