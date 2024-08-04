#include "GameOver.h"
#include "TextureManager.h"


GameOver::~GameOver() { delete sprite_; }

void GameOver::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("over.png");
	// スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {0, 0});
}

void GameOver::Update() {
	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// スペースキーでシーン終了
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || joyState.Gamepad.wButtons) {
		finished_ = true;
	}
}

void GameOver::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
}
