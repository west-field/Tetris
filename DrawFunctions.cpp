#include "DrawFunctions.h"
#include <DxLib.h>
#include <cassert>

namespace my {
	/// <summary>
	/// �O���t�B�b�N�����[�h����
	/// </summary>
	/// <param name="path">�摜�t�@�C���̃p�X</param>
	/// <returns>�摜�̃n���h��</returns>
	int MyLoadGraph(const TCHAR* path)
	{
		int handle = LoadGraph(path);
		assert(handle >= 0);//���藧���Ȃ��Ƃ��̓N���b�V��
		return handle;
	}
}//end of namespace my