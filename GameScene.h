#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;
#include "vector"

// ゲームシーン
class GameScene {
public:
	GameScene();
	~GameScene();
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	
};