#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <list>

// GameSceneの前方宣言
class GameScene;

/// <summary>
/// 障害物
/// </summary>
class Obstacle {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	// 半径のゲッター
	//bool isRadius() const { return radius_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnColision();
	// 死亡フラグのゲッター
	bool isDead() const { return isDead_; }

	void isDai(bool isDead);

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	// 半径
	float radius_ = 1.0f;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	// サイズ
	float size;
	// 耐久地
	float hp = 5;
	// 死亡フラグ
	bool isDead_ = false;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;
};