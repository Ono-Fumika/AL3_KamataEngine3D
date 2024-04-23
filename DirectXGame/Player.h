#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "input.h"
#include "PlayerBullet.h"

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Intialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection&viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	
	// ImGuiで値を入力する変数
	float player[3] = {0, 0, 0};

	// 弾
	PlayerBullet* bullet_ = nullptr;
};
