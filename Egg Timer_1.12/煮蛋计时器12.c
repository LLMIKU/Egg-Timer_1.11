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
֧����;��������
�����˽ṹ������
ÿ��һ�Թ�������
�����ʱ��־ 2017.8.21_13:34
ÿ��һ�԰����ȡʵ�� 2017.8.26_20:19
�����Ҫ����ÿ��һ�ԣ���Ҫ�Լ����һ�� �ļ�����3 ���ܺ��TBTwords.txt
������Ĭ�������ļ�·��ΪE:\Program Files (x86)
������ж�������� 2017.8.26_22:54
��������ʾЧ����ж������ʾ
�����·���������Ͱ汾����������TBTlog.h�
����˶������汾���õ�֧�� 2018.1.25_17:26
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
	*day = tmTime->tm_yday + 1;//��ȡϵͳ��ǰ����һ���еĵڼ���
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
		printf("\n��ʱֹͣʱ�̣�%s\n\n", tmpStr);
	}
	if (*model == 'w')
	{
		sprintf(str3,
			"CreateObject(\"SAPI.SpVoice\").Speak\"�����Ǳ���ʱ��%s��\
			���Ѿ�������%d���ӣ���ע����Ϣ��\
			you have already worked for %d minutes, please pay attention to your health,\
			\"",tmpStr, minutes, minutes);
	}
	if (*model == 'r')
	{
		sprintf(str3,
			"CreateObject(\"SAPI.SpVoice\").Speak\"�����Ǳ���ʱ��%s��\
			���Ѿ���Ϣ��%d���ӣ���ע�⹤��״̬��\
			you have already rested for %d minutes, please pay attention to your work\"",
			tmpStr, minutes, minutes);
	}
	m = fputs(str3, fp);
	if (m == 0)
	{
		printf("���ڿ�ʼ����������\n");
	}
	else printf("���ޣ�������ĳ�ִ��������޷���������������\n");
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
		default:num = 0; break;/* printf("fun TransNumbers() err: ��ȡ���ִ���\n"); ret = -1; return ret;*/
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
	strcpy(str2, filePathStr);//����Ϊ�˷����������filePathStr���Ƹ�str2
	
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
		fgets(str3, 256, fp);//�޸����ڣ�2017.8.15_11:32
		strcat(filestr, str3);
		int n = strlen(str3);
		str3[n + 1] = ';';//�ֶ����ַ���'\0'�����';'����������ֹ�ַ���'='��ʲô��û�ж��жϳ���
		p = strstr(str3, part);
		if (p != NULL)
		{
			flag = 1;//�ҵ���������
			ptmp = strchr(str3, '=');
			if (ptmp == NULL)
			{
				system("color D");
				printf("������%sû��'='��\n", part);
				printf("���ڳ�ʼ���������ú���\n");
				system("pause");
				fclose(fp);
				Initial_TBT_Set(str2);
				fp = fopen(str2, "r+");
				//�˴���Ӵ���״̬
				ret = -2;
				goto End;
			}
			ptmp = ptmp + 1;
			while (1)//��ȡ������
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
						printf("������%sû�����ó�ʼֵ��\n", part);
						printf("���ڳ�ʼ���������ú���\n");
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
			if (*partValue == 0)//��������partValue��ַ��������0��˵�������������
			{
				ret = TransNumbers(partValueTmp, partValue);
				if (ret != 0)
				{
					printf("fun ReadIni() err: TransNumbers()\n");
				}
				break;
			}
			else//����������ַ����ĵ�ַ������ǿ������ת���ٿ���
			{
				strcpy((char *)partValue, partValueTmp);
				if (*partValueTmp != 'r' && *partValueTmp != 'w')
				{
					system("color D");
					printf("������%s���ô���\n", part);
					printf("���ڳ�ʼ���������ú���\n");
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
		printf("û���ҵ�%s������\n", part);
		printf("���ڳ�ʼ���������ú���\n");
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
		printf("\n  ������̫���ˣ����Ǽ�⵽����������1.11��1.10�汾������ļ�\n");
		printf("\n  ��������'0'����'Enter'��ȷ�ϣ����ǽ�Ϊ������1.10�汾������ļ���1.12�汾\n");
		printf("\n  ��������'1'����'Enter'��ȷ�ϣ����ǽ�Ϊ������1.11�汾������ļ���1.12�汾\n");
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
			printf("\n  ������̫���ˣ����Ǽ�⵽����������1.11�汾������ļ�\n");
		printf("\n  ��Ҫ��1.11�汾���ļ�����ȫ��Ӧ�õ��µ�1.12�汾��\n");
		printf("\n  ��Ȼ�����ǲ�����������ŵ�����ļ����κ�ɾ�����޸�\n");
		printf("\n  ��Ϊ����������������Ҫ��������Ҫ����\n");
		printf("\n  ������ĸ'y'����'Enter'��ȷ�ϣ����ǽ�Ϊ����������ļ���1.12�汾\n");
		printf("\n  ������ĸ'n'����'Enter'��ȷ�ϣ����ǽ�Ϊ1.12�汾����ȫ�µ�����\n");
		while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N')
		{
			scanf("%c", &ch);
			if (ch == 'y' || ch == 'Y')
			{
				if (fp11set != NULL)
				{
					printf("\n  ����TBTset.ini...\n");
					CopySet("TBTset.ini", fp11set);
				}
				if (fp11log != NULL)
				{
					printf("\n  ����TBTlog.txt...\n");
					CopySet("TBTlog.txt", fp11log);
				}
				if (fp11words != NULL)
				{
					printf("\n  ����TBTwords.txt...\n");
					CopySet("TBTwords.txt", fp11words);
				}
				printf("\n  �������!\n");
				system("pause");
			}
			if (ch == 'n' || ch == 'N')
			{
				printf("\n  �������ǽ�Ϊ������ȫ�µ�����\n");
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
			printf("\n  ������̫���ˣ����Ǽ�⵽����������1.10�汾������ļ�\n");
		printf("\n  ��Ҫ��1.10�汾���ļ�����ȫ��Ӧ�õ��µ�1.12�汾��\n");
		printf("\n  ��Ȼ�����ǲ�����������ŵ�����ļ����κ�ɾ�����޸�\n");
		printf("\n  ��Ϊ����������������Ҫ��������Ҫ����\n");
		printf("\n  ������ĸ'y'����'Enter'��ȷ�ϣ����ǽ�Ϊ����������ļ���1.12�汾\n");
		printf("\n  ������ĸ'n'����'Enter'��ȷ�ϣ����ǽ�Ϊ1.12�汾����ȫ�µ�����\n");
		while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N')
		{
			scanf("%c", &ch);
			if (ch == 'y' || ch == 'Y')
			{
				if (fp10set != NULL)
				{
					printf("\n  ����TBTset.ini...\n");
					CopySet("TBTset.ini", fp10set);
				}
				if (fp10log != NULL)
				{
					printf("\n  ����TBTlog.txt...\n");
					CopySet("TBTlog.txt", fp10log);
				}
				if (fp10words != NULL)
				{
					printf("\n  ����TBTwords.txt...\n");
					CopySet("TBTwords.txt", fp10words);
				}
				printf("\n  �������!\n");
				system("pause");
			}
			if (ch == 'n' || ch == 'N')
			{
				printf("\n  �������ǽ�Ϊ������ȫ�µ�����\n");
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
		printf("fun CreateIni() �����ַ����ΪNULL\n");
		ret = -1;
		return ret;
	}
	if (*filename == (char)NULL)
	{
		printf("fun CreateIni() ������ļ�������Ϊ��\n");
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
		int status = _mkdir(filePath); /*����·��*/
		(!status) ? (printf("New directory created (%s): %s\n",TBTversion,filePath)) :
			(printf("Unable to create new directory (%s): %s\n",TBTversion,filePath));
		CheckPreIni();//��������汾������
		fp = fopen(filePathStr, "r+");//�������´������ļ�
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

void TimeBroadcast(int i, const char *model)//�������Ϊ���� 
{
	int minutes = 0;
	minutes = i / 60;
	char tmpStr[64];
	char str[128];
	GetCurTime(tmpStr);//��ȡ��ǰʱ�� 
	sprintf(str, "%s", tmpStr);

	char filePathStr[128] = filePath;
	char fileNameStr[128] = "voice.VBS";
	strcat(filePathStr, fileNameStr);
	CreateVbs(str, minutes, model);//д����ʱ�����ļ�
	ShellExecuteA(0, "open", filePathStr, 0, 0, 1);//ִ���������� 
	Sleep(20000);//��������Ԥ��ʱ��20s 
	//Sleep(1000);
	remove(filePathStr);//ɾ����ʱ�����ļ�

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
	char lockstatu[5] = "��";
	char soundstatu[5] = "��";
	char testStatu[5] = "��";
	char time_resetStatu[5] = "��";
	char all_resetStatu[5] = "��";
	char informationStatu[7] = "����ʾ";
	/*if (TBT->test != 1)
		system("cls");*/
	system("cls");
	if (TBT->lock == 1)
		strcpy(lockstatu, "��");
	if (TBT->sound == 1)
		strcpy(soundstatu, "��");
	if (TBT->test == 1)
		strcpy(testStatu, "��");
	if (TBT->time_reset == 1)
		strcpy(time_resetStatu, "��");
	if (TBT->all_reset == 1)
		strcpy(all_resetStatu, "��");
	if (TBT->information == 1)
		strcpy(informationStatu, "��ʾ");
	if (i <= 59)
		printf("\n\n  ������  ��%d��\n", i);
	else if (i % 60 == 0)
	{
		printf("\n\n  ������  ��%d����\n", i / 60);
	}
	else if (i % 60 != 0)
		printf("\n\n  ������  ��%d����  ��%d��\n", i / 60, i % 60);

	if (TBT->information == 0)
		return;
	printf("\n\n����״̬(%s):\n",TBTversion);
	printf("��ʱ��%d����  ģʽ��%s\n\n", TBT->minutes, model);
	printf("��Ļ����%s\n\n", lockstatu);
	printf("������%s  �������ͣ�%d\n\n", soundstatu, TBT->music);
	printf("���ٲ��ԣ�%s\n\n", testStatu);
	printf("��ʱ״̬��ʼ����%s  ", time_resetStatu);
	printf("�������ó�ʼ����%s\n", all_resetStatu);
	//printf("������״̬��%s\n", informationStatu);
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
	sprintf(scan, "%d��", d);
	char filePathStr[128] = filePath;
	char fileNameStr[128] = "TBTwords.txt";
	strcat(filePathStr, fileNameStr);
	FILE *fp = fopen(filePathStr, "r");
	if (fp == NULL)
	{
		printf("can not open the words file\n");
		return;
	}

	/*366��ÿ��һ�Բ���ģ��*/
	//int flag = 0;
	//for (d = 1; d <= 366; d++)
	//{
	//	sprintf(scan, "%d��", d);
	//	while (!feof(fp))
	//	{
	//		fgets(str, 100, fp);
	//		myStrChange(str, out);//����
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
	//			sprintf(scan, "%d��", d + 1);
	//			flag = 1;
	//		}
	//		memset(str, 0, 100);
	//	}
	//	rewind(fp);
	//	flag = 0;
	//	Sleep(1000);
	//}

	/*ÿ��һ�Ե�����Ŵ�ӡģ��*/
	int flag = 0;
	while (!feof(fp))
	{
		fgets(str, 100, fp);
		myStrChange(str, out);//����
		p = strstr(out, scan);//�����ַ������Ƿ�������
		if (flag == 1)//����Ѿ��ɹ���������һ�ε����
		{
			if (p != NULL)//����������ڶ��ε����
			{
				*p = '\0';//�ַ����е�ǰλ�ø�Ϊ�ַ���������'\0'
				printf("%s", out);
				break;//��ź������Ѵ�ӡ��ϣ�������ѭ��
			}
			printf("%s", out);
		}
		if (p != NULL&&flag == 0)//���������������ǵ�һ����������Ҫ�����
		{
			printf("\nÿ��һ�ԣ�%s", p + strlen(scan));//��ӡ����ź������
			sprintf(scan, "%d��", d + 1);//��������ż�һ
			flag = 1;//�Ѿ���������ţ���flag��һ
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
	printf("\n  ����Ҫж�ص���ʱ��(%s)����������ļ���\n",TBTversion);
	printf("  �⽫�������ļ�ʱ��־(%s)��Զ��ʧ��ȷ��Ҫ����ж����\n",TBTversion);
	system("pause");
	printf("\n  ���������һ�α�����ʱ��־�Ļ����ˣ���Ȼȷ��Ҫ����ж����\n");
	system("pause");
	printf("\n  ȷ�����Ѿ��رյ�TBTset.ini��ҳ�棬���������һ�����ļ���\n");
	system("pause");
	
	char filePathStr[128] = filePath;
	int status = _rmdir(filePathStr); /*ɾ���������ļ���*/
	int delFlag = 0;
	(!status) ? (delFlag=1) :
		(delFlag=0);
	char delCommand[256] = "rd /s /q \"";
	strcat(delCommand, filePathStr);
	strcat(delCommand, "\"");
	system(delCommand);
	system("pause");
	system("cls");
	printf("\n  �Ѿ�ɾ������������ļ����������˳�������֮��ɾ�������򼴿�\n");
	printf("  �ܸ��������� D\n");
}

void CountDay(int YearDay)
{
	printf("\n�����%d�죬��%d�ܵ���%d\n",
		YearDay, (YearDay) / 7 + 1, (YearDay) % 7);
}

int checkAllSet(data *TBT, char *model,int *seconds)
{
	int ret = 0;
	ret = RecheckIni(TBT, model);//��ȡ�����ļ�
	if (ret != 0)
	{
		printf("fun main() err: RecheckIni()\n");
		return ret;
	}
	if (TBT->test != 2)
		InfoPrint(*seconds, TBT, model);//��ӡҪ��ʾ����Ϣ
	if (TBT->day_words == 1){
		int YearDay = 0;
		GetYearDay(&YearDay);//��ȡϵͳʱ�䣺������һ���еĵڼ���
		GetDayWords(YearDay);//ÿ��һ�Թ���
		CountDay(YearDay);//����ڼ��죬�ڼ��ܵļ��㹦��
	}
	if (TBT->uninstall == 1){
		TBTuninstall();//ж�ع���
		return -2;
	}
	if (TBT->all_reset == 1){
		system("color B");
		printf("����Ҫ���³�ʼ��������������\n");
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
		printf("���ڴ��ڼ�ʱ��ʼ״̬�����¿�ʼ��ʱ�뽫time_reset��������Ϊ0\n");
		Sleep(500);
		system("color");
	}
	if (TBT->sound == 0 && TBT->lock == 0){
		system("color A");
		printf("\n���ܽ� ��Ļ������������screen lock �� ��������������sound ͬʱ�رգ������ʱ�����޷�����\n");
		if (TBT->test != 1)
			Sleep(500);
		system("color");
	}
	if (TBT->test == 1 || TBT->test == 2)
	{
		*seconds = TBT->minutes * 60;
		InfoPrint(*seconds, TBT, model);
		printf("\n�����ʱ�յ�ļ�ʱ�����ֲ����Ѿ�����\n");
		return 0;
	}
	return 0;
}

int checkTime(int *seconds,const data *TBT,const char *model)
{
	int sec = *seconds;
	if (sec == TBT->minutes * 60){
		ITCAST_LOG(__FILE__, __LINE__, LogLevel[2], TBT->minutes, model);//��д������־
		if (TBT->sound == 1){
			Music(TBT->music);//��������
			TimeBroadcast(sec, model);//ʱ�䲥������
		}
		if (TBT->lock == 1){
			system("rundll32.exe user32.dll,LockWorkStation");
		}
		printf("\n��Ҫ���¿�ʼ��ʱ��\n");
		system("pause");
		*seconds = 0;
		return 0;
	}
	if (sec > TBT->minutes * 60){
		system("color E");
		printf("\n��ʱ�Ѿ������������õ�ʱ��\n");
		Sleep(500);
	}
	return 0;
}

void Timer(char *title, clock_t *start, clock_t *middle, clock_t *finish, double *totaltime)
{
	printf("\n%sʱ��Ϊ:", title);
	*finish = clock();//��ȡ��ǰʱ��
	*totaltime = (double)(*finish - *middle) / CLOCKS_PER_SEC;//�����м�ʱ��
	printf("%f�� ", *totaltime);
	*totaltime = (double)(*finish - *start) / CLOCKS_PER_SEC;//��������ʱ��
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
	char model[128] = { "work" };//����ReadIni�������ƣ��ַ�����ע�ⲻҪ��ʼ��Ϊ0
	ret = CreateIni("TBTset.ini");//û�������ļ��򴴽�һ�������ļ�
	if (ret != 0){
		printf("fun main() err: CreateIni()\n");
		system("pause");
		return;
	}

	//clock_t start, middle, finish;//��ʱ
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
		Sleep(900);//��ʱ

		ret = checkTime(&seconds, TBT, model);
		if (ret != 0)
			break;
		//Timer("��ʱ��", &start, &middle, &finish, &totaltime);//����һ��ʵ��ʱ��
	}
End:	system("pause");
}