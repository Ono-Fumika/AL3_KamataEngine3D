#pragma once
#include "input.h"
#include "Sprite.h"
#include "DirectXCommon.h"

/// <summary>
/// ゲームオーバーシーン
/// </summary>
class GameOver {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameOver();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 終了フラグのゲッター
	bool IsFinished() const { return finished_; }

private:
	// 終了フラグ
	bool finished_ = false;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	Sprite* sprite_ = nullptr;
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;
};
