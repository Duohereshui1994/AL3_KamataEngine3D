#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "titleMoji.h"
class TitleScene {
private:
	bool finished_ = false;

	Model* modelTitle_ = nullptr;
	ViewProjection viewProjection_;

	titleMoji* moji_ = nullptr;

public:
	TitleScene();
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }
};
