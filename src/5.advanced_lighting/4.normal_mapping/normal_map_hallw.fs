#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform float shininess;

//uniform vec3 lightPos;
uniform vec3 viewPos;

#define NR_POINT_LIGHTS_MAX 20
uniform int nr_point_lights;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLights[NR_POINT_LIGHTS_MAX];
uniform SpotLight spotLight;
uniform DirLight dirLight;

// function prototypes
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
        FragColor = vec4(1.0); // set all 4 vector values to 1.0
     //   vec4 result = vec4(0.0);
        // obtain normal from normal map in range [0,1]
        vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
        // transform normal vector to range [-1,1]
        normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

        //vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
        vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);


        vec3 result = vec3(0.0);
        result = CalcDirLight(dirLight, normal, viewDir);
        //for(int i = 0; i < nr_point_lights; i++)
        for(int i = 0; i < nr_point_lights; i++){
           result += CalcPointLight(pointLights[i], normal, fs_in.TangentFragPos, viewDir);
        }
       // result += CalcPointLight(pointLights[0], normal, fs_in.TangentFragPos, viewDir);
       // result += CalcPointLight(pointLights[1], normal, fs_in.TangentFragPos, viewDir);

        //result += CalcSpotLight(spotLight, normal, fs_in.TangentFragPos, viewDir);
        FragColor = vec4(result, 1.0);

}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // combine results
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    vec3 spec_color = texture(specularMap, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = light.ambient * color;

    //vec3 ambient = light.ambient * vec3(texture(light.diffuse, fs_in.TexCoords));
    //vec3 diffuse = light.diffuse * diff * vec3(texture(light.diffuse, fs_in.TexCoords));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color;

    //vec3 specular = light.specular * spec * vec3(texture(light.specular, fs_in.TexCoords));
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec_color * spec;

    return (ambient + diffuse + specular);
}
// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

    // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;

    vec3 spec_color = texture(specularMap, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = light.ambient * color;
    // diffuse
    //fs_in.TangentLightPos = light.position;
    //vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    vec3 lightDir = normalize(light.position - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * color;
    // specular
    //vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec_color * spec;

    float distance    = length(light.position - fs_in.TangentFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
     // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    vec3 spec_color = texture(specularMap, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = light.ambient * color;
    // diffuse
    //fs_in.TangentLightPos = light.position;
    //vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    vec3 lightDir = normalize(light.position - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * color;
    // specular
    //vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec_color * spec;

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
   // vec3 ambient = light.ambient * vec3(texture(light.diffuse, fs_in.TexCoords));
   // vec3 diffuse = light.diffuse * diff * vec3(texture(light.diffuse, fs_in.TexCoords));
   // vec3 specular = light.specular * spec * vec3(texture(light.specular, fs_in.TexCoords));


    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
