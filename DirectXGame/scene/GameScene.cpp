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

	// 解放模型
	//delete model_;

	delete _modelBlock;

	delete _modelGoal;

	delete _modelSkydemo;

	delete _modelPlayerOBJ;

	delete _modelEnemyOBJ;

	// 解放debug相机
	delete debugCamera_;

	// 解放地图块变换
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	// 解放敌人
	for (Enemy* enemy : _enemies) {
		delete enemy;
	}
	_enemies.clear();

	// 解放粒子
	delete deathParticles_;

	// 解放地图块
	delete _mapChipField;

	delete _cameraController;

	delete _goal;
	//===================================================================
}

// 初始化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//=================phase======================
	phase_ = Phase::kPlay;

	//===================================================================

	isDebugCameraActive = false;

	debugCamera_ = new DebugCamera(/*画面横幅*/ WinApp::kWindowWidth, /*画面縦幅*/ WinApp::kWindowHeight);

	//====================Model=========================

	//model_ = Model::Create();

	_modelBlock = Model::CreateFromOBJ("block", true); // block model

	_modelGoal = Model::CreateFromOBJ("goal", true); // goal model

	_modelSkydemo = Model::CreateFromOBJ("skydome", true); // 天球モデル

	_modelPlayerOBJ = Model::CreateFromOBJ("playerOBJ", true); // player model

	_modelEnemyOBJ = Model::CreateFromOBJ("enemyOBJ", true); // enemy model

	_modelParticleOBJ = Model::CreateFromOBJ("particle", true); // enemy model

	//===================viewProjection_初始化===============================

	viewProjection_.Initialize();

	//====================天球==========================

	_skydome = new Skydome();

	_skydome->Initialize(_modelSkydemo, &viewProjection_);

	//======================MapChip==========================

	_mapChipField = new MapChipField();
	_mapChipField->LoadMapChipCsv("Resources/block.csv");

	//======================Goal=========================

	_goal = new Goal();

	Vector3 goalPosition = _mapChipField->GetMapChipPositionByIndex(98, 16);

	_goal->Initialize(_modelGoal, &viewProjection_, goalPosition);

	//======================Player=========================

	_player = new Player();

	Vector3 playerPosition = _mapChipField->GetMapChipPositionByIndex(5, 18);

	_player->Initialize(_modelPlayerOBJ, &viewProjection_, playerPosition, this);

	_player->SetMapChipField(_mapChipField);

	//======================Enemy==============================

	//_enemy = new Enemy();

	// Vector3 enemyPosition = _mapChipField->GetMapChipPositionByIndex(10, 18);

	//_enemy->Initialize(_modelEnemyOBJ, &viewProjection_, enemyPosition);

	// 多个敌人生成
	// 位置
	Vector3 enemyPosition[enemyCount];
	for (uint32_t i = 0; i < enemyCount; ++i) {
		Enemy* newEnemy = new Enemy();
		enemyPosition[i] = _mapChipField->GetMapChipPositionByIndex(0, 18 - i * 2);
		newEnemy->Initialize(_modelEnemyOBJ, &viewProjection_, enemyPosition[i]);
		_enemies.push_back(newEnemy);
	}

	//======================生成粒子====================================

	deathParticles_ = new DeathParticles();

	// deathParticles_->Initialize(_modelParticleOBJ, &viewProjection_, playerPosition);

	//======================生成地图====================================

	GenerateBlocks();

	//========================追踪相机========================================

	_cameraController = new CameraController();
	_cameraController->Initalize(&viewProjection_);

	//====================================================
	// 设置相机的可移动区域
	CameraController::Rect cameraArea;

	cameraArea.left = 21.0f;
	cameraArea.right = 200.0f;
	cameraArea.bottom = 12.0f;
	cameraArea.top = 40.0f;

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

//#ifdef _DEBUG
//	if (input_->TriggerKey(DIK_SPACE)) {
//		isDebugCameraActive = !isDebugCameraActive;
//	}
//	ImGui::Begin("Debug1");
//	ImGui::Text("move y = %f", _player->GetVelocity().y);
//
//	ImGui::End();
//#endif // _DEBUG

	//=======================phase更新================
	if (Input::GetInstance()->PushKey(DIK_BACKSPACE)) {
		_player->OnCollision(_goal);
	}

	ChangePhase();

	//=============================================
	// if (isDebugCameraActive == true) {
	//	debugCamera_->Update();
	//	viewProjection_.matView = debugCamera_->GetViewProjection().matView;
	//	viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
	//	viewProjection_.TransferMatrix();
	//} else {
	//	viewProjection_.UpdateMatrix();
	//}

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

	//=======================天球描画================
	_skydome->Draw();

	//=======================地图块描画================
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			//model_->Draw(*worldTransformBlock, viewProjection_);
			_modelBlock->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	//======================Goal====================

	_goal->Draw();

	//=======================Player描画================
	if (!isDead_) {
		_player->Draw();
	}

	//=======================Enemy描画================

	// if (_enemy != nullptr)
	//	_enemy->Draw();

	for (Enemy* enemy : _enemies) {
		if (enemy != nullptr) {
			enemy->Draw();
		}
	}

	//=======================粒子描画================

	if (deathParticles_ != nullptr) {
		deathParticles_->Draw();
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

void GameScene::CheckAllCollisions() {
#pragma region player and enemy
	// プレイヤーと敌人の衝突判定
	AABB aabb1, aabb2;

	aabb1 = _player->GetAABB();

	for (Enemy* enemy : _enemies) {
		aabb2 = enemy->GetAABB();
		if (IsCollision(aabb1, aabb2)) {
			_player->OnCollision(enemy);
			enemy->OnCollision(_player);
		}
	}
#pragma endregion

#pragma region player and goal
	// プレイヤーとGoalの衝突判定
	AABB aabb3;

	aabb3 = _goal->GetAABB();

	if (IsCollision(aabb1, aabb3)) {
		_player->OnCollision(_goal);
		_goal->OnCollision(_player);
	}

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

void GameScene::ChangePhase() {
	//=======================天球更新================
	_skydome->Update();

	//=======================地图块更新================
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->UpdateMatrix();
		}
	}

	//===================Goal=================

	_goal->Update();

	//=======================Enemy更新================
	//_enemy->Update();

	for (Enemy* enemy : _enemies) {
		enemy->Update();
	}

	//=======================追踪相机更新================
	_cameraController->Update();

	//===========================阶段=============================

	switch (phase_) {

		// 游玩状态
	case Phase::kPlay:
		//=======================Player更新================
		_player->Update();

		//=======================碰撞更新================
		// all collisions check
		CheckAllCollisions();

		//=======================死亡判定================
		if (isDead_) {
			const Vector3& deathPosition = _player->GetWorldTransform().translation_;
			deathParticles_->Initialize(_modelParticleOBJ, &viewProjection_, deathPosition);
			phase_ = Phase::kDeath;
		}
		break;

		// 玩家死亡
	case Phase::kDeath:

		//=======================粒子更新================

		if (deathParticles_ != nullptr) {
			deathParticles_->Update();
		}

		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}

		break;
	}
}
