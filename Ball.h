#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class Ball {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//  ワールド変換データ
	WorldTransform worldTransform_;

	Vector3 velocity = {};

	float gravity = -0.02f;

private:

	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//
	Camera* camera_ = nullptr;
	
};