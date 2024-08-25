#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete modelTitle_;
	delete moji_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	viewProjection_.Initialize();

	modelTitle_ = Model::CreateFromOBJ("title", true);

	bgTextureHandle_ = TextureManager::Load("title.png");

	moji_ = new titleMoji();
	Vector3 position = {0.0f, 0.0f, 0.0f};
	moji_->Initialize(modelTitle_, &viewProjection_, position);
}

void TitleScene::Update() {
	moji_->Update();
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {
	// コマンドリストの取得	获取命令列表
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	moji_->Draw();
	Model::PostDraw();
}
