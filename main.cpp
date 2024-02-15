
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "ImGuiManager.h"

#include <vector>

#include "TextureManager.h"

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

    ImGuiManager* imgui = ImGuiManager::Create();
    ImGuiManager::Initialize(winApp_->GetHwnd(), dxCommon_);

    //スプライト共通部の初期化
    spriteCommon_ = new SpriteCommon;
    spriteCommon_->Initialize(dxCommon_);

    //テクスチャマネージャ
    TextureManager::GetInstance()->Initialize(dxCommon_);
    TextureManager::GetInstance()->LoadTexture(L"Resources/mario.jpg");
    TextureManager::GetInstance()->LoadTexture(L"Resources/reimu.png");

    std::vector<Sprite*> sprite_;
    for (int i = 0; i < 5; i++) {
        Sprite* temp = new Sprite();
        if(i%2 == 0)   temp->Initialize(dxCommon_,spriteCommon_, L"Resources/mario.jpg");
        else if(i%2 == 1)  temp->Initialize(dxCommon_, spriteCommon_, L"Resources/reimu.png");
        temp->SetPosition({ (float)i * 120,0 });

        sprite_.push_back(temp);
    }
   

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
        ImGuiManager::NewFrame();
        imgui->ShowDemo();

        //入力
        input_->Update();

        ////移動
        //DirectX::XMFLOAT2 pos = sprite_->GetPosition();
        //pos.x += 0.01f;
        //sprite_->SetPosition(pos);
        ////回転
        //float rot = sprite_->GetRotation();
        //rot += 0.005f;
        //sprite_->SetRotation(rot);
        ////色
        //DirectX::XMFLOAT4 color = sprite_->GetColor();
        //color.x -= 00.1f;
        //    if (color.x < 0) {
        //        color.x = 1.0f;
        //    }
        //    sprite_->SetColor(color);

        ////サイズ
        //DirectX::XMFLOAT2 size = sprite_->GetSize();
        //size.y += 0.01;
        //sprite_->SetSize(size);

        for (int i = 0; i < 5; i++) {
            sprite_[i]->Update();
        }
        //更新前処理
        ImGuiManager::CreateCommand();
        dxCommon_->PreDraw();

        for (int i = 0; i < 5; i++) {
            sprite_[i]->Draw();
        }

        //更新後処理
        ImGuiManager::CommandsExcute(dxCommon_->GetCommandList());
        dxCommon_->PostDraw();

    }




    for (int i = 0; i < 5; i++) {
        delete sprite_[i];
    }
   
    TextureManager::GetInstance()->Finalize();

    delete dxCommon_;

    delete imgui;

    delete input_;

    delete spriteCommon_;
   

    winApp_->Finalize();
    delete winApp_;

    


    return 0;
}
