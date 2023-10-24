#version 330 core
out vec4 FragColor;

in vec3 gNormals;
in vec3 gWorldPos_FS_in ;
in float visibility;
in float gl_ClipDistance[1];

vec3 blinnPhongDir(vec3 norm, vec3 viewDir);

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};                                                                        

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

uniform sampler2D snow;
uniform sampler2D rock;

uniform DirLight dirLight;
uniform Material mat ;
uniform vec3 viewPos ;
uniform float cutoff;
uniform vec3 fogColour = vec3(1.0,1.0,1.0);
uniform float waterLevel;

void main()
{   
    vec3 viewDir = normalize(viewPos - gWorldPos_FS_in);
    vec3 norm = normalize(gNormals);

    vec3 result = blinnPhongDir(norm, viewDir);
    if(viewPos.y >= waterLevel)
    {
        FragColor = mix(vec4(fogColour,1.0),vec4(result, 1.0), visibility);
    }
    else
    {
        FragColor = mix(vec4(0.0,0.0,0.1,1.0),vec4(result, 1.0), visibility / 4);
    }
}

vec3 blinnPhongDir(vec3 norm, vec3 viewDir)
{
    //triplanar mapping
    vec3 blendPercent = normalize(abs(gNormals));
    float b = blendPercent.x + blendPercent.y + blendPercent.z;
    blendPercent = blendPercent / vec3(b);

    //Blending snow texture
    vec4 xAxisSnow = texture2D(snow, gWorldPos_FS_in.yz);
    vec4 yAxisSnow = texture2D(snow, gWorldPos_FS_in.xz);
    vec4 zAxisSnow = texture2D(snow, gWorldPos_FS_in.xy);
    vec4 tpTexSnow = xAxisSnow*blendPercent.x + yAxisSnow*blendPercent.y + zAxisSnow*blendPercent.z;
    //Blending rock texture
    vec4 xAxisRock = texture2D(rock, gWorldPos_FS_in.yz);
    vec4 yAxisRock = texture2D(rock, gWorldPos_FS_in.xz);
    vec4 zAxisRock = texture2D(rock, gWorldPos_FS_in.xy);
    vec4 tpTexRock = xAxisRock*blendPercent.x + yAxisRock*blendPercent.y + zAxisRock*blendPercent.z;

    //Calculate height
    float height = cutoff;

    vec4 terrainTex;
    terrainTex = smoothstep(tpTexRock, tpTexSnow, clamp(vec4(gWorldPos_FS_in.y / height), 0.0, 1.0));

    vec3 lightDir = normalize(-dirLight.direction);//Distance between light and the fragment in world space normalized
    //ambient
    vec3 ambient = dirLight.ambient * mat.ambient; //Calculate ambient level
    //diffuse
    float diffLevel = dot(norm, lightDir); //Angle between the normal and the light direction
    diffLevel = max(diffLevel, 0.0);
    vec3 diffuse = diffLevel * dirLight.diffuse * terrainTex.xyz; //Calculate diffuse level
    //specular
    vec3 halfway = normalize(lightDir + viewDir); //Calculate the halfway point between the view and light
    float specLevel = dot(halfway, norm); //Angle between the view direction and the reflection
    specLevel = max(specLevel, 0.0);
    specLevel = pow(specLevel, mat.shininess); //Increase the factor by the shininess
    vec3 specular = specLevel * mat.specular * dirLight.specular; //Calculate specular level

    vec3 result = ambient + diffuse + specular;
    return result;
}
