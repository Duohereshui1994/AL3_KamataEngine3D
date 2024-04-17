#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"//DebugCameraを利用するため	为了使用DebugCamera
#include "Player.h"

/// <summary>
/// ゲームシーン	GameScene
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化	初始化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画	
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;	//输入
	Audio* audio_ = nullptr;	//音频Audio

	//===================================================================

	//3Dモデル	3D模型
	
	Model* model_ = nullptr;

	//テクスチャハンドル	图片控制器 存入图片用的
	
	uint32_t textureHandle_ = 0;
	
	//ビュープロジェクション		视图投影
	
	ViewProjection viewProjection_;


	Player* player_ = nullptr;

	//===================================================================

	/// <summary>
	/// ゲームシーン用	GameScene用
	/// </summary>
};
