#pragma once
#include "Model.h"
#include "WorldTransform.h"
class Player {
public:
	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画	
	/// </summary>
	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
};
