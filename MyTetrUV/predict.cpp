
#include "stdafx.h"
#include "../Core/game.h"
#include "../Core/MainFrm.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include ".\Source\.\Library\audio.h"
#include ".\Source\.\Library\gameutil.h"
#include ".\Source\.\Library\gamecore.h"
#include "Shlwapi.h"
#include "../Game/config.h"
#include "../Game/mygame.h"
#include <filesystem>
#include <experimental/filesystem> // Header file for pre-standard implementation
#include <comdef.h>
#include"atltypes.h"
#include"afxwin.h"
#include"predict.h"

namespace game_framework {

	Predict::Predict()
	{

	}

	void Predict::blockPredict(int x, int y, int xDistance, int yDistance, int height, int length, int array[4][4])
	{
		_x = x;
		_y = y;
		_xDistance = xDistance;
		_yDistance = yDistance;
		_height = height;
		_length = length;
		for (int i = 0; i < 4; i++) {
			for (int l = 0; l < 4; l++) {
				if (array[i][l] == 0) {
					_array[i][l] = 0;
				}
				else {
					_array[i][l] = 8;
				}
			}
		}
	}
}