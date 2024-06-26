#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <math.h>

// Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

//
Matrix4x4 Multiply(Matrix4x4 matrix1, Matrix4x4 matrix2);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// X
Matrix4x4 MakeRotateXMatrix(float radian);
// Y
Matrix4x4 MakeRotateYMatrix(float radian);
// Z

Matrix4x4 MakeRotateZMatrix(float radian);
// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translation);

// Vector3の足し算
Vector3& operator+=(Vector3& lhs, const Vector3& rhv);

// EaseInOut関数 02_05 補足
float EaseInOut(float x1, float x2, float t);
float Lerp(float x1, float x2, float t);
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
