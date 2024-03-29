/**
 * @file oled.c
 * @author ziqian移植至YEWANhub
 * @brief 实现OLED的驱动和一些基础图形的绘制
 * @version V1.0.0
 * @date 2023-06-13
 * @encoding GB2312
 * @details 在绘制旋转图形时，角度参数是角度制
 */
/******************************************************************************
	说明: 仅使用硬件SPI，无DMA
	----------------------------------------------------------------
	GND    电源地
	VCC  接5V或3.3v电源
	D0   接PB13（SCL）
	D1   接PB15（SDA）
	RES  接PD9
	DC   接PD11
	CS   接PD13
	----------------------------------------------------------------
******************************************************************************/

/**
 * 以下为用xoy直角坐标系中的y=x，y=-x和坐标轴将其划分为8个区域后
 * 得到的区域编号，在后续函数编写过程中会运用到
 */
/******************************************************************************

			八		一
		七				二
		六				三
			五		四

******************************************************************************/
#include "oled.h"
#include "oledfont.h"
#include "spi.h"
//#include "dma.h"

//OLED的显存
//存放格式如下（箭头左边为缓存数组的下标，右边为OLED页标）：
// 0 -> [7]0 1 2 3 ... 127
// 1 -> [6]0 1 2 3 ... 127
// 2 -> [5]0 1 2 3 ... 127
// 3 -> [4]0 1 2 3 ... 127
// 4 -> [3]0 1 2 3 ... 127
// 5 -> [2]0 1 2 3 ... 127
// 6 -> [1]0 1 2 3 ... 127
// 7 -> [0]0 1 2 3 ... 127
//每个字节存储的数据对应于OLED，高位在y轴方向（坐标原点在第7页第0列）
u8 OLED_GRAM[OLED_COL_MAX][OLED_PAGE_MAX];

//m^n
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}

/**
 * @brief 主机向从机（SSD1306）写入一个字节
 * @param[dat] 要写入的数据/命令
 * @param[cmd] 数据/命令标志 0,表示命令;1,表示数据
 * @return None
 */
static void OLED_WR_Byte(u8 dat,u8 cmd)
{
	SPI_WR_Byte(dat, cmd);   	  
}

/**
 * @brief 设置OLED光标起始点
 * @details 页只能设置成页起始位置
 * @param col 所要设置的光标位置的列坐标
 * @param page 所要设置的光标位置的页坐标
 * @return None
 */
static void OLED_SetPos(u8 col, u8 page)
{
	OLED_WR_Byte (0xb0+page,OLED_CMD);			// 设置页坐标
	OLED_WR_Byte (0x00+col&0X0F,OLED_CMD);		// 设置列地址（低4位）
	OLED_WR_Byte (0x10+((col&0XF0)>>4),OLED_CMD);	// 设置列地址（高4位）
}

/**
 * @brief 更新缓存到SSD1306
 * @return None
 */
void OLED_Refresh_Gram(void)
{
#if REFRESH_MODE == 0
	// 水平地址模式下的刷新函数
	u8 i,n;
	for(i=0;i<OLED_PAGE_MAX;i++)
	{
		OLED_SetPos(REFRESH_START_POS_COL, (REFRESH_START_POS_PAGE+i)%8);
		for(n=0;n<OLED_COL_MAX;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
	}
#elif REFRESH_MODE == 1
	// 垂直地址模式下的刷新函数
	u8 i,n;
	OLED_SetPos(REFRESH_START_POS_COL, REFRESH_START_POS_PAGE);
	for(i=0;i<OLED_COL_MAX;i++)
		for(n=0;n<OLED_PAGE_MAX;n++)
			OLED_WR_Byte(OLED_GRAM[i][n],OLED_DATA);
#elif USE_DMA && REFRESH_MODE == 2
	// 垂直地址模式下的DMA刷新函数
#if REFRESH_DMA_PREVENT_DISLOCATION_MODE
	// 预防显示错位
	OLED_SetPos(REFRESH_START_POS_COL, REFRESF_START_POS_PAGE);
#endif
	MyDMA_Enable();
#endif
}

/**
 * @brief OLED开显示
 * @return None
 */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);
	OLED_WR_Byte(0X14,OLED_CMD);
	OLED_WR_Byte(0XAF,OLED_CMD);
}

/**
 * @brief OLED关显示
 * @return None
 */
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);
	OLED_WR_Byte(0X10,OLED_CMD);
	OLED_WR_Byte(0XAE,OLED_CMD);
}

/**
 * @brief OLED清屏
 * @return None
 */
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<OLED_PAGE_MAX;i++)for(n=0;n<OLED_COL_MAX;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();
}

/**
 * @brief OLED画点
 * @details 若SHOW_EVERY_STEEP==1，则会在每一次调用该函数时进行刷新
 * @param x 所绘制的点的横坐标
 * @param y 所绘制的点的纵坐标
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_DrawPoint(u8 x,u8 y,u8 mode)
{
	u8 pos,bx,temp=0;
	if(x>OLED_COL_MAX-1||y>OLED_ROW_MAX-1)return;
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(mode)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;
#if SHOW_EVERY_STEEP
	OLED_Refresh_Gram();
#endif
}

/**
 * @brief OLED读点
 * @details 读取的是此时的OLED_GRAM中的信息，因此不能反映此时的屏幕状态
 * @param x 所读的点的横坐标
 * @param y 所读的点的纵坐标
 * @return int8_t 所读点的状态
 */
int8_t OLED_ReadPoint(u8 x,u8 y)
{
	u8 pos,bx,temp;
	if(x>OLED_COL_MAX-1||y>OLED_ROW_MAX-1)return -1;
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(OLED_GRAM[x][pos]&temp) return 1;
	else return 0;
}

//bool OLED_ReadPoint_by_CMD(u8 x, u8 y)
//{
//	// only 6800 and 8080
//}

/* ---------------- base graphics ---------------- */

