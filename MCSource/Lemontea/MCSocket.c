#include "MCSocket.h"

static void create_and_bind_socket(MCSocket* this, MCSocketType socket_type, char* ip, char* port);

onload(MCSocketClientInfo)
{
	binding(MCSocketClientInfo, void, dumpInfo);
	binding(MCSocketClientInfo, void, bye);
	return claz;
}

oninit(MCSocketClientInfo)
{
	//nothing to init
	return obj;
}

method(MCSocketClientInfo, void, dumpInfo, voida)
{
	printf("accept a client: %s\n", obj->address.sa_data);
}

method(MCSocketClientInfo, void, bye, voida)
{
	//nothing to do
}

onload(MCSocket)
{
binding(MCSocket, MCSocket*, initWithTypeIpPort, MCSocketType socket_type, char* ip, char* port);
binding(MCSocket, int, listeningStart);
binding(MCSocket, MCSocketClientInfo*, acceptARequest);
binding(MCSocket, void, recv);
binding(MCSocket, void, recvfrom);
binding(MCSocket, void, recvmsg);
binding(MCSocket, void, send);
binding(MCSocket, void, sendto);
binding(MCSocket, void, sendmsg);
binding(MCSocket, void, bye);
return claz;
}

oninit(MCSocket)
{
	//nothing to init
	return obj;
}

method(MCSocket, MCSocket*, initWithTypeIpPort, MCSocketType socket_type, char* ip, char* port)
{
	create_and_bind_socket(obj, socket_type, ip, port);
	return obj;
}

static void create_and_bind_socket(MCSocket* this, MCSocketType socket_type, char* ip, char* port)
{
	//int sfd = ;
	//BOOL isServer;
	struct addrinfo hint, *result;
	runtime_log("begin prepare hint.\n");

	//prepare hint
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_family = AF_UNSPEC;
	//hint.ai_protocol = IPPROTO_IP;
	switch(socket_type){
		case MCSocket_Server_TCP:
			hint.ai_socktype = SOCK_STREAM;
			hint.ai_flags = AI_PASSIVE;
			this->isServer = 1;
		break;
		case MCSocket_Server_UDP:
			hint.ai_socktype = SOCK_DGRAM;
			hint.ai_flags = AI_PASSIVE;
			this->isServer = 1;
		break;
		case MCSocket_Client_TCP:
			hint.ai_socktype = SOCK_STREAM;
			this->isServer = 0;
		break;
		case MCSocket_Client_UDP:
			hint.ai_socktype = SOCK_DGRAM;
			this->isServer = 0;
		break;
	}
	//get addrinfo linklist
	if(getaddrinfo(ip, port, &hint, &result)){
		error_log("can not getaddrinfo, return mull\n");
		return;
	}
	runtime_log("get the addrinfo linklist.\n");

	//try every addrinfo in list to create and bind until success
	struct addrinfo* iterator;
	for(iterator = result; iterator!=mull; iterator = iterator->ai_next)
	{		
		this->sfd = socket(iterator->ai_family, iterator->ai_socktype, iterator->ai_protocol);
		if (this->isServer)
		{
			if (this->sfd==-1)
				continue;
			if (!bind(this->sfd, iterator->ai_addr, iterator->ai_addrlen))
			{
				//memcpy(&this->peeraddrinfo, iterator, sizeof(struct addrinfo));
				break;//bind success
			}			
				
		}else{
			if (this->sfd==-1)
				continue;
			if (!connect(this->sfd, iterator->ai_addr, iterator->ai_addrlen))
				break;//connect success
		}
		close(this->sfd);
	}
	//free linklist
	freeaddrinfo(result);
	//return socket fd
	//return sfd;
}

method(MCSocket, void, bye, voida)
{
	close(obj->sfd);
}

//EADDRINUSE
//EBADF
//ENOTSOCK
//EOPNOTSUPP
method(MCSocket, int, listeningStart, voida)
{
	if(obj->isServer!=1)return -1;
	return listen(obj->sfd, MCSocket_Queue_Length);
}

method(MCSocket, MCSocketClientInfo*, acceptARequest, voida)
{
	if (obj->isServer!=1)return mull;
	MCSocketClientInfo* clientinfo = new(MCSocketClientInfo);
	clientinfo->returnSfd = accept(obj->sfd, &clientinfo->address, &clientinfo->address_len);
	return clientinfo;
}

method(MCSocket, void, recv, voida)
{

}

method(MCSocket, void, recvfrom, voida)
{

}

method(MCSocket, void, recvmsg, voida)
{

}

method(MCSocket, void, send, voida)
{

}

method(MCSocket, void, sendto, voida)
{

}

method(MCSocket, void, sendmsg, voida)
{

}

