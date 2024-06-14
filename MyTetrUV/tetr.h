#ifndef TETR_H
#define TETR_H

#include <list>
#include <vector>
#include"atltypes.h"
#include"afxwin.h"
#include".\Source\.\Library\gameutil.h"

using namespace std;

namespace game_framework {

	class Tetr {
	public:
		Tetr();
		void setBlock(int i);
		void twist();
		int getArray(int i, int j);

		int _x;
		int _y;
		int _xDistance;
		int _yDistance;
		int _color;
		int _array[4][4];
		int _height;
		int _length;
		int _stage;
	};
}

#endif