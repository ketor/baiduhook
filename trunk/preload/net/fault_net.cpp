/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/


 
 
 
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#include "common.h"
#include "bhook.h"

extern "C"{
	int FAULT_(getpeername)(int s, struct sockaddr *name, socklen_t *namelen)
	{
	#ifdef DEBUG
	       printf("getpeernamehook\n");   
	#endif
			int Ret = 0;
			typedef int(*REALGETPEERNAME)(int s, struct sockaddr *name, socklen_t *namelen);
			REALGETPEERNAME RealGetPeerName = (REALGETPEERNAME)get_real_func("getpeername");
			if(NULL == RealGetPeerName)
			{
				printf("realrun getpeername error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("getpeername",&err);
			if( 0 == Ret )
			{
					errno = err;
					return -1;
			}
			return RealGetPeerName(s,name,namelen); 		
	}
	int FAULT_(socket)(int domain, int type, int protocol)
	{
	#ifdef DEBUG
	       printf("sockethook\n");   
	#endif
			int Ret = 0;
			typedef int(*REALSOCKET)(int domain, int type, int protocol);
			REALSOCKET RealSocket = (REALSOCKET)get_real_func("socket");
			if(NULL == RealSocket)
			{
				printf("realrun socket error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("socket",&err);
			if( 0 == Ret )
			{
					errno = err;
					return -1;
			}
			return RealSocket(domain,type,protocol); 				
	}
	int FAULT_(listen)(int s, int backlog)
	{
	#ifdef DEBUG
	       printf("listenhook\n");   
	#endif
			int Ret = 0;
			typedef int(*REALLISTEN)(int S,int backlog);
			REALLISTEN RealListen = (REALLISTEN)get_real_func("listen");
			if(NULL == RealListen)
			{
				printf("realrun listen error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("listen",&err);
			if( 0 == Ret )
			{
					errno = err;
					return -1;
			}
			return RealListen(s,backlog); 	
		
	}
	int FAULT_(bind)(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen)
	{
	#ifdef DEBUG
	       printf("bindhook\n");   
	#endif
			int Ret = 0;
			typedef int(*REALBIND)(int sockfd,const struct sockaddr *serv_addr,socklen_t addrlen);
			REALBIND RealBind = (REALBIND)get_real_func("bind");
			if(NULL == RealBind)
			{
				printf("realrun bind error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("bind",&err);
			if( 0 == Ret )
			{
					errno = err;
					return -1;
			}
			return RealBind(sockfd,serv_addr,addrlen);
	}
	int FAULT_(getsockname)(int s, struct sockaddr *name, socklen_t *namelen)
	{
	#ifdef DEBUG
	       printf("getsocknamehook\n");   
	#endif
			int Ret = 0;
			typedef int(*REALGETSOCKNAME)(int s, struct sockaddr *name, socklen_t *namelen);
			REALGETSOCKNAME RealGetSockName = (REALGETSOCKNAME)get_real_func("getsockname");
			if(NULL == RealGetSockName)
			{
				printf("realrun getsockname error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("getsockname",&err);
			if( 0 == Ret )
			{
					errno = err;
					return -1;
			}
			return RealGetSockName(s,name,namelen);
	}
	int FAULT_(connect)(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen)
	{
	#ifdef DEBUG
	       printf("connecthook\n");   
	#endif
			int Ret = 0;
			typedef int(*REALCONNECT)(int sockfd,const struct sockaddr *serv_addr,socklen_t addrlen);
			REALCONNECT RealConnect = (REALCONNECT)get_real_func("connect");
			if(NULL == RealConnect)
			{
				printf("realrun connect error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("connect",&err);
			if( 0 == Ret )
			{
					errno = err;
					return -1;
			}
			return RealConnect(sockfd,serv_addr,addrlen);
	}
	ssize_t FAULT_(send)(int s, const void *buf, size_t len, int flags)
	{
	#ifdef DEBUG
	       printf("sendhook\n");   
	#endif	
			int Ret = 0;
			typedef int(*REALSEND)(int s, const void *buf, size_t len, int flags);
			REALSEND RealSend = (REALSEND)get_real_func("send");
			if(NULL == RealSend)
			{
				printf("realrun send error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("send",&err);
			if( 0 == Ret )
			{
					errno = err;
					return -1;
			}
			return RealSend(s,buf,len,flags);	
	}
	ssize_t FAULT_(recv)(int s, void *buf, size_t len, int flags)
	{
	#ifdef DEBUG
	       printf("recvhook\n");   
	#endif	
			int Ret = 0;
			typedef ssize_t(*REALRECV)(int s, void *buf, size_t len, int flags);
			REALRECV RealRecv = (REALRECV)get_real_func("recv");
			if(NULL == RealRecv)
			{
				printf("realrun recv error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("recv",&err);
			if( 0 == Ret )
			{
					errno = err;
					return -1;
			}
			return RealRecv(s,buf,len,flags);			
	}
	ssize_t FAULT_(recvfrom)(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
	{
	#ifdef DEBUG
	       printf("recvhook\n");   
	#endif	
			int Ret = 0;
			typedef ssize_t(*REALRECVFROM)(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
			REALRECVFROM RealRecvFrom = (REALRECVFROM)get_real_func("recvfrom");
			if(NULL == RealRecvFrom)
			{
				printf("realrun recvfrom error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("recvfrom",&err);
			if( 0 == Ret )
			{
					errno = (long)err;
					return -1;
			}
			return RealRecvFrom(s,buf,len,flags, from, fromlen);			
	}
	ssize_t FAULT_(sendto)(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen)
	{
	#ifdef DEBUG
	       printf("sendtohook\n");   
	#endif	
			int Ret = 0;
			typedef ssize_t(*REALSENDTO)(int s, const void *buf, size_t len, int flags,const struct sockaddr *to, socklen_t tolen);
			REALSENDTO RealSendTo = (REALSENDTO)get_real_func("sendto");
			if(NULL == RealSendTo)
			{
				printf("realrun sendto error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("sendto",&err);
			if( 0 == Ret )
			{
					errno = (long)err;
					return -1;
			}
			return RealSendTo(s,buf,len,flags,to,tolen);				
	}
	int FAULT_(getsockopt)(int s, int level, int optname, void *optval, socklen_t *optlen)
	{
	#ifdef DEBUG
	       printf("getsockopthook\n");   
	#endif
			int Ret = 0;
			typedef int(*REALGETSOCKOPT)(int s, int level, int optname, void *optval, socklen_t *optlen);
			REALGETSOCKOPT RealGetSockOpt = (REALGETSOCKOPT)get_real_func("getsockopt");
			if(NULL == RealGetSockOpt)
			{
				printf("realrun getsockopt error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("getsockopt",&err);
			if( 0 == Ret )
			{
					errno = (long)err;
					return -1;
			}
			return RealGetSockOpt(s,level,optname,optval,optlen);	
	}
	int FAULT_(setsockopt)(int s, int level, int optname, const void *optval, socklen_t *optlen)
	{
	#ifdef DEBUG
	       printf("setsockoptthook\n");   
	#endif
			int Ret = 0;
			typedef int(*REALSETSOCKOPT)(int s, int level, int optname, const void *optval, socklen_t *optlen);
			REALSETSOCKOPT RealSetSockOpt = (REALSETSOCKOPT)get_real_func("setsockopt");
			if(NULL == RealSetSockOpt)
			{
				printf("realrun setsockopt error\n");
				return -1;
			}
			int err;
			Ret = fault_fail("setsockopt",&err);
			if( 0 == Ret )
			{
					errno = (long)err;
					return -1;
			}
			return RealSetSockOpt(s,level,optname,optval,optlen);	
	}



}



//void __attribute__ ((constructor)) netinit(void)
SO_MAIN(fault_net)
{
	SO_MAIN_START(fault_net);
	//attach
	attach_func("getpeername", (void *)FAULT_(getpeername));
	attach_func("socket", (void *)FAULT_(socket));
	attach_func("listen", (void *)FAULT_(listen));
	attach_func("bind", (void *)FAULT_(bind));
	attach_func("getsockname", (void *)FAULT_(getsockname));
	attach_func("connect", (void *)FAULT_(connect));
	attach_func("send", (void *)FAULT_(send));
	attach_func("recv", (void *)FAULT_(recv));
	attach_func("recvfrom", (void *)FAULT_(recvfrom));
	attach_func("sendto", (void *)FAULT_(sendto));
	attach_func("getsockopt", (void *)FAULT_(getsockopt));
	attach_func("setsockopt", (void *)FAULT_(setsockopt));
}










/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
