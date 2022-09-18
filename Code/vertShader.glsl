STRINGIFY(
#version 330 core

in vec3 aPos;
in vec3 aNormal;
in vec2 aTextCoord;

mat4 mModel;
mat3 mNormal;
mat4 mViewProj;

varying vec3 vNormal;

void main()
{
   gl_Position =  mViewProj * mModel * vec4(aPos,1.0);
   vNormal =  mViewProj * mNormal * vec4(aNormal,0.0);
}
)