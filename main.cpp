
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "SpriteCommon.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    //ポインタ置き場
    Input* input_ = nullptr;
    WinApp* winApp_ = nullptr;
    DirectXCommon* dxCommon_ = nullptr;
    SpriteCommon* spriteCommon_ = nullptr;
 

//WindowsAPI初期化処理
    winApp_ = new WinApp();
    winApp_->Initialize();

 //DirectX初期化処理
    dxCommon_ = new DirectXCommon();
    dxCommon_->Initialize(winApp_);

    //スプライト共通部の初期化
    spriteCommon_ = new SpriteCommon;
    spriteCommon_->Initialize(dxCommon_);


    Sprite* sprite_ = new Sprite();
    sprite_->Initialize(dxCommon_, spriteCommon_);

    // DirectX初期化処理　ここまで
#pragma endregion
    //生成、初期化
    input_ = new Input();
    input_->Initialize(winApp_);

    // ゲームループ
    while (true) {
        if (winApp_->Update() == true) {
            break;
        }

        //入力
        input_->Update();

        dxCommon_->PreDraw();

        sprite_->Draw();

        dxCommon_->PostDraw();

    }

    delete input_;
    delete dxCommon_;

    winApp_->Finalize();
    delete winApp_;

    delete spriteCommon_;
    delete sprite_;

    return 0;
}
