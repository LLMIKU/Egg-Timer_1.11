#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <direct.h>
#include "TBTlog.h"
/*
支持中途更改配置
定义了结构体数组
每日一言功能完善
加入计时日志 2017.8.21_13:34
每日一言按序读取实现 2017.8.26_20:19
如果需要启用每日一言，需要自己添加一个 文件加密3 加密后的TBTwords.txt
更改了默认配置文件路径为E:\Program Files (x86)
加入了卸载配置项 2017.8.26_22:54
更改了显示效果和卸载项提示
添加了路径控制器和版本控制器（在TBTlog.h里）
添加了对以往版本配置的支持 2018.1.25_17:26
*/

typedef struct _data
{
	int minutes;
	int lock;
	int sound;
	int music;
	int day_words;
	int test;
	int information;
	int time_reset;
	int all_reset;
	int uninstall;
}data;

void TBTdataInitial(data *TBT)
{
	TBT->minutes = 0;
	TBT->lock = 0;
	TBT->sound = 0;
	TBT->music = 0;
	TBT->day_words = 0;
	TBT->test = 0;
	TBT->information = 0;
	TBT->time_reset = 0;
	TBT->all_reset = 0;
	TBT->uninstall = 0;
}

static int GetCurTime(char* strTime)
{
	struct tm*		tmTime = NULL;
	size_t			timeLen = 0;
	time_t			tTime = 0;

	tTime = time(NULL);
	tmTime = localtime(&tTime);
	//timeLen = strftime(strTime, 33, "%Y(Y)%m(M)%d(D)%H(H)%M(M)%S(S)", tmTime);
	timeLen = strftime(strTime, 33, "%Y.%m.%d %H:%M", tmTime);

	return timeLen;
}

int GetYearDay(int *day)
{
	struct tm*		tmTime = NULL;
	size_t			timeLen = 0;
	time_t			tTime = 0;

	tTime = time(NULL);
	tmTime = localtime(&tTime);
	//timeLen = strftime(strTime, 33, "%Y(Y)%m(M)%d(D)%H(H)%M(M)%S(S)", tmTime);
	*day = tmTime->tm_yday + 1;//获取系统当前天是一年中的第几天
	return 0;
}

void CreateVbs(char *tmpStr, int minutes, const char *model)
{
	char filePathStr[128] = filePath;
	char fileNameStr[128] = "voice.VBS";
	strcat(filePathStr, fileNameStr);
	char str3[1024];
	int m = 0;
	FILE *fp = fopen(filePathStr, "w");
	if (fp == NULL)
	{
		printf("can not find or create a .VBS file\n");
		return;
	}
	else
	{
		printf("open the VBS file succeed!\n");
		printf("\n计时停止时刻：%s\n\n", tmpStr);
	}
	if (*model == 'w')
	{
		sprintf(str3,
			"CreateObject(\"SAPI.SpVoice\").Speak\"现在是北京时间%s，\
			您已经工作了%d分钟，请注意休息，\
			you have already worked for %d minutes, please pay attention to your health,\
			\"",tmpStr, minutes, minutes);
	}
	if (*model == 'r')
	{
		sprintf(str3,
			"CreateObject(\"SAPI.SpVoice\").Speak\"现在是北京时间%s，\
			您已经休息了%d分钟，请注意工作状态，\
			you have already rested for %d minutes, please pay attention to your work\"",
			tmpStr, minutes, minutes);
	}
	m = fputs(str3, fp);
	if (m == 0)
	{
		printf("现在开始语音播报！\n");
	}
	else printf("噢噢，发生了某种错误，我们无法启动语音播报！\n");
	fclose(fp);
	return;
}

int TransNumbers(char *StrTmp, int *Number)
{
	int ret = 0;
	int count = 0;
	int i = 0, j = 0;
	int num = 0;
	char *tmp = StrTmp;
	while (*tmp)
	{
		count++;
		tmp++;
	}
	tmp = StrTmp;
	for (i = 0; i < count; i++, tmp++)
	{
		switch (*tmp)
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
		default:num = 0; break;/* printf("fun TransNumbers() err: 读取数字错误\n"); ret = -1; return ret;*/
		}
		for (j = i; j < count - 1; j++)
		{
			num = num * 10;
		}
		(*Number) += num;
	}
	return ret;
}

