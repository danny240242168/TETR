#ifndef PREDICT_H
#define PREDICT_H

#include <list>
#include <vector>
#include"atltypes.h"
#include"afxwin.h"
#include".\Source\.\Library\gameutil.h"

using namespace std;

namespace game_framework {

	class Predict {
	public:
		Predict();
		void blockPredict(int x, int y, int xDistance, int yDistance, int height, int length, int array[4][4]);

		int _x;
		int _y;
		int _xDistance;
		int _yDistance;
		int _color;
		int _array[4][4];
		int _height;
		int _length;
	};
}

#endif