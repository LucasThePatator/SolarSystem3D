#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Input uniform values
uniform samplerCube cubeMap;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Fetch color from texture map
    vec3 color = texture(cubeMap, fragPosition.xyz).rgb;

    // Calculate final fragment color
    finalColor = vec4(pow(color, vec3(2.2)), 1.0);
}