int Initial_TBT_Set(char *filename)
{
	FILE *fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("fun Initial_TBT_Set() err:can not create a .ini file\n");
		system("pause");
		return -1;
	}
	fputs("count minutes(not beyond 25*60)=50;\n", fp);
	fputs("model(work or rest)=work;\n\n", fp);
	fputs("screen lock(0 or 1)=0;\n", fp);
	fputs("sound(0 or 1)=1;\n", fp);
	fputs("music(1 or 2)=1;\n\n", fp);
	fputs("day_words(0 or 1)=1;\n\n", fp);
	fputs("test(0 or 1)=0;\n", fp);
	fputs("screen information(0 or 1)=1;\n", fp);
	fputs("time_reset(0 or 1)=0;\n", fp);
	fputs("all_reset(0 or 1)=0;\n", fp);
	fputs("\nuninstall(0 or 1)=0;\n", fp);
	fclose(fp);
	printf("Succeed: .ini file created\n");
	return 0;
}

int ReadIni(char *filename, char *part, int *partValue)
{
	int ret = 0;
	char filePathStr[128] = filePath;
	char fileNameStr[128] = { 0 };
	strcpy(fileNameStr, filename);
	strcat(filePathStr, fileNameStr);
	char str2[220] = { 0 };
	strcpy(str2, filePathStr);//这里为了方便起见，将filePathStr复制给str2
	
	FILE *fp = fopen(str2, "r+");
	if (fp == NULL)
	{
		printf("fun ReadIni() err:can not open the file\n");
		ret = -1;
		return ret;
	}
	char str3[1024] = { 0 };
	int flag = 0;
	char filestr[1024] = { 0 };
	char partValueTmp[128] = { 0 };
	while (!feof(fp))
	{
		char *p = NULL;
		char *ptmp = NULL, *pBegain = NULL, *pEnd = NULL;
		fgets(str3, 256, fp);//修改日期：2017.8.15_11:32
		strcat(filestr, str3);
		int n = strlen(str3);
		str3[n + 1] = ';';//手动在字符串'\0'后添加';'结束符，防止字符串'='后什么都没有而中断出错
		p = strstr(str3, part);
		if (p != NULL)
		{
			flag = 1;//找到了配置项
			ptmp = strchr(str3, '=');
			if (ptmp == NULL)
			{
				system("color D");
				printf("配置项%s没有'='！\n", part);
				printf("现在初始化所有配置好吗\n");
				system("pause");
				fclose(fp);
				Initial_TBT_Set(str2);
				fp = fopen(str2, "r+");
				//此处添加错误状态
				ret = -2;
				goto End;
			}
			ptmp = ptmp + 1;
			while (1)//读取配置项
			{
				if (*ptmp == ' ')
				{
					ptmp++;
				}
				else
				{
					if (*ptmp == '\n' || *ptmp == ';' || *ptmp == '\0')
					{
						system("color B");
						printf("配置项%s没有配置初始值！\n", part);
						printf("现在初始化所有配置好吗\n");
						system("pause");
						fclose(fp);
						Initial_TBT_Set(str2);
						fp = fopen(str2, "r+");
						ret = -3;
						goto End;
					}
					pBegain = ptmp;
					break;
				}
			}
			while (1)
			{
				if (*ptmp == ';' || *ptmp == '\n' || *ptmp == ' ' || *ptmp == '\0')
				{
					break;
				}
				else
					ptmp++;
			}
			pEnd = ptmp;
			memcpy(partValueTmp, pBegain, pEnd - pBegain);
			partValueTmp[pEnd - pBegain] = '\0';
			if (*partValue == 0)//如果传入的partValue地址的内容是0，说明传入的是数据
			{
				ret = TransNumbers(partValueTmp, partValue);
				if (ret != 0)
				{
					printf("fun ReadIni() err: TransNumbers()\n");
				}
				break;
			}
			else//否则传入的是字符串的地址，进行强制类型转换再拷贝
			{
				strcpy((char *)partValue, partValueTmp);
				if (*partValueTmp != 'r' && *partValueTmp != 'w')
				{
					system("color D");
					printf("配置项%s配置错误！\n", part);
					printf("现在初始化所有配置好吗\n");
					system("pause");
					fclose(fp);
					Initial_TBT_Set(str2);
					fp = fopen(str2, "r+");
					ret = -2;
					goto End;
				}
				break;
			}
		}
	}
End:
	if (flag == 0)
	{
		system("color D");
		printf("没有找到%s配置项\n", part);
		printf("现在初始化所有配置好吗\n");
		system("pause");
		fclose(fp);
		Initial_TBT_Set(str2);
		fp = fopen(str2, "r+");
		ret = -4;
	}
	fclose(fp);
	return ret;
}

