#version 330 core
out vec4 FragColour;

in vec2 texCoords;
in vec3 fragPos;

uniform sampler2D waterDudv;
uniform sampler2D waterNormals;
uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D depthMap;
uniform float screenW;
uniform float screenH;
uniform float elapsed;
uniform float near;
uniform float far;
uniform int dudvTiling = 1;
uniform vec4 waterColour;

vec3 blinnPhongDir(vec3 norm, vec3 viewDir);

struct Material {
    vec3 specular;    
    float specularity;
};                                                                        

struct DirLight {
    vec3 direction;
    vec3 specular;
}; 

uniform DirLight dirLight;
uniform Material mat ;
uniform float waterLevel;

uniform vec3 viewPos;

void main()
{
    vec3 viewDir = normalize(viewPos - fragPos);
    //Dudv map
	vec2 dudv1 = (texture(waterDudv, vec2(texCoords.x  + (elapsed * 0.003), texCoords.y) * dudvTiling).rg * 2.0 - 1.0) * 0.02;
	vec2 dudv2 = (texture(waterDudv, vec2(texCoords.x  , texCoords.y + (elapsed * 0.003)) * dudvTiling).rg * 2.0 - 1.0) * 0.02;
    vec2 dudv = dudv1 + dudv2;
    //Normal map
    vec4 normMap = texture(waterNormals, dudv);
    //Normals are using blue on the y axis since it needs to be aligned with the world up value
    vec3 norm = vec3(normMap.r * 2.0 - 1.0, normMap.b * 2.0, normMap.g * 2.0 - 1.0);
    norm = normalize(norm);

    //Converting texture to normalized device coordinates
    vec2 ndcXY = gl_FragCoord.xy;
    vec2 reflectTex = vec2((ndcXY.x)/screenW, (-ndcXY.y)/screenH) + dudv;//Negative as it is a reflection
    vec2 refractTex = clamp(vec2((ndcXY.x)/screenW, (ndcXY.y)/screenH) + dudv, 0.001, 0.999);

    //Clamping values to prevent unwated tiling
    reflectTex.x = clamp(reflectTex.x, 0.001, 0.999);
    reflectTex.y = clamp(reflectTex.y, -0.999, -0.001);

    //Depth
    float depth = texture(depthMap, vec2((ndcXY.x)/screenW, (ndcXY.y)/screenH)).r;
    float groundDist = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
    depth = gl_FragCoord.z;
    float waterSurfaceDist = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
    float waterDepth = groundDist - waterSurfaceDist;

    //Fresnel effect
    vec4 reflec = texture(reflection, reflectTex);
    vec4 refrac = texture(refraction, refractTex) * clamp((waterDepth/2.0), 0.0, 1.0);
    float refractFactor = dot(viewDir, vec3(0.0,1.0,0.0));
    refractFactor = pow(refractFactor, 0.5);

    //results
    vec3 normRes = blinnPhongDir(norm, viewDir) * clamp((waterDepth/200.0), 0.0, 1.0);
    vec4 fresnel = mix(reflec, refrac, refractFactor) + waterColour;

    if(viewPos.y >= waterLevel)
    {
	    FragColour = fresnel + vec4(normRes,1.0);
        FragColour.a = clamp((waterDepth/5.0),0.0,1.0);
    }
    else
    {
       FragColour = vec4(0.0,0.0,0.1,0.7);
    }
}

vec3 blinnPhongDir(vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirLight.direction);//Distance between light and the fragment in world space normalized
    //specular
    vec3 halfway = normalize(lightDir + viewDir); //Calculate the halfway point between the view and light
    float specLevel = dot(halfway, norm); //Angle between the view direction and the reflection
    specLevel = max(specLevel, 0.0);
    specLevel = pow(specLevel, mat.specularity); //Increase the factor by the shininess
    vec3 specular = specLevel * mat.specular * dirLight.specular; //Calculate specular level

    vec3 result = specular;
    return result;
}