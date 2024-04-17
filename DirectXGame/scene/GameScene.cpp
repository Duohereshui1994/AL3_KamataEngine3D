#include "GameScene.h"
#include "AxisIndicator.h"   //軸方向を利用するため	想要使用轴方向必须导入
#include "ImGuiManager.h"    //ImGuiを利用するため	想要使用ImGui必须导入
#include "PrimitiveDrawer.h" //Lineを描画するため		想要画线必须导入
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene() {}


GameScene::~GameScene() {
	//===================================================================

	delete model_;

	delete player_;

	//===================================================================
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//===================================================================

	// ファイル名を指定してテクスチャを読み込む	
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデルの生成		生成3D模型
	model_ = Model::Create();

	// ビュープロジェクションの初期化	初始化投影视图
	viewProjection_.Initialize();

	player_ = new Player();

	player_->Initialize(model_, textureHandle_, &viewProjection_);

	//===================================================================
}


void GameScene::Update() {

	//===================================================================

	player_->Update();

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

	player_->Draw();

	//===================================================================

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理	前景精灵绘制预处理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
