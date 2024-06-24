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
#include "Skydome.h"
#include "MapChipField.h"
#include "Player.h"
#include "Enemy.h"
#include "DeathParticles.h"
#include "CameraController.h"




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

	/// <summary>
	/// 检查场景所有碰撞
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 切换阶段
	/// </summary>
	void ChangePhase();

	/// <summary>
	/// flag getter
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }


private: // メンバ変数	成员变量
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr; // 输入
	Audio* audio_ = nullptr; // 音频Audio
	//========================phase切换参数============================
	//flag
	bool isDead_ = false;

	//===================================================================

	bool isDebugCameraActive = false;

	DebugCamera* debugCamera_ = nullptr;

	Model* model_ = nullptr;

	Model* _modelSkydemo = nullptr;

	Model* _modelPlayerOBJ = nullptr;

	Model* _modelEnemyOBJ = nullptr;

	Model* _modelParticleOBJ = nullptr;

	ViewProjection viewProjection_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	//天球
	Skydome* _skydome = nullptr;

	//MapChip
	MapChipField* _mapChipField = nullptr;

	//player
	Player* _player = nullptr;

	//enemy
	//Enemy* _enemy = nullptr;
	std::list<Enemy*> _enemies;//多个敌人
	static inline const uint32_t enemyCount = 3;

	//粒子
	DeathParticles* deathParticles_ = nullptr;


	//追踪camera
	CameraController* _cameraController = nullptr;

	/// <summary>
	/// create blocks
	/// </summary>
	void GenerateBlocks();

	//===============================phase=============================

	enum class Phase {
		kPlay,
		kDeath,
	};

	Phase phase_;

	//===================================================================

	/// <summary>
	/// ゲームシーン用	GameScene用
	/// </summary>
};
