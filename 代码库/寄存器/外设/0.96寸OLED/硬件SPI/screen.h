/**
 * @file screen.h
 * @author YEWANhub
 * @brief 利用OLED的基础图形绘制算法实现的动画
 * @version V1.0.0
 * @date 2022-06-10
 * @encoding GB2312
 */
#ifndef __SCREEN_H
#define __SCREEN_H

#include "sys.h"
#include "oled.h"

/**
 * @struct Circle_Dir
 * @brief 具有移动方向的圆结构体
 */
typedef struct{
	Circle c;
	u8 dir;
}Circle_Dir;
/**
 * @enum Circle_Dir_Typedef
 * @brief 具有移动方向的圆的移动方向枚举
 */
typedef enum{
	Circle_Dir_Up,
	Circle_Dir_Down,
	Circle_Dir_Left,
	Circle_Dir_Right,
	Circle_Dir_Max
}Circle_Dir_Typedef;

/**
 * @enum Fill_Dir_Typedef
 * @brief 填充方向枚举
 * @param Fill_Into 由外向里填充
 * @param Fill_Out 由里向外填充
 */
typedef enum{
	Fill_Into,
	Fill_Out
}Fill_Dir_Typedef;

void OLED_Draw_Rotating_Line_Angle(u8 const x0, u8 const y0, u8 const lenght, u8 mode);
void OLED_Draw_Filling_Circle(u8 const x0, u8 const y0, u8 const r, u8 dir, u8 mode);

static bool Judge_Ball_is_Out_of_Bounds(Circle_Dir *c);
static bool Judge_Two_Balls_is_Embed(Circle_Dir *c1, Circle_Dir *c2);

void OLED_Show_Sliding_Door(u8 const x0, u8 const y0, u8 const a, u8 const b, u8 mode);
void OLED_Show_Boncing_Ball(void);

void OLED_Show_Shrinking_Rectangle(u8 const x0, u8 const y0, u8 const a, u8 const b, u8 mode);
void OLED_Show_Live_of_Circle(u8 const x0, u8 const y0, u8 const r, u8 mode);
void OLED_Show_Filling_Circle_to_Whole_Screen(u8 const x0, u8 const y0, u8 mode);
void OLED_Show_Rotating_Ellipse(u8 const x0, u8 const y0, u8 const a, u8 const b, u16 const angle, u8 mode);
void OLED_Show_Rotating_Filled_Ellipse(u8 const x0, u8 const y0, u8 const a, u8 const b, u16 const angle, u8 mode);
void OLED_Show_Rotating_Two_Vertical_Ellipses(u8 const x0, u8 const y0, u8 const a, u8 const b, u16 const angle, u8 mode);
void OLED_Show_Rotating_Telescoping_Two_Vertical_Ellipses(u8 const x0, u8 const y0, u8 const a, u8 const b, u16 const angle, u8 mode);
void OLED_Show_Rectangle_to_Circle(u8 const x0, u8 const y0, u8 const a, u8 const b, u8 mode);
void OLED_Show_Rectangle_to_LikeCircle(u8 const x0, u8 const y0, u8 const a, u8 const b, u8 mode);
void OLED_Show_Rectangle_to_LikeCircle_Fillet_Overflow(u8 const x0, u8 const y0, u8 const a, u8 const b, u8 mode);
void OLED_Show_Rectangle_Catched_by_Elliipse(u8 const x0, u8 const y0, u8 const a, u8 const b, u8 mode);
void OLED_Show_DNA(u8 const x0, u8 const y0, u8 const a, u8 const b, u8 mode);

void Demo_01(void);

#endif
