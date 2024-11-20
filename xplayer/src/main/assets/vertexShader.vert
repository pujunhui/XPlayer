#version 300 es

in vec3 aPosition;//顶点坐标
in vec2 aTexCoord;//材质坐标

out vec2 vTexCoord;//输出的材质坐标

void main() {
    //将纹理坐标的 Y 分量反转
    vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
    //设置顶点的最终位置
    gl_Position = vec4(aPosition, 1.0);
}