#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float alpha;

out vec4 FragColor;

void main()
{
    //FragColor = vec4(ourColor, 1.0f);
    FragColor = mix(texture(ourTexture1, TexCoord),
                    texture(ourTexture2, TexCoord), alpha);
}