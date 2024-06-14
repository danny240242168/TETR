#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <iostream>
#include <random>
#include"../../tetr.h"
#include"../../predict.h"
#include <chrono>
#include <thread>
#include <string>

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
	CAudio *music = CAudio::Instance();
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	if (game_mode == 1) {
		speed = 30;
	}
	else if (game_mode == 2) {
		speed = 30 - (blitz_level - 1) * 5;
		if (speed <= 4) {
			speed = 4;
		}
	}
	else if (game_mode == 3) {
		speed = 30 - ((zen_score - zen_score_limit) / zen_speed_control);
	}
	else if (game_mode == 4) {
		speed = 28 - ((clear_rows / 10 + 1) - 1) * 4;
		if (speed <= 4) {
			speed = 4;
		}
	}

	down_time++;
	if (game_state == 1) {
		if (down_time % speed == 0) {
			down_time = 0;
			BlocksDown();
		}

		if (down_time % 4 == 0) {
			if (blocks_right_key == 1) {
				BlocksRight();
			}

			if (blocks_left_key == 1) {
				BlocksLeft();
			}

			if (blocks_down_key == 1) {
				BlocksDown();
			}
		}
	}
	if (game_fail_judge == 1 && game_mode != 3) {
		game_state = 3;
	}

	else if (game_fail_judge == 1 && game_mode == 3) {
		gameover_clock = clock();
		for (int i = 0; i < 23; i++) {
			for (int l = 0; l < 10; l++) {
				map[i][l] = 0;
				map_base_blocks[i][l] = 0;
			}
		}
		ShowBlocksNow();
		BlocksPredict();
		game_fail_judge = 0;
	}

	if (game_mode == 2) {
		if (clear_rows >= blitz_level * 2 + 1) {
			clear_rows = clear_rows - (blitz_level * 2 + 1);
			blitz_level++;
		}
	}

	if (game_mode == 3) {
		if (zen_score > 10000) {
			zen_level = 2;
			zen_speed_control = 990;
			zen_score_limit = 10000;
		}

		if (zen_score > 25000) {
			zen_level = 3;
			zen_speed_control = 1320;
			zen_score_limit = 25000;
		}

		if (zen_score > 45000) {
			zen_level = 4;
			zen_speed_control = 1650;
			zen_score_limit = 45000;
		}

		if (zen_score > 70000) {
			zen_level = 5;
			zen_speed_control = 1970;
			zen_score_limit = 70000;
		}

		if (zen_score > 95000) {
			zen_level = 6;
			zen_speed_control = 2300;
			zen_score_limit = 95000;
		}

	}

	if (game_mode == 2 && two_minutes - (clock() - init_time) < 16) {
		game_state = 2;
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{

	for (int i = 0; i < 6; i++) {
		CreateBlocks();
	}
	countdown_begin = clock();
	game_state = -1;

	mode1.LoadBitmapByString({ "resources/mode1.bmp" });
	mode1.SetTopLeft(0, 0);
	mode2.LoadBitmapByString({ "resources/mode2.bmp" });
	mode2.SetTopLeft(0, 0);
	mode3.LoadBitmapByString({ "resources/mode3.bmp" });
	mode3.SetTopLeft(0, 0);
	mode4.LoadBitmapByString({ "resources/mode4.bmp" });
	mode4.SetTopLeft(0, 0);
	mode.LoadBitmapByString({ "resources/choose_mode.bmp" });
	mode.SetTopLeft(0, 0);
	finish.LoadBitmapByString({ "resources/Finish.bmp" }, RGB(255, 255, 255));
	finish.SetTopLeft(580, 270);
	game_block.LoadBitmapByString({ "resources/game_block.bmp" }, RGB(0, 0, 0));
	game_block.SetTopLeft(638, 267);
	GameOver.LoadBitmapByString({ "resources/gameover.bmp" });
	background2.LoadBitmapByString({ "resources/background.bmp" });
	background2.SetTopLeft(0, 0);
	score.LoadBitmapByString({ "resources/score.bmp" });
	score.SetTopLeft(0, 0);
	fourty_lines_longButton.LoadBitmapByString({ "resources/40lines_long_button.bmp" });
	fourty_lines_longButton.SetTopLeft(395, 119);
	blitz_longButton.LoadBitmapByString({ "resources/blitz_long_button.bmp" });
	blitz_longButton.SetTopLeft(395, 267);
	zen_originButton.LoadBitmapByString({ "resources/zen_origin_button.bmp" });
	zen_originButton.SetTopLeft(395, 416);
	zen_longButton.LoadBitmapByString({ "resources/zen_long_button.bmp" });
	zen_longButton.SetTopLeft(395, 416);
	custom_longButton.LoadBitmapByString({ "resources/custom_long_button.bmp" });
	custom_longButton.SetTopLeft(395, 565);
	game_block_left.LoadBitmapByString({ "resources/game_block_left.bmp" }, RGB(0, 0, 0));
	game_block_right.LoadBitmapByString({ "resources/game_block_right.bmp" }, RGB(0, 0, 0));
	game_block_1_1.LoadBitmapByString({ "resources/game_block_1_1.bmp" }, RGB(0, 0, 0));
	game_block_1_2.LoadBitmapByString({ "resources/game_block_1_2.bmp" }, RGB(0, 0, 0));
	game_block_2_1.LoadBitmapByString({ "resources/game_block_2_1.bmp" }, RGB(0, 0, 0));
	game_block_2_2.LoadBitmapByString({ "resources/game_block_2_2.bmp" }, RGB(0, 0, 0));
	game_block_2_3.LoadBitmapByString({ "resources/game_block_2_3.bmp" }, RGB(0, 0, 0));
	game_block_2_4.LoadBitmapByString({ "resources/game_block_2_4.bmp" }, RGB(0, 0, 0));

	CreateMainMap();
	CreateNextMap();
	CreateSaveMap();

	BlocksPredict();
	ShowBlocksNow();
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB) {
		if (game_mode == 1) {
			finish_this_game = 5;
			if (clear_rows >= finish_this_game) {
				game_state = 2;
			}
			CDC *pDC = CDDraw::GetBackCDC();
			CTextDraw::ChangeFontLog(pDC, 28, "微軟正黑體", RGB(255, 255, 255), 50);
			CTextDraw::Print(pDC, 735, 747, "/5");
			CDDraw::ReleaseBackCDC();
		}

		if (game_mode == 2) {
			if (blitz_level < 6) {
				blitz_level++;
			}
		}
	}
	if (nChar == VK_RIGHT) {
		if (game_state == 1) {
			blocks_right_key = 1;
		}
	}
	if (nChar == VK_LEFT) {
		if (game_state == 1) {
			blocks_left_key = 1;
		}
	}
	if (nChar == VK_DOWN) {
		if (game_state == 1) {
			blocks_down_key = 1;
		}
	}

	if (nChar == VK_SPACE || nChar == VK_PROCESSKEY) {
		if (game_state == 1) {
			BlocksSpace();
		}
	}

	if (nChar == VK_UP) {
		if (game_state == 1) {
			BlocksTurn();
		}
	}

	if (nChar == VK_SHIFT) {
		if (game_state == 1) {
			BlocksShift();
		}
	}

	if (nChar == VK_ESCAPE) {
		ZenTetrVector.clear();
		ZenTetrArray.clear();
		if (game_mode == 3) {
			for (int i = 0; i < 23; i++) {
				for (int l = 0; l < 10; l++) {
					if (map_base_blocks[i][l] == 9) {
						zen_blocks[i][l] = map[i][l];
					}
				}
			}
			zen_score_temp = zen_score;
			zen_level_temp = zen_level;
		}

		for (int i = 0; i < 6; i++) {
			ZenTetrArray.push_back(TetrArray[begin_for_array + i]);
			ZenTetrVector.push_back(TetrVector[begin_for_array + i]);
		}

		state = 1;
		Init();
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT) {
		if (game_state == 1) {
			blocks_right_key = 0;
		}
	}
	if (nChar == VK_LEFT) {
		if (game_state == 1) {
			blocks_left_key = 0;
		}
	}
	if (nChar == VK_DOWN) {
		if (game_state == 1) {
			blocks_down_key = 0;
		}
	}
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	if (state == 1 && point.y < 245 && point.y > 115 && point.x < 1914 && point.x > 397) {
		game_mode = 1;
		state = 2;
	}
	if (state == 1 && point.y < 397 && point.y > 269 && point.x < 1914 && point.x > 397) {
		game_mode = 2;
		state = 2;
	}
	if (state == 1 && point.y < 539 && point.y > 323 && point.x < 1914 && point.x > 397) {
		game_mode = 3;
		state = 2;
	}
	if (state == 1 && point.y < 723 && point.y > 563 && point.x < 1914 && point.x > 397) {
		game_mode = 4;
		state = 2;
	}//---------------
	if (state == 2 && game_mode == 1 && point.y < 393 && point.y > 310 && point.x < 1610 && point.x > 1330) {
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
		ChangeNextMap();
		ChangeSaveMap();
		//zen_before = true;
		state = 3;
	}
	if (state == 2 && game_mode == 2 && point.y < 456 && point.y > 360 && point.x < 1670 && point.x > 1350) {
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
		ChangeNextMap();
		ChangeSaveMap();
		//zen_before = true;
		state = 3;
	}
	if (state == 2 && game_mode == 3 && point.y < 435 && point.y > 340 && point.x < 1675 && point.x > 1350) {
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
		if (zen_before) {
			TetrVector.clear();
			TetrArray.clear();
			for (int i = 0; i < 23; i++) {
				for (int l = 0; l < 10; l++) {
					map[i][l] = zen_blocks[i][l];
					if (zen_blocks[i][l] != 0) {
						map_base_blocks[i][l] = 9;
					}
					zen_blocks[i][l] = 0;
				}
			}
			for (int i = 0; i < 6; i++) {
				TetrArray.push_back(ZenTetrArray[i]);
				TetrVector.push_back(ZenTetrVector[i]);
			}
			ZenTetrArray.clear();
			ZenTetrVector.clear();
			BlocksPredict();
			BlocksDown();
		}
		score_times = zen_score_temp;
		zen_score = zen_score_temp;
		zen_level = zen_level_temp;

		ChangeNextMap();
		ChangeSaveMap();

		ShowBlocksNow();
		state = 3;
		game_state = -1;
		zen_before = true;
	}
	//偵測在mode4下，init_board_width的變化
	//if (state == 2 && game_mode == 4 && point.y < 456 && point.y > 360 && point.x < 1670 && point.x > 1350) {
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
		//state = 3;
	//}
	if (state == 2 && game_mode == 4 && point.y < 423 && point.y > 404 && point.y > 404 && point.x < 1628 && point.x > 1609) {//up
		if (init_board_width < 10) {
			init_board_width++;
		}
	}
	if (state == 2 && game_mode == 4 && point.y < 423 && point.x < 1645 && point.x > 1628) {//down
		if (init_board_width > 4) {
			init_board_width--;
		}
	}
	if (state == 2 && game_mode == 4 && point.y < 280 && point.y > 208 && point.x < 1673 && point.x > 1383) {
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
		ChangeNextMap();
		ChangeSaveMap();
		//zen_before = true;
		finish_this_game = 150;
		state = 3;//暫時
	}
	if (state == 2 && point.y < 156 && point.y > 98 && point.x < 148 && point.x > 0) {
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
		state = 1;
		Init();
	}
	if (game_state == 3 && point.y < 210 && point.y > 118 && point.x < 1885 && point.x > 400) {//失敗結束retry
		state = 3;
		Retry();
	}
	if (game_state == 2 && point.y < 156 && point.y > 98 && point.x < 148 && point.x > 0) {//成功Back
		state = 1;
		game_state = 1;
		Init();
	}
	if (game_state == 2 && point.y < 474 && point.y > 386 && point.x < 1672 && point.x > 1382) {//成功Again
		state = 3;
		Retry();
	}
	if (game_state == 3 && point.y < 319 && point.y > 227 && point.x < 1885 && point.x > 400) {
		state = 1;
		Init();
	}//----
}
void CGameStateRun::Init() {
	for (int i = 0; i < 23; i++) {
		for (int l = 0; l < 10; l++) {
			map[i][l] = 0;
			map_base_blocks[i][l] = 0;
		}
	}

	clear_rows = 0;
	TetrVector.clear();
	TetrArray.clear();
	begin_for_array = 0;

	for (int i = 0; i < 6; i++) {
		CreateBlocks();
	}

	TetrVector[begin_for_array]._x = 1;
	TetrVector[begin_for_array]._y = 4;

	game_state = 1;
	ShowBlocksNow();
	BlocksPredict();
	BlocksDown();
	game_fail_judge = 0;
	score_temp = 0;
	blocks_save = 0;

	countdown_number = 5;
	init_time = clock();
	game_state = -1;
	finish_this_game = 40;
	init_time_initialize_count = 0;
	two_minutes = 120000;

	two_minutes_score = 0;
	score_times = 0;
	blitz_level = 1;

	countdown_begin = clock() - 900;
	check_go = 0;
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	if (state == 1 && point.y < 245 && point.y > 115 && point.x < 1914 && point.x > 397) {
		fourty_lines_longButton.SetTopLeft(304, 119);
	}
	else {
		fourty_lines_longButton.SetTopLeft(395, 119);
	}
	if (state == 1 && point.y < 397 && point.y > 269 && point.x < 1914 && point.x > 397) {
		blitz_longButton.SetTopLeft(304, 267);
	}
	else {
		blitz_longButton.SetTopLeft(395, 267);
	}
	if (state == 1 && point.y < 546 && point.y > 416 && point.x < 1914 && point.x > 397) {//zen mode
		mouseOnZenButtonCheck = 1;
		zen_longButton.SetTopLeft(304, 416);
	}
	else {
		mouseOnZenButtonCheck = 0;
		zen_longButton.SetTopLeft(395, 416);
	}
	if (state == 1 && point.y < 723 && point.y > 563 && point.x < 1914 && point.x > 397) {
		custom_longButton.SetTopLeft(304, 565);
	}
	else {
		custom_longButton.SetTopLeft(395, 565);
	}
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{

}
void CGameStateRun::OnShow() {
	if (state == 1) {
		mode.ShowBitmap();
		fourty_lines_longButton.ShowBitmap();
		blitz_longButton.ShowBitmap();
		zen_originButton.ShowBitmap();
		if (mouseOnZenButtonCheck == 1) {
			zen_longButton.ShowBitmap();
			zen_originButton.UnshowBitmap();
		}
		else {
			zen_longButton.UnshowBitmap();
			zen_originButton.ShowBitmap();
		}
		custom_longButton.ShowBitmap();
	}
	else if (state == 2) {
		if (game_mode == 1) {
			mode1.ShowBitmap();
		}
		else if (game_mode == 2) {
			mode2.ShowBitmap();
		}
		else if (game_mode == 3) {
			mode3.ShowBitmap();
		}
		else if (game_mode == 4) {
			mode4.ShowBitmap();
			//數字顯示
			CDC *pDC = CDDraw::GetBackCDC();
			CTextDraw::ChangeFontLog(pDC, 20, "微軟正黑體", RGB(247, 193, 4), 50);
			sprintf(s_init_board_width, "%d", init_board_width);	
			CTextDraw::Print(pDC, 462, 395, s_init_board_width);
			CDDraw::ReleaseBackCDC();
		}
	}
	else if (state == 3) {//==========================================================================================
		background2.ShowBitmap();
		if (game_mode == 4) {
			game_block_2_1.SetTopLeft(904, 267);
			game_block_2_2.SetTopLeft(968, 267);
			if (init_board_width == 4) {
				game_block_left.SetTopLeft(734, 267);
				game_block_right.SetTopLeft(1032, 267);
				game_block_2_1.ShowBitmap();
				game_block_2_2.ShowBitmap();
				game_block_left.ShowBitmap();
				game_block_right.ShowBitmap();
			}
			else if (init_board_width == 5) {
				game_block_1_1.SetTopLeft(872, 267);
				game_block_left.SetTopLeft(702, 267);
				game_block_right.SetTopLeft(1032, 267);
				game_block_1_1.ShowBitmap();
				game_block_2_1.ShowBitmap();
				game_block_2_2.ShowBitmap();
				game_block_left.ShowBitmap();
				game_block_right.ShowBitmap();
			}
			else if (init_board_width == 6) {
				game_block_1_1.SetTopLeft(872, 267);
				game_block_1_2.SetTopLeft(1032, 267);
				game_block_left.SetTopLeft(702, 267);
				game_block_right.SetTopLeft(1064, 267);
				game_block_1_1.ShowBitmap();
				game_block_1_2.ShowBitmap();
				game_block_2_1.ShowBitmap();
				game_block_2_2.ShowBitmap();
				game_block_left.ShowBitmap();
				game_block_right.ShowBitmap();
			}
			else if (init_board_width == 7) {
				game_block_1_2.SetTopLeft(1032, 267);
				game_block_2_3.SetTopLeft(840,267);
				game_block_left.SetTopLeft(670, 267);
				game_block_right.SetTopLeft(1064, 267);
				game_block_1_2.ShowBitmap();
				game_block_2_1.ShowBitmap();
				game_block_2_2.ShowBitmap();
				game_block_2_3.ShowBitmap();
				game_block_left.ShowBitmap();
				game_block_right.ShowBitmap();
			}
			else if (init_board_width == 8) {
				game_block_2_3.SetTopLeft(840, 267);
				game_block_2_4.SetTopLeft(1032, 267);
				game_block_left.SetTopLeft(670, 267);
				game_block_right.SetTopLeft(1096, 267);
				game_block_2_1.ShowBitmap();
				game_block_2_2.ShowBitmap();
				game_block_2_3.ShowBitmap();
				game_block_2_4.ShowBitmap();
				game_block_left.ShowBitmap();
				game_block_right.ShowBitmap();
			}
			else if (init_board_width == 9) {
				game_block_1_1.SetTopLeft(808, 267);
				game_block_2_3.SetTopLeft(840, 267);
				game_block_2_4.SetTopLeft(1032, 267);
				game_block_left.SetTopLeft(638, 267);
				game_block_right.SetTopLeft(1096, 267);
				game_block_1_1.ShowBitmap();
				game_block_2_1.ShowBitmap();
				game_block_2_2.ShowBitmap();
				game_block_2_3.ShowBitmap();
				game_block_2_4.ShowBitmap();
				game_block_left.ShowBitmap();
				game_block_right.ShowBitmap();
			}
			else if (init_board_width == 10) {
				game_block.ShowBitmap();
			}
		}
		else {
			game_block.ShowBitmap();
		}
		if (game_state == 1 && init_time_initialize_count == 0) {
			init_time = clock();
			init_time_initialize_count += 1;
		}
		else if (game_state == -1) {
			init_time = clock();
			if (game_mode == 1) {
				ShowMode1();
			}
			else if (game_mode == 2) {
				ShowMode2();
			}
			else if (game_mode == 3) {
				ShowMode3();
			}
			else if (game_mode == 4) {
				ShowMode4();
			}

			if (clock() - countdown_begin > 900) {
				CDC *pDC = CDDraw::GetBackCDC();
				if (game_state != 1) {
					if (check_go == 1) {
						// 在 "GO" 文字顯示 3 秒後進入遊戲狀態 1
						check_go = 0;
						//countdown_begin = clock();
						game_state = 1;
					}
					if (countdown_number == 0) {
						CTextDraw::ChangeFontLog(pDC, 75, "微軟正黑體", RGB(247, 193, 4), 50);
						CTextDraw::Print(pDC, 880, 500, "GO");
						check_go = 1;
						countdown_begin = clock(); // 更新計時器的起始時間
					}
					else {
						sprintf(s_countdown_number, "%d", countdown_number--);
						CTextDraw::ChangeFontLog(pDC, 100, "微軟正黑體", RGB(247, 193, 4), 50);
						if (countdown_number <= 2) {
							CTextDraw::Print(pDC, 920, 480, s_countdown_number);
						}
						countdown_begin = clock(); // 更新計時器的起始時間
					}
				}
				CDDraw::ReleaseBackCDC();
				Sleep(900); // 等待文字渲染完成
			}
		}

		if (game_state == 1 && game_mode == 1) {
			ShowMode1();
		}
		else if (game_state == 1 && game_mode == 2) {
			ShowMode2();
		}
		else if (game_state == 1 && game_mode == 3) {
			ShowMode3();
		}
		else if (game_state == 1 && game_mode == 4) {
			ShowMode4();
		}

		for (int i = 0; i < 23; i++) {
			for (int l = 0; l < 10; l++) {
				BlocksVector[map[i][l] * 230 + 10 * i + l].ShowBitmap();
			}
		}

		for (int i = 0; i < 5; i++) {
			BlocksVectorNext[i + ((TetrArray[begin_for_array + i + 1]) - 1) * 5].ShowBitmap();
		}

		BlocksSave[blocks_save].ShowBitmap();

		if (game_state == 2) {
			finish.ShowBitmap();
			score.ShowBitmap();
			if (game_mode == 1) {
				CDC *pDC = CDDraw::GetBackCDC();
				CTextDraw::ChangeFontLog(pDC, 60, "微軟正黑體", RGB(255, 255, 255), 50);
				CTextDraw::Print(pDC, 750, 220, big_passed_time);
				CTextDraw::ChangeFontLog(pDC, 45, "微軟正黑體", RGB(255, 255, 255), 50);
				CTextDraw::Print(pDC, 918, 242, small_passed_time);
				CDDraw::ReleaseBackCDC();

			}
			else if (game_mode == 2) {
				sprintf(my_score, "%d", two_minutes_score);
				CDC *pDC = CDDraw::GetBackCDC();
				CTextDraw::ChangeFontLog(pDC, 60, "微軟正黑體", RGB(255, 255, 255), 50);
				CTextDraw::Print(pDC, 880, 220, my_score);
				CDDraw::ReleaseBackCDC();
			}
		}

		if (game_state == 3) {
			GameOver.ShowBitmap();
		}
	}
}

void CGameStateRun::ShowBlocksNow() {
	for (int i = 0; i < TetrVector[begin_for_array]._height; i++) {
		for (int l = 0; l < TetrVector[begin_for_array]._length; l++) {
			if (TetrVector[begin_for_array].getArray(i + TetrVector[begin_for_array]._xDistance, l + TetrVector[begin_for_array]._yDistance) != 0) {
				map[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = TetrVector[begin_for_array]._color;
				map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = TetrVector[begin_for_array]._color;
			}
		}
	}
}


//----------------------------------------------------Show-----------------------------------------------------------------


void CGameStateRun::ShowMode1() {
	CDC *pDC = CDDraw::GetBackCDC();

	game_passed_times = clock() - init_time;
	game_passed_minutes = game_passed_times / 60000;
	game_passed_seconds = (game_passed_times / 1000) % 60;
	game_passed_milliseconds = game_passed_times % 1000;

	if (game_state != 1) {
		game_passed_milliseconds = 0;
	}

	sprintf(big_passed_time, "%d:%02d", game_passed_minutes, game_passed_seconds);
	sprintf(small_passed_time, ".%03d", game_passed_milliseconds);
	sprintf(lines_clear_number, "%d", clear_rows);

	CTextDraw::ChangeFontLog(pDC, 22, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 705, 695, "LINES");
	CTextDraw::ChangeFontLog(pDC, 43, "微軟正黑體", RGB(255, 255, 255), 50);
	if (clear_rows > 9) {
		CTextDraw::Print(pDC, 652, 725, lines_clear_number);
	}
	else {
		CTextDraw::Print(pDC, 695, 725, lines_clear_number);
	}
	CTextDraw::ChangeFontLog(pDC, 28, "微軟正黑體", RGB(255, 255, 255), 50);
	if (finish_this_game == 5) {
		CTextDraw::Print(pDC, 735, 747, "/ 5");
	}
	else {
		CTextDraw::Print(pDC, 735, 747, "/40");
	}
	CTextDraw::ChangeFontLog(pDC, 22, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 720, 800, "TIME");
	CTextDraw::ChangeFontLog(pDC, 43, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 593, 825, big_passed_time);
	CTextDraw::ChangeFontLog(pDC, 28, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 715, 847, small_passed_time);
	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::ShowMode2() {
	CDC *pDC = CDDraw::GetBackCDC();
	two_minutes_countdown = two_minutes - (clock() - init_time) >= 0 ? two_minutes - (clock() - init_time) : 0;
	game_passed_minutes = two_minutes_countdown / 60000;
	game_passed_seconds = (two_minutes_countdown / 1000) % 60;
	game_passed_milliseconds = two_minutes_countdown % 1000;
	two_minutes_score = score_times;
	if (game_state != 1) {
		game_passed_milliseconds = 990;
	}

	sprintf(big_passed_time, "%d:%02d", game_passed_minutes, game_passed_seconds);
	sprintf(small_passed_time, ".%03d", game_passed_milliseconds);
	sprintf(lines_clear_number, "%d", clear_rows);
	sprintf(blitz_level_char, "%d", blitz_level);
	sprintf(blitz_finish_row, "%d", blitz_level * 2 + 1);
	sprintf(my_score, "%d", two_minutes_score);
	//small_passed_time[3] = '0';

	CTextDraw::ChangeFontLog(pDC, 23, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 1195, 800, "SCORE");

	CTextDraw::ChangeFontLog(pDC, 32, "微軟正黑體", RGB(255, 255, 255), 50);
	if (two_minutes_score < 9) {
		CTextDraw::Print(pDC, 1235, 830, my_score);
	}
	else if (two_minutes_score < 99) {
		CTextDraw::Print(pDC, 1223, 830, my_score);
	}
	else if (two_minutes_score < 999) {
		CTextDraw::Print(pDC, 1211, 830, my_score);
	}
	else if (two_minutes_score < 9999) {
		CTextDraw::Print(pDC, 1199, 830, my_score);
	}
	else {
		CTextDraw::Print(pDC, 1187, 830, my_score);
	}

	CTextDraw::ChangeFontLog(pDC, 22, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 700, 615, "LEVEL");
	CTextDraw::ChangeFontLog(pDC, 43, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 728, 640, blitz_level_char);

	CTextDraw::ChangeFontLog(pDC, 22, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 705, 695, "LINES");
	CTextDraw::ChangeFontLog(pDC, 43, "微軟正黑體", RGB(255, 255, 255), 50);
	if (clear_rows > 9) {
		CTextDraw::Print(pDC, 652, 725, lines_clear_number);
	}
	else {
		CTextDraw::Print(pDC, 695, 725, lines_clear_number);
	}
	CTextDraw::ChangeFontLog(pDC, 28, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 735, 747, "/");
	CTextDraw::ChangeFontLog(pDC, 28, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 750, 747, blitz_finish_row);
	CTextDraw::ChangeFontLog(pDC, 22, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 720, 800, "TIME");
	CTextDraw::ChangeFontLog(pDC, 43, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 593, 825, big_passed_time);
	CTextDraw::ChangeFontLog(pDC, 28, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 715, 847, small_passed_time);
	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::ShowMode3() {
	CDC *pDC = CDDraw::GetBackCDC();
	zen_score = score_times;

	sprintf(zen_level_char, "%d", zen_level);
	sprintf(zen_score_char, "%d", zen_score);

	CTextDraw::ChangeFontLog(pDC, 22, "微軟正黑體", RGB(255, 255, 255), 50);
	if (zen_score < 9) {
		CTextDraw::Print(pDC, 966, 912, zen_score_char);
	}
	else if (zen_score < 99) {
		CTextDraw::Print(pDC, 958, 912, zen_score_char);
	}
	else if (zen_score < 999) {
		CTextDraw::Print(pDC, 950, 912, zen_score_char);
	}
	else if (zen_score < 9999) {
		CTextDraw::Print(pDC, 943, 912, zen_score_char);
	}
	else {
		CTextDraw::Print(pDC, 935, 912, zen_score_char);
	}

	CTextDraw::ChangeFontLog(pDC, 28, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 966, 948, zen_level_char);

	CDDraw::ReleaseBackCDC();
}


void CGameStateRun::ShowMode4() {
	CDC *pDC = CDDraw::GetBackCDC();
	game_passed_times = clock() - init_time;
	game_passed_minutes = game_passed_times / 60000;
	game_passed_seconds = (game_passed_times / 1000) % 60;
	game_passed_milliseconds = game_passed_times % 1000;
	two_minutes_score = score_times;

	if (game_state != 1) {
		game_passed_milliseconds = 0;
	}

	sprintf(big_passed_time, "%d:%02d", game_passed_minutes, game_passed_seconds);
	sprintf(small_passed_time, ".%03d", game_passed_milliseconds);
	sprintf(lines_clear_number, "%d", clear_rows);
	sprintf(custom_level_char, "%d", clear_rows/10 + 1);
	sprintf(my_score, "%d", two_minutes_score);
	//small_passed_time[3] = '0';

	CTextDraw::ChangeFontLog(pDC, 23, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 1195, 800, "SCORE");

	CTextDraw::ChangeFontLog(pDC, 32, "微軟正黑體", RGB(255, 255, 255), 50);
	if (two_minutes_score < 9) {
		CTextDraw::Print(pDC, 1235, 830, my_score);
	}
	else if (two_minutes_score < 99) {
		CTextDraw::Print(pDC, 1223, 830, my_score);
	}
	else if (two_minutes_score < 999) {
		CTextDraw::Print(pDC, 1211, 830, my_score);
	}
	else if (two_minutes_score < 9999) {
		CTextDraw::Print(pDC, 1199, 830, my_score);
	}
	else {
		CTextDraw::Print(pDC, 1187, 830, my_score);
	}

	CTextDraw::ChangeFontLog(pDC, 22, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 700, 615, "LEVEL");
	CTextDraw::ChangeFontLog(pDC, 37, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 728, 640, custom_level_char);

	CTextDraw::ChangeFontLog(pDC, 22, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 705, 695, "LINES");
	CTextDraw::ChangeFontLog(pDC, 37, "微軟正黑體", RGB(255, 255, 255), 50);
	if (clear_rows < 9) {
		CTextDraw::Print(pDC, 672, 730, lines_clear_number);
	}
	else if (clear_rows < 99){
		CTextDraw::Print(pDC, 664, 730, lines_clear_number);
	}
	else {
		CTextDraw::Print(pDC, 657, 730, lines_clear_number);
	}
	CTextDraw::ChangeFontLog(pDC, 28, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 725, 747, "/");
	CTextDraw::ChangeFontLog(pDC, 28, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 740, 747, "150");
	CTextDraw::ChangeFontLog(pDC, 22, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 720, 800, "TIME");
	CTextDraw::ChangeFontLog(pDC, 43, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 593, 825, big_passed_time);
	CTextDraw::ChangeFontLog(pDC, 28, "微軟正黑體", RGB(255, 255, 255), 50);
	CTextDraw::Print(pDC, 715, 847, small_passed_time);
	CDDraw::ReleaseBackCDC();
}


//------------------------------------------------方塊清除--------------------------------------------------------------------



void CGameStateRun::BlocksPredict() {

	for (int i = 0; i < 23; i++) {
		for (int l = 0; l < 10; l++) {
			if (map[i][l] == 8) {
				map[i][l] = 0;
			}
		}
	}

	predict._x = TetrVector[begin_for_array]._x;
	predict._y = TetrVector[begin_for_array]._y;
	predict._xDistance = TetrVector[begin_for_array]._xDistance;
	predict._yDistance = TetrVector[begin_for_array]._yDistance;
	predict._height = TetrVector[begin_for_array]._height;
	predict._length = TetrVector[begin_for_array]._length;

	for (int i = 0; i < 4; i++) {
		for (int l = 0; l < 4; l++) {
			if (TetrVector[begin_for_array]._array[i][l] == 0) {
				predict._array[i][l] = 0;
			}
			else if (TetrVector[begin_for_array]._array[i][l] == TetrVector[begin_for_array]._color) {
				predict._array[i][l] = 8;
			}
		}
	}

	predict_space_judge = 0;

	for (int k = predict._x + 1; k < predict._x + 3; k++) {
		if (predict_space_judge == 0) {
			PredictBlocksDown();
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int l = 0; l < 4; l++) {
			if (predict._array[i + predict._xDistance][l + predict._yDistance] == 8) {
				map[predict._x + i + predict._xDistance][predict._y + l + predict._yDistance] = 8;
			}
		}
	}
}

void CGameStateRun::PredictBlocksDown() {
	for (int i = 0; i < predict._height; i++) {
		for (int l = 0; l < predict._length; l++) {
			if (predict._array[i + predict._xDistance][l + predict._yDistance] == 8 && map_base_blocks[predict._x + i + predict._xDistance + 1][predict._y + l + predict._yDistance] == 9 || predict._x == 23 - predict._height - predict._xDistance) {
				predict_space_judge = 1;
				break;
			}
		}
	}

	if (predict._x == 23 - predict._height) {
		predict_space_judge = 1;
	}

	else if (predict._x < 23 - predict._height && predict_space_judge == 0) {
		for (int i = 0; i < predict._height; i++) {
			for (int l = 0; l < predict._length; l++) {
				if (predict._array[i + predict._xDistance][l + predict._yDistance] == 8) {
					map[predict._x + i + predict._xDistance][predict._y + l + predict._yDistance] = 0;
				}
			}
		}
		predict._x++;
	}
	if (game_state == 1) {
		ShowBlocksNow();
	}
}


void CGameStateRun::BlocksRowClean() {
	if (game_mode != 4) {
		clean_blocks_judge = 10;
	}
	else {
		clean_blocks_judge = init_board_width;
	}
	for (int i = TetrVector[begin_for_array - 1]._x + TetrVector[begin_for_array - 1]._xDistance; i < TetrVector[begin_for_array - 1]._x + TetrVector[begin_for_array - 1]._xDistance + TetrVector[begin_for_array - 1]._height; i++) {
		clean_blocks = 0;
		for (int l = 0; l < 10; l++) {
			if (map[i][l] != 0) {
				clean_blocks++;
			}
		}
		if (clean_blocks == clean_blocks_judge) {
			score_temp += 1;
			for (int k = i; k > 3; k--) {
				i--;
				for (int m = 0; m < 10; m++) {
					map[k][m] = map[k - 1][m];
					map_base_blocks[k][m] = map_base_blocks[k - 1][m];
				}
			}
			for (int m = 0; m < 10; m++) {
				map[3][m] = 0;
				map_base_blocks[3][m] = 0;
			}
			clear_rows++;
		}
	if (score_temp == 1) {
		score_times += 400;
	}
	else if (score_temp == 2) {
		score_times += 800;
	}
	else if (score_temp == 3) {
		score_times += 1300;
	}
	else if(score_temp==4) {
		score_times += 2000;
	}
	score_temp = 0;
	}
	if (clear_rows >= finish_this_game && ( game_mode == 1 || game_mode == 4)) {
		for (int i = 0; i < TetrVector[begin_for_array]._height; i++) {
			for (int l = 0; l < TetrVector[begin_for_array]._length; l++) {
				if (map[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] != 0) {
					map[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
					map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
				}
			}
		}
		game_state = 2;
	}
}








//------------------------------------------------方塊動作--------------------------------------------------------------------









void CGameStateRun::BlocksDown() {
	if (TetrVector[begin_for_array]._x == predict._x) {
		for (int a = 0; a < 23; a++) {
			for (int b = 0; b < 10; b++) {
				if (map[a][b] != 0) {
					map_base_blocks[a][b] = 9;
				}
			}
		}
		space_judge = 1;
		if (game_state == 1) {
			GameFailJudge();
			if (game_fail_judge == 0) {
				CreateBlocks();
				begin_for_array++;
			}
		}
		score_times += 24 + std::rand() % (40 - 24 + 1);
		BlocksRowClean();
		shift_judge = 0;
		BlocksPredict();
	}

	else if ((TetrVector[begin_for_array]._x) < 23 - TetrVector[begin_for_array]._height) {
		for (int i = 0; i < TetrVector[begin_for_array]._height; i++) {
			for (int l = 0; l < TetrVector[begin_for_array]._length; l++) {
				if (map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] == TetrVector[begin_for_array]._color && map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] != 9) {
					map[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
					map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
				}
			}
		}
		(TetrVector[begin_for_array]._x)++;
	}
	if (game_state == 1) {
		ShowBlocksNow();
	}
}



void CGameStateRun::BlocksRight() {
	if (game_mode != 4) {
		max_board_judge = 10;
	}
	else {
		if (init_board_width % 2 == 0) {
			max_board_judge = 10 - (10 - init_board_width) / 2;
		}
		else {
			max_board_judge = 10 - (10 - init_board_width + 1) / 2;
		}
	}
	
	for_blocks_right = 0;
	for (int i = 0; i < TetrVector[begin_for_array]._height; i++) {
		for (int l = 0; l < TetrVector[begin_for_array]._length; l++) {
			if (map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] == TetrVector[begin_for_array]._color  &&  map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance + 1] == 9) {
				for_blocks_right = 1;
				break;
			}
		}
	}
	if ((TetrVector[begin_for_array]._y) < max_board_judge - TetrVector[begin_for_array]._length - TetrVector[begin_for_array]._yDistance) {
		for (int i = 0; i < TetrVector[begin_for_array]._height; i++) {
			for (int l = 0; l < TetrVector[begin_for_array]._length; l++) {
				if (map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] != 0 && map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance + 1] != 9) {
					map[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
					map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
				}
			}
		}
		if (for_blocks_right == 0) {
			(TetrVector[begin_for_array]._y)++;
		}
	}
	BlocksPredict();
	ShowBlocksNow();
}



void CGameStateRun::BlocksLeft() {
	if (game_mode != 4) {
		min_board_judge = 0;
	}
	else 
	{
		if (init_board_width % 2 == 0) {
			min_board_judge = (10 - init_board_width) / 2;
		}
		else {
			min_board_judge = (10 - init_board_width - 1) / 2;
		}
	}
	
	for_blocks_left = 0;
	for (int i = 0; i < TetrVector[begin_for_array]._height; i++) {
		for (int l = 0; l < TetrVector[begin_for_array]._length; l++) {
			if (map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] == TetrVector[begin_for_array]._color  &&  map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance - 1] == 9) {
				for_blocks_left = 1;
				break;
			}
		}
	}
	if ((TetrVector[begin_for_array]._y) > min_board_judge - TetrVector[begin_for_array]._yDistance) {
		for (int i = 0; i < TetrVector[begin_for_array]._height; i++) {
			for (int l = 0; l < TetrVector[begin_for_array]._length; l++) {
				if (map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] != 0 && map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance - 1] != 9) {
					map[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
					map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
				}
			}
		}
		if (for_blocks_left == 0) {
			(TetrVector[begin_for_array]._y)--;
		}
	}
	BlocksPredict();
	ShowBlocksNow();
}



