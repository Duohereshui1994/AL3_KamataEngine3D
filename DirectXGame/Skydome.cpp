#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	_model = model;
	_worldTransform.Initialize();
	_viewProjection = viewProjection;
	_worldTransform.scale_ = {100.0f, 100.0f, 100.0f};	//天球の大きさを設定　元のサイズは1、今回は100倍に
}

void Skydome::Update() { _worldTransform.UpdateMatrix(); }	//自作関数で天球の位置を更新

void Skydome::Draw() { _model->Draw(_worldTransform, *_viewProjection); }
