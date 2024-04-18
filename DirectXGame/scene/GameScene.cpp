#include "GameScene.h"
#include "AxisIndicator.h"   //軸方向を利用するため
#include "ImGuiManager.h"    //ImGuiを利用するため
#include "PrimitiveDrawer.h" //Lineを描画するため
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	//===================================================================

	// デストラクタ
	delete sprite_;

	delete model_;

	delete debugCamera_;

	//===================================================================
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//===================================================================

	// サウンドデータの読み込む
	soundDataHandle_ = audio_->LoadWave("fanfare.wav");

	// 音声再生
	//audio_->PlayWave(soundDataHandle_);
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	// 3Dモデルの生成
	model_ = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(/*画面横幅*/ 1080, /*画面縦幅*/ 720);

	// 軸方向を利用する
	// 表示有効
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	// Line描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(/*&viewProjection_*/ &debugCamera_->GetViewProjection());

	//===================================================================
}

void GameScene::Update() {

	//===================================================================

	// スプライトの移動
	Vector2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);

	// スペースキー押した瞬間 音声停止
	if (input_->TriggerKey(DIK_SPACE)) {
		if (audio_->IsPlaying(voiceHandle_)) {
			audio_->StopWave(voiceHandle_);
		} else {
			voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
		}
	}

	// デバッグテキストの表示
	ImGui::Begin("Debug1"); // Window作成
	ImGui::Text("Kamata Taro %d,%d,%d", 2050, 12, 31);
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("InputFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End(); // Window作成

	// Demo Window
	ImGui::ShowDemoWindow();

	// デバッグカメラの更新
	debugCamera_->Update();

	//===================================================================
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//===================================================================

	// model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// モデルとデバッグカメラ連動
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	//===================================================================

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//===================================================================

	// Lineの描画
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
	PrimitiveDrawer::GetInstance()->DrawLine3d({10, 0, 0}, {10, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {10, 0, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	//===================================================================
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//===================================================================

	sprite_->Draw();

	//===================================================================

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
