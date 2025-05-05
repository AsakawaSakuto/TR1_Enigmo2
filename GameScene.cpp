#include "GameScene.h"
using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	
}

void GameScene::Initialize() {

}

void GameScene::Update() {

}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());



	// 3Dモデル描画後処理
	Model::PostDraw();
}