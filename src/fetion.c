/*
 * fetion.c
 *
 *  Created on: 2013年8月23日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#include "fetion.h"

#define MAXBUFFER 10240
static const char* FETION_HOST= "f.10086.cn";
static const char* FETION_LOGIN_URL="/huc/user/space/login.do?m=submit&fr=space";
static const char* FETION_LOGIN_URL2="/im/login/cklogin.action";
static const char* FETION_SEND_SELF="/im/user/sendMsgToMyselfs.action";
static const char* FETION_LOGOUT="/im/index/logoutsubmit.action";
static const char* FETION_CSRFTOKEN="/im/chat/toinputMsg.action?touserid=";
static const char* userAgent="Mozilla/5.0 (Windows NT 5.1; rv:14.0) Gecko/20100101 Firefox/14.0.1";

int sendFetionByPHPAPI(
		const char* fetionUser,
		const char* fetionPasswd,
		const char* fetionTo,
		const char* fetionContent,
		int fetionRepeat)
{
	char* argv=(char*)malloc(sizeof(char)*512);
		char* result=(char*)calloc(256,sizeof(char));
		int state=0;

		sprintf(argv,"user=%s&passwd=%s&to=%s&msg=%s&repeat=%d",
				fetionUser,fetionPasswd,fetionTo,fetionContent,fetionRepeat);

		post_with_argv(FEITION_PHPURL,argv,result);
		if(!strstr(result,"success")){
			state=1;
		}
		free(argv);
		free(result);
		return state;
}

static char cookies[1024]="";
int sendFetionMSG(const char *fetionUser,
			const char *fetionPasswd,
			const char *fetionTo,
			const char *fetionContent,
			int fetionRepeat)
{
	int state=0;
	loginFetion(fetionUser,fetionPasswd);	//登陆

	state=sendMSG(fetionUser,fetionTo,fetionContent,fetionRepeat);

	logonFetion();//注销登陆
	return state;
}

int loginFetion(const char *fetionUser,const char *fetionPasswd)
{
	char data[64];

	sprintf(data,"mobilenum=%s&password=%s",fetionUser,fetionPasswd);
	char result[MAXBUFFER]="";
//	char *result=(char*)calloc(MAXBUFFER,sizeof(char));
	int ret;
	debug("FETION_LOGIN\n");
	ret=postWithCookies(FETION_LOGIN_URL,data,result);

	getCookiesWithResult(result);
	memset(result,'\0',MAXBUFFER);
	debug("FETION_LOGIN2\n");
	ret=postWithCookies("FETION_LOGIN_URL2","",result);
//	free(result);
	return ret;
}

int sendMSG(const char *fetionUser,const char *fetionTo, const char *fetionContent,int fetionRepeat)
{
	int ret;
	if(!strcmp(fetionUser,fetionTo)){//发给自己
		ret=msgToSelf(fetionContent,fetionRepeat);
	}else{
		char uid[16]="";
		debug("TogetUID...\n");
		getUid(fetionTo,uid);
		debug("UID:%s\n",uid);
		ret=msgToUser(uid,fetionContent,fetionRepeat);
	}
	return ret;
}

int msgToSelf(const char *fetionContent,int fetionRepeat){
	int ret;
	char data[256]="";
	char result[MAXBUFFER];
	sprintf(data,"msg=%s",fetionContent);

	while(fetionRepeat>0){
		memset(result,'\0',MAXBUFFER);
		postWithCookies(FETION_SEND_SELF,data,result);
		fetionRepeat--;
	}
	if(strstr(result,"成功") == NULL){
			ret=1;
	}
	return ret;
}

int msgToUser(const char* uid,const char* fetionContent,int fetionRepeat)
{
	int ret=0;
	char CsrfToken[128]="";
	debug("TogetCsrfToken...\n");
	getCsrfToken(uid,CsrfToken);
	debug("CsrfToken:%s\n",CsrfToken);
	char url[128];
	char data[512];
	sprintf(url,"/im/chat/sendMsg.action?touserid=%s",uid);
	sprintf(data,"msg=%s&csrfToken=%s",fetionContent,CsrfToken);
	char result[MAXBUFFER];
	while(fetionRepeat>0){
		memset(result,'\0',MAXBUFFER);
		postWithCookies(url,data,result);
		fetionRepeat--;
	}
	if(strstr(result,"成功") == NULL){
		ret=1;
	}
	return ret;
}

void getUid(const char *fetionTo,char* uid){
	static char* url="/im/index/searchOtherInfoList.action";
	char data[64]="";
	char result[MAXBUFFER];
	//char *result=(char*)calloc(MAXBUFFER,sizeof(char));
	sprintf(data,"searchText=%s",fetionTo);
	postWithCookies(url,data,result);
	char *p;
	int repeat=0;
	while((p=strstr(result,"touserid="))==NULL && repeat<11){
		memset(result,'\0',MAXBUFFER);
		postWithCookies(url,data,result);
		repeat++;
	}
	if(strstr(result,"touserid=")==NULL){
		return;
	}
	p+=9;
	sscanf(p,"%[^&]",uid);
	//free(result);
}

void getCsrfToken(const char* uid,char* CsrfToken)
{
	char url[64]="";
	sprintf(url,"/im/chat/toinputMsg.action?touserid=%s",uid);
	char result[MAXBUFFER]="";
	postWithCookies(url,"",result);
	int repeat=0;
	char *p;
		while((p=strstr(result,"csrfToken"))==NULL && repeat<11){
			memset(result,'\0',MAXBUFFER);
			postWithCookies(url,"",result);
			repeat++;
		}
		if(strstr(result,"csrfToken") == NULL){
			return;
		}

		p+=18;
		sscanf(p,"%[^\"]",CsrfToken);
}
void logonFetion(){
	char result[MAXBUFFER]="";
	postWithCookies(FETION_LOGOUT,"",result);
}
int getCookiesWithResult(const char* result)
{
	int state=0;
	char* p=result;
	char* cookie=(char*)calloc(512,sizeof(char));
	while((p=strstr(p,"Set-Cookie: ")) != NULL){
		p+=12;
		sscanf(p,"%[^;]",cookie);
		strcat(cookies,cookie);
		strcat(cookies,"; ");
		memset(cookie,'\0',512);
	}
	int n=strlen(cookies);
	//cookies[n-2]=' ';
	//cookies[n-1]='\0';
	if(n<256){
		state=1;
	}
	free(cookie);

	return state;
}

int postWithCookies(const char* uri,const char* data,char* result)
{
	int state;
	SOCK *sock=SocketClient(FETION_HOST,"80",SOCK_TCP);

	char request[4096]="";

	sprintf(request,"POST %s HTTP/1.1\r\nHost: %s\r\nCookie: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nUser-Agent: %s\r\nContent-Length: %d\r\nConnection:  close\r\n\r\n%s\n",
			uri,FETION_HOST,cookies,userAgent,strlen(data),data);
	SocketWrite(sock,request,strlen(request));
	//SocketRead(sock,result,MAXBUFFER);

	struct timeval timeout = {1,50000};
	setsockopt(sock->sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(struct timeval));
	char temp[1500];
	int size;
	do{
		memset(temp,'\0',1500);
		size=recv(sock->sockfd,temp,1500,0);
		strcat(result,temp);
	}while(size>0);

	SocketClose(sock);
	char* p=result;
	if((p=strstr(p,"302 Found"))!=NULL){
		char uri[128]="";
		p=strstr(p,"Location:");
		sscanf(p,"%*s%s",uri);
		sscanf(uri,"http://f.10086.cn%s",uri);
		memset(result,'\0',MAXBUFFER);
		postWithCookies(uri,data,result);
	}
	return state;
}
