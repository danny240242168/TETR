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
#include <experimental/filesystem>
#include <comdef.h>
#include "atltypes.h"
#include "afxwin.h"
#include "tetr.h"

namespace game_framework {

	Tetr::Tetr()
	{
		_x = 1;
		_y = 3;
		_array[4][4];
		_stage = 1;
	}

	int Tetr::getArray(int i, int j)
	{
		return _array[i][j];
	}

	void Tetr::twist() {
		if (_color != 1) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					_array[i][j] = 0;
				}
			}
		}
		switch (this -> _color)
		{
		case 1:
			break;
		case 2:
			if (_stage == 1) {
				_stage = 2;
				_array[0][2] = 2;
				_array[1][1] = 2;
				_array[1][2] = 2;
				_array[2][1] = 2;
				_height = 3;
				_length = 2;
				_xDistance = 0;
				_yDistance = 1;
			}
			else if (_stage == 2) {
				_stage = 3;
				_array[1][0] = 2;
				_array[1][1] = 2;
				_array[2][1] = 2;
				_array[2][2] = 2;
				_height = 2;
				_length = 3;
				_xDistance = 1;
				_yDistance = 0;
			}
			else if (_stage == 3) {
				_stage = 4;
				_array[0][1] = 2;
				_array[1][0] = 2;
				_array[1][1] = 2;
				_array[2][0] = 2;
				_height = 3;
				_length = 2;
				_xDistance = 0;
				_yDistance = 0;
			}
			else {
				_stage = 1;
				_array[0][0] = 2;
				_array[0][1] = 2;
				_array[1][1] = 2;
				_array[1][2] = 2;
				_height = 2;
				_length = 3;
				_xDistance = 0;
				_yDistance = 0;
			}
			break;
		case 3:
			if (_stage == 1) {
				_stage = 2;
				_array[0][1] = 3;
				_array[1][1] = 3;
				_array[1][2] = 3;
				_array[2][2] = 3;
				_height = 3;
				_length = 2;
				_xDistance = 0;
				_yDistance = 1;

			}
			else if (_stage == 2) {
				_stage = 3;
				_array[1][1] = 3;
				_array[1][2] = 3;
				_array[2][0] = 3;
				_array[2][1] = 3;
				_height = 2;
				_length = 3;
				_xDistance = 1;
				_yDistance = 0;

			}
			else if (_stage == 3) {
				_stage = 4;
				_array[0][0] = 3;
				_array[1][0] = 3;
				_array[1][1] = 3;
				_array[2][1] = 3;
				_height = 3;
				_length = 2;
				_xDistance = 0;
				_yDistance = 0;

			}
			else {
				_stage = 1;
				_array[0][1] = 3;
				_array[0][2] = 3;
				_array[1][0] = 3;
				_array[1][1] = 3;
				_height = 2;
				_length = 3;
				_xDistance = 0;
				_yDistance = 0;
			}
			break;
		case 4:
			if (_stage == 1)
			{
				_stage = 2;
				_array[0][2] = 4;
				_array[1][2] = 4;
				_array[2][2] = 4;
				_array[3][2] = 4;
				_height = 4;
				_length = 1;
				_xDistance = 0;
				_yDistance = 2;
			}
			else if (_stage == 2) {
				_stage = 3;
				_array[2][0] = 4;
				_array[2][1] = 4;
				_array[2][2] = 4;
				_array[2][3] = 4;
				_height = 1;
				_length = 4;
				_xDistance = 2;
				_yDistance = 0;
			}
			else if (_stage == 3) {
				_stage = 4;
				_array[0][1] = 4;
				_array[1][1] = 4;
				_array[2][1] = 4;
				_array[3][1] = 4;
				_height = 4;
				_length = 1;
				_xDistance = 0;
				_yDistance = 1;
			}
			else {
				_stage = 1;
				_array[1][0] = 4;
				_array[1][1] = 4;
				_array[1][2] = 4;
				_array[1][3] = 4;
				_height = 1;
				_length = 4;
				_xDistance = 1;
				_yDistance = 0;
			}
			break;
		case 5:
			if (_stage == 1) {
				_stage = 2;
				_array[0][1] = 5;
				_array[1][1] = 5;
				_array[1][2] = 5;
				_array[2][1] = 5;
				_height = 3;
				_length = 2;
				_xDistance = 0;
				_yDistance = 1;
			}
			else if (_stage == 2) {
				_stage = 3;
				_array[1][0] = 5;
				_array[1][1] = 5;
				_array[1][2] = 5;
				_array[2][1] = 5;
				_height = 2;
				_length = 3;
				_xDistance = 1;
				_yDistance = 0;
			}
			else if (_stage == 3) {
				_stage = 4;
				_array[0][1] = 5;
				_array[1][0] = 5;
				_array[1][1] = 5;
				_array[2][1] = 5;
				_height = 3;
				_length = 2;
				_xDistance = 0;
				_yDistance = 0;
			}
			else {
				_stage = 1;
				_array[0][1] = 5;
				_array[1][0] = 5;
				_array[1][1] = 5;
				_array[1][2] = 5;
				_height = 2;
				_length = 3;
				_xDistance = 0;
				_yDistance = 0;
			}
			break;
		case 6:
			if (_stage == 1) {
				_stage = 2;
				_array[0][1] = 6;
				_array[0][2] = 6;
				_array[1][1] = 6;
				_array[2][1] = 6;
				_height = 3;
				_length = 2;
				_xDistance = 0;
				_yDistance = 1;
			}
			else if (_stage == 2) {
				_stage = 3;
				_array[1][0] = 6;
				_array[1][1] = 6;
				_array[1][2] = 6;
				_array[2][2] = 6;
				_height = 2;
				_length = 3;
				_xDistance = 1;
				_yDistance = 0;
			}
			else if (_stage == 3) {
				_stage = 4;
				_array[0][1] = 6;
				_array[1][1] = 6;
				_array[2][0] = 6;
				_array[2][1] = 6;
				_height = 3;
				_length = 2;
				_xDistance = 0;
				_yDistance = 0;
			}
			else {
				_stage = 1;
				_array[0][0] = 6;
				_array[1][0] = 6;
				_array[1][1] = 6;
				_array[1][2] = 6;
				_height = 2;
				_length = 3;
				_xDistance = 0;
				_yDistance = 0;
			}
			break;
		case 7:
			if (_stage == 1) {
				_stage = 2;
				_array[0][1] = 7;
				_array[1][1] = 7;
				_array[2][1] = 7;
				_array[2][2] = 7;
				_height = 3;
				_length = 2;
				_xDistance = 0;
				_yDistance = 1;
			}
			else if (_stage == 2) {
				_stage = 3;
				_array[1][0] = 7;
				_array[1][1] = 7;
				_array[1][2] = 7;
				_array[2][0] = 7;
				_height = 2;
				_length = 3;
				_xDistance = 1;
				_yDistance = 0;
			}
			else if (_stage == 3) {
				_stage = 4;
				_array[0][0] = 7;
				_array[0][1] = 7;
				_array[1][1] = 7;
				_array[2][1] = 7;
				_height = 3;
				_length = 2;
				_xDistance = 0;
				_yDistance = 0;
			}
			else {
				_stage = 1;
				_array[0][2] = 7;
				_array[1][0] = 7;
				_array[1][1] = 7;
				_array[1][2] = 7;
				_height = 2;
				_length = 3;
				_xDistance = 0;
				_yDistance = 0;
			}
			break;
		}
	}

	void Tetr::setBlock(int i)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				_array[i][j] = 0;
			}
		}
		_xDistance = 0;
		_yDistance = 0;
		_height = 2;
		_length = 3;
		_x = 1;
		_y = 3;

		if (i == 1) {
			_array[1][1] = 1;
			_array[1][2] = 1;
			_array[2][1] = 1;
			_array[2][2] = 1;
			_height = 2;
			_length = 2;
			_xDistance = 1;
			_yDistance = 1;
			_color = 1;
		}
		else if (i == 2) {
			_array[0][0] = 2;
			_array[0][1] = 2;
			_array[1][1] = 2;
			_array[1][2] = 2;
			_color = 2;
		}
		else if (i == 3) {
			_array[0][1] = 3;
			_array[0][2] = 3;
			_array[1][0] = 3;
			_array[1][1] = 3;
			_color = 3;
		}
		else if (i == 4) {
			_array[1][0] = 4;
			_array[1][1] = 4;
			_array[1][2] = 4;
			_array[1][3] = 4;
			_height = 1;
			_length = 4;
			_xDistance = 1;
			_yDistance = 0;
			_color = 4;
		}
		else if (i == 5) {
			_array[0][1] = 5;
			_array[1][0] = 5;
			_array[1][1] = 5;
			_array[1][2] = 5;
			_color = 5;
		}
		else if (i == 6) {
			_array[0][0] = 6;
			_array[1][0] = 6;
			_array[1][1] = 6;
			_array[1][2] = 6;
			_color = 6;
		}
		else if (i == 7) {
			_array[0][2] = 7;
			_array[1][0] = 7;
			_array[1][1] = 7;
			_array[1][2] = 7;
			_color = 7;
		}
	}
}