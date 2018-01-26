#define _CRT_SECURE_NO_WARNINGS
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TBTlog.h"

#define ITCAST_DEBUG_FILE_	"socketclient.log"
#define ITCAST_MAX_STRING_LEN 		10240

//Level类别
#define IC_NO_LOG_LEVEL			0
#define IC_DEBUG_LEVEL			1
#define IC_INFO_LEVEL			2
#define IC_WARNING_LEVEL		3
#define IC_ERROR_LEVEL			4

int  LogLevel[5] = { IC_NO_LOG_LEVEL, IC_DEBUG_LEVEL, IC_INFO_LEVEL, IC_WARNING_LEVEL, IC_ERROR_LEVEL };

//Level的名称
char ICLevelName[5][10] = { "NOLOG", "DEBUG", "INFO", "WARNING", "ERROR" };

static int ITCAST_Error_GetCurTime(char* strTime)
{
	struct tm*		tmTime = NULL;
	size_t			timeLen = 0;
	time_t			tTime = 0;

	tTime = time(NULL);
	tmTime = localtime(&tTime);
	//timeLen = strftime(strTime, 33, "%Y(Y)%m(M)%d(D)%H(H)%M(M)%S(S)", tmTime);
	timeLen = strftime(strTime, 33, "%Y.%m.%d %H:%M:%S", tmTime);

	return timeLen;
}

static void ITCAST_Error_Core(const char *file, int line, int level, int status, const char *fmt, va_list args)
{
	char str[ITCAST_MAX_STRING_LEN];
	int	 strLen = 0;
	char tmpStr[64];
	int	 tmpStrLen = 0;
	char	fileName[1024];
	sprintf(fileName, "c:\\itcast\\%s", ITCAST_DEBUG_FILE_);
	char filePathStr[128] = filePath;
	char fileNameStr[128] = "TBTlog.txt";
	strcat(filePathStr, fileNameStr);
	FILE *pf = fopen(filePathStr, "a+");
	if (pf == NULL)
	{
		printf("日志文件创建失败！\n");
		system("pause");
		return;
	}

	//初始化
	memset(str, 0, ITCAST_MAX_STRING_LEN);
	memset(tmpStr, 0, 64);

	//加入LOG时间
	tmpStrLen = ITCAST_Error_GetCurTime(tmpStr);
	tmpStrLen = sprintf(str, "[%s] ", tmpStr);
	strLen = tmpStrLen;

	//加入LOG等级
	tmpStrLen = sprintf(str + strLen, "[%s] ", ICLevelName[level]);
	strLen += tmpStrLen;

	//加入LOG状态
	if (status != 0)
	{
		tmpStrLen = sprintf(str + strLen, "[minutes is %d] ", status);
	}
	else
	{
		tmpStrLen = sprintf(str + strLen, "[SUCCESS] ");
	}
	strLen += tmpStrLen;

	//加入LOG信息
	tmpStrLen = vsprintf(str + strLen, fmt, args);
	strLen += tmpStrLen;

	//加入LOG发生文件
	tmpStrLen = sprintf(str + strLen, " [%s]", file);
	strLen += tmpStrLen;

	//加入LOG发生行数
	tmpStrLen = sprintf(str + strLen, " [%d]\n", line);
	strLen += tmpStrLen;

	//写入LOG文件
	fputs(str, pf);

	//关闭文件
	fclose(pf);

	return;
}


void ITCAST_LOG(const char *file, int line, int level, int status, const char *fmt, ...)
{
	va_list args;

	//判断是否需要写LOG
	//	if(level!=IC_DEBUG_LEVEL && level!=IC_INFO_LEVEL && level!=IC_WARNING_LEVEL && level!=IC_ERROR_LEVEL)
	if (level == IC_NO_LOG_LEVEL)
	{
		return;
	}

	//调用核心的写LOG函数
	va_start(args, fmt);
	ITCAST_Error_Core(file, line, level, status, fmt, args);
	va_end(args);

	return;
}
