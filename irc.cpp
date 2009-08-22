/* 
 * File:   irc.cpp
 * Author: eddie
 * 
 * Created on August 20, 2009, 11:16 AM
 */
#include "irc.h"

IRC::IRC()
{
	this->ccerr = CrissCross::CC_ERR_NONE;
	this->last_msg = 0;
	this->last_recv = 0;
	this->buffer = new char[4096];
	this->msg_buf = new char[4096];
	this->timeout = 200;
	this->conn = new TCPSocket();
	this->my_server = NULL;
	this->my_nick = NULL;
	this->my_host = NULL;
}

IRC::~IRC()
{
	delete this->conn;
	this->conn = NULL;
	delete [] this->buffer;
	this->buffer = NULL;
	delete [] this->msg_buf;
	this->msg_buf = NULL;
}

LList<char *> IRC::split(char *str,char *delim)
{
	LList<char *> list;
	char *tmp = strtok(str,delim);
	while (tmp != NULL) {
		list.insert(tmp);
		tmp = strtok(NULL, delim);
	}
	return list;
}

int IRC::connect(char *server,char *nick,u_short port,char *pass)
{
	ccerr = conn->Connect(server,port);
	if (ccerr != 0)
		return ccerr;
	this->my_server = server;
	if (pass != NULL) {
		strcpy(this->msg_buf,"PASS ");
		strcat(this->msg_buf,pass);
		this->send(this->msg_buf);
	}
	this->nick(nick);
	this->my_nick = nick;
	this->user(nick,"0",this->my_server);
	char *line = this->readline();
	LList<char *> results = this->split(line," ");
	int i = 0;
	while (!results.valid(1)) {
		char *line = this->readline();
		results = this->split(line," ");
	}
	while (!strcmp(results.get(1),"001")) {
	    if (strcmp(results.get(1),"PING")) {
		    strcpy(this->msg_buf,"PONG ");
		    strcat(this->msg_buf,results.get(2));
		    this->send(this->msg_buf);
	    } else if (strcmp(results.get(1),"433") || strcmp(results.get(1),"432")) {
		    stringstream out;
		    out << ++i;
		    this->nick(strcat(nick,out.str().c_str()));
		    this->my_nick = strcat(nick,out.str().c_str());
	    }
	    line = this->readline();
	    results = this->split(line," ");
	}

	strcpy(this->msg_buf,"WHOIS ");
	strcat(this->msg_buf,this->my_nick);
	this->send(this->msg_buf);
	do {
		line = this->readline();
		results = this->split(line," ");
		if (strcmp(results.get(1),"311"))
			this->my_host = strcat(results.get(4),strcat("@",results.get(5)));
	} while (this->my_host == NULL);

	return 0;
}

char *IRC::readline()
{
	while (1) {
		const char *n = "\n";
		size_t pos = strcspn(this->buffer,n) + 1;
		if (pos != strlen(this->buffer)) {
			char ret[++pos];
			strncpy(ret,this->buffer,--pos);
			ret[++pos] = '\0';
			this->buffer = this->buffer+(--pos);
			printf("%s",ret);
			return ret;
		}
		this->read();
	}
}

int IRC::read()
{
	u_int len = 4096;
	ccerr = conn->Read(this->buffer,&len);
	if (ccerr != 0) {
		return ccerr;
	}
	this->last_recv = time(NULL);
	return 0;
}

int IRC::send(char *message)
{
	ccerr = conn->Send(strcat(message,"\r\n"));
	if (ccerr != 0)
		return ccerr;
	this->last_msg = time(NULL);
	return 0;
}

void IRC::nick(char *newnick)
{
	strcpy(this->msg_buf,"NICK ");
	strcat(this->msg_buf,newnick);
	this->send(this->msg_buf);
}

void IRC::user(char *username, char *host, char *server, char *realname)
{
	if (realname == NULL)
		realname = username;
	strcpy(this->msg_buf,"USER ");
	strcat(this->msg_buf,username);
	strcat(this->msg_buf," ");
	strcat(this->msg_buf,host);
	strcat(this->msg_buf," ");
	strcat(this->msg_buf,server);
	strcat(this->msg_buf," :");
	strcat(this->msg_buf,realname);
	this->send(this->msg_buf);
}

void IRC::oper(char *user, char *password)
{
	strcpy(this->msg_buf,"OPER ");
	strcat(this->msg_buf,user);
	strcat(this->msg_buf," ");
	strcat(this->msg_buf,password);
	this->send(this->msg_buf);
}

void IRC::quit(char *message)
{
	strcpy(this->msg_buf,"QUIT ");
	strcat(this->msg_buf,message);
	this->send(this->msg_buf);
}

void IRC::join(char *channel, char *password)
{
	if (password != NULL) {
		strcpy(this->msg_buf,"JOIN ");
		strcat(this->msg_buf,channel);
		strcat(this->msg_buf," ");
		strcat(this->msg_buf,password);
		this->send(this->msg_buf);
	} else {
		strcpy(this->msg_buf,"JOIN ");
		strcat(this->msg_buf,channel);
		this->send(this->msg_buf);
	}
}

void IRC::part(char *channel)
{
	strcpy(this->msg_buf,"PART ");
	strcat(this->msg_buf,channel);
	this->send(this->msg_buf);
}

void IRC::msg(char *channel, char *message)
{
	strcpy(this->msg_buf,"PRIVthis->msg_buf ");
	strcat(this->msg_buf,channel);
	strcat(this->msg_buf," :");
	strcat(this->msg_buf,message);
	this->send(this->msg_buf);
}

void IRC::ping(char *server, char *server2)
{
	if (server2 != NULL) {
		strcpy(this->msg_buf,"PING ");
		strcat(this->msg_buf,server);
		strcat(this->msg_buf," ");
		strcat(this->msg_buf,server2);
		this->send(this->msg_buf);
	} else {
		strcpy(this->msg_buf,"PING ");
		strcat(this->msg_buf,server);
		this->send(this->msg_buf);
	}
}

void IRC::pong(char *daemon, char *daemon2)
{
	if (daemon2 != NULL) {
		strcpy(this->msg_buf,"PONG ");
		strcat(this->msg_buf,daemon);
		strcat(this->msg_buf," ");
		strcat(this->msg_buf,daemon2);
		this->send(this->msg_buf);
	} else {
		strcpy(this->msg_buf,"PONG ");
		strcat(this->msg_buf,daemon);
		this->send(this->msg_buf);
	}
}