void CopySet(char *desFileName,FILE *src)
{
	char ch;
	char filePathStr[128] = filePath;
	strcat(filePathStr, desFileName);
	FILE *des = fopen(filePathStr, "w");
	if (des != NULL)
	{
		while ((ch = fgetc(src)) != EOF)
			fputc(ch, des);
		fclose(des);
	}
}

void CheckPreIni()
{
	char filePathStr[128] = "D:/Documents/TBTset.ini";
	int version = -1;
	FILE *fp10set = fopen("D:/Documents/TBTset.ini", "r+");
	FILE *fp10log = fopen("D:/Documents/TBTlog.txt", "r+");
	FILE *fp10words = fopen("D:/Documents/TBTwords.txt", "r+");
	FILE *fp11set = fopen("D:/Program Files (x86)/TBTset.ini", "r+");
	FILE *fp11log = fopen("D:/Program Files (x86)/TBTlog.txt", "r+");
	FILE *fp11words = fopen("D:/Program Files (x86)/TBTwords.txt", "r+");
	if ((fp11set != NULL || fp11log != NULL || fp11words != NULL) &&
		(fp10set != NULL || fp10log != NULL || fp10words != NULL))
	{
		system("color B");
		printf("\n  这真是太棒了！我们检测到您还保留着1.11和1.10版本的相关文件\n");
		printf("\n  输入数字'0'并按'Enter'键确认，我们将为您复制1.10版本的相关文件到1.12版本\n");
		printf("\n  输入数字'1'并按'Enter'键确认，我们将为您复制1.11版本的相关文件到1.12版本\n");
		while (version != 0 && version != 1)
		{
			scanf("%d", &version);
			if (version == 0)
				goto V0;
			if (version == 1)
				goto V1;
		}
	}
V1:	if (fp11set != NULL || fp11log != NULL || fp11words != NULL)
	{
		char ch = 0;
		system("color B");
		if (version == -1)
			printf("\n  这真是太棒了！我们检测到您还保留着1.11版本的相关文件\n");
		printf("\n  想要把1.11版本的文件配置全部应用到新的1.12版本吗？\n");
		printf("\n  当然，我们不会对您保留着的相关文件做任何删除或修改\n");
		printf("\n  因为看起来它们是您想要保留的重要数据\n");
		printf("\n  输入字母'y'并按'Enter'键确认，我们将为您复制相关文件到1.12版本\n");
		printf("\n  输入字母'n'并按'Enter'键确认，我们将为1.12版本生成全新的配置\n");
		while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N')
		{
			scanf("%c", &ch);
			if (ch == 'y' || ch == 'Y')
			{
				if (fp11set != NULL)
				{
					printf("\n  复制TBTset.ini...\n");
					CopySet("TBTset.ini", fp11set);
				}
				if (fp11log != NULL)
				{
					printf("\n  复制TBTlog.txt...\n");
					CopySet("TBTlog.txt", fp11log);
				}
				if (fp11words != NULL)
				{
					printf("\n  复制TBTwords.txt...\n");
					CopySet("TBTwords.txt", fp11words);
				}
				printf("\n  复制完成!\n");
				system("pause");
			}
			if (ch == 'n' || ch == 'N')
			{
				printf("\n  现在我们将为您生成全新的配置\n");
				break;
			}
		}
		fclose(fp11log);
		fclose(fp11set);
		fclose(fp11words);
		return;
	}
V0:	if (fp10set != NULL || fp10log != NULL || fp10words != NULL)
	{
		char ch = 0;
		system("color B");
		if (version == -1)
			printf("\n  这真是太棒了！我们检测到您还保留着1.10版本的相关文件\n");
		printf("\n  想要把1.10版本的文件配置全部应用到新的1.12版本吗？\n");
		printf("\n  当然，我们不会对您保留着的相关文件做任何删除或修改\n");
		printf("\n  因为看起来它们是您想要保留的重要数据\n");
		printf("\n  输入字母'y'并按'Enter'键确认，我们将为您复制相关文件到1.12版本\n");
		printf("\n  输入字母'n'并按'Enter'键确认，我们将为1.12版本生成全新的配置\n");
		while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N')
		{
			scanf("%c", &ch);
			if (ch == 'y' || ch == 'Y')
			{
				if (fp10set != NULL)
				{
					printf("\n  复制TBTset.ini...\n");
					CopySet("TBTset.ini", fp10set);
				}
				if (fp10log != NULL)
				{
					printf("\n  复制TBTlog.txt...\n");
					CopySet("TBTlog.txt", fp10log);
				}
				if (fp10words != NULL)
				{
					printf("\n  复制TBTwords.txt...\n");
					CopySet("TBTwords.txt", fp10words);
				}
				printf("\n  复制完成!\n");
				system("pause");
			}
			if (ch == 'n' || ch == 'N')
			{
				printf("\n  现在我们将为您生成全新的配置\n");
				break;
			}
				
		}
		fclose(fp10log);
		fclose(fp10set);
		fclose(fp10words);
		return;
	}
}

