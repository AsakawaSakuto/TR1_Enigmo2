#include "Reflector.h"

void Reflector::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {
	// NULLポインタチェック
#ifdef DEBUG_
	assert(model);
#endif // DEBUG_
	// 引数をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	//
	camera_ = camera;
}

void Reflector::Update() {}

void Reflector::Draw() { 
	model_->Draw(worldTransform_, *camera_, textureHandle_);
}