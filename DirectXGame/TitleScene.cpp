#include "TitleScene.h"

void TitleScene::Initialize() {};

void TitleScene::Update() {

	// スペースキーでタイトルシーン終了
	if (Input::GetInstance()->PushKey(DIK_SPACE)){
		finished_ = true;
	}
};

void TitleScene::Draw(){};
