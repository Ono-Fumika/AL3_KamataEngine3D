#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// ゲームシーン
/// </summary>
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3& translation_, Vector3& rotation_);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//// ビュープロジェクションを取得
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 移動する数値
	Vector3 velocity_ = {0.0f, 0.0f, 0.1f};
	// 回転する数値
	Vector3 rotate_ = {0.0f, 0.0f, 0.0f};

};