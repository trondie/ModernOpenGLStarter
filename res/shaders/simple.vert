#version 430 core

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal_in;
in layout(location = 2) vec2 texCoord; 
//layout (location = 11) in vec3 vertexTangent;

uniform layout(location = 3) mat4 M;
uniform layout(location = 4) mat4 V;
uniform layout(location = 5) mat4 P;

out mat4 MV; 
out vec2 TexCoord;

out layout(location = 20) vec3 normal_out;
out vec3 vertexPosition;

void main()
{
    MV = V * M;

    /**
    * Need to preserve the normals wrt deformations, etc. 
    * Explanation : https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
    */
    normal_out = normalize(transpose(inverse(mat3(MV))) * normal_in);
 
    // The position of the vertex when projected
    gl_Position = P * V * M * vec4(position, 1.0f);

    // The position of the vertex in model-view space, for the fragment shader to calculate its vectors without the projection matrix 
    // (preserving angles, etc)
    vec4 vertexPosition4 = V * M  * vec4(position, 1.0f);

    vertexPosition = vec3(vertexPosition4) / vertexPosition4.w; 

    TexCoord = texCoord;
}