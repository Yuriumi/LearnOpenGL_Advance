#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

float near = 0.1f;
float far = 100.0f;

uniform sampler2D texture1;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{   
    vec4 texColor = texture(texture1,TexCoords);
    
    FragColor = texColor;
    // float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    // FragColor = vec4(vec3(depth), 1.0);
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}