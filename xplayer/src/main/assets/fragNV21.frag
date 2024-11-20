#version 300 es

precision mediump float;//精度

in vec2 vTexCoord;//顶点着色器传递的坐标

uniform sampler2D yTexture;//输入的材质（不透明灰度，单像素）
uniform sampler2D vuTexture;

out vec4 FragColor;//输出的片段颜色

void main(){
    // 读取 Y 分量
    float y = texture(yTexture, vTexCoord).r;
    float v = texture(vuTexture, vTexCoord).r - 0.5;
    float u = texture(vuTexture, vTexCoord).a - 0.5;

    vec3  rgb = mat3(1.0, 1.0, 1.0,
    0.0, -0.39465, 2.03211,
    1.13983, -0.58060, 0.0)* vec3(y, u, v);
    //输出像素颜色
    FragColor = vec4(rgb, 1.0);
}