int CreateIni(char *filename)
{
	int ret = 0;
	if (filename == NULL)
	{
		printf("fun CreateIni() 传入地址不能为NULL\n");
		ret = -1;
		return ret;
	}
	if (*filename == (char)NULL)
	{
		printf("fun CreateIni() 传入的文件名不能为空\n");
		ret = -2;
		return ret;
	}
	
	char filePathStr[128] = filePath;
	char fileNameStr[128] = { 0 };
	strcpy(fileNameStr, filename);
	strcat(filePathStr, fileNameStr);
	FILE *fp = fopen(filePathStr, "r+");
	if (fp == NULL)
	{
		int status = _mkdir(filePath); /*创建路径*/
		(!status) ? (printf("New directory created (%s): %s\n",TBTversion,filePath)) :
			(printf("Unable to create new directory (%s): %s\n",TBTversion,filePath));
		CheckPreIni();//检查以往版本的配置
		fp = fopen(filePathStr, "r+");//尝试重新打开配置文件
		if (fp == NULL)
		{
			printf("We can't find the necessary .ini file\n");
			system("color B");
			printf("I will create a TBTset.ini file for you. Okay?\n");
			system("pause");
			ret = Initial_TBT_Set(filePathStr);
			return ret;
		}
	}
	fclose(fp);
	return ret;
}

void TimeBroadcast(int i, const char *model)//传入参数为秒数 
{
	int minutes = 0;
	minutes = i / 60;
	char tmpStr[64];
	char str[128];
	GetCurTime(tmpStr);//获取当前时间 
	sprintf(str, "%s", tmpStr);

	char filePathStr[128] = filePath;
	char fileNameStr[128] = "voice.VBS";
	strcat(filePathStr, fileNameStr);
	CreateVbs(str, minutes, model);//写入临时语音文件
	ShellExecuteA(0, "open", filePathStr, 0, 0, 1);//执行语音播报 
	Sleep(20000);//语音播报预留时间20s 
	//Sleep(1000);
	remove(filePathStr);//删除临时语音文件

	return;
}

void Music(int music)
{
	int j, k;
	if (music == 1)
	{
		for (j = 0; j < 4; j++)
		{
			for (k = 0; k < 3; k++)
			{
				Beep(1480, 150);
				Sleep(30);
			}
			Beep(1480, 150);
			if (j == 3) break;
			Sleep(1500);
		}
		return;
	}
	if (music == 2)
	{
		Beep(494, 200);
		Beep(523, 300);
		Beep(578, 400);
		Beep(659, 500);
		Beep(698, 600);
		Beep(784, 700);
		Beep(880, 800);
		Beep(932, 900);
		Beep(1046, 1000);
		Beep(1175, 1100);
		Beep(1381, 1200);
		Beep(1480, 1300);
		Beep(1661, 1400);
		return;
	}
}

