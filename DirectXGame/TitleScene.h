#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "titleMoji.h"
#include "Player.h"

class TitleScene {
private:
	bool finished_ = false;

	DirectXCommon* dxCommon_ = nullptr;

	uint32_t bgTextureHandle_ = 0;
	Sprite* sprite_ = nullptr;

	Model* modelTitle_ = nullptr;
	ViewProjection viewProjection_;

	titleMoji* moji_ = nullptr;
	Player* player_ = nullptr;

public:
	TitleScene();
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }
};