/**
 * @brief OLED画线
 * @details 可以改变OLED_DRAW_LINE_BRESENHAM的宏定义来选择实现方法
 * @param x1 端点一的横坐标
 * @param y1 端点一的纵坐标
 * @param x2 端点二的横坐标
 * @param y2 端点二的纵坐标
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
#if OLED_DRAW_LINE_BRESENHAM
	int16_t dx = x2 - x1, dy = y2 - y1;
	Point_int16_t p = { .x = 0, .y = 0 };  //防止当 p.x=127或p.y=63 时卡死
	uint8_t Point_Position_Inverse_Flag = 0X00;  // 0,1,2,3,4bit分别表示第一、二、三、四象限及关于 y=x 对称标识
	
	if(dy==0)			// k==0
	{
		if(dx<0)
		{
			dx = -dx;
			x1 ^= x2; x2 ^= x1; x1 ^= x2;
		}
		for(; p.x<=dx; p.x++)
			OLED_DrawPoint(p.x+x1, p.y+y1, mode);
	}
	else if(dx==0)			// k不存在
	{
		if(dy<0)
		{
			dy = -dy;
			y1 ^= y2; y2 ^= y1; y1 ^= y2;
		}
		for(; p.y<=dy; p.y++)
			OLED_DrawPoint(p.x+x1, p.y+y1, mode);
	}
	else
	{
		//将 (x1, y1) 和 (x2, y2) 处理成第一象限中的情况
		//第三象限转换之后可以看成本来就是在第一象限中
		if(dx>0 && dy>0) Point_Position_Inverse_Flag |= 0X01;
		else if(dx>0 && dy <0)				//处理第二象限
		{
			Point_Position_Inverse_Flag |= 0X02;
			dy = -dy;
			y2 = 2 * y1 - y2;
		}
		else if(dx<0 && dy < 0)			//处理第三象限
		{
			Point_Position_Inverse_Flag |= 0X04;
			dx = -dx; dy = -dy;
			x1 ^= x2; x2 ^= x1; x1 ^= x2;
			y1 ^= y2; y2 ^= y1; y1 ^= y2;
		}
		else if(dx<0 && dy >0)		//处理第四象限
		{
			Point_Position_Inverse_Flag |= 0X08;
			dx = -dx;
			x2 = 2 * x1 - x2;
		}
		//将 (x1, y1) 和 (x2, y2) 处理成 0<k<=1 的情形（做变换 { x0'=x1, y0'=y1 }）
		if(dx<dy)
		{
			Point_Position_Inverse_Flag |= 0X10;
			Point const temp = { .x = x2, .y = y2 };
			x2 = x1 - y1 + temp.y;
			y2 = -x1 + y1 + temp.x;
			dx = x2 - x1; dy = y2 - y1;  // 重新计算 dx, dy 然后再处理成第一象限中的情况
		}
		
		int16_t incrE = 2 * dy, incrNE = 2 * (dy - dx);
		int16_t d = 2 * dy - dx;
		
		OLED_DrawPoint(p.x+x1, p.y+y1, mode);
		//开始计算点坐标偏移量
		for(p.x++; p.x<=dx; p.x++)
		{
			if(d<0){			//从东方衍生出新的像素
				d += incrE;
			}else{				//从东北方衍生出新的像素
				p.y++;
				d += incrNE;
			}
			
			//绘制点
			/* -- 相对于点 (x0, y0) 为原点的反函数之坐标 --
			   -- x'= x0-y0+y --
			   -- y'=-x0+y0+x -- */
			switch(Point_Position_Inverse_Flag&0X0F)
			{
				case 0X01:		//第一、三象限
				case 0X04:
					if(Point_Position_Inverse_Flag&0X10)
						OLED_DrawPoint(x1+p.y, y1+p.x, mode);
					else
						OLED_DrawPoint(x1+p.x, y1+p.y, mode);
					break;
				case 0X02:		//第二象限
					if(Point_Position_Inverse_Flag&0X10)
						OLED_DrawPoint(x1+p.y, y1-p.x, mode);
					else
						OLED_DrawPoint(x1+p.x, y1-p.y, mode);
					break;
				case 0X08:		//第四象限
					if(Point_Position_Inverse_Flag&0X10)
						OLED_DrawPoint(x1-p.y, y1+p.x, mode);
					else
						OLED_DrawPoint(x1-p.x, y1+p.y, mode);
					break;
				default: break;
			}
		}
	}
#else
	u32 t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2-x1;
	delta_y = y2-y1; 
	uRow = x1; 
	uCol = y1;
	if(delta_x>0) incx=1;
	else if(delta_x==0) incx=0;
	else {incx = -1; delta_x = -delta_x;} 
	if(delta_y>0) incy = 1; 
	else if(delta_y==0) incy = 0;
	else{incy = -1; delta_y = -delta_y;} 
	if(delta_x>delta_y) distance = delta_x;
	else distance = delta_y; 
	for(t=0; t<=distance+1; t++ )
	{  
		OLED_DrawPoint(uRow, uCol, mode);
		
		xerr += delta_x; 
		yerr += delta_y;
		if(xerr>distance)
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		}
	}
#endif
}

/**
 * @brief OLED角度画线（参数采用极坐标表示法）
 * @param x0 极点的横坐标
 * @param y0 极点的纵坐标
 * @param lenght 直线的极径
 * @param angle 直线的极角
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_DrawLine_Angle(u8 x0, u8 y0, u8 lenght, u16 angle, u8 mode)
{
	Point_Signed p;
	float rad = angle * (PI / 180);
	float sin_rad = sin(rad), cos_rad = cos(rad);
	
	p.x = lenght * cos_rad;
	p.y = lenght * sin_rad;
	
	OLED_DrawLine(x0, y0, p.x+x0, p.y+y0, mode);
}

/**
 * @brief OLED画方
 * @param x1 方形某一对角线上的端点一的横坐标
 * @param y1 方形某一对角线上的端点一的纵坐标
 * @param x2 方形某一对角线上的端点二的横坐标
 * @param y2 方形某一对角线上的端点二的纵坐标
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_DrawRectangle(u8 x1,u8 y1,u8 x2,u8 y2, u8 mode)
{
	OLED_DrawLine(x1,y1,x2,y1,mode);
	OLED_DrawLine(x1,y2,x2,y2,mode);
	OLED_DrawLine(x1,y1,x1,y2,mode);
	OLED_DrawLine(x2,y1,x2,y2,mode);
}

/**
 * @brief OLED画填充方
 * @details 采用的填充方法是以y轴方向为填充方向，遍历(y1:y2+1)填充直线
 * @details 因此矩形边不平行于坐标轴时，会存在没有填充上的情况
 * @param x1 方形某一对角线上的端点一的横坐标
 * @param y1 方形某一对角线上的端点一的纵坐标
 * @param x2 方形某一对角线上的端点二的横坐标
 * @param y2 方形某一对角线上的端点二的纵坐标
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_Filled_Rectangle(u8 x1,u8 y1,u8 x2,u8 y2, u8 mode)
{
	uint8_t i;
	
	// 处理点坐标
	if(y1>y2) y1 ^= y2, y2 ^= y1, y1 ^= y2;
	
	for(i=y1; i<y2; i++)
		OLED_DrawLine(x1, i, x2, i, mode);
}

/**
 * @brief OLED画方，智能处理溢出屏幕，具有一定的容错率，以中心点为坐标原点
 * @details 如果矩形边会溢出屏幕，将智能处理几何中心
 * @param x0 矩形中心的横坐标
 * @param y0 矩形中心的纵坐标
 * @param a 矩形的上下边的长
 * @param b 矩形的左右边的长
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_DrawRectangle_Intelligent_Overflow(u8 x0, u8 y0, u8 a, u8 b, u8 mode)
{
	uint8_t Overflow_Flag = 0X00;  // 0, 1, 2, 3bit 分别表示行低高位溢出、列低高位溢出
	Point p1, p2;
	
	// 初步处理并标记溢出
	if(x0-a/2<0)
	{
		Overflow_Flag |= 0X01;
		b = (1.0 * b) * x0 / a;
		a = 2 * x0;
	}
	if(y0-b/2<0)
	{
		Overflow_Flag |= 0X04;
		a = (1.0 * a) * (2 * y0) / b;
		b = 2 * y0;
	}
	// 最终处理
	if(x0+a/2>=OLED_COL_MAX)
	{
		Overflow_Flag |= 0X02;
		b = 1.0 * b * (2 * (OLED_COL_MAX - x0)) / b;
		a = 2 * (OLED_COL_MAX - x0);
	}
	if(y0+b/2>=OLED_ROW_MAX)
	{
		Overflow_Flag |= 0X08;
		a = 1.0 * a * (2 * (OLED_ROW_MAX - y0)) / a;
		b = 2 * (OLED_ROW_MAX - y0);
	}
	
	p1.x = x0 - a / 2; p1.y = y0 - b / 2;
	p2.x = x0 + a / 2; p2.y = y0 + b / 2;
	
	OLED_DrawLine(p1.x, p2.y, p2.x, p2.y, mode);
	OLED_DrawLine(p1.x, p1.y, p2.x, p1.y, mode);
	OLED_DrawLine(p1.x, p2.y, p1.x, p1.y, mode);
	OLED_DrawLine(p2.x, p2.y, p2.x, p1.y, mode);
}

/**
 * @brief OLED画标准圆角矩形，预留圆角区域，以中心点为坐标原点
 * @method 先按照矩形的画法画出一个缺角矩形，然后按照画圆的方法画出四个圆角
 * @param x0 圆角矩形中心的横坐标
 * @param y0 圆角矩形中心的纵坐标
 * @param a 圆角矩形的上下边的长
 * @param b 圆角矩形的左右边的长
 * @param r 圆角矩形的圆角的半径
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_Rounded_Rectangle(u8 x0, u8 y0, u8 a, u8 b, u8 r, u8 mode)
{
	Point p1, p2;
	
	p1.x = x0-a/2; p1.y = y0-b/2;
	p2.x = x0+a/2; p2.y = y0+b/2;
	
	//防止圆角溢出
	if(r>a/2 || r>b/2) r = (a<b)?(a/2):(b/2);
	
	//画缺角矩形
	OLED_DrawLine( p1.x+r,	p2.y,	p2.x-r,	p2.y,	mode );  // 上
	OLED_DrawLine( p1.x+r,	p1.y,	p2.x-r,	p1.y,	mode );  // 下
	OLED_DrawLine( p1.x,	p2.y-r,	p1.x,	p1.y+r,	mode );  // 左
	OLED_DrawLine( p2.x,	p2.y-r,	p2.x,	p1.y+r,	mode );  // 右
	
	//画圆角
	Point p = {
		.x = 0,
		.y = r
	};
	// 因为取点是一区域
	OLED_Draw_8_Pixels_Spread_Out_From_Center(x0, y0, p.x+x0, p.y+y0, a/2-r, b/2-r, mode);
	int8_t d = 1 - r;
	for(p.x=1; p.x<=p.y; p.x++)
	{
		if(d<0){				/* goEast */
			d += 2 * p.x + 1;
		}
		else{					/* goSouthEast */
			p.y--;
			d += 2 * (p.x - p.y) + 1;
		}
		OLED_Draw_8_Pixels_Spread_Out_From_Center(x0, y0, p.x+x0, p.y+y0, a/2-r, b/2-r, mode);
	}
}

