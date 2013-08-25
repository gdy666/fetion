/*
 * main.c
 *
 *  Created on: 2013年8月18日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */
#include "fetion.h"
#define DEBUG

void debug(const char *fmt, ...)
{
	#ifdef DEBUG
		#include <stdarg.h>
		 va_list ap;
		 va_start(ap, fmt);
		 vprintf(fmt, ap);
		 va_end(ap);
	#endif
}
int main()
{
	//利用C 实现的接口发送
	sendFetionMSG("13800138000","yeso123456","13800138000","测试测试测试",3);
	//利用第三方PHP接口发送
	sendFetionByPHPAPI("13800138000","yeso123456","13800138000","测试测试测试",3);
	return 0;
}
