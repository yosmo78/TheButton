STRINGIFY(
#version 330 core\n

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 mModel;
uniform mat3 mNormal;
uniform mat4 mViewProj;

varying vec3 worldPos;
varying vec3 vNormal;

void main()
{

   gl_Position =  (mViewProj * mModel) * vec4(aPos,1.0);
   worldPos = (mModel * vec4(aPos,1.0)).xyz;
   // vNormal =  mNormal*aNormal;
   vNormal =  aNormal;
}
)