/**
 * @brief OLED画标准圆角矩形，使用擦除法，以中心点为坐标原点
 * @method 先按照矩形的画法画出一个缺角矩形，然后按照画圆的方法画出四个圆角
 * @param x0 圆角矩形中心的横坐标
 * @param y0 圆角矩形中心的纵坐标
 * @param a 圆角矩形的上下边的长
 * @param b 圆角矩形的左右边的长
 * @param r 圆角矩形的圆角的半径
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_Rounded_Rectangle_Erasure_Method(u8 x0, u8 y0, u8 a, u8 b, u8 r, u8 mode)
{
	Point p1, p2;
	
	p1.x = x0-a/2; p1.y = y0-b/2;
	p2.x = x0+a/2; p2.y = y0+b/2;
	
	//防止圆角溢出
	if(r>a/2 || r>b/2) r = (a<b)?(a/2):(b/2);
	
	//画缺角矩形
	OLED_DrawRectangle( p1.x, p1.y, p2.x, p2.y, mode );
	OLED_DrawLine( p1.x, p1.y, p1.x+r,	p1.y,	!mode);  // 上
	OLED_DrawLine( p2.x, p1.y, p2.x-r,	p1.y,	!mode);
	OLED_DrawLine( p1.x, p2.y, p1.x+r,	p2.y,	!mode);  // 下
	OLED_DrawLine( p2.x, p2.y, p2.x-r,	p2.y,	!mode);
	OLED_DrawLine( p1.x, p1.y, p1.x,	p1.y+r,	!mode);  // 左
	OLED_DrawLine( p1.x, p2.y, p1.x,	p2.y-r,	!mode);
	OLED_DrawLine( p2.x, p1.y, p2.x,	p1.y+r,	!mode);  // 右
	OLED_DrawLine( p2.x, p2.y, p2.x,	p2.y-r,	!mode);
	
	//画圆角
	Point p = {
		.x = 0,
		.y = r
	};
	// 因为取点是一区域
	OLED_Draw_8_Pixels_Spread_Out_From_Center(x0, y0, p.x+x0, p.y+y0, a/2-r, b/2-r, mode);
	int8_t d = 1 - r;
	for(p.x=1; p.x<=p.y; p.x++)
	{
		if(d<0){				/* goEast */
			d += 2 * p.x + 1;
		}
		else{					/* goSouthEast */
			p.y--;
			d += 2 * (p.x - p.y) + 1;
		}
		OLED_Draw_8_Pixels_Spread_Out_From_Center(x0, y0, p.x+x0, p.y+y0, a/2-r, b/2-r, mode);
	}
}

/**
 * @brief OLED画标准圆角矩形，但是圆角会溢出，以中心点为坐标原点
 * @method 先按照矩形的画法画出一个缺角矩形，然后按照画圆的方法画出四个圆角
 * @param x0 圆角矩形中心的横坐标
 * @param y0 圆角矩形中心的纵坐标
 * @param a 圆角矩形的上下边的长
 * @param b 圆角矩形的左右边的长
 * @param r 圆角矩形的圆角的半径
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_Rounded_Rectangle_Fillet_Overflow(u8 x0, u8 y0, u8 a, u8 b, u8 r, u8 mode)
{
	Point p1, p2;
	
	p1.x = x0-a/2; p1.y = y0-b/2;
	p2.x = x0+a/2; p2.y = y0+b/2;
	
	//画缺角矩形
	OLED_DrawRectangle( p1.x, p1.y, p2.x, p2.y, mode );
	OLED_DrawLine( p1.x, p1.y, p1.x+r,	p1.y,	!mode);  // 上
	OLED_DrawLine( p2.x, p1.y, p2.x-r,	p1.y,	!mode);
	OLED_DrawLine( p1.x, p2.y, p1.x+r,	p2.y,	!mode);  // 下
	OLED_DrawLine( p2.x, p2.y, p2.x-r,	p2.y,	!mode);
	OLED_DrawLine( p1.x, p1.y, p1.x,	p1.y+r,	!mode);  // 左
	OLED_DrawLine( p1.x, p2.y, p1.x,	p2.y-r,	!mode);
	OLED_DrawLine( p2.x, p1.y, p2.x,	p1.y+r,	!mode);  // 右
	OLED_DrawLine( p2.x, p2.y, p2.x,	p2.y-r,	!mode);
	
	//画圆角
	Point p = {
		.x = 0,
		.y = r
	};
	// 因为取点是一区域
	OLED_Draw_8_Pixels_Spread_Out_From_Center(x0, y0, p.x+x0, p.y+y0, a/2-r, b/2-r, mode);
	int8_t d = 1 - r;
	for(p.x=1; p.x<=p.y; p.x++)
	{
		if(d<0){				/* goEast */
			d += 2 * p.x + 1;
		}
		else{					/* goSouthEast */
			p.y--;
			d += 2 * (p.x - p.y) + 1;
		}
		OLED_Draw_8_Pixels_Spread_Out_From_Center(x0, y0, p.x+x0, p.y+y0, a/2-r, b/2-r, mode);
	}
}

