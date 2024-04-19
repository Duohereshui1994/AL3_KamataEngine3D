#include "GameScene.h"
#include "AxisIndicator.h"   //軸方向を利用するため	想要使用轴方向必须导入
#include "ImGuiManager.h"    //ImGuiを利用するため	想要使用ImGui必须导入
#include "PrimitiveDrawer.h" //Lineを描画するため		想要画线必须导入
#include "TextureManager.h"
#include <cassert>

//构造函数
GameScene::GameScene() {}

//析构函数
GameScene::~GameScene() {
	//===================================================================

	// 释放sprite

	//delete sprite_;

	//释放模型 model

	//delete model_;

	//释放DebugCamera

	//delete debugCamera_;

	//===================================================================
}

//初始化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//===================================================================

	// デバッグカメラの生成	生成DebugCamera
	
	//debugCamera_ = new DebugCamera(/*画面横幅*/ WinApp::kWindowWidth, /*画面縦幅*/ WinApp::kWindowHeight);

	
	// サウンドデータの読み込む	读取音频文件

	//soundDataHandle_ = audio_->LoadWave("fanfare.wav");

	// 音声再生	播放声音 两个都是能播放声音的，建议使用第二种

	//audio_->PlayWave(soundDataHandle_);
	//voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);//第二个参数是是否循环播放

	// ファイル名を指定してテクスチャを読み込む		读取图片

	//textureHandle_ = TextureManager::Load("mario.jpg");

	// スプライトの生成	生成Sprite
	
	//sprite_ = Sprite::Create(textureHandle_, /*左上角坐标*/{100, 50});

	// 3Dモデルの生成		生成3D模型

	//model_ = Model::Create();

	// ワールドトランスフォームの初期化	初始化世界转换（坐标）

	//worldTransform_.Initialize();

	// ビュープロジェクションの初期化	初始化投影视图

	//viewProjection_.Initialize();

	// 軸方向を利用する 轴方向利用相关
	
	// 表示有効	显示轴方向表示

	//AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向が参照するビュープロジェクションを指定する（アドレス渡し）	设定轴方向的参照投影视角（传递地址）
	
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	// Line描画が参照するビュープロジェクションを指定する（アドレス渡し） 设定画线功能的参照投影视角（传递地址）

	//PrimitiveDrawer::GetInstance()->SetViewProjection(/*&viewProjection_*/ &debugCamera_->GetViewProjection());

	//===================================================================
}

//Scene更新
void GameScene::Update() {

	//===================================================================

	// スプライトの移動
	//Vector2 position = sprite_->GetPosition();

	//position.x += 2.0f;
	//position.y += 1.0f;

	//sprite_->SetPosition(position);

	// スペースキー押した瞬間 音声停止		按下Space键 停止播放声音

	//这段是读取输入按键
	//if (input_->TriggerKey(DIK_SPACE)) {
	//
	//	//停止播放声音
	//	audio_->StopWave(voiceHandle_);
	//}

	// デバッグテキストの表示		显示DebugText

	//ImGui::Begin("Debug1"); // Window作成 的起始点
	//ImGui::Text("Kamata Taro %d,%d,%d", 2050, 12, 31);//显示文字，和Printf相似
	//ImGui::InputFloat3("InputFloat3", inputFloat3);//输入框
	//ImGui::SliderFloat3("InputFloat3", inputFloat3, 0.0f, 1.0f);//滑块调整框
	//ImGui::End(); // Window作成 的结束点

	// Demo Window	显示ImGui的Demo Window

	//ImGui::ShowDemoWindow();

	// デバッグカメラの更新	更新DebugCamera

	//debugCamera_->Update();

	//===================================================================
}

//Scene描画
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

	//普通描画3D模型
	
	// model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// モデルとデバッグカメラ連動		3D模型和DebugCamera的连动
	
	//model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	//===================================================================

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//===================================================================

	//Lineの描画		画线 画线必须在3D对象描画后处理后调用 上限4096根线

	//PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});//起点XYZ坐标,终点XYZ坐标，RGBA
	//PrimitiveDrawer::GetInstance()->DrawLine3d({10, 0, 0}, {10, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
	//PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {10, 0, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	//===================================================================
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理	前景精灵绘制预处理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる		可以在这里追加前景精灵的描绘处理
	/// </summary>

	//===================================================================

	//描画Sprite

	//sprite_->Draw();

	//===================================================================

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