int RecheckIni(data *TBT, char *model)
{
	int ret = 0;
	TBTdataInitial(TBT);
	strcpy(model, "work");
	ret = ReadIni("TBTset.ini", "count minutes", &TBT->minutes);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(count minutes)\n");
		system("pause");
		return ret;
	}
	ret = ReadIni("TBTset.ini", "model", model);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(model)\n");
		system("pause");
		return ret;
	}
	ret = ReadIni("TBTset.ini", "lock", &TBT->lock);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(lock)\n");
		system("pause");
		return ret;
	}
	ret = ReadIni("TBTset.ini", "sound", &TBT->sound);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(sound)\n");
		system("pause");
		return ret;
	}
	ret = ReadIni("TBTset.ini", "music", &TBT->music);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(music)\n");
		system("pause");
		return ret;
	}
	ret = ReadIni("TBTset.ini", "words", &TBT->day_words);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(day_words)\n");
		system("pause");
		return ret;
	}
	ret = ReadIni("TBTset.ini", "test", &TBT->test);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(test)\n");
		system("pause");
		return ret;
	}
	ret = ReadIni("TBTset.ini", "information", &TBT->information);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(information)\n");
		system("pause");
		return ret;
	}
	ret = ReadIni("TBTset.ini", "time_reset", &TBT->time_reset);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(time_reset)\n");
		system("pause");
		return ret;
	}
	ret = ReadIni("TBTset.ini", "all_reset", &TBT->all_reset);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(all_reset)\n");
		system("pause");
		return ret;
	}
	ret = ReadIni("TBTset.ini", "uninstall", &TBT->uninstall);
	if (ret != 0)
	{
		printf("fun RecheckIni() err: ReadIni(uninstall)\n");
		system("pause");
		return ret;
	}
	return ret;
}

void InfoPrint(int i, data *TBT, char *model)
{
	char lockstatu[5] = "关";
	char soundstatu[5] = "关";
	char testStatu[5] = "关";
	char time_resetStatu[5] = "关";
	char all_resetStatu[5] = "关";
	char informationStatu[7] = "不显示";
	/*if (TBT->test != 1)
		system("cls");*/
	system("cls");
	if (TBT->lock == 1)
		strcpy(lockstatu, "开");
	if (TBT->sound == 1)
		strcpy(soundstatu, "开");
	if (TBT->test == 1)
		strcpy(testStatu, "开");
	if (TBT->time_reset == 1)
		strcpy(time_resetStatu, "开");
	if (TBT->all_reset == 1)
		strcpy(all_resetStatu, "开");
	if (TBT->information == 1)
		strcpy(informationStatu, "显示");
	if (i <= 59)
		printf("\n\n  现在是  第%d秒\n", i);
	else if (i % 60 == 0)
	{
		printf("\n\n  现在是  第%d分钟\n", i / 60);
	}
	else if (i % 60 != 0)
		printf("\n\n  现在是  第%d分钟  第%d秒\n", i / 60, i % 60);

	if (TBT->information == 0)
		return;
	printf("\n\n配置状态(%s):\n",TBTversion);
	printf("定时：%d分钟  模式：%s\n\n", TBT->minutes, model);
	printf("屏幕锁：%s\n\n", lockstatu);
	printf("声音：%s  音乐类型：%d\n\n", soundstatu, TBT->music);
	printf("快速测试：%s\n\n", testStatu);
	printf("计时状态初始化：%s  ", time_resetStatu);
	printf("所有配置初始化：%s\n", all_resetStatu);
	//printf("配置项状态：%s\n", informationStatu);
	return;
}

int myStrChange(const char *in, char *out)
{
	int ret = 0;
	int i = 0, j = 0;
	int len = strlen(in);
	j = len - 1;
	if (in == NULL || out == NULL)
	{
		ret = -1;
		return ret;
	}
	while (j >= i)
	{
		if (in[i] != '\n'&&in[j] != '\n')
		{
			out[i] = in[j];
			out[j] = in[i];
		}
		else
		{
			out[i] = in[i];
			out[j] = in[j];
		}
		i++;
		j--;
	}
	out[len] = '\0';
	return ret;
}

