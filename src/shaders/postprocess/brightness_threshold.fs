#version 330 core

in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float threshold;

out vec4 BrightColor;

void main()
{
    BrightColor = texture(texture0, fragTexCoord);
    float brightness = dot(BrightColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness >= 0.9)
    BrightColor = vec4(BrightColor.rgb, 1.0);
    else
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}