#include "DrawFunctions.h"
#include <DxLib.h>
#include <cassert>

namespace my {
	/// <summary>
	/// グラフィックをロードする
	/// </summary>
	/// <param name="path">画像ファイルのパス</param>
	/// <returns>画像のハンドル</returns>
	int MyLoadGraph(const TCHAR* path)
	{
		int handle = LoadGraph(path);
		assert(handle >= 0);//成り立たないときはクラッシュ
		return handle;
	}
}//end of namespace my