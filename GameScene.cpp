#include "GameScene.h"
using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete modelBall_;
}

void GameScene::Initialize() {

	// 入力のインスタンス取得
	input_ = Input::GetInstance();

	// カメラの初期化
	camera_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	ball_ = new Ball();
	modelBall_ = Model::CreateFromOBJ("Ball", true);
	textureBall_ = TextureManager::Load("Ball/Ball.jpg");
	ball_->Initialize(modelBall_, textureBall_, &camera_);

	reflector_ = new Reflector();
	modelRedlector_ = Model::CreateFromOBJ("Reflector", true);
	textureReflector = TextureManager::Load("Reflector/Reflector.jpg");
	reflector_->Initialize(modelRedlector_, textureReflector, &camera_);
}

void GameScene::Update() {

	#ifdef _DEBUG // デバッグモードのみ有効

	// SPACEキーで切り替え
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDegubCameraActive_) {
			isDegubCameraActive_ = false;
		} else {
			isDegubCameraActive_ = true;
		}
	}

	if (isDegubCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();

		camera_.matView = debugCamera_->GetCamera().matView;             // ビュー行列
		camera_.matProjection = debugCamera_->GetCamera().matProjection; // プロジェクション行列

		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();
	}

#endif // _DEBUG
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	ball_->Draw();

	reflector_->Draw();

	// 3Dモデル描画後処理
	Model::PostDraw();
}