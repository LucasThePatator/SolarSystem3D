#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;

// Output fragment color
out vec4 finalColor;

float pi = 3.141592;
float period = 12;
void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 rgb = 1.5 * texelColor.rgb * (1 + 0.2 * cos(2 * pi * time * texelColor.r / period));
    finalColor = vec4(rgb, 1);
    //finalColor = pow(finalColor, vec4(1.0 / 2.2));
}