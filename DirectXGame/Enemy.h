#pragma once
#include "WorldTransform.h"
#include "Model.h"

/// <summary>
/// 自キャラ
/// </summary>
class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave     // 離脱する
	};

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;

	// フェーズ
	Phase phase_ = Phase::Approach;
	// 接近フェーズの速度
	Vector3 approachSpeed_ = Vector3{0.0f, 0.1f, -0.5f};
	// 離脱フェーズの速度
	Vector3 leaveSpeed_ = Vector3{-0.1f, 0.0f, 0.0f};
};