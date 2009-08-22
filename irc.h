/* 
 * File:   irc.h
 * Author: eddie
 *
 * Created on August 20, 2009, 11:16 AM
 */

#ifndef _IRC_H
#define	_IRC_H

#include "universal_include.h"

class IRC {
public:
	IRC();
	virtual ~IRC();

	char *my_server;
	char *my_nick;
	char *my_host;
	time_t last_recv;
	char *buffer;
	time_t last_msg;
	char *msg_buf;
	unsigned short timeout;

	int connect(char *server,char *nick,u_short port = 6667,char *pass = NULL);
	int read();
	char *readline();
	int send(char *message);
	void parse();
	LList<char *> split(char *str,char *delim);

	void nick(char *newnick);
	void oper(char *user,char *password);
	void quit(char *message);
	void join(char *channel,char *password=NULL);
	void part(char *channel);
	void msg(char *channel,char *message);
	void ping(char *server,char *server2=NULL);
	void pong(char *daemon,char *daemon2=NULL);
protected:
	int ccerr;
private:
	TCPSocket *conn;
	void user(char *username,char *host,char *server,char *realname=NULL);
};

#endif	/* _IRC_H */