void CGameStateRun::BlocksTurn() {
	for (int i = 0; i < TetrVector[begin_for_array]._height; i++) {
		for (int l = 0; l < TetrVector[begin_for_array]._length; l++) {
			if (map[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] != 0 && map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] != 9) {
				map[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
				map_base_blocks[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
			}
		}
	}
	TetrVector[begin_for_array].twist();
	if ((TetrVector[begin_for_array]._y) + TetrVector[begin_for_array]._length > max_board_judge) {
		TetrVector[begin_for_array]._y -= (TetrVector[begin_for_array]._y) + TetrVector[begin_for_array]._length - max_board_judge;
	}
	if ((TetrVector[begin_for_array]._y) + TetrVector[begin_for_array]._yDistance < min_board_judge) {
		TetrVector[begin_for_array]._y = min_board_judge;
	}
	BlocksPredict();
	ShowBlocksNow();
}



void CGameStateRun::BlocksShift() {
	if (blocks_save == 0) {
		shift_judge = 1;
		blocks_save = TetrArray[begin_for_array];
		TetrSave = TetrVector[begin_for_array];
		CreateBlocks();
		begin_for_array++;
		for (int i = 0; i < TetrSave._height; i++) {
			for (int l = 0; l < TetrSave._length; l++) {
				if (map[TetrSave._x + i + TetrSave._xDistance][TetrSave._y + l + TetrSave._yDistance] != 0 && map_base_blocks[TetrSave._x + i + TetrSave._xDistance][TetrSave._y + l + TetrSave._yDistance] != 9) {
					map[TetrSave._x + i + TetrSave._xDistance][TetrSave._y + l + TetrSave._yDistance] = 0;
				}
			}
		}
		TetrSave._x = 4;
		TetrSave._y = 3;
		TetrSave._stage = 4;
		TetrSave.twist();
		BlocksPredict();
		ShowBlocksNow();
	}

	else if (blocks_save != 0) {
		if (shift_judge == 0) {
			for (int i = 0; i < TetrVector[begin_for_array]._height; i++) {
				for (int l = 0; l < TetrVector[begin_for_array]._length; l++) {
					if (TetrVector[begin_for_array].getArray(i + TetrVector[begin_for_array]._xDistance, l + TetrVector[begin_for_array]._yDistance) != 0) {
						map[(TetrVector[begin_for_array]._x) + i + TetrVector[begin_for_array]._xDistance][(TetrVector[begin_for_array]._y) + l + TetrVector[begin_for_array]._yDistance] = 0;
					}
				}
			}
			temp = TetrArray[begin_for_array];
			TetrTemp = TetrVector[begin_for_array];

			TetrTemp._x = 4;
			TetrTemp._y = 3;
			TetrTemp._stage = 4;
			TetrTemp.twist();

			TetrArray[begin_for_array] = blocks_save;
			TetrVector[begin_for_array] = TetrSave;

			blocks_save = temp;
			TetrSave = TetrTemp;

			shift_judge = 1;
			BlocksPredict();
			ShowBlocksNow();
		}
	}
}



void CGameStateRun::BlocksSpace() {
	space_judge = 0;
	for (int k = (TetrVector[begin_for_array]._x) + 1; k < (TetrVector[begin_for_array]._x) + 3; k++) {
		if (space_judge == 0) {
			BlocksDown();
		}
	}
	BlocksPredict();
}










//------------------------------------------------開始時動作--------------------------------------------------------------------








void CGameStateRun::CreateMainMap() {
	for (int i = 0; i < 230; i++) {
		CMovingBitmap T;
		BlocksVector1.push_back(T);
		BlocksVector2.push_back(T);
		BlocksVector3.push_back(T);
		BlocksVector4.push_back(T);
		BlocksVector5.push_back(T);
		BlocksVector6.push_back(T);
		BlocksVector7.push_back(T);
		BlocksVector8.push_back(T);
		BlocksVector9.push_back(T);

		BlocksVector1[i].LoadBitmapByString({ "resources/transparent.bmp" }, RGB(255, 255, 255));
		BlocksVector2[i].LoadBitmapByString({ "resources/squre.bmp" });
		BlocksVector3[i].LoadBitmapByString({ "resources/red.bmp" });
		BlocksVector4[i].LoadBitmapByString({ "resources/green.bmp" });
		BlocksVector5[i].LoadBitmapByString({ "resources/4.bmp" });
		BlocksVector6[i].LoadBitmapByString({ "resources/T.bmp" });
		BlocksVector7[i].LoadBitmapByString({ "resources/L_blue.bmp" });
		BlocksVector8[i].LoadBitmapByString({ "resources/L_orange.bmp" });
		BlocksVector9[i].LoadBitmapByString({ "resources/gray.bmp" });
	}

	BlocksVector.reserve(9 * 230);
	BlocksVector.insert(BlocksVector.end(), BlocksVector1.begin(), BlocksVector1.end());
	BlocksVector.insert(BlocksVector.end(), BlocksVector2.begin(), BlocksVector2.end());
	BlocksVector.insert(BlocksVector.end(), BlocksVector3.begin(), BlocksVector3.end());
	BlocksVector.insert(BlocksVector.end(), BlocksVector4.begin(), BlocksVector4.end());
	BlocksVector.insert(BlocksVector.end(), BlocksVector5.begin(), BlocksVector5.end());
	BlocksVector.insert(BlocksVector.end(), BlocksVector6.begin(), BlocksVector6.end());
	BlocksVector.insert(BlocksVector.end(), BlocksVector7.begin(), BlocksVector7.end());
	BlocksVector.insert(BlocksVector.end(), BlocksVector8.begin(), BlocksVector8.end());
	BlocksVector.insert(BlocksVector.end(), BlocksVector9.begin(), BlocksVector9.end());

	for (int i = 0; i < 23; i++) {
		for (int l = 0; l < 10; l++) {
			BlocksVector[10 * i + l].SetTopLeft(The_Point_X_Of_Map + l * 32, The_Point_Y_Of_Map + i * 32);
			BlocksVector[230 + 10 * i + l].SetTopLeft(The_Point_X_Of_Map + l * 32, The_Point_Y_Of_Map + i * 32);
			BlocksVector[460 + 10 * i + l].SetTopLeft(The_Point_X_Of_Map + l * 32, The_Point_Y_Of_Map + i * 32);
			BlocksVector[690 + 10 * i + l].SetTopLeft(The_Point_X_Of_Map + l * 32, The_Point_Y_Of_Map + i * 32);
			BlocksVector[920 + 10 * i + l].SetTopLeft(The_Point_X_Of_Map + l * 32, The_Point_Y_Of_Map + i * 32);
			BlocksVector[1150 + 10 * i + l].SetTopLeft(The_Point_X_Of_Map + l * 32, The_Point_Y_Of_Map + i * 32);
			BlocksVector[1380 + 10 * i + l].SetTopLeft(The_Point_X_Of_Map + l * 32, The_Point_Y_Of_Map + i * 32);
			BlocksVector[1610 + 10 * i + l].SetTopLeft(The_Point_X_Of_Map + l * 32, The_Point_Y_Of_Map + i * 32);
			BlocksVector[1840 + 10 * i + l].SetTopLeft(The_Point_X_Of_Map + l * 32, The_Point_Y_Of_Map + i * 32);
		}
	}
}



void CGameStateRun::CreateNextMap() {
	for (int i = 0; i < 5; i++) {
		CMovingBitmap T;
		BlocksVectorNext1.push_back(T);
		BlocksVectorNext2.push_back(T);
		BlocksVectorNext3.push_back(T);
		BlocksVectorNext4.push_back(T);
		BlocksVectorNext5.push_back(T);
		BlocksVectorNext6.push_back(T);
		BlocksVectorNext7.push_back(T);
		BlocksSave.push_back(T);

		BlocksVectorNext1[i].LoadBitmapByString({ "resources/O.bmp" });
		BlocksVectorNext2[i].LoadBitmapByString({ "resources/Z.bmp" });
		BlocksVectorNext3[i].LoadBitmapByString({ "resources/S.bmp" });
		BlocksVectorNext4[i].LoadBitmapByString({ "resources/I.bmp" });
		BlocksVectorNext5[i].LoadBitmapByString({ "resources/TT.bmp" });
		BlocksVectorNext6[i].LoadBitmapByString({ "resources/LB.bmp" });
		BlocksVectorNext7[i].LoadBitmapByString({ "resources/LO.bmp" });
	}

	BlocksVectorNext.reserve(7 * 5);
	BlocksVectorNext.insert(BlocksVectorNext.end(), BlocksVectorNext1.begin(), BlocksVectorNext1.end());
	BlocksVectorNext.insert(BlocksVectorNext.end(), BlocksVectorNext2.begin(), BlocksVectorNext2.end());
	BlocksVectorNext.insert(BlocksVectorNext.end(), BlocksVectorNext3.begin(), BlocksVectorNext3.end());
	BlocksVectorNext.insert(BlocksVectorNext.end(), BlocksVectorNext4.begin(), BlocksVectorNext4.end());
	BlocksVectorNext.insert(BlocksVectorNext.end(), BlocksVectorNext5.begin(), BlocksVectorNext5.end());
	BlocksVectorNext.insert(BlocksVectorNext.end(), BlocksVectorNext6.begin(), BlocksVectorNext6.end());
	BlocksVectorNext.insert(BlocksVectorNext.end(), BlocksVectorNext7.begin(), BlocksVectorNext7.end());

	for (int i = 0; i < 5; i++) {
		BlocksVectorNext[i].SetTopLeft(The_Point_X_Of_Next_Blocks, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[5 + i].SetTopLeft(The_Point_X_Of_Next_Blocks, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[10 + i].SetTopLeft(The_Point_X_Of_Next_Blocks, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[15 + i].SetTopLeft(The_Point_X_Of_Next_Blocks, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[20 + i].SetTopLeft(The_Point_X_Of_Next_Blocks, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[25 + i].SetTopLeft(The_Point_X_Of_Next_Blocks, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[30 + i].SetTopLeft(The_Point_X_Of_Next_Blocks, The_Point_Y_Of_Next_Blocks + i * 91);
	}
}

void CGameStateRun::ChangeNextMap() {
	if (game_mode != 4) {
		max_board_judge = 10;
	}
	else {
		if (init_board_width % 2 == 0) {
			max_board_judge = 10 - (10 - init_board_width) / 2;
		}
		else {
			max_board_judge = 10 - (10 - init_board_width + 1) / 2;
		}
	}

	for (int i = 0; i < 5; i++) {
		BlocksVectorNext[i].SetTopLeft(The_Point_X_Of_Next_Blocks - (10 - max_board_judge)*32, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[5 + i].SetTopLeft(The_Point_X_Of_Next_Blocks - (10 - max_board_judge) * 32, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[10 + i].SetTopLeft(The_Point_X_Of_Next_Blocks - (10 - max_board_judge) * 32, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[15 + i].SetTopLeft(The_Point_X_Of_Next_Blocks - (10 - max_board_judge) * 32, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[20 + i].SetTopLeft(The_Point_X_Of_Next_Blocks - (10 - max_board_judge) * 32, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[25 + i].SetTopLeft(The_Point_X_Of_Next_Blocks - (10 - max_board_judge) * 32, The_Point_Y_Of_Next_Blocks + i * 91);
		BlocksVectorNext[30 + i].SetTopLeft(The_Point_X_Of_Next_Blocks - (10 - max_board_judge) * 32, The_Point_Y_Of_Next_Blocks + i * 91);
	}
}


void CGameStateRun::CreateSaveMap() {
	for (int i = 0; i < 8; i++) {
		CMovingBitmap T;
		BlocksSave.push_back(T);
	}
	BlocksSave[0].LoadBitmapByString({ "resources/0.bmp" });
	BlocksSave[1].LoadBitmapByString({ "resources/O.bmp" });
	BlocksSave[2].LoadBitmapByString({ "resources/Z.bmp" });
	BlocksSave[3].LoadBitmapByString({ "resources/S.bmp" });
	BlocksSave[4].LoadBitmapByString({ "resources/I.bmp" });
	BlocksSave[5].LoadBitmapByString({ "resources/TT.bmp" });
	BlocksSave[6].LoadBitmapByString({ "resources/LB.bmp" });
	BlocksSave[7].LoadBitmapByString({ "resources/LO.bmp" });

	for (int i = 0; i < 8; i++) {
		BlocksSave[i].SetTopLeft(655, 300);
	}
}

void CGameStateRun::ChangeSaveMap() {
	if (game_mode != 4) {
		min_board_judge = 0;
	}
	else
	{
		if (init_board_width % 2 == 0) {
			min_board_judge = (10 - init_board_width) / 2;
		}
		else {
			min_board_judge = (10 - init_board_width - 1) / 2;
		}
	}

	for (int i = 0; i < 8; i++) {
		BlocksSave[i].SetTopLeft(655 + min_board_judge*32, 300);
	}
}



void CGameStateRun::CreateBlocks() {
	rand_number = rand() % 7 + 1;
	if (TetrArray.size() < 7 && TetrArray.size() == 1 && rand_number == TetrArray[0]) {
		rand_number = TetrArray.back() + 2;
		rand_number %= 7;
		rand_number++;
	}

	else if (TetrArray.size() < 7 && TetrArray.size() >= 2) {
		int list[7] = { 0, 0, 0, 0, 0, 0, 0 };
		int size = TetrArray.size();
		for (int i = 0; i < size; i++) {
			list[TetrArray[i] - 1] = 1;
		}

		for (int i = 0; i < 6; i++) {
			rand_number = rand() % 7 + 1;
			if (list[rand_number - 1] == 0) {
				break;
			}
		}
	}

	if (TetrArray.size() >= 6) {
		int list[7] = { 0, 0, 0, 0, 0, 0, 0 };
		int size = TetrArray.size();
		for (int i = 2; i < 6; i++) {
			list[TetrArray[begin_for_array + i] - 1] = 1;
		}

		for (int i = 0; i < 20; i++) {
			rand_number = rand() % 7 + 1;
			if (list[rand_number - 1] == 0) {
				break;
			}
		}
	}

	TetrArray.push_back(rand_number);
	Tetr T;
	T.setBlock(rand_number);
	TetrVector.push_back(T);
}








//------------------------------------------------結束後動作--------------------------------------------------------------------








void CGameStateRun::Retry() {
	Init();
	GotoGameState(GAME_STATE_RUN);
}



void CGameStateRun::GameFailJudge() {
	for (int i = 0; i < TetrVector[begin_for_array + 1]._height; i++) {
		for (int l = 0; l < TetrVector[begin_for_array + 1]._length; l++) {
			if (map[(TetrVector[begin_for_array + 1]._x) + i + TetrVector[begin_for_array + 1]._xDistance][(TetrVector[begin_for_array + 1]._y) + l + TetrVector[begin_for_array + 1]._yDistance] != 0) {
				game_fail_judge = 1;
				if (game_mode != 3) {
					game_state = 3;
				}
			}
		}
	}
}



//------------------------------------------------旋轉判斷--------------------------------------------------------------------




void CGameStateRun::BlocksTurnJudge() {
	if (clock() - gameover_clock > 1500) {
		game_block.SetTopLeft(638, game_block.GetLeft() - 2);
		//game_block.ShowBitmap();
		gameover_clock = clock();
	}
}