void GetDayWords(int day)
{
	char str[100];
	char out[100];
	int d = day;
	char scan[16];
	char *p = NULL;
	int first = 0;
	sprintf(scan, "%d、", d);
	char filePathStr[128] = filePath;
	char fileNameStr[128] = "TBTwords.txt";
	strcat(filePathStr, fileNameStr);
	FILE *fp = fopen(filePathStr, "r");
	if (fp == NULL)
	{
		printf("can not open the words file\n");
		return;
	}

	/*366天每日一言测试模块*/
	//int flag = 0;
	//for (d = 1; d <= 366; d++)
	//{
	//	sprintf(scan, "%d、", d);
	//	while (!feof(fp))
	//	{
	//		fgets(str, 100, fp);
	//		myStrChange(str, out);//解密
	//		p = strstr(out, scan);
	//		if (flag == 1)
	//		{
	//			if (p != NULL)
	//			{
	//				*p = '\0';
	//				printf("%s", out);
	//				break;
	//			}
	//			printf("%s", out);
	//		}
	//		if (p != NULL&&flag == 0)
	//		{
	//			printf("%s", p /*+ strlen(scan)*/);
	//			sprintf(scan, "%d、", d + 1);
	//			flag = 1;
	//		}
	//		memset(str, 0, 100);
	//	}
	//	rewind(fp);
	//	flag = 0;
	//	Sleep(1000);
	//}

	/*每日一言单个序号打印模块*/
	int flag = 0;
	while (!feof(fp))
	{
		fgets(str, 100, fp);
		myStrChange(str, out);//解密
		p = strstr(out, scan);//搜索字符串中是否包含序号
		if (flag == 1)//如果已经成功搜索到第一次的序号
		{
			if (p != NULL)//如果搜索到第二次的序号
			{
				*p = '\0';//字符串中当前位置改为字符串结束符'\0'
				printf("%s", out);
				break;//序号后内容已打印完毕，故跳出循环
			}
			printf("%s", out);
		}
		if (p != NULL&&flag == 0)//如果搜索到序号且是第一次搜索到想要的序号
		{
			printf("\n每日一言：%s", p + strlen(scan));//打印出序号后的内容
			sprintf(scan, "%d、", d + 1);//搜索的序号加一
			flag = 1;//已经搜索到序号，故flag置一
		}
		memset(str, 0, 100);
	}

	fclose(fp);
}

void TBTuninstall()
{
	FILE *fpuninstall = NULL;
	system("cls");
	system("color B");
	printf("\n  您想要卸载掉计时器(%s)的所有相关文件吗？\n",TBTversion);
	printf("  这将导致您的计时日志(%s)永远丢失，确定要继续卸载吗？\n",TBTversion);
	system("pause");
	printf("\n  这是您最后一次保留计时日志的机会了，仍然确定要继续卸载吗？\n");
	system("pause");
	printf("\n  确保您已经关闭掉TBTset.ini的页面，否则会留下一个空文件夹\n");
	system("pause");
	
	char filePathStr[128] = filePath;
	int status = _rmdir(filePathStr); /*删除创建的文件夹*/
	int delFlag = 0;
	(!status) ? (delFlag=1) :
		(delFlag=0);
	char delCommand[256] = "rd /s /q \"";
	strcat(delCommand, filePathStr);
	strcat(delCommand, "\"");
	system(delCommand);
	system("pause");
	system("cls");
	printf("\n  已经删除掉所有相关文件，现在请退出本程序，之后删除本程序即可\n");
	printf("  很高兴遇见你 D\n");
}

void CountDay(int YearDay)
{
	//char weekday[7][10] = { "日", "一", "二", "三", "四", "五", "六" };
	int w = (YearDay) % 7;
	int weekAddition = (w == 0) ? (0) : (1);
	printf("\n今年第%d天\n",
		YearDay, (YearDay) / 7 + weekAddition);
}

