#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
vec3 getNormal();

in vec3 posES[];
in vec3 normES[];
in vec2 texES[];
in float visibilityES[];

out vec3 gNormals;
out vec3 gWorldPos_FS_in;
out vec2 gTex;
out float visibility;
out float gl_ClipDistance[1];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 clipPlane;

void main()
{
   //Calculate TBN here
   for(int i = 0 ; i < 3; i++)
   {
      gl_Position = projection * view * model * vec4(posES[i],1.0);
      gl_ClipDistance[0] = dot(vec4(posES[i], 1.0), clipPlane);

      gWorldPos_FS_in = posES[i];
      visibility = visibilityES[i];
      gTex = texES[i];
      //gNormals = getNormal();    
      gNormals = normES[i];
      EmitVertex();
  }
     EndPrimitive();

}


vec3 getNormal()
{
    vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
    return normalize(cross(a, b));
}