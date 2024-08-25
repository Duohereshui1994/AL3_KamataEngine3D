#include "TitleScene.h"
#include "TextureManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete modelTitle_;
	delete moji_;
	delete sprite_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	viewProjection_.Initialize();

	modelTitle_ = Model::CreateFromOBJ("title", true);

	bgTextureHandle_ = TextureManager::Load("bg.png");
	sprite_ = Sprite::Create(bgTextureHandle_, {0,0});

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

	sprite_->Draw();

	moji_->Draw();

	Model::PostDraw();
}
