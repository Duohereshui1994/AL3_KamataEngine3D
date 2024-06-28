#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete modelTitle_;
	delete moji_;
}

void TitleScene::Initialize() {
	viewProjection_.Initialize();

	modelTitle_ = Model::CreateFromOBJ("title", true);

	moji_ = new titleMoji();
	Vector3 position = {640.0f, 100.0f, 0.0f};
	moji_->Initialize(modelTitle_, &viewProjection_, position);
}

void TitleScene::Update() {
	moji_->Update();
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw() { moji_->Draw(); }
