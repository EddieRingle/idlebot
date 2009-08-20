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

	string my_server;
	string my_nick;
	string my_host;
	string last_recv;
	char *recv_buf;
	string last_msg;
	unsigned short timeout;

	int connect(string server,string nicks[3],u_short port = 6667,string password = "");
	int receive();
	int send(string message);
	void parse();
	char **split(char *str,char *delim);

	void nick(string newnick);
	void oper(string user,string password);
	void quit(string message);
	void join(string channel,string password="");
	void part(string channel);
	void msg(string channel,string message);
	void ping(string server,string server2="");
	void pong(string daemon,string daemon2="");
protected:
	int ccerr;
private:
	TCPSocket *conn;

	void user(string username,string host,string server,string realname="");
};

#endif	/* _IRC_H */

