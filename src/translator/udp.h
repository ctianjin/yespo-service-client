#ifndef udp_h
#define udp_h

#ifdef __MACH__
typedef int socklen_t;
#endif

#include <errno.h>

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
/*#include <winsock2.h>*/
#include <WinSock.h>
#include <io.h>

typedef int socklen_t;
typedef SOCKET Socket;

#define EWOULDBLOCK             WSAEWOULDBLOCK
#define EINPROGRESS             WSAEINPROGRESS
#define EALREADY                WSAEALREADY
#define ENOTSOCK                WSAENOTSOCK
#define EDESTADDRREQ            WSAEDESTADDRREQ
#define EMSGSIZE                WSAEMSGSIZE
#define EPROTOTYPE              WSAEPROTOTYPE
#define ENOPROTOOPT             WSAENOPROTOOPT
#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
#define EOPNOTSUPP              WSAEOPNOTSUPP
#define EPFNOSUPPORT            WSAEPFNOSUPPORT
#define EAFNOSUPPORT            WSAEAFNOSUPPORT
#define EADDRINUSE              WSAEADDRINUSE
#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
#define ENETDOWN                WSAENETDOWN
#define ENETUNREACH             WSAENETUNREACH
#define ENETRESET               WSAENETRESET
#define ECONNABORTED            WSAECONNABORTED
#define ECONNRESET              WSAECONNRESET
#define ENOBUFS                 WSAENOBUFS
#define EISCONN                 WSAEISCONN
#define ENOTCONN                WSAENOTCONN
#define ESHUTDOWN               WSAESHUTDOWN
#define ETOOMANYREFS            WSAETOOMANYREFS
#define ETIMEDOUT               WSAETIMEDOUT
#define ECONNREFUSED            WSAECONNREFUSED
#define ELOOP                   WSAELOOP
#define EHOSTDOWN               WSAEHOSTDOWN
#define EHOSTUNREACH            WSAEHOSTUNREACH
#define EPROCLIM                WSAEPROCLIM
#define EUSERS                  WSAEUSERS
#define EDQUOT                  WSAEDQUOT
#define ESTALE                  WSAESTALE
#define EREMOTE                 WSAEREMOTE

typedef LONGLONG Int64; 
inline int getErrno() { return WSAGetLastError(); }

#else

typedef int Socket;
static const Socket INVALID_SOCKET = -1;
static const int SOCKET_ERROR = -1;

inline int closesocket( Socket fd ) { return close(fd); };

inline int getErrno() { return errno; }

#define WSANOTINITIALISED  EPROTONOSUPPORT

#endif

/// Open a UDP socket to receive on the given port - if port is 0, pick a a
/// port, if interfaceIp!=0 then use ONLY the interface specified instead of
/// all of them  
Socket
openPort( unsigned short port, unsigned int interfaceIp,
          bool verbose);


/// recive a UDP message 
bool 
getMessage( Socket fd, char* buf, int* len,
            unsigned int* srcIp, unsigned short* srcPort,
            bool verbose);


/// send a UDP message 
bool 
sendMessage( Socket fd, char* msg, int len, 
             unsigned int dstIp, unsigned short dstPort,
             bool verbose);


/// set up network - does nothing in unix but needed for windows
void
initNetwork();

#endif