/**
 * @brief OLED画标准圆角矩形，但是圆角会溢出，使用的是擦除法，以中心点为坐标原点
 * @method 先按照矩形的画法画出一个缺角矩形，然后按照画圆的方法画出四个圆角
 * @param x0 圆角矩形中心的横坐标
 * @param y0 圆角矩形中心的纵坐标
 * @param a 圆角矩形的上下边的长
 * @param b 圆角矩形的左右边的长
 * @param r 圆角矩形的圆角的半径
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_Rounded_Rectangle_Fillet_Overflow_Erasure_Method(u8 x0, u8 y0, u8 a, u8 b, u8 r, u8 mode)
{
	Point p1, p2;
	
	p1.x = x0-a/2; p1.y = y0-b/2;
	p2.x = x0+a/2; p2.y = y0+b/2;
	
	//画缺角矩形
	OLED_DrawRectangle( p1.x, p1.y, p2.x, p2.y, mode );
	OLED_DrawLine( p1.x, p1.y, p1.x+r,	p1.y,	!mode);  // 上
	OLED_DrawLine( p2.x, p1.y, p2.x-r,	p1.y,	!mode);
	OLED_DrawLine( p1.x, p2.y, p1.x+r,	p2.y,	!mode);  // 下
	OLED_DrawLine( p2.x, p2.y, p2.x-r,	p2.y,	!mode);
	OLED_DrawLine( p1.x, p1.y, p1.x,	p1.y+r,	!mode);  // 左
	OLED_DrawLine( p1.x, p2.y, p1.x,	p2.y-r,	!mode);
	OLED_DrawLine( p2.x, p1.y, p2.x,	p1.y+r,	!mode);  // 右
	OLED_DrawLine( p2.x, p2.y, p2.x,	p2.y-r,	!mode);
	
	//画圆角
	Point p = {
		.x = 0,
		.y = r
	};
	// 因为取点是一区域
	OLED_Draw_8_Pixels_Spread_Out_From_Center(x0, y0, p.x+x0, p.y+y0, a/2-r, b/2-r, mode);
	int8_t d = 1 - r;
	for(p.x=1; p.x<=p.y; p.x++)
	{
		if(d<0){				/* goEast */
			d += 2 * p.x + 1;
		}
		else{					/* goSouthEast */
			p.y--;
			d += 2 * (p.x - p.y) + 1;
		}
		OLED_Draw_8_Pixels_Spread_Out_From_Center(x0, y0, p.x+x0, p.y+y0, a/2-r, b/2-r, mode);
	}
}

/**
 * @brief OLED放置 4 个像素，以中心点为坐标原点
 * @param x0 4个点的中心的横坐标
 * @param y0 4个点的中心的纵坐标
 * @param x 4个点中的一个位于第一象限上的点的横坐标
 * @param y 4个点中的一个位于第一象限上的点的纵坐标
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_4_Pixels(u8 x0, u8 y0, u8 x, u8 y, u8 mode)
{
	OLED_DrawPoint(x,		y,			mode);				/* 第一象限 */
	OLED_DrawPoint(x,		2*y0-y,		mode);				/* 第二象限 */
	OLED_DrawPoint(2*x0-x,	2*y0-y,		mode);				/* 第三象限 */
	OLED_DrawPoint(2*x0-x,	y,			mode);				/* 第四象限 */
}

/**
 * @brief OLED放置 2 条镜像线，这两条镜像线关于中心点处的x轴对称
 * @param x0 4个点的中心的横坐标
 * @param y0 4个点的中心的纵坐标
 * @param x 4个端点中的一个位于第一象限上的点的横坐标
 * @param y 4个端点中的一个位于第一象限上的点的纵坐标
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_4_Pixels_Lines(u8 x0, u8 y0, u8 x, u8 y, u8 mode)
{
	OLED_DrawLine(2*x0-x,	y,			x,			y,			mode);	/* x轴上方的线 */
	OLED_DrawLine(2*x0-x,	2*y0-y,		x,			2*y0-y,		mode);	/* x轴下方的线 */
}

/**
 * @brief OLED放置 4 个绕中心旋转的像素，以中心点为坐标原点
 * @param x0 4个点的中心的横坐标
 * @param y0 4个点的中心的纵坐标
 * @param x 4个端点中的一个位于第一象限上的点的横坐标
 * @param y 4个端点中的一个位于第一象限上的点的纵坐标
 * @param angle 相对x轴的旋转角度（逆时针）
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
// x' = xcosb - ysinb
// y' = xsinb + ycosb
// 计算对称点前需要变换一下坐标，因此使用 int16_t 数据类型以满足负数和范围需求
void OLED_Draw_4_Pixels_Rotate(u8 x0, u8 y0, int16_t x, int16_t y, u16 angle, u8 mode)
{
	float rad = angle * (PI / 180);
	float sin_rad = sin(rad), cos_rad = cos(rad);
	x -= x0; y -= y0;  // 得到基础偏移量
	float x_s = x*sin_rad,		x_c = x*cos_rad,		y_s = y*sin_rad,		y_c = y*cos_rad;
	float _x_s = (-x)*sin_rad,	_x_c = (-x)*cos_rad,	_y_s = (-y)*sin_rad,	_y_c = (-y)*cos_rad;
	
	//注意加上中心点的坐标，因为计算结果是偏移量
	OLED_DrawPoint(x_c-y_s		+x0,	x_s+y_c		+y0,	mode);					/* 第一象限 */
	OLED_DrawPoint(x_c-_y_s		+x0,	x_s+_y_c	+y0,	mode);					/* 第二象限 */
	OLED_DrawPoint(_x_c-_y_s	+x0,	_x_s+_y_c	+y0,	mode);					/* 第三象限 */
	OLED_DrawPoint(_x_c-y_s		+x0,	_x_s+y_c	+y0,	mode);					/* 第四象限 */
}

/**
 * @brief OLED画 2 条绕中心旋转镜像线，以中心点为坐标原点
 * @param x0 4个点的中心的横坐标
 * @param y0 4个点的中心的纵坐标
 * @param x 4个端点中的一个位于第一象限上的点的横坐标
 * @param y 4个端点中的一个位于第一象限上的点的纵坐标
 * @param angle 相对x轴的旋转角度（逆时针）
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
// x' = xcosb - ysinb
// y' = xsinb + ycosb
// 计算对称点前需要变换一下坐标，因此使用 int16_t 数据类型以满足负数和范围需求
void OLED_Draw_4_Pixels_Rotate_Lines(u8 x0, u8 y0, int16_t x, int16_t y, u16 angle, u8 mode)
{
	float rad = angle * (PI / 180);
	float sin_rad = sin(rad), cos_rad = cos(rad);
	x -= x0; y -= y0;  // 得到基础偏移量
	float x_s = x*sin_rad,		x_c = x*cos_rad,		y_s = y*sin_rad,		y_c = y*cos_rad;
	float _x_s = (-x)*sin_rad,	_x_c = (-x)*cos_rad,	_y_s = (-y)*sin_rad,	_y_c = (-y)*cos_rad;
	
	//注意加上中心点的坐标，因为计算结果是偏移量
	OLED_DrawLine(_x_c-y_s +x0,  _x_s+y_c  +y0, x_c-y_s  +x0, x_s+y_c  +y0, mode);	/* 对称轴上方 */
	OLED_DrawLine(_x_c-_y_s	+x0, _x_s+_y_c +y0, x_c-_y_s +x0, x_s+_y_c +y0, mode);	/* 对称轴下方 */
}

/**
 * @brief 以某一点为中心绘制4个点，这4个点和中心点之间的距离为 (dx, dy)
 * @details 偏移距离是想读与一区域上的点而言的
 * @param x0 对称中心的横坐标
 * @param y0 对称中心的纵坐标
 * @param x 需要绘制的4个点中，位于一区域的点的横坐标
 * @param y 需要绘制的4个点中，位于一区域的点的纵坐标
 * @param dx 偏移距离的横轴分量
 * @param dy 偏移距离的纵轴分量
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_4_Pixels_Spread_Out_From_Center(u8 x0, u8 y0, u8 x, u8 y, u8 dx, u8 dy, u8 mode)
{
	OLED_DrawPoint(x		+dx,	y		+dy,	mode);		/* 第一象限 */
	OLED_DrawPoint(x		+dx,	2*y0-y	-dy,	mode);		/* 第二象限 */
	OLED_DrawPoint(2*x0-x	-dx,	2*y0-y	-dy,	mode);		/* 第三象限 */
	OLED_DrawPoint(2*x0-x	-dx,	y		+dy,	mode);		/* 第四象限 */
}

/**
 * @brief OLED放置 8 个像素
 * @param x0 8个点的中心的横坐标
 * @param y0 8个点的中心的纵坐标
 * @param x 8个端点中的一个位于一区域上的点的横坐标
 * @param y 8个端点中的一个位于一区域上的点的纵坐标
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
/* --------------------------------------------------- *
 * 一	x = x,					y = y
 * 二	x = y -y0+x0,			y = x-x0+y0
 * 三	x = y-y0+x0,			y = 2*y0-(x-x0+y0)
 * 四	x = x,					y = 2*y0-y
 * 五	x = 2*x0-x,				y = 2*y0-y
 * 六	x = 2*x0-(y-y0+x0),		y = 2*y0-(x-x0+y0)
 * 七	x = 2*x0-(y-y0+x0),		y = x-x0+y0
 * 八	x = 2*x0-x,				y = y
 * --------------------------------------------------- *
 */
void OLED_Draw_8_Pixels(u8 x0, u8 y0, u8 x, u8 y, u8 mode)
{
	OLED_DrawPoint(x,			y,				mode);				/* 一 */
	OLED_DrawPoint(y+x0-y0,		x-x0+y0,		mode);				/* 二 */
	OLED_DrawPoint(y+x0-y0,		-x+x0+y0,		mode);				/* 三 */
	OLED_DrawPoint(x,			-y+y0+y0,		mode);				/* 四 */
	OLED_DrawPoint(-x+x0+x0,	-y+y0+y0,		mode);				/* 五 */
	OLED_DrawPoint(-y+x0+y0,	-x+x0+y0,		mode);				/* 六 */
	OLED_DrawPoint(-y+x0+y0,	x-x0+y0,		mode);				/* 七 */
	OLED_DrawPoint(-x+x0+x0,	y,				mode);				/* 八 */
}

/**
 * @brief OLED放置 4 条镜像线，这 4 条镜像线关于中心点处的x轴对称
 * @details 一区域中的点的特点是：dy>dx>0
 * @param x0 8个点的中心的横坐标
 * @param y0 8个点的中心的纵坐标
 * @param x 8个端点中的一个位于一区域上的点的横坐标
 * @param y 8个端点中的一个位于一区域上的点的纵坐标
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_8_Pixels_Lines(u8 x0, u8 y0, u8 x, u8 y, u8 mode)
{
	OLED_DrawLine(2*x0-x,	y,			x,			y,			mode);	/* x轴上方的线 */
	OLED_DrawLine(-y+x0+y0,	x-x0+y0,	y+x0-y0,	x-x0+y0,	mode);
	OLED_DrawLine(-y+x0+y0,	-x+x0+y0,	y+x0-y0,	-x+x0+y0,	mode);	/* x轴下方的线 */
	OLED_DrawLine(2*x0-x,	2*y0-y,		x,			2*y0-y,		mode);
}

/**
 * @brief 以某一点为中心绘制8个点，这八个点和中心点之间的距离为 (dx, dy)
 * @details 偏移距离是想读与一区域上的点而言的
 * @param x0 对称中心的横坐标
 * @param y0 对称中心的纵坐标
 * @param x 8个端点中的一个位于一区域上的点的横坐标
 * @param y 8个端点中的一个位于一区域上的点的纵坐标
 * @param dx 偏移距离的横轴分量
 * @param dy 偏移距离的纵轴分量
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_8_Pixels_Spread_Out_From_Center(u8 x0, u8 y0, u8 x, u8 y, u8 dx, u8 dy, u8 mode)
{
	OLED_DrawPoint(x		+dx,	y			+dy,	mode);		/* 一 */
	OLED_DrawPoint(y+x0-y0	+dx,	x-x0+y0		+dy,	mode);		/* 二 */
	OLED_DrawPoint(y+x0-y0	+dx,	-x+x0+y0	-dy,	mode);		/* 三 */
	OLED_DrawPoint(x		+dx,	-y+y0+y0	-dy,	mode);		/* 四 */
	OLED_DrawPoint(-x+x0+x0	-dx,	-y+y0+y0	-dy,	mode);		/* 五 */
	OLED_DrawPoint(-y+x0+y0	-dx,	-x+x0+y0	-dy,	mode);		/* 六 */
	OLED_DrawPoint(-y+x0+y0	-dx,	x-x0+y0		+dy,	mode);		/* 七 */
	OLED_DrawPoint(-x+x0+x0	-dx,	y			+dy,	mode);		/* 八 */
}

/**
 * @brief OLED放置 8 个绕中心旋转的像素，以中心点为坐标原点
 * @param x0 8个点的中心的横坐标
 * @param y0 8个点的中心的纵坐标
 * @param x 8个端点中的一个位于一区域上的点的横坐标
 * @param y 8个端点中的一个位于一区域上的点的纵坐标
 * @param angle 相对x轴的旋转角度（逆时针）
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
// x' = xcosb - ysinb
// y' = xsinb + ycosb
// 计算对称点前需要变换一下坐标，因此使用 int16_t 数据类型以满足负数和范围需求
void OLED_Draw_8_Pixels_Rotate(u8 x0, u8 y0, int16_t x, int16_t y, u16 angle, u8 mode)
{
	float rad = angle * (PI / 180);
	float sin_rad = sin(rad), cos_rad = cos(rad);
	//计算旋转后的偏移量（下方这些并没有真正算出偏移量，只是减少计算时间），1表示负号，大写的 X,Y 用来区分横纵坐标
	float	x_X_s = (x-x0)*sin_rad,				x_X_c = (x-x0)*cos_rad,\
			y_Y_s = (y-y0)*sin_rad,				y_Y_c = (y-y0)*cos_rad,\
			y_x_1y_X_s = (y+x0-y0-x0)*sin_rad,	y_x_1y_X_c = (y+x0-y0-x0)*cos_rad,\
			x_1x_y_Y_s = (x-x0+y0-y0)*sin_rad,	x_1x_y_Y_c = (x-x0+y0-y0)*cos_rad,\
			_x_x_y_Y_s = (-x+x0+y0-y0)*sin_rad,	_x_x_y_Y_c = (-x+x0+y0-y0)*cos_rad,\
			_y_x_y_X_s = (-y+x0+y0-x0)*sin_rad,	_y_x_y_X_c = (-y+x0+y0-x0)*cos_rad,\
			_x_x_x_X_s = (-x+x0+x0-x0)*sin_rad,	_x_x_x_X_c = (-x+x0+x0-x0)*cos_rad,\
			_y_y_y_Y_s = (-y+y0+y0-y0)*sin_rad,	_y_y_y_Y_c = (-y+y0+y0-y0)*cos_rad;
	
	//注意加上中心点的坐标，因为计算结果是偏移量
	OLED_DrawPoint( x_X_c-y_Y_s				+x0,	x_X_s+y_Y_c				+y0,	mode );				/* 一 */
	OLED_DrawPoint( y_x_1y_X_c-x_1x_y_Y_s	+x0,	y_x_1y_X_s+x_1x_y_Y_c	+y0,	mode );				/* 二 */
	OLED_DrawPoint( y_x_1y_X_c-_x_x_y_Y_s	+x0,	y_x_1y_X_s+_x_x_y_Y_c	+y0,	mode );				/* 三 */
	OLED_DrawPoint( x_X_c-_y_y_y_Y_s		+x0,	x_X_s+_y_y_y_Y_c		+y0,	mode );				/* 四 */
	OLED_DrawPoint( _x_x_x_X_c-_y_y_y_Y_s	+x0,	_x_x_x_X_s+_y_y_y_Y_c	+y0,	mode );				/* 五 */
	OLED_DrawPoint( _y_x_y_X_c-_x_x_y_Y_s	+x0,	_y_x_y_X_s+_x_x_y_Y_c	+y0,	mode );				/* 六 */
	OLED_DrawPoint( _y_x_y_X_c-x_1x_y_Y_s	+x0,	_y_x_y_X_s+x_1x_y_Y_c	+y0,	mode );				/* 七 */
	OLED_DrawPoint( _x_x_x_X_c-y_Y_s		+x0,	_x_x_x_X_s+y_Y_c		+y0,	mode );				/* 八 */
}

/**
 * @brief OLED画圆
 * @method brasenham方法
 * @param x0 圆心的横坐标
 * @param y0 圆心的纵坐标
 * @param r 圆的半径
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_DrawCircle(u8 x0, u8 y0, u8 r, u8 mode)
{
	Point p = {
		.x = 0,
		.y = r
	};
	OLED_Draw_8_Pixels(x0, y0, p.x+x0, p.y+y0, mode);
	int8_t d = 1 - r;
	for(p.x=1; p.x<=p.y; p.x++)
	{
		if(d<0){				/* goEast */
			d += 2 * p.x + 1;
		}
		else{					/* goSouthEast */
			p.y--;
			d += 2 * (p.x - p.y) + 1;
		}
		OLED_Draw_8_Pixels(x0, y0, p.x+x0, p.y+y0, mode);
	}
}

/**
 * @brief OLED画填充圆
 * @method brasenham方法
 * @param x0 圆心的横坐标
 * @param y0 圆心的纵坐标
 * @param r 圆的半径
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_Filled_Circle(u8 x0, u8 y0, u8 r, u8 mode)
{
	Point p = {
		.x = 0,
		.y = r
	};
	OLED_Draw_8_Pixels_Lines(x0, y0, p.x+x0, p.y+y0, mode);
	int8_t d = 1 - r;
	for(p.x=1; p.x<=p.y; p.x++)
	{
		if(d<0){				/* goEast */
			d += 2 * p.x + 1;
		}
		else{					/* goSouthEast */
			p.y--;
			d += 2 * (p.x - p.y) + 1;
		}
		OLED_Draw_8_Pixels_Lines(x0, y0, p.x+x0, p.y+y0, mode);
	}
}

/**
 * @brief OLED画任意弧段，以圆心为坐标原点
 * @param x0 弧段圆心的横坐标
 * @param y0 弧段圆心的纵坐标
 * @param r 弧段半径
 * @param thet 弧段圆心角
 * @param angle 弧段相对于x轴的旋转角度（逆时针）（角度制）
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_DrawArc(u8 x0, u8 y0, u8 r, u8 thet, u8 angle, u8 mode)
{}

/**
 * @brief OLED画标准椭圆
 * @method brasenham方法
 * @param x0 椭圆中心的横坐标
 * @param y0 椭圆中心的纵坐标
 * @param a 椭圆的长半轴长
 * @param b 椭圆的短半轴长
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_DrawEllipse(u8 x0, u8 y0, u8 a, u8 b, u8 mode)
{
#if OLED_DRAW_ELLIPSE_BRESENHAM
	/* --------------- Bresenham --------------- */
	Point_Signed p = { .x = 0, .y = b };
	float d1 = b * b + a * a * (-b + 0.25);
	
	OLED_Draw_4_Pixels(x0, y0, x0+p.x, y0+p.y, mode);
	while (b * b * (p.x + 1) < a * a * (p.y - 0.5))
	{
		if (d1 <= 0)
		{
			d1 += b * b * (2 * p.x + 3);
			p.x++;
		}
		else
		{
			d1 += (b * b * (2 * p.x + 3) + a * a * (-2 * p.y + 2));
			p.x++;
			p.y--;
		}
		OLED_Draw_4_Pixels(x0, y0, x0+p.x, y0+p.y, mode);
	}
	float d2 = b * b * (p.x + 0.5) * (p.x + 0.5) + a * a * (p.y - 1) * (p.y - 1) - a * a * b * b;
	while (p.y > 0)
	{
		if (d2 <= 0)
		{
			d2 += b * b * (2 * p.x + 2) + a * a * (-2 * p.y + 3);
			p.x++;
			p.y--;
		}
		else
		{
			d2 += a * a * (-2 * p.y + 3);
			p.y--;
		}
		OLED_Draw_4_Pixels(x0, y0, x0+p.x, y0+p.y, mode);
	}
	/* --------------- Bresenham --------------- */
#endif
}

/**
 * @brief OLED画标准填充椭圆
 * @param x0 椭圆中心的横坐标
 * @param y0 椭圆中心的纵坐标
 * @param a 椭圆的长半轴长
 * @param b 椭圆的短半轴长
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_Filled_Ellipse(u8 x0, u8 y0, u8 a, u8 b, u8 mode)
{
#if OLED_DRAW_ELLIPSE_BRESENHAM
	/* --------------- Bresenham --------------- */
	Point_Signed p = { .x = 0, .y = b };
	float d1 = b * b + a * a * (-b + 0.25);
	
	OLED_Draw_4_Pixels_Lines(x0, y0, x0+p.x, y0+p.y, mode);
	while (b * b * (p.x + 1) < a * a * (p.y - 0.5))
	{
		if (d1 <= 0)
		{
			d1 += b * b * (2 * p.x + 3);
			p.x++;
		}
		else
		{
			d1 += (b * b * (2 * p.x + 3) + a * a * (-2 * p.y + 2));
			p.x++;
			p.y--;
		}
		OLED_Draw_4_Pixels_Lines(x0, y0, x0+p.x, y0+p.y, mode);
	}
	float d2 = b * b * (p.x + 0.5) * (p.x + 0.5) + a * a * (p.y - 1) * (p.y - 1) - a * a * b * b;
	while (p.y > 0)
	{
		if (d2 <= 0)
		{
			d2 += b * b * (2 * p.x + 2) + a * a * (-2 * p.y + 3);
			p.x++;
			p.y--;
		}
		else
		{
			d2 += a * a * (-2 * p.y + 3);
			p.y--;
		}
		OLED_Draw_4_Pixels_Lines(x0, y0, x0+p.x, y0+p.y, mode);
	}
	/* --------------- Bresenham --------------- */
#endif
}

/**
 * @brief OLED画两个相互垂直的标准椭圆
 * @param x0 两个椭圆中心的横坐标
 * @param y0 两个椭圆中心的纵坐标
 * @param a 两个椭圆的长半轴长
 * @param b 两个椭圆的短半轴长
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_Two_Vertical_Ellipses(u8 x0, u8 y0, u8 a, u8 b, u8 mode)
{
#if OLED_DRAW_ELLIPSE_BRESENHAM
	/* --------------- Bresenham --------------- */
	Point_Signed p = { .x = 0, .y = b };
	float d1 = b * b + a * a * (-b + 0.25);
	
	OLED_Draw_8_Pixels(x0, y0, x0+p.x, y0+p.y, mode);
	while (b * b * (p.x + 1) < a * a * (p.y - 0.5))
	{
		if (d1 <= 0)
		{
			d1 += b * b * (2 * p.x + 3);
			p.x++;
		}
		else
		{
			d1 += (b * b * (2 * p.x + 3) + a * a * (-2 * p.y + 2));
			p.x++;
			p.y--;
		}
		OLED_Draw_8_Pixels(x0, y0, x0+p.x, y0+p.y, mode);
	}
	float d2 = b * b * (p.x + 0.5) * (p.x + 0.5) + a * a * (p.y - 1) * (p.y - 1) - a * a * b * b;
	while (p.y > 0)
	{
		if (d2 <= 0)
		{
			d2 += b * b * (2 * p.x + 2) + a * a * (-2 * p.y + 3);
			p.x++;
			p.y--;
		}
		else
		{
			d2 += a * a * (-2 * p.y + 3);
			p.y--;
		}
		OLED_Draw_8_Pixels(x0, y0, x0+p.x, y0+p.y, mode);
	}
	/* --------------- Bresenham --------------- */
#endif
}

/**
 * @brief OLED画非标准椭圆
 * @method brasenham方法
 * @param x0 椭圆中心的横坐标
 * @param y0 椭圆中心的纵坐标
 * @param a 椭圆的长半轴长
 * @param b 椭圆的短半轴长
 * @param angle 相对于x轴的旋转角度（逆时针）（角度制）
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
// x' = xcosb - ysinb
// y' = xsinb + ycosb
void OLED_DrawEllipse_Rotate(u8 x0, u8 y0, u8 a, u8 b, u16 angle, u8 mode)
{
#if OLED_DRAW_ELLIPSE_BRESENHAM
	/* --------------- Bresenham --------------- */
	Point_Signed p = { .x = 0, .y = b };
	float d1 = b * b + a * a * (-b + 0.25);
	
	OLED_Draw_4_Pixels_Rotate(x0, y0, x0+p.x, y0+p.y, angle, mode);
	while (b * b * (p.x + 1) < a * a * (p.y - 0.5))
	{
		if (d1 <= 0)
		{
			d1 += b * b * (2 * p.x + 3);
			p.x++;
		}
		else
		{
			d1 += (b * b * (2 * p.x + 3) + a * a * (-2 * p.y + 2));
			p.x++;
			p.y--;
		}
		OLED_Draw_4_Pixels_Rotate(x0, y0, x0+p.x, y0+p.y, angle, mode);
	}
	float d2 = b * b * (p.x + 0.5) * (p.x + 0.5) + a * a * (p.y - 1) * (p.y - 1) - a * a * b * b;
	while (p.y > 0)
	{
		if (d2 <= 0)
		{
			d2 += b * b * (2 * p.x + 2) + a * a * (-2 * p.y + 3);
			p.x++;
			p.y--;
		}
		else
		{
			d2 += a * a * (-2 * p.y + 3);
			p.y--;
		}
		OLED_Draw_4_Pixels_Rotate(x0, y0, x0+p.x, y0+p.y, angle, mode);
	}
	/* --------------- Bresenham --------------- */
#endif
}

/**
 * @brief OLED画非标准椭圆
 * @param x0 椭圆中心的横坐标
 * @param y0 椭圆中心的纵坐标
 * @param a 椭圆的长半轴长
 * @param b 椭圆的短半轴长
 * @param angle 相对于x轴的旋转角度（逆时针）（角度制）
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
// x' = xcosb - ysinb
// y' = xsinb + ycosb
void OLED_Draw_Filled_Ellipse_Rotate(u8 x0, u8 y0, u8 a, u8 b, u16 angle, u8 mode)
{
#if OLED_DRAW_ELLIPSE_BRESENHAM
	/* --------------- Bresenham --------------- */
	Point_Signed p = { .x = 0, .y = b };
	float d1 = b * b + a * a * (-b + 0.25);
	
	OLED_Draw_4_Pixels_Rotate_Lines(x0, y0, x0+p.x, y0+p.y, angle, mode);
	while (b * b * (p.x + 1) < a * a * (p.y - 0.5))
	{
		if (d1 <= 0)
		{
			d1 += b * b * (2 * p.x + 3);
			p.x++;
		}
		else
		{
			d1 += (b * b * (2 * p.x + 3) + a * a * (-2 * p.y + 2));
			p.x++;
			p.y--;
		}
		OLED_Draw_4_Pixels_Rotate_Lines(x0, y0, x0+p.x, y0+p.y, angle, mode);
	}
	float d2 = b * b * (p.x + 0.5) * (p.x + 0.5) + a * a * (p.y - 1) * (p.y - 1) - a * a * b * b;
	while (p.y > 0)
	{
		if (d2 <= 0)
		{
			d2 += b * b * (2 * p.x + 2) + a * a * (-2 * p.y + 3);
			p.x++;
			p.y--;
		}
		else
		{
			d2 += a * a * (-2 * p.y + 3);
			p.y--;
		}
		OLED_Draw_4_Pixels_Rotate_Lines(x0, y0, x0+p.x, y0+p.y, angle, mode);
	}
	/* --------------- Bresenham --------------- */
#endif
}

/**
 * @brief OLED画两个相互垂直的非标准椭圆
 * @param x0 两个椭圆中心的横坐标
 * @param y0 两个椭圆中心的纵坐标
 * @param a 两个椭圆的长半轴长
 * @param b 两个椭圆的短半轴长
 * @param angle 相对于x轴的旋转角度（逆时针）（角度制）
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
// x' = xcosb - ysinb
// y' = xsinb + ycosb
void OLED_Draw_Two_Vertical_Ellipses_Rotate(u8 x0, u8 y0, u8 a, u8 b, u16 angle, u8 mode)
{
#if OLED_DRAW_ELLIPSE_BRESENHAM
	/* --------------- Bresenham --------------- */
	Point_Signed p = { .x = 0, .y = b };
	float d1 = b * b + a * a * (-b + 0.25);
	
	OLED_Draw_8_Pixels_Rotate(x0, y0, x0+p.x, y0+p.y, angle, mode);
	while (b * b * (p.x + 1) < a * a * (p.y - 0.5))
	{
		if (d1 <= 0)
		{
			d1 += b * b * (2 * p.x + 3);
			p.x++;
		}
		else
		{
			d1 += (b * b * (2 * p.x + 3) + a * a * (-2 * p.y + 2));
			p.x++;
			p.y--;
		}
		OLED_Draw_8_Pixels_Rotate(x0, y0, x0+p.x, y0+p.y, angle, mode);
	}
	float d2 = b * b * (p.x + 0.5) * (p.x + 0.5) + a * a * (p.y - 1) * (p.y - 1) - a * a * b * b;
	while (p.y > 0)
	{
		if (d2 <= 0)
		{
			d2 += b * b * (2 * p.x + 2) + a * a * (-2 * p.y + 3);
			p.x++;
			p.y--;
		}
		else
		{
			d2 += a * a * (-2 * p.y + 3);
			p.y--;
		}
		OLED_Draw_8_Pixels_Rotate(x0, y0, x0+p.x, y0+p.y, angle, mode);
	}
	/* --------------- Bresenham --------------- */
