#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "EnemyBullet.h"
#include <list>

/// <summary>
/// 自キャラ
/// </summary>
class Enemy {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

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

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave     // 離脱する
	};

	// 接近フェーズ初期化
	void ApproachInitialize();

	//  接近フェーズの更新関数
	void ApproachUpdate();

	// 発射間隔
	static const int kFireInterval = 60;

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
	Vector3 approachSpeed_ = Vector3{0.0f, 0.0f, 0.0f};
	// 離脱フェーズの速度
	Vector3 leaveSpeed_ = Vector3{0.0f, 0.0f, 0.0f};

	// 弾
	std::list<EnemyBullet*> bullets_;
	// 発射タイマー
	int32_t fireTimer = 0;
};