int checkAllSet(data *TBT, char *model,int *seconds)
{
	int ret = 0;
	ret = RecheckIni(TBT, model);//读取配置文件
	if (ret != 0)
	{
		printf("fun main() err: RecheckIni()\n");
		return ret;
	}
	if (TBT->test != 2)
		InfoPrint(*seconds, TBT, model);//打印要显示的信息
	if (TBT->day_words == 1){
		int YearDay = 0;
		GetYearDay(&YearDay);//获取系统时间：现在是一年中的第几天
		GetDayWords(YearDay);//每日一言功能
		CountDay(YearDay);//今年第几天，第几周的计算功能
	}
	if (TBT->uninstall == 1){
		TBTuninstall();//卸载功能
		return -2;
	}
	if (TBT->all_reset == 1){
		system("color B");
		printf("您想要重新初始化所有配置项吗？\n");
		system("pause");
		char filePathStr[128] = filePath;
		char fileNameStr[128] = "TBTset.ini";
		strcat(filePathStr, fileNameStr);
		Initial_TBT_Set(filePathStr);
		return 0;
	}
	if (TBT->time_reset == 1){
		*seconds = 0;
		system("color E");
		printf("现在处于计时初始状态，重新开始计时请将time_reset配置项设为0\n");
		Sleep(500);
		system("color");
	}
	if (TBT->sound == 0 && TBT->lock == 0){
		system("color A");
		printf("\n不能将 屏幕锁提醒配置项screen lock 和 语音提醒配置项sound 同时关闭，否则计时器将无法提醒\n");
		if (TBT->test != 1)
			Sleep(500);
		system("color");
	}
	if (TBT->test == 1 || TBT->test == 2)
	{
		*seconds = TBT->minutes * 60;
		InfoPrint(*seconds, TBT, model);
		printf("\n到达计时终点的计时器表现测试已经启动\n");
		return 0;
	}
	return 0;
}

int checkTime(int *seconds,const data *TBT,const char *model)
{
	int sec = *seconds;
	if (sec == TBT->minutes * 60){
		ITCAST_LOG(__FILE__, __LINE__, LogLevel[2], TBT->minutes, model);//先写工作日志
		if (TBT->sound == 1){
			Music(TBT->music);//播放音乐
			TimeBroadcast(sec, model);//时间播报提醒
		}
		if (TBT->lock == 1){
			system("rundll32.exe user32.dll,LockWorkStation");
		}
		printf("\n想要重新开始计时吗？\n");
		system("pause");
		*seconds = 0;
		return 0;
	}
	if (sec > TBT->minutes * 60){
		system("color E");
		printf("\n计时已经超过您所设置的时间\n");
		Sleep(500);
	}
	return 0;
}

void Timer(char *title, clock_t *start, clock_t *middle, clock_t *finish, double *totaltime)
{
	printf("\n%s时间为:", title);
	*finish = clock();//获取当前时间
	*totaltime = (double)(*finish - *middle) / CLOCKS_PER_SEC;//计算中间时间
	printf("%f秒 ", *totaltime);
	*totaltime = (double)(*finish - *start) / CLOCKS_PER_SEC;//计算整体时间
	printf("totaltime:%f\n", totaltime);
	*middle = *finish;
}

void main()
{
	int ret = 0;
	int i = 0, j = 0, k = 0;
	int YearDay = 0;
	data TBTdata;
	data *TBT = &TBTdata;
	TBTdataInitial(TBT);
	char model[128] = { "work" };//由于ReadIni函数限制，字符串请注意不要初始化为0
	ret = CreateIni("TBTset.ini");//没有配置文件则创建一个配置文件
	if (ret != 0){
		printf("fun main() err: CreateIni()\n");
		system("pause");
		return;
	}

	//clock_t start, middle, finish;//测时
	//double totaltime;
	//start = clock();
	//middle = start;
	
	int seconds = 0;
	ret = checkAllSet(TBT, model, &seconds);
	if (ret != 0)
		goto End;
	for (seconds = 0; seconds < (TBT->minutes) * 3600; seconds++)
	{
		ret = checkAllSet(TBT, model, &seconds);
		if (ret != 0)
			break;

		system("color 07");
		Sleep(900);//延时

		ret = checkTime(&seconds, TBT, model);
		if (ret != 0)
			break;
		//Timer("计时器", &start, &middle, &finish, &totaltime);//测试一秒实际时长
	}
End:	system("pause");
}
