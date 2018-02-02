// 编译该程序，请先安装 EasyX v20100825或者后续版本
// http://www.easyx.cn
//整理版：整体下降式累加填充
//修复超过60分钟时长不会计入的bug
//修改0点前后的归入日期 2018/02/02_20:30
#define _CRT_SECURE_NO_WARNINGS
#include <graphics.h>
#include <time.h>
#include<stdlib.h>
#include<stdio.h>
#include <conio.h>
#include<math.h>
#include<string.h>
#define filePath "D:/Program Files (x86)/TimeBroadcastTimer/"

void charToNum(char *inCh, int *outNum)//字符转数字
{
	int num = 0;
	switch (*inCh)
	{
	case '0':num = 0; break;
	case '1':num = 1; break;
	case '2':num = 2; break;
	case '3':num = 3; break;
	case '4':num = 4; break;
	case '5':num = 5; break;
	case '6':num = 6; break;
	case '7':num = 7; break;
	case '8':num = 8; break;
	case '9':num = 9; break;
	default:num = 0; break;
	}
	*outNum = num;
}

void strToNum(char *inStr, int strLength, int intCount, int *outNum)//字符串转数字
{
	int i = 0, j = 0;
	int k = 0;
	int num = 0;
	char *tmp = inStr;
	for (i = 0; i < strLength; i++, tmp++)
	{
		charToNum(tmp, &num);
		for (j = k; j < intCount - 1; j++)//j=k
		{
			if (*tmp >= '0'&&*tmp <= '9')//避免符号单位等影响
				num = num * 10;
		}
		if (*tmp >= '0'&&*tmp <= '9')//避免符号单位等影响
			k++;
		(*outNum) += num;
	}
}

int TransNumbers(char *StrTmp, int *Number)//把传入字符串转换为数字
{
	int ret = 0;
	int intCount = 0;//整数位数
	int intCount2 = 0;//小数点后0之后的整数位数
	int length = 0;//字符长度
	int i = 0, j = 0;
	int deciFlag = 0;//小数点标志
	int deciCount = 0;//小数点后0的个数
	int deciNum = 0;//小数部分
	int num = 0;
	char *tmp = StrTmp;
	//读取整数部分
	while (*tmp)
	{
		if (*tmp >= '0'&&*tmp <= '9')//避免符号单位等影响数字位数计数
			intCount++;
		if (*tmp == '.')
			break;
		length++;
		tmp++;
	}
	tmp = StrTmp;
	strToNum(tmp, length, intCount, Number);
	//读取小数部分
	tmp = strstr(StrTmp, ".");
	if (tmp == NULL)
		return ret;
	deciCount = 1;//小数点计入
	while (*++tmp)
	{
		if (*tmp != '0')
			break;
		deciCount++;//计算小数点及之后连续0的位数
	}
	tmp = strstr(StrTmp, ".");
	tmp += deciCount;//跳到小数点后连续0后的整数部分
	length = 0;
	while (*tmp)
	{
		if (*tmp >= '0'&&*tmp <= '9')//避免符号单位等影响数字位数计数
			intCount2++;
		length++;
		tmp++;
	}
	tmp = strstr(StrTmp, ".");
	tmp += deciCount;
	strToNum(tmp, length, intCount2, &deciNum);
	for (i = 0; i < deciCount + intCount2 - 1; i++)
	{
		deciNum = deciNum / 10;
	}
	//整数部分与小数部分相加
	(*Number) += deciNum;
	return ret;
}

