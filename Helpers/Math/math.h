#pragma once

#include "../../models.h"

#include <cmath>

vec3d Vector_Add(vec3d &v1, vec3d &v2);
vec3d Vector_Sub(vec3d &v1, vec3d &v2);
vec3d Vector_Mul(vec3d &v1, float k);
vec3d Vector_Div(vec3d &v1, float k);

float Vector_DotProduct(vec3d &v1, vec3d &v2);
float Vector_Length(vec3d &v);

vec3d Vector_Normalise(vec3d &v);
vec3d Vector_CrossProduct(vec3d &v1, vec3d &v2);
vec3d Vector_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd, float &t);
vec3d Matrix_MultiplyVector(mat4x4 &m, vec3d &i);

mat4x4 Matrix_MakeIdentity();
mat4x4 Matrix_MakeRotationX(float fAngleRad);
mat4x4 Matrix_MakeRotationY(float fAngleRad);
mat4x4 Matrix_MakeRotationZ(float fAngleRad);
mat4x4 Matrix_MakeTranslation(float x, float y, float z);
mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
mat4x4 Matrix_MultiplyMatrix(mat4x4 &m1, mat4x4 &m2);
mat4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up);
mat4x4 Matrix_QuickInverse(mat4x4 &m);
int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2);