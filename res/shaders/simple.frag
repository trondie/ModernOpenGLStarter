#version 430 core

in layout(location = 20) vec3 normal;

out vec4 color;

uniform layout(location = 6) vec3 lightPosition1; 

in vec3 vertexPosition;

in vec2 TexCoord;  
layout(binding=10) uniform sampler2D Tex1;

/**
 * Phong shading : I = i_a * k_a + SIGMA( (k_d*i_d * (L * N) + k_s * i_s * (R * V)^alpha ) )
 *
 * http://math.hws.edu/graphicsbook/c7/light-equation.png
 */
vec4 contribution(vec3 lightSource) {

    vec3 ambient = 0.5 * vec3(1.0, 1.0, 1.0); 
    vec3 diffuseColor = vec3(0.9, 0.4, 0.2);
    vec3 specularColor = vec3(1.0, 1.0, 1.0);

    // Ligh direction
    vec3 lightDirection = normalize(normalize(lightSource) - normalize(vertexPosition));

    // Diffuse contribution
    float kd = 0.1; 
    float id = 1.0; 
    float diffuse = kd * max(dot(lightDirection, normalize(normal)), 0.0) * id;

    // Specular contribution 
    float ks = 1.0; 
    float is = 1.0; 
    vec3 reflectVec = reflect(-lightDirection, normalize(normal));    
    float specularAngle = max(dot(reflectVec,  normalize(-vertexPosition)), 0.0);
    float specular = pow(specularAngle, 10.0);
   
    return vec4(ambient + diffuse * diffuseColor + specular * specularColor, 1.0f); 
}

void main()
{
    vec4 materialColor = vec4(1.0, 0.0, 0.0, 1.0);

    // Light source intensity
    float I1 = 0.7;

    // Contribution wrt phong shading
    vec4 colorp = I1 * contribution(lightPosition1);

    color = texture(Tex1, TexCoord) * colorp;
    //color = texture(Tex1, TexCoord);
}