int ReadLog(char *filename, char *part, int dynamic)
{
	int ret = 0;
	int j = 0;
	char filePathStr[128] = filePath;
	char fileNameStr[128] = { 0 };
	strcpy(fileNameStr, filename);
	strcat(filePathStr, fileNameStr);
	//填充浅蓝色背景方块
	int x = 0, y = 0;
	for (x = 100; x <= 700; x += 20){
		for (y = 538; y >= 78; y -= 20){
			setcolor(RGB(0, 50, 0));//设置背景方块边界为浅绿
			int points[] = { x, y, x + 10, y, x + 10, y - 16, x, y - 16 };
			setfillstyle(RGB(0, 50, 50));//设置背景方块填充色为浅蓝
			fillpoly(4, points);//填充方块
		}
	}

	FILE *fp = fopen(filePathStr, "r+");
	if (fp == NULL)
	{
		printf("fun ReadLog() err:can not open the file\n");
		ret = -1;
		return ret;
	}
	char str3[1024] = { 0 };
	int dayMinute[33] = { 0 };
	int totalTime = 0;
	int time3hour = 0;//计数超过3小时的天数
	int time8hour = 0;//计数超过8小时的天数
	int dayHour[18] = { 0 };
	int x1Collection[33][256] = { 0 };
	int x2Collection[33][256] = { 0 };
	int y1Collection[33][256] = { 0 };
	int y2Collection[33][256] = { 0 };
	int collection = 255;
	int amMinute[33] = { 0 };
	int pmMinute[33] = { 0 };
	int evMinute[33] = { 0 };

	while (!feof(fp))
	{
		char *p = NULL;
		char *pBegain = NULL, *pEnd = NULL;
		memset(str3, 0, 1024 * sizeof(char));
		fgets(str3, 256, fp);
		char partValueTmp[128] = { 0 };
		int n = strlen(str3);
		p = strstr(str3, part);
		if (p != NULL)
		{
			int date = 0;
			int hour = 0;
			int minute = 0;
			int timeLen = 0;
			printf("%s", str3);
			pBegain = str3 + 9;
			pEnd = pBegain + 2;
			memcpy(partValueTmp, pBegain, pEnd - pBegain);
			partValueTmp[pEnd - pBegain] = '\0';
			TransNumbers(partValueTmp, &date);//获取日期

			pBegain = str3 + 12;
			pEnd = pBegain + 2;
			memcpy(partValueTmp, pBegain, pEnd - pBegain);
			partValueTmp[pEnd - pBegain] = '\0';
			TransNumbers(partValueTmp, &hour);//获取小时

			pBegain = str3 + 15;
			pEnd = pBegain + 2;
			memcpy(partValueTmp, pBegain, pEnd - pBegain);
			partValueTmp[pEnd - pBegain] = '\0';
			TransNumbers(partValueTmp, &minute);//获取分钟

			pBegain = strstr(str3, "minutes is ");
			pBegain = pBegain + strlen("minutes is ");
			pEnd = pBegain + 2;
			while (*pEnd != ']')
				pEnd++;
			memcpy(partValueTmp, pBegain, pEnd - pBegain);
			partValueTmp[pEnd - pBegain] = '\0';
			TransNumbers(partValueTmp, &timeLen);//获取计时时长
			dayMinute[date] += timeLen;//计算每天的工作时长

			printf("date:%d hour:%d minute:%d timeLen:%d\n", date, hour, minute, timeLen);
			int preMinute = minute - timeLen;
			int preHour = hour;
			int i = 1;
			while (preMinute < 0){
				preMinute = minute + i * 60 - timeLen;
				preHour = preHour - 1;
				i++;
			}
			
			printf("before Time: preHour:%d preMinute:%d\n", preHour, preMinute);
	
			//填充实际的工作时间
			setcolor(RGB(0, 50, 50));//设置浅蓝色绘图色
			int x1 = 100 + (date - 1) * 20;
			int x2 = 110 + (date - 1) * 20;
			int y1 = 60 + hour * 20 + minute / 3;
			int y2 = 60 + preHour * 20 + preMinute / 3;
			if (preHour < 0)//如果之前的小时小于0点，说明是前一天
			{
				int bx1 = 100 + (date - 2) * 20;
				int bx2 = 110 + (date - 2) * 20;
				int by1 = 539;
				int by2 = 539 - (timeLen / 3 - (hour * 20 + minute) / 3);
				x1Collection[date-1][collection] = bx1;
				x2Collection[date-1][collection] = bx2;
				y1Collection[date-1][collection] = by1;
				y2Collection[date-1][collection] = by2;
				dayMinute[date - 1] = dayMinute[date - 1] + (-preHour) * 60 - preMinute;
				dayMinute[date] = timeLen - ((-preHour) * 60 - preMinute);
				timeLen = dayMinute[date];
				int points[] = { bx1, by1, bx2, by1, bx2, by2, bx1, by2 };
				setfillstyle(RGB(0, 200, 200));//设置亮蓝色填充色
				fillpoly(4, points);
				y2 = 60;
			}
			if (dayMinute[date] == timeLen)
				collection = 0;
			x1Collection[date][collection] = x1;
			x2Collection[date][collection] = x2;
			y1Collection[date][collection] = y1;
			y2Collection[date][collection] = y2;
			collection++;
			int points[] = { x1, y1, x2, y1, x2, y2, x1, y2 };
			setfillstyle(RGB(0, 200, 200));//设置亮蓝色填充色
			fillpoly(4, points);
		}
	}
	
	int m = 0;
	//整体动态下落
	if (dynamic == 1)
	{
		system("pause");
		int dayLen[33][257] = { 0 };
		setfillstyle(RGB(0, 200, 200));//设置亮蓝色填充色
		BeginBatchDraw();
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		for (int pixel = 0; pixel < 480; pixel++)
		{
			int flag = 0;
			for (m = 1; m <= 31; m++)
			{
				for (collection = 250; collection >= 0; collection--)
				{
					if (x1Collection[m][collection] != 0)
					{
						x1 = x1Collection[m][collection];
						x2 = x2Collection[m][collection];
						y1 = y1Collection[m][collection];
						y2 = y2Collection[m][collection];

						if (pixel == 0)
							dayLen[m][collection] = dayLen[m][collection + 1] + (y1 - y2);

						if (y2 <= 539 - dayLen[m][collection])
						{
							flag = 1;
							int points[] = { x1, y1, x2, y1, x2, y2, x1, y2 };
							fillpoly(4, points);
							floodfill(x1, y1, RGB(0, 50, 50));
							//FlushBatchDraw();
							y1Collection[m][collection]++;
							y2Collection[m][collection]++;
						}
					}
				}
			}
			if (flag == 1)
			{
				Sleep(1);
				FlushBatchDraw();
			}
		}
		EndBatchDraw();
	}
	//累加段 整体变色
	if (dynamic == 1)
	{
		Sleep(50);
		setfillstyle(RGB(200, 0, 200));//设置亮红色填充色
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		for (m = 1; m <= 31; m++)
		{
			for (collection = 250; collection >= 0; collection--)
			{
				if (x1Collection[m][collection] != 0)
				{
					x1 = x1Collection[m][collection];
					x2 = x2Collection[m][collection];
					y1 = y1Collection[m][collection] - 1;
					y2 = y2Collection[m][collection] - 1;
					int points[] = { x1, y1, x2, y1, x2, y2, x1, y2 };
					fillpoly(4, points);
				}
			}
		}
	}
	//收缩
	if (dynamic == 1)
	{
		Sleep(20);
		int dayLen[33][257] = { 0 };
		BeginBatchDraw();
		setfillstyle(RGB(200, 0, 200));//设置亮红色填充色
		for (int pixel = 0; pixel < 2; pixel++)
		{
			int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
			for (collection = 250; collection >= 0; collection--)
			{
				for (m = 1; m <= 31; m++)
				{
					if (x1Collection[m][collection] != 0)
					{
						x1 = x1Collection[m][collection];
						x2 = x2Collection[m][collection];
						if (x1 != ((x1 + x2) / 2 - 3))
						{
							x1Collection[m][collection]++;
							x2Collection[m][collection]--;
						}
					}
				}
			}
			for (collection = 150; collection >= 0; collection--)
			{
				int flag = 0;
				for (m = 1; m <= 31; m++)
				{
					if (x1Collection[m][collection] != 0)
					{
						x1 = x1Collection[m][collection];
						x2 = x2Collection[m][collection];
						y1 = y1Collection[m][collection] - 1;
						y2 = y2Collection[m][collection] - 1;
						if ((x1 == ((x1 + x2) / 2 - 3)) || (x1 == ((x1 + x2) / 2 - 4)))
						{
							flag = 1;
							int points[] = { x1, y1, x2, y1, x2, y2, x1, y2 };
							fillpoly(4, points);
							floodfill(x1, y1, RGB(0, 50, 50));
							//Sleep(100);

						}
					}
				}
				if (flag == 1)
				{
					FlushBatchDraw();
					Sleep(60);
					flag = 0;
				}

			}
		}
		EndBatchDraw();
	}

	//填充一天的总工作时长（亮红色部分）
	int i = 0;
	for (i = 1; i <= 31; i++){
		int tx1 = 102 + (i - 1) * 20;
		int tx2 = 108 + (i - 1) * 20;
		int ty1 = 539;
		int ty2 = 539 - dayMinute[i] / 3;
		int points[] = { tx1, ty1, tx2, ty1, tx2, ty2, tx1, ty2 };
		setfillstyle(RGB(200, 0, 200));//设置亮红色为填充色
		if (dayMinute[i] > 0)
			fillpoly(4, points);//填充一天的总工作时长
		for (j = 3; j <= 8; j++){
			if (dayMinute[i] >= 60 * j)
				dayHour[j]++;
		}
		totalTime += dayMinute[i];
	}
	//填充标题和其他信息
	char s[32] = { "0" };
	setcolor(RGB(0, 200, 0));//设置浅绿色绘图色
	sprintf_s(s, "%s", part);
	outtextxy(372, 14, s);
	setcolor(RGB(0, 100, 0));//设置浅绿色绘图色
	for (j = 3; j <= 8; j++){
		sprintf_s(s, "%dHour-%dday", j, dayHour[j]);
		outtextxy(712, 533 - j * 20, s);
	}
	setcolor(RGB(0, 200, 200));//设置亮蓝色绘图色
	sprintf_s(s, "Total-Time");
	outtextxy(678, 14, s);
	sprintf_s(s, "%dHour %dMinute", totalTime / 60, totalTime % 60);
	outtextxy(678, 30, s);
	fclose(fp);//关闭文件
	return ret;
}
void printLog(int dynamic)
{
	int i = 0, j = 0;
	initgraph(800, 600);//初始化图形界面800*600
	line(90, 60, 90, 540);//画Y坐标轴
	line(90, 540, 710, 540);//画X坐标轴

	int x = 0, y = 0;//横轴表示X轴
	char s[16] = { "0" };
	i = 23;
	setfont(8, 0, "等线");
	setcolor(RGB(0, 60, 60));//设置浅蓝色绘图色
	for (x = 100; x <= 700; x += 20){
		for (y = 538; y >= 78; y -= 20){
			sprintf_s(s, "%d", i);//标示24小时
			outtextxy(x - 9, y - 2 - 20, s);
			i--;
			if (i < 0)
				i = 23;
		}
	}

	setcolor(RGB(0, 90, 0));//设置绿色绘图色
	line(90, 380, 710, 380);//8小时工作时长线
	line(90, 480, 710, 480);//3小时工作时长线
	setcolor(RGB(90, 90, 0));//设置红色绘图色
	line(30, 220, 710, 220);//8点
	line(30, 300, 710, 300);//12点
	line(30, 320, 710, 320);//13点
	line(30, 420, 710, 420);//18点
	setfont(14, 0, "宋体");
	setcolor(RGB(255, 255, 255));//设置白色绘图色
	i = 23;//重设初始时刻为23点
	j = 1;//重设初始日期为1号
	for (x = 100; x <= 700; x += 20){
		for (y = 538; y >= 78; y -= 20){
			if (x == 100){
				sprintf_s(s, "%d", i);//打印24小时
				outtextxy(x - 20, y - 6 - 20, s);
				i--;
			}
			if (y == 538){
				sprintf_s(s, "%d", j);//打印日期
				outtextxy(x - 1, y + 6, s);
				j++;
			}
		}
	}
	sprintf_s(s, "Date");
	outtextxy(698, 558, s);//标注X轴为Date
	sprintf_s(s, "Time");
	outtextxy(46, 52, s);//标注Y轴为Time
	ReadLog("TBTlog.txt", "2017.12.", dynamic);
}

void main()
{
	printLog(1);
	system("pause");
	printLog(0);
	system("pause");
}
