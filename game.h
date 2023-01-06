#pragma once

namespace Game
{
	//ウィンドウモード設定
	constexpr bool kWindowMode = true;
	//ウィンドウ名
	const wchar_t* const kTitleText = L"テトリス";
	//ウィンドウサイズ
	constexpr int kScreenWidth = 1280;//640
	constexpr int kScreenHeight = 720;//480;//
	//カラーモード
	constexpr int kColorDepth = 32;	//32 or 16
};