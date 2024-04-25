#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	_model = model;
	_worldTransform.Initialize();
	_viewProjection = viewProjection;
}

void Skydome::Update() { _worldTransform.TransferMatrix(); }

void Skydome::Draw() { _model->Draw(_worldTransform, *_viewProjection); }
