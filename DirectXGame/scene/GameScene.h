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

	// デバッグカメラ		DebugCamera

	// DebugCamera* debugCamera_ = nullptr;

	// スプライト		Sprite

	// Sprite* sprite_ = nullptr;

	// 3Dモデル	3D模型

	Model* model_ = nullptr;

	// サウンドデータハンドル	声音控制器 加载声音用

	// uint32_t soundDataHandle_ = 0;

	// 音声再生ハンドル	声音播放控制器 控制声音的播放循环等用

	// uint32_t voiceHandle_ = 0;

	// テクスチャハンドル	图片控制器 存入图片用的

	// uint32_t textureHandle_ = 0;

	// ワールドトランスフォーム		WorldTransform 世界变换（坐标）

	// WorldTransform worldTransform_;

	// ビュープロジェクション		视图投影

	 ViewProjection viewProjection_;

	// ImGUIで値を入力する変数 在ImGUI中输入值的变量

	// float inputFloat3[3] = {0, 0, 0};

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	//===================================================================

	/// <summary>
	/// ゲームシーン用	GameScene用
	/// </summary>
};
