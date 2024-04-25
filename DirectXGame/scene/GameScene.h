#pragma once

#include "Audio.h"
#include "DebugCamera.h" //DebugCameraを利用するため	为了使用DebugCamera
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include <cassert>




/// <summary>
/// ゲームシーン	GameScene
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ	构造函数
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ		析构函数
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化	初始化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理	每帧处理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画		描画
	/// </summary>
	void Draw();

private: // メンバ変数	成员变量
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr; // 输入
	Audio* audio_ = nullptr; // 音频Audio

	//===================================================================

	bool isDebugCameraActive = false;

	DebugCamera* debugCamera_ = nullptr;

	Model* model_ = nullptr;

	Model* _modelSkydemo = nullptr;

	ViewProjection viewProjection_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;


	//===================================================================

	/// <summary>
	/// ゲームシーン用	GameScene用
	/// </summary>
};
