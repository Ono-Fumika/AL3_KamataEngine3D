#pragma once
#include "WorldTransform.h"

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減産
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
// 内席
float Dot(const Vector3& v1, const Vector3& v2);
// 長さ（ノルム）
float Length(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3& v);
// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

//行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
//行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
