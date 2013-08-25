/*
 * fetion.h
 *
 *  Created on: 2013年8月23日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#ifndef FETION_H_
#define FETION_H_
#include "sock.h"
#include "curlutil.h"
/*
 *（对302重定向只是简单处理了一下，如果网络有问题，还是推荐使用PHP接口）
 * 通过飞信接口发送信息
 * fetionUser:飞信帐号
 * fetionPasswd:飞信密码
 * fetionTo:收件人
 * fetionContent:信息内容
 * 发送成功返回:0
 */
int sendFetionMSG(const char *fetionUser,
			const char *fetionPasswd,
			const char *fetionTo,
			const char *fetionContent,
			int fetionRepeat);

/*
 * 通过第三方飞信PHP接口发送信息
 * 参数：
 * fetionUser:飞信帐号
 * fetionPasswd:飞信密码
 * fetionTo:对方手机(飞信好友)
 * fetionContent：内容
 * fetionRepeat:重复次数
 * 返回值为0时发送成功。
 */
#define FEITION_PHPURL "http://sendfetion.sinaapp.com/feixin.php"
int sendFetionByPHPAPI(
		const char* fetionUser,
		const char* fetionPasswd,
		const char* fetionTo,
		const char* fetionContent,
		int fetionRepeat);

#endif /* FETION_H_ */
