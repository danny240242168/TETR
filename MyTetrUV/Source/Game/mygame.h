/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include"../../tetr.h"
#include"../../predict.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int state = 0;
		void load_background();
		CMovingBitmap logo;								// csie的logo
		CMovingBitmap background;


	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作

		void BlocksDown();
		void BlocksRight();
		void BlocksLeft();
		void BlocksTurn();
		void BlocksShift();
		void BlocksSpace();


		void ShowBlocksNow();
		void BlocksRowClean();

		void Retry();
		void Init();
		void CreateMainMap();
		void CreateNextMap();
		void CreateSaveMap();
		void CreateBlocks();

		void GameFailJudge();
		void BlocksTurnJudge();

		void BlocksPredict();
		void PredictBlocksDown();

		void ShowMode1();
		void ShowMode2();
		void ShowMode3();
		void ShowMode4();

		void ChangeNextMap();
		void ChangeSaveMap();


		Predict predict;

		int rand_number;

		bool gameover_clock_start = false;
		int gameover_clock = 0;

		int game_fail_judge = 0;
		int predict_space_judge;

		int space_judge;
		int space_row;
		int for_blocks_right;
		int for_blocks_left;

		int clean_blocks;
		int clean_blocks_judge;

		int blocks_left_key = 0;
		int blocks_right_key = 0;
		int blocks_down_key = 0;

		int down_time = 0;
		int init_time_initialize_count = 0;
		int countdown_number = 5;
		int countdown_begin = 0;
		int check_go = 0;
		//---------for顯示-----------
		char s_countdown_number[1] = {};
		char s_init_board_width[2] = {};

		//---------------------------
		std::vector<CMovingBitmap> BlocksVector1;
		std::vector<CMovingBitmap> BlocksVector2;
		std::vector<CMovingBitmap> BlocksVector3;
		std::vector<CMovingBitmap> BlocksVector4;
		std::vector<CMovingBitmap> BlocksVector5;
		std::vector<CMovingBitmap> BlocksVector6;
		std::vector<CMovingBitmap> BlocksVector7;
		std::vector<CMovingBitmap> BlocksVector8;
		std::vector<CMovingBitmap> BlocksVector9;
		std::vector<CMovingBitmap> BlocksVector;

		std::vector<CMovingBitmap> BlocksVectorNext;
		std::vector<CMovingBitmap> BlocksVectorNext1;
		std::vector<CMovingBitmap> BlocksVectorNext2;
		std::vector<CMovingBitmap> BlocksVectorNext3;
		std::vector<CMovingBitmap> BlocksVectorNext4;
		std::vector<CMovingBitmap> BlocksVectorNext5;
		std::vector<CMovingBitmap> BlocksVectorNext6;
		std::vector<CMovingBitmap> BlocksVectorNext7;
		std::vector<CMovingBitmap> BlocksSave;

		CMovingBitmap GameOver;
		CMovingBitmap finish;
		CMovingBitmap game_block;
		CMovingBitmap mode;
		CMovingBitmap mode1;
		CMovingBitmap mode2;
		CMovingBitmap mode3;
		CMovingBitmap mode4;
		int init_board_width = 10;
		int max_board_judge = 10;
		int min_board_judge = 0;
		CMovingBitmap score;
		CMovingBitmap fourty_lines_longButton;
		CMovingBitmap blitz_longButton;
		int mouseOnZenButtonCheck = 0;
		CMovingBitmap zen_originButton;
		CMovingBitmap zen_longButton;
		CMovingBitmap custom_longButton;
		CMovingBitmap game_block_left;
		CMovingBitmap game_block_right;
		CMovingBitmap game_block_1_1;
		CMovingBitmap game_block_1_2;
		CMovingBitmap game_block_2_1;
		CMovingBitmap game_block_2_2;
		CMovingBitmap game_block_2_3;
		CMovingBitmap game_block_2_4;
		int Start3 = 0;
		int SpaceForI = 0;

		int blitz_level = 1;
		int zen_level = 1;
		int zen_score = 0;

		int blocks_save = 0;
		int temp = 0;
		Tetr TetrSave;
		Tetr TetrTemp;
		int shift_judge = 0;

		int clear_rows = 0;
		int game_state = 0;
		int state = 1;
		int game_mode = 0;

		int speed = 30;
		int zen_speed_control = 660;
		int zen_score_limit = 0;

		//-----------------------時間的變數-----------------------
		int init_time = 0;//到遊戲開始總共經過的時間
		char big_passed_time[10] = {};
		char small_passed_time[3] = {};
		int game_passed_times = 0;
		int game_passed_minutes = 0;
		int game_passed_seconds = 0;
		char lines_clear_number[2] = {};
		int game_passed_milliseconds = 0;

		int two_minutes_countdown = 0;
		char custom_level_char[3] = {};
		char blitz_level_char[2] = {};
		char blitz_finish_row[2] = {};
		char zen_level_char[2] = {};
		char zen_score_char[10] = {};
		int two_minutes = 120000;

		int two_minutes_score = 0;
		char my_score[10] = {};
		int score_times = 0;
		int score_temp = 0;
		//--------------------------------------------------------
		std::vector<Tetr> TetrVector;
		std::vector<int> TetrArray;

		int begin_for_array = 0;

		int The_Point_X_Of_Map = 809;
		//int The_Point_X_Of_Map = 30;
		int The_Point_Y_Of_Map = 172;

		//int The_Point_X_Of_Next_Blocks = 973;
		//int The_Point_Y_Of_Next_Blocks = 212;
		int The_Point_X_Of_Next_Blocks = 1169;
		int The_Point_Y_Of_Next_Blocks = 310;
		int finish_this_game = 40;
		int map[23][10] =
		{ {0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0} };

		int map_base_blocks[23][10] =
		{ {0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0} };

		int zen_blocks[23][10] =
		{ {0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0} };

		int zen_score_temp = 0;
		int zen_level_temp = 1;
		std::vector<Tetr> ZenTetrVector;
		std::vector<int> ZenTetrArray;
		bool zen_before = false;

	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
		CMovingBitmap background2;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
	};

}

