#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
in mat3 TBN;

// Input uniform values
uniform sampler2D diffuseMap; //Diffuse
uniform sampler2D specularMap; //Specular
uniform sampler2D normalMap; //normal
uniform sampler2D nightEmissionMap; //diffuse night
uniform vec4 colDiffuse;

uniform float roughness;

// Output fragment color
out vec4 finalColor;

// NOTE: Add your custom variables here

#define     MAX_LIGHTS              4
#define     LIGHT_DIRECTIONAL       0
#define     LIGHT_POINT             1
#define     M_PI 3.1415926535897932384626433832795

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
};

// Input lighting values
uniform Light lights[MAX_LIGHTS];
uniform vec4 ambient;
uniform vec3 viewPos;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(diffuseMap, fragTexCoord);
    vec4 texelNight = texture(nightEmissionMap, fragTexCoord);
    //vec3 normal = normalize(fragNormal);

    vec3 normal = texture(normalMap, vec2(fragTexCoord.x, fragTexCoord.y)).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(normal * TBN);

    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);
    vec4 specularTexelColor = texture(specularMap, fragTexCoord);
    vec3 n = 1.33 * specularTexelColor.rgb;

    vec4 tint = colDiffuse * fragColor;

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (lights[i].enabled == 1)
        {
            vec3 light = vec3(0.0);

            if (lights[i].type == LIGHT_DIRECTIONAL)
            {
                light = -normalize(lights[i].target - lights[i].position);
            }

            if (lights[i].type == LIGHT_POINT)
            {
                light = normalize(lights[i].position - fragPosition);
            }

            vec3 H = normalize(viewD + light);

            float NdotL = dot(normal, light);
            float HdotN = dot(H, normal);
            float NdotV = dot(normal, viewD);
            float VdotH = dot(viewD, H);

            if(NdotL > 0.01)
            {
                vec3 lightDot = lights[i].color.rgb*NdotL;

                float alphaSquared = roughness * roughness;
                float D = pow(HdotN, (2 / alphaSquared) - 2) / (M_PI * alphaSquared);
                float G = min(1.0, min(2 * HdotN * NdotV / VdotH, 2 * HdotN * NdotL / VdotH));
                vec3 F0 = pow(n - 1, vec3(2)) / pow(n + 1, vec3(2));
                vec3 F = F0 + (1 - F0) * pow(1 - VdotH, 5);

                vec3 rs = D * G * F / (4 * NdotL * NdotV);
                //finalColor += vec4(F,1);
                finalColor += vec4(lightDot * tint.rgb * texelColor.rgb * ((1 - 0.75 * specularTexelColor.rgb) + 0.75 * specularTexelColor.rgb * rs), 1);

            } else
            {
                finalColor += NdotV * 0.04 * texelNight * (1 - specularTexelColor.r);
            }
        }
    }
    finalColor += texelColor*(ambient/10.0)*tint;

    // Gamma correction
    finalColor = pow(finalColor, vec4(1.0/2.2));
    finalColor.a = 1.0;
}