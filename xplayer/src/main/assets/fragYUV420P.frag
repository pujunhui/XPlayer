#version 300 es

precision mediump float;//精度

in vec2 vTexCoord;//从顶点着色器传来的纹理坐标

uniform sampler2D yTexture;//输入的材质（不透明灰度图，单像素）
uniform sampler2D uTexture;
uniform sampler2D vTexture;

out vec4 FragColor;//输出的片段颜色

void main() {
    float y = texture(yTexture, vTexCoord).r;
    float u = texture(uTexture, vTexCoord).r - 0.5;
    float v  = texture(vTexture, vTexCoord).r - 0.5;

    vec3 rgb = mat3(
    1.0, 1.0, 1.0,
    0.0, -0.39465, 2.03211,
    1.13983, -0.5806, 0.0
    ) * vec3(y, u, v);
    //输出像素颜色
    FragColor = vec4(rgb, 1.0);
}