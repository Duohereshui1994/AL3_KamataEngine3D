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

	delete _modelSkydemo;

	delete _modelPlayerOBJ;

	delete debugCamera_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete _mapChipField;

	delete _cameraController;
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

	_modelSkydemo = Model::CreateFromOBJ("skydome", true); // 天球モデル

	_modelPlayerOBJ = Model::CreateFromOBJ("playerOBJ", true); // player model

	viewProjection_.Initialize();

	_skydome = new Skydome();

	_skydome->Initialize(_modelSkydemo, &viewProjection_);

	_mapChipField = new MapChipField();
	_mapChipField->LoadMapChipCsv("Resources/block.csv");

	_player = new Player();

	Vector3 playerPosition = _mapChipField->GetMapChipPositionByIndex(1, 18);

	_player->Initialize(_modelPlayerOBJ, &viewProjection_, playerPosition);

	GenerateBlocks();

	_cameraController = new CameraController();
	_cameraController->Initalize(&viewProjection_);

	//====================================================
	// 设置相机的可移动区域
	CameraController::Rect cameraArea;
	//cameraArea.left = 10.0f;
	//cameraArea.right = 188.0f;
	//cameraArea.bottom = 5.0f;
	//cameraArea.top = 100.0f;

	cameraArea.left = 0.0f;
	cameraArea.right = 200.0f;
	cameraArea.bottom = 5.0f;
	cameraArea.top = 100.0f;

	// 设置相机的可移动区域
	_cameraController->SetMoveableArea(cameraArea);
	//====================================================

	_cameraController->SetTarget(_player);
	_cameraController->Reset();

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

	_skydome->Update();

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

	_player->Update();

	_cameraController->Update();
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

	_skydome->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			model_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	_player->Draw();
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

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = _mapChipField->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = _mapChipField->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (_mapChipField->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = _mapChipField->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}
