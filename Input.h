#pragma once
#include <Windows.h>

#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <wrl.h>

#include "WinApp.h"

//using namespace Microsoft::WRL;

class Input
{
public:
	void Initialize(WinApp* winApp);

	void Update();

	//任意のボタンを押されている
	bool PushKey(BYTE keyNumber);

	//任意のボタンが押された瞬間
	bool TriggerKey(BYTE keyNumber);

	//任意のボタンが離された瞬間


private:
	WinApp* winApp_ = nullptr;

	Microsoft::WRL::ComPtr<IDirectInput8> directInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;

	BYTE key[256] = {};
	BYTE keyPre[256] = {};
};

