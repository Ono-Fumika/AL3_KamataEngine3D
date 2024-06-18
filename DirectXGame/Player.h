#pragma once
#include "Model.h"
#include "input.h"
#include "PlayerBullet.h"
#include <list>
#include <cassert>
#include <Sprite.h>
#include "ViewProjection.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Intialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection & viewProjection);

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

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	// 3Dレティクルのワールド座標
	Vector3 GetWorldPosition3DReticle();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnColision();

	// 親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent);

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

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
	std::list<PlayerBullet*> bullets_;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
};
