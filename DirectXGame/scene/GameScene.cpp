#include "GameScene.h"
#include "AxisIndicator.h"   //軸方向を利用するため	想要使用轴方向必须导入
#include "ImGuiManager.h"    //ImGuiを利用するため	想要使用ImGui必须导入
#include "PrimitiveDrawer.h" //Lineを描画するため		想要画线必须导入
#include "TextureManager.h"

// 构造函数
GameScene::GameScene() {}

// 析构函数
GameScene::~GameScene() {
	//===================================================================

	delete model_;

	delete debugCamera_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	//===================================================================
}

// 初始化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//===================================================================

	isDebugCameraActive = false;

	debugCamera_ = new DebugCamera(/*画面横幅*/ WinApp::kWindowWidth, /*画面縦幅*/ WinApp::kWindowHeight);

	model_ = Model::Create();

	viewProjection_.Initialize();

	const uint32_t kNumberBlockHorizontal = 20;
	const uint32_t kNumberBlockVertical = 10;
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	int map[kNumberBlockVertical][kNumberBlockHorizontal] = {
	    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	};

	worldTransformBlocks_.resize(kNumberBlockVertical);
	for (uint32_t i = 0; i < kNumberBlockVertical; ++i) {
		worldTransformBlocks_[i].resize(kNumberBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumberBlockVertical; ++i) {
		for (uint32_t j = 0; j < kNumberBlockHorizontal; ++j) {
			if (map[i][j] == 1) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			}
		}
	}

	//===================================================================
}

// Scene更新
void GameScene::Update() {

	//===================================================================

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive = !isDebugCameraActive;
	}
	ImGui::Begin("Debug1");
	ImGui::Text("Press Space To Change Camera");

	ImGui::Text("isDebugCameraActive = %d", isDebugCameraActive);
	ImGui::End();
#endif // _DEBUG

	if (isDebugCameraActive == true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->UpdateMatrix();
		}
	}
	//===================================================================
}

// Scene描画
void GameScene::Draw() {

	// コマンドリストの取得	获取命令列表
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理	背景Sprite绘制预处理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる	可以在这里下面追加背景Sprite的描绘处理
	/// </summary>

	// スプライト描画後処理	Sprite绘制后处理
	Sprite::PostDraw();
	// 深度バッファクリア		深度缓冲区清除
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理	三维对象绘制预处理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる	可以在此处下面添加三维对象的绘制处理
	/// </summary>

	//===================================================================

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			model_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	//===================================================================

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理	前景精灵绘制预处理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる		可以在这里追加前景精灵的描绘处理
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
