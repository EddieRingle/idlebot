/* 
 * File:   irc.cpp
 * Author: eddie
 * 
 * Created on August 20, 2009, 11:16 AM
 */
#include "irc.h"

IRC::IRC()
{
	this->recv_buf = "not null";
	this->timeout = 200;
	this->conn = new TCPSocket();
}

IRC::~IRC()
{
	delete this->conn;
	*this->conn = NULL;
}

int IRC::connect(string server,string nicks[3],u_short port,string password)
{
	ccerr = conn->Connect(server.c_str(),port);
	if (ccerr != 0)
		return ccerr;
	this->my_server = server;
	if (password != "")
		this->send("PASS "+password);
	int i = 0;
	this->nick(nicks[i]);
	this->my_nick = nicks[i];
	this->user(nicks[i]+"~idlebot","0",this->my_server);
	ccerr = this->receive();
	if (ccerr != 0)
		return ccerr;
	char **results = this->split(this->recv_buf," ");
	while (results[1] != "001") {
	    if (results[1] == "PING") {
		    this->send("PONG "+(string)results[1]);
	    } else if (results[1] == "433" || results[1] == "432") {
		    if (!nicks[++i].empty()) {
			    this->nick(nicks[i]);
			    this->my_nick = nicks[i];
		    } else {
			    stringstream out;
			    out << i;
			    this->nick(nicks[0]+out.str());
			    this->my_nick = nicks[0]+out.str();
		    }
	    }
	    this->receive();
	    results = this->split(this->recv_buf," ");
	}

	this->send("WHOIS "+this->my_nick);
	this->my_host = "";
	do {
		this->receive();
		results = this->split(this->recv_buf," ");
		if (results[1] == "311") {
			stringstream s;
			s << results[4] << "@" << results[5];
			this->my_host = s.str();
		}
	} while (this->my_host == "");

	return 0;
}

char** IRC::split(char *str,char *delim)
{
	int i = 0;
	char **results;
	char *tmp = strtok(str,delim);
	while (tmp != NULL) {
		results[i] = tmp;
		strtok(NULL,delim);
		i++;
	}
	return results;
}

int IRC::receive()
{
	u_int len = 4092;
	char *buffer;
	ccerr = conn->Read(buffer,&len);
	if (ccerr != 0) {
		return ccerr;
	}
	if (this->recv_buf != "") {
		this->last_recv = this->recv_buf;
	} else {
		cout << "no data recved." << endl;
	}

	return 0;
}

int IRC::send(string message)
{
	this->last_msg = message;
	ccerr = conn->Send(message+"\r\n");
	if (ccerr != 0)
		return ccerr;
	cout << "<< " << message;
	return 0;
}

void IRC::nick(string newnick)
{
	this->send("NICK "+newnick);
}

void IRC::user(string username, string host, string server, string realname)
{
	if (realname == "")
		string realname = username;
	this->send("USER "+username+" "+host+" "+server+" :"+realname);
}

void IRC::oper(string user, string password)
{
	this->send("OPER "+user+" "+password);
}

void IRC::quit(string message)
{
	this->send("QUIT "+message);
}

void IRC::join(string channel, string password)
{
	if (password != "") {
		this->send("JOIN "+channel+" "+password);
	} else {
		this->send("JOIN "+channel);
	}
}

void IRC::part(string channel)
{
	this->send("PART "+channel);
}

void IRC::msg(string channel, string message)
{
	this->send("PRIVMSG "+channel+" :"+message);
}

void IRC::ping(string server, string server2)
{
	if (server2 != "") {
		this->send("PING "+server+" "+server2);
	} else {
		this->send("PING "+server);
	}
}

void IRC::pong(string daemon, string daemon2)
{
	if (daemon2 != "") {
		this->send("PONG "+daemon+" "+daemon2);
	} else {
		this->send("PONG "+daemon);
	}
}