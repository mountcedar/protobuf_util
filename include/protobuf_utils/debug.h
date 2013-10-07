/**
 * @file debug.h
 * @author Osamu Sugiyama
 * @brief debug関係のマクロを定義するヘッダファイル
 */
#pragma once

/* デバッグプリントレベルの定義 */
/**
   @brief デバッグレベルの定義。 常に表示したい場合はこのレベルを指定すること
*/
#define DEBUGPRINT_LEVEL_BASE (1)

/**
   @brief デバッグレベルの定義。 システムエラー時の出力に使用
   @details
   システムエラー時とはシステムのノーマルシーケンス時に起こらないエラーの状態をさす
*/
#define DEBUGPRINT_LEVEL_ERROR_BASE (DEBUGPRINT_LEVEL_BASE << 1)

/**
   @brief デバッグレベルの定義。Warning時の出力に使用
   @details
   かならずしもシステムエラーではないが、warningしたい状態で用いる
*/
#define DEBUGPRINT_LEVEL_WARNING_BASE (DEBUGPRINT_LEVEL_BASE << 8)

/**
   @brief デバッグレベルの定義。Notice時の出力に使用
   @details
   警告したい情報を出力したい状態で用いる
*/
#define DEBUGPRINT_LEVEL_NOTICE_BASE  (DEBUGPRINT_LEVEL_BASE << 8)

/**
   @brief デバッグレベルの定義。Infoの出力に使用
   @details
   未定義
 */
#define DEBUGPRINT_LEVEL_INFO_BASE (DEBUGPRINT_LEVEL_BASE << 8)


// @note forcely activate debug print option
#define ENABLE_DEBUGPRINT
#ifdef ENABLE_DEBUGPRINT

#if !defined(DEBUG_LEVEL)

/**
   DEBUG_LEVELのデフォルト値
 */
#define DEBUG_LEVEL DEBUGPRINT_LEVEL_BASE

#endif /* DEBUG_LEVEL */

#include <stdio.h>
#include <assert.h>

/**
   @brief デバッグプリントの出力する対象を指定するマクロ変数
 */
#define DEBUGOUT stderr

/* printするときのフォントの設定 */
#define DEBUGPRINT_FONT_INITIAL    "0"  ///< 初期状態 
#define DEBUGPRINT_FONT_BOLD       "1"  ///< 太字  
#define DEBUGPRINT_FONT_UNDERLINE  "4"  ///< 下線
#define DEBUGPRINT_FONT_BRINK      "5"  ///< 点滅
#define DEBUGPRINT_FONT_REVERSE    "7"  ///< 色反転
   
/* printするときのフォント色の設定 */
#define DEBUGPRINT_COLOR_FG_BLACK  "30" ///< 黒(文字色) 
#define DEBUGPRINT_COLOR_FG_RED    "31" ///< 赤(文字色) 
#define DEBUGPRINT_COLOR_FG_GREEN  "32" ///< 緑(文字色)  
#define DEBUGPRINT_COLOR_FG_YELLOW "33" ///< 黄色(文字色) 
#define DEBUGPRINT_COLOR_FG_BLUE   "34" ///< 青(文字色) 
#define DEBUGPRINT_COLOR_FG_PURPLE "35" ///< 紫(文字色) 
#define DEBUGPRINT_COLOR_FG_AQUA   "36" ///< 水色(文字色)
#define DEBUGPRINT_COLOR_FG_WHITE  "37" ///< 白(文字色)

/* printするときの背景色の設定 */
#define DEBUGPRINT_COLOR_BG_BLACK  "40" ///< 白(背景色)
#define DEBUGPRINT_COLOR_BG_RED    "41" ///< 赤(背景色)
#define DEBUGPRINT_COLOR_BG_GREEN  "42" ///< 緑(背景色)
#define DEBUGPRINT_COLOR_BG_YELLOW "44" ///< 黄色(背景色)
#define DEBUGPRINT_COLOR_BG_BLUE   "44" ///< 青(背景色)
#define DEBUGPRINT_COLOR_BG_PURPLE "45" ///< 紫(背景色)
#define DEBUGPRINT_COLOR_BG_AQUA   "46" ///< 水色(背景色)
#define DEBUGPRINT_COLOR_BG_WHITE  "47" ///< 白(背景色)

/**
   @brief ファイルのバージョン出力の定義
   @details
   ファイルのコンパイル時間情報をDEBUGOUTに出力する
 */
#define DEBUG_VERSION_PRINT()					\
	fprintf(DEBUGOUT,					\
		"COMPILED[%s # %s]\n",				\
		__DATE__, __TIME__, __FILE__, __LINE__)


/**
   @brief デバッグプリントの定義

   @details
   formatに従ってメッセージを関数名、ファイル名、行番号と共に
   DEBUGOUTに出力する

   @param[in] format fprintfのformatと同じ
   @param[in] args fprintfの残りの引数と同義
 */
#define DEBUG_PRINT(format, args...)				\
	fprintf(DEBUGOUT, "[%s : %s, line %d] "format,		\
		__FUNCTION__, __FILE__, __LINE__, ##args)


/**
   @brief デバッグプリントの定義

   @details
   formatに従ってメッセージを関数名、ファイル名、行番号と共に
   DEBUGOUTに出力し、改行する．

   @param[in] format fprintfのformatと同じ
   @param[in] args fprintfの残りの引数と同義
 */
#define DEBUG_PRINTLN(format, args...)				\
	fprintf(DEBUGOUT, "[%s : %s, line %d] "format"\n",	\
		__FUNCTION__, __FILE__, __LINE__, ##args)


/**
   @brief デバッグプリントの定義。色つき

   @details 
   formatに従ってメッセージを関数名、ファイル名、行番号と共にDEBUGOUTに
   指定のcolorフォーマットで出力する。フォーマットはフォント形式と文字
   色と背景色から構成される。組み合わせる場合は間を;で区切る。例えば太
   字、文字色・赤、背景色・白を指定したい場合、1;31;47 となる。

   @param[in] level この出力のデバッグプリントレベル。
   @param[in] color 色を指定する。
   @param[in] format fprintfのformatと同じ
   @param[in] args fprintfの残りの引数と同義
 */
#define DEBUG_COLOR_PRINT(color, format, args...)			\
	fprintf(DEBUGOUT,						\
		"\x1b["color"m""[%s : %s, line %d] "format"\x1b[0m",	\
		 __FUNCTION__, __FILE__, __LINE__, ##args)

/**
   @brief デバッグプリントの定義。色つき

   @details 
   formatに従ってメッセージを関数名、ファイル名、行番号と共にDEBUGOUTに
   指定のcolorフォーマットで出力する。フォーマットはフォント形式と文字
   色と背景色から構成される。組み合わせる場合は間を;で区切る。例えば太
   字、文字色・赤、背景色・白を指定したい場合、1;31;47 となる。

   @param[in] level この出力のデバッグプリントレベル。
   @param[in] color 色を指定する。
   @param[in] format fprintfのformatと同じ
   @param[in] args fprintfの残りの引数と同義
 */
#define DEBUG_COLOR_PRINTLN(color, format, args...)			 \
	fprintf(DEBUGOUT,						 \
		"\x1b["color"m""[%s : %s, line %d] "format"\x1b[0m""\n", \
		 __FUNCTION__, __FILE__, __LINE__, ##args)



/**
   @brief エラープリントの定義
   @details
   定義されたデバッグレベルがlevelよりも高いとき、formatに従ってメッセージを
   DEBUGOUTに赤/太字で出力する
   @param[in] format fprintfのformatと同じ
   @param[in] args fprintfの残りの引数と同義
*/
#define ERROR_PRINT(format, args...)					  \
	DEBUG_COLOR_PRINT(DEBUGPRINT_COLOR_FG_RED";"DEBUGPRINT_FONT_BOLD, \
			  format,					  \
			  ##args)

/**
   @brief エラープリントの定義
   @details
   定義されたデバッグレベルがlevelよりも高いとき、formatに従ってメッセージを
   DEBUGOUTに赤/太字で出力し、改行する．
   @param[in] format fprintfのformatと同じ
   @param[in] args fprintfの残りの引数と同義
*/
#define ERROR_PRINTLN(format, args...)					      \
	DEBUG_COLOR_PRINTLN(DEBUGPRINT_COLOR_FG_RED";"DEBUGPRINT_FONT_BOLD,   \
			  format,					      \
			  ##args)


#if defined(DEBUGPRINT_ALL)

#define DebugPrint(level, format, args...) \
	DEBUG_PRINT(format, ##args)

#define DebugColorPrint(level, color, format, args...)	\
	DEBUG_COLOR_PRINT(color, format, ##args)

#else /* DEBUGPRINT_ALL */

/**
   @brief デバッグプリントの定義
   @details
   定義されたデバッグレベルがlevelよりも高いとき、formatに従ってメッセージを
   DEBUGOUTに出力する
   @param[in] level この出力のデバッグプリントレベル。
   @param[in] format fprintfのformatと同じ
   @param[in] args fprintfの残りの引数と同義
 */
#define DebugPrint(level, format, args...)				\
     do {								\
	  if (level & DEBUG_LEVEL) {					\
		  fprintf(DEBUGOUT, "(0x%04x)", level);			\
		  DEBUG_PRINT(format, ##args);				\
	  }								\
     } while(0)

/**
   @brief デバッグプリントの定義。色つき
   @details 
   定義されたデバッグレベルがlevelよりも高いとき、formatに従ってメッセー
   ジをDEBUGOUTに指定のcolorフォーマットで出力する。
   @param[in] level この出力のデバッグプリントレベル。
   @param[in] color 色を指定する。
   @param[in] format fprintfのformatと同じ
   @param[in] args fprintfの残りの引数と同義
 */
#define DebugColorPrint(level, color, format, args...)			\
	do {								\
		if(level & DEBUG_LEVEL) {				\
			fprintf(DEBUGOUT, "(0x%04x)", level);		\
			DEBUG_COLOR_PRINT(color, format, ##args);	\
		}							\
	} while(0)

#endif /* DEBUGPRINT_ALL */

/**
   @brief エラープリントの定義
   @details
   定義されたデバッグレベルがlevelよりも高いとき、formatに従ってメッセージを
   DEBUGOUTに赤/太字で出力する
   @param[in] level この出力のデバッグプリントレベル。
   @param[in] format fprintfのformatと同じ
   @param[in] args fprintfの残りの引数と同義
*/
#define ErrorPrint(level, format, args...)				\
	DebugColorPrint(level,						\
			DEBUGPRINT_COLOR_FG_RED";"DEBUGPRINT_FONT_BOLD, \
			format,						\
			##args)


/**
   @brief assertの定義
   @details
   assertの起きた位置をDEBUGOUTに出力した後、assertする
*/
#define ASSERT(value)							\
     do {								\
	  DEBUG_VERSION_PRINT();					\
	  assert(value);						\
     } while(0)


#else /* ENABLE_DEBUGPRINT */

#include <assert.h>

#define DebugPrint(args...) do{} while(0)
#define DebugColorPrint(args...) do{} while(0)
#define ErrorPrint(args...) do{} while(0)
#define ASSERT(value) assert(value)

#endif /* ENABLE_DEBUGPRINT */