#endif
}

/**
 * @brief OLED画一个努诺三角形
 * @param x0 几何中心的横坐标
 * @param y0 几何中心的纵坐标
 * @param r 努诺三角形边长对应弧段的半径
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_Nuno_Triangle(u8 x0, u8 y0, u8 r, u8 mode)
{}

/* ---------------- base graphics ---------------- */

/* ---------------- derived graphics ---------------- */

/* ---------------- derived graphics ---------------- */

/* ---------------- animals ---------------- */

/**
 * @brief OLED画鱼
 * @method 鱼尾用三角形表示，鱼身用部分椭圆表示，鱼头用努诺三角形表示
 * @param x0 鱼所在矩形的几何中心的横坐标
 * @param y0 鱼所在矩形的几何中心的纵坐标
 * @param size 鱼的大小，指鱼的长度
 * @param dir 鱼头的方向，false 表示向左，true 表示向右
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Draw_Fish(u8 x0, u8 y0, u8 size, bool dir, u8 mode)
{}

/* ---------------- animals ---------------- */

/* ---------------- message ---------------- */

/**
 * @brief OLED画BMP
 * @param x BMP的左上角的顶点的横坐标
 * @param y BMP的左上角的顶点的纵坐标
 * @param p BMP数组的首地址
 * @param width BMP的宽度
 * @param height BMP的高度
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_ShowBMP(u8 x,u8 y,const u8 *p,u8 width,u8 height,u8 mode)
{
	u8 u_x, u_y;
	u8 temp;
	u16 csize;
	u16 i, j;
	
	if(x>OLED_COL_MAX-1 || y>OLED_ROW_MAX-1) return;
	u_x = x; u_y = y;
	csize = (height/8 + ((height%8)?1:0))*width;
	for(i=0; i<csize; i++)
	{
		temp = *p;
		for(j=0; j<8; j++)
		{
			temp&0X80 ? OLED_DrawPoint(u_x,u_y,mode) : OLED_DrawPoint(u_x,u_y,!mode);
			temp <<= 1;
			u_y++;
			if((u_y-y)==height)
			{
				u_y = y;
				u_x++;
				break;
			}
		}
		p++;
	}
}

/**
 * @brief OLED填充区块
 * @details 执行完此函数后直接更新OLED
 * @param x1 区块的一个对角线的端点一的横坐标
 * @param y1 区块的一个对角线的端点一的纵坐标
 * @param x2 区块的一个对角线的端点二的横坐标
 * @param y2 区块的一个对角线的端点二的纵坐标
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,mode);
	}
	OLED_Refresh_Gram();
}

/**
 * @brief OLED显示字符
 * @param x 字符的左上角的顶点的横坐标
 * @param y 字符的左上角的顶点的纵坐标
 * @param chr 需要显示的字符
 * @param size 字符的大小，有12、15、24可选
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);
	chr=chr-' ';
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t];
		else if(size==16)temp=asc2_1608[chr][t];
		else if(size==24)temp=asc2_2412[chr][t];
		else return;
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

/**
 * @brief OLED显示数字
 * @param x 数字的左上角的顶点的横坐标
 * @param y 数字的左上角的顶点的纵坐标
 * @param num 需要显示的数字
 * @param len 数字的位数
 * @param size 数字的大小，有12、15、24可选
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 mode)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,mode);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
}

/**
 * @brief OLED显示字符串
 * @param x 字符串的左上角的顶点的横坐标
 * @param y 字符串的左上角的顶点的纵坐标
 * @param p 需要显示的字符串的首地址
 * @param size 字符串的大小，有12、15、24可选
 * @param mode 绘制模式，FILL，填充1；CLEAR，填充0
 * @return None
 */
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode)
{	
    while((*p<='~')&&(*p>=' '))
    {       
        if(x>(OLED_COL_MAX-(size/2))){x=0;y+=size;}
        if(y>(OLED_ROW_MAX-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,mode);	 
        x+=size/2;
        p++;
    }
}

/* ---------------- message ---------------- */

/* ---------------- inition ---------------- */

/**
 * @brief 初始化OLED相关IO
 * @return None
 */
static void OLED_GPIO_Init(void)
{
	SPI_GPIO_Init();
}

/**
 * @brief 初始化OLED
 * @return None
 */
void OLED_Init(void)
{
	OLED_GPIO_Init();
#if USE_DMA && REFRESH_MODE == 2
	MyDMA_Init();
#endif
					  
	OLED_WR_Byte(0xAE,OLED_CMD);  // 关闭OLED显示
	OLED_WR_Byte(0xD5,OLED_CMD);  // 设置时钟分频因子、震荡频率
	OLED_WR_Byte(80,OLED_CMD);    // [3,0]分频因子，[7,4]震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD);  // 设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD);  // 默认0X3F(1/64)
	OLED_WR_Byte(0xD3,OLED_CMD);  // 设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD);  // 默认为0

	OLED_WR_Byte(0x40,OLED_CMD);  // 设置显示开始行，[5,0]行数
								
	OLED_WR_Byte(0x8D,OLED_CMD);  // 电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD);  // bit2，开始/关闭
	OLED_WR_Byte(0x20,OLED_CMD);  // 设置内存地址模式
	OLED_WR_Byte(0x01,OLED_CMD);  // [1,0]，01->列地址模式；10->行（页）地址模式；默认10
	OLED_WR_Byte(0xA1,OLED_CMD);  // 段重定义设置，bit0：0->0；1->127
	OLED_WR_Byte(0xC0,OLED_CMD);  // 设置COM扫描方向；[3,0]，这里选用普通模式
	OLED_WR_Byte(0xDA,OLED_CMD);  // 设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD);  // [5,4]配置
		 
	OLED_WR_Byte(0x81,OLED_CMD);  // 对比度设置
	OLED_WR_Byte(0x7F,OLED_CMD);  // 亮度设置（越大越亮）[7,0]，默认0X7F
	OLED_WR_Byte(0xD9,OLED_CMD);  // 设置预充电周期
	OLED_WR_Byte(0xF1,OLED_CMD);  // [3,0]PHASE 1；[7,4]PHASE 2
	OLED_WR_Byte(0xDB,OLED_CMD);  // 设置VCOMH电压倍率
	OLED_WR_Byte(0x30,OLED_CMD);  // [6,4]：000->0.65*VCC；001->0.77*VCC；011->0.83*VCC

	OLED_WR_Byte(0xA4,OLED_CMD);  // 全局显示开启，bit0：1->开启；0->关闭（白屏/黑屏）
	OLED_WR_Byte(0xA6,OLED_CMD);  // 设置显示方式：bit0：1->反向显示；0->正常显示
	OLED_WR_Byte(0xAF,OLED_CMD);  // 开启显示
	
	OLED_Clear();
}

/* ---------------- inition ---------------- */
