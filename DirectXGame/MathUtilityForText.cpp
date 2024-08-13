#include "MathUtilityForText.h"
#include <cassert>
#include <cmath>
#include <numbers>

// Vector3の足し算
Vector3& operator+=(Vector3& lhv, const Vector3& rhv) {
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}

Matrix4x4 Multiply(Matrix4x4 matrix1, Matrix4x4 matrix2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
			}
		}
	}
	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 ans = {0};
	ans.m[0][0] = scale.x;
	ans.m[1][1] = scale.y;
	ans.m[2][2] = scale.z;
	ans.m[3][3] = 1;
	return ans;
}

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 ans = {0};
	ans.m[0][0] = 1;
	ans.m[1][1] = 1;
	ans.m[2][2] = 1;
	ans.m[3][0] = translate.x;
	ans.m[3][1] = translate.y;
	ans.m[3][2] = translate.z;
	ans.m[3][3] = 1;
	return ans;
}

// X
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 ans = {0};
	ans.m[0][0] = 1;
	ans.m[1][1] = cosf(radian);
	ans.m[1][2] = sinf(radian);
	ans.m[2][1] = -sinf(radian);
	ans.m[2][2] = cosf(radian);
	ans.m[3][3] = 1;
	return ans;
}
// Y
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 ans = {0};
	ans.m[0][0] = cosf(radian);
	ans.m[0][2] = -sinf(radian);
	ans.m[1][1] = 1;
	ans.m[2][0] = sinf(radian);
	ans.m[2][2] = cosf(radian);
	ans.m[3][3] = 1;
	return ans;
}
// Z
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 ans = {0};
	ans.m[0][0] = cosf(radian);
	ans.m[0][1] = sinf(radian);
	ans.m[1][0] = -sinf(radian);
	ans.m[1][1] = cosf(radian);
	ans.m[2][2] = 1;
	ans.m[3][3] = 1;
	return ans;
}

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translation) {
	Matrix4x4 ans;
	// スケール
	Matrix4x4 ScallMat = {scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 1};
	// 拡大＊回転＊平行移動でワールド変換行列に
	//Matrix4x4 ansMat = MatrixMultiply(ScaleMat, RotateMatAll);
	//ansMat = MatrixMultiply(ansMat, TranslateMat);

	ans = Multiply(Multiply(MakeScaleMatrix(scale), Multiply(Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)), MakeRotateZMatrix(rotate.z))), MakeTranslateMatrix(translation));
	return ans;
}

// アフィン変換行列の作成
Matrix4x4 PlayerAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	// エラー対策（使用しない）
	Vector3 dm = scale;

	// 回転＊平行移動だけをワールド変換行列に
	Matrix4x4 ansMat = Multiply(MakeRotateYMatrix(rotate.y), MakeTranslateMatrix(translate));

	return ansMat;
}

// EaseInOut関数 02_05 p22
float EaseInOut(float x1, float x2, float t) {
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, easedT);
}

float Lerp(float x1, float x2, float t) { return (1.0f - t) * x1 + t * x2; }
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)); }

// 2項演算 Vector3の足し算  02_06 補足p5
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

// 代入演算子オーバーロード
//  Vector3の掛け算 02_06 補足p11
Vector3& operator*=(Vector3 v, float s) {
	// TODO: return ステートメントをここに挿入します
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

// 二項演算子オーバーロード
//  Vector3の掛け算 02_06 補足p12
const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && // x軸
	       (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && // y軸
	       (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);   // z軸
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 ans;
	ans.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	ans.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	ans.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	ans.x /= w;
	ans.y /= w;
	ans.z /= w;

	return ans;
}
