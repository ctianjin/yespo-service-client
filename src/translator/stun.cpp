#include <cassert>
#include <cstring>
#include <iostream>
#include <cstdlib>   
#include <errno.h>

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>
#else

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <net/if.h>

#endif


#if defined(__sparc__) || defined(WIN32)
#define NOSSL
#endif
#define NOSSL

#include "udp.h"
#include "stun.h"


using namespace std;


static void
computeHmac(char* hmac, const char* input, int length, const char* key, int keySize);

static bool 
stunParseAtrAddress( char* body, unsigned int hdrLen,  StunAtrAddress4& result )
{
   if ( hdrLen != 8 )
   {
      clog << "hdrLen wrong for Address" <<endl;
      return false;
   }
   result.pad = *body++;
   result.family = *body++;
   if (result.family == IPv4Family)
   {
      UInt16 nport;
      memcpy(&nport, body, 2); body+=2;
      result.ipv4.port = ntohs(nport);
		
      UInt32 naddr;
      memcpy(&naddr, body, 4); body+=4;
      result.ipv4.addr = ntohl(naddr);
      return true;
   }
   else if (result.family == IPv6Family)
   {
      clog << "ipv6 not supported" << endl;
   }
   else
   {
      clog << "bad address family: " << result.family << endl;
   }
	
   return false;
}

static bool 
stunParseAtrChangeRequest( char* body, unsigned int hdrLen,  StunAtrChangeRequest& result )
{
   if ( hdrLen != 4 )
   {
      clog << "hdr length = " << hdrLen << " expecting " << sizeof(result) << endl;
		
      clog << "Incorrect size for ChangeRequest" << endl;
      return false;
   }
   else
   {
      memcpy(&result.value, body, 4);
      result.value = ntohl(result.value);
      return true;
   }
}

static bool 
stunParseAtrError( char* body, unsigned int hdrLen,  StunAtrError& result )
{
   if ( hdrLen >= sizeof(result) )
   {
      clog << "head on Error too large" << endl;
      return false;
   }
   else
   {
      memcpy(&result.pad, body, 2); body+=2;
      result.pad = ntohs(result.pad);
      result.errorClass = *body++;
      result.number = *body++;
		
      result.sizeReason = hdrLen - 4;
      memcpy(&result.reason, body, result.sizeReason);
      result.reason[result.sizeReason] = 0;
      return true;
   }
}

static bool 
stunParseAtrUnknown( char* body, unsigned int hdrLen,  StunAtrUnknown& result )
{
   if ( hdrLen >= sizeof(result) )
   {
      return false;
   }
   else
   {
      if (hdrLen % 4 != 0) return false;
      result.numAttributes = hdrLen / 4;
      for (int i=0; i<result.numAttributes; i++)
      {
         memcpy(&result.attrType[i], body, 2); body+=2;
         result.attrType[i] = ntohs(result.attrType[i]);
      }
      return true;
   }
}


static bool 
stunParseAtrString( char* body, unsigned int hdrLen,  StunAtrString& result )
{
   if ( hdrLen >= STUN_MAX_STRING )
   {
      clog << "String is too large" << endl;
      return false;
   }
   else
   {
      if (hdrLen % 4 != 0)
      {
         clog << "Bad length string " << hdrLen << endl;
         return false;
      }
		
      result.sizeValue = hdrLen;
      memcpy(&result.value, body, hdrLen);
      result.value[hdrLen] = 0;
      return true;
   }
}


static bool 
stunParseAtrIntegrity( char* body, unsigned int hdrLen,  StunAtrIntegrity& result )
{
   if ( hdrLen != 20)
   {
      clog << "MessageIntegrity must be 20 bytes" << endl;
      return false;
   }
   else
   {
      memcpy(&result.hash, body, hdrLen);
      return true;
   }
}


bool
stunParseMessage( char* buf, unsigned int bufLen, StunMessage& msg, bool verbose)
{
   if (verbose) clog << "Received stun message: " << bufLen << " bytes" << endl;
   memset(&msg, 0, sizeof(msg));
	
   if (sizeof(StunMsgHdr) > bufLen)
   {
      clog << "Bad message" << endl;
      return false;
   }
	
   memcpy(&msg.msgHdr, buf, sizeof(StunMsgHdr));
   msg.msgHdr.msgType = ntohs(msg.msgHdr.msgType);
   msg.msgHdr.msgLength = ntohs(msg.msgHdr.msgLength);
	
   if (msg.msgHdr.msgLength + sizeof(StunMsgHdr) != bufLen)
   {
      clog << "Message header length doesn't match message size: "
           << msg.msgHdr.msgLength << " - " << bufLen << endl;
      return false;
   }
	
   char* body = buf + sizeof(StunMsgHdr);
   unsigned int size = msg.msgHdr.msgLength;
	
   //clog << "bytes after header = " << size << endl;
	
   while ( size > 0 )
   {
      // !jf! should check that there are enough bytes left in the buffer
		
      StunAtrHdr* attr = reinterpret_cast<StunAtrHdr*>(body);
		
      unsigned int attrLen = ntohs(attr->length);
      int atrType = ntohs(attr->type);
		
      //if (verbose) clog << "Found attribute type=" << AttrNames[atrType] << " length=" << attrLen << endl;
      if ( attrLen+4 > size ) 
      {
         clog << "claims attribute is larger than size of message " 
              <<"(attribute type="<<atrType<<")"<< endl;
         return false;
      }
		
      body += 4; // skip the length and type in attribute header
      size -= 4;
		
      switch ( atrType )
      {
         case MappedAddress:
            msg.hasMappedAddress = true;
            if ( stunParseAtrAddress(  body,  attrLen,  msg.mappedAddress )== false )
            {
               clog << "problem parsing MappedAddress" << endl;
               return false;
            }
          
					
            break;  

         case ResponseAddress:
            msg.hasResponseAddress = true;
            if ( stunParseAtrAddress(  body,  attrLen,  msg.responseAddress )== false )
            {
               clog << "problem parsing ResponseAddress" << endl;
               return false;
            }
          
            break;  
				
         case ChangeRequest:
            msg.hasChangeRequest = true;
            if (stunParseAtrChangeRequest( body, attrLen, msg.changeRequest) == false)
            {
               clog << "problem parsing ChangeRequest" << endl;
               return false;
            }
            else
            {
               if (verbose) clog << "ChangeRequest = " << msg.changeRequest.value << endl;
            }
            break;
				
         case SourceAddress:
            msg.hasSourceAddress = true;
            if ( stunParseAtrAddress(  body,  attrLen,  msg.sourceAddress )== false )
            {
               clog << "problem parsing SourceAddress" << endl;
               return false;
            }

            break;  
				
         case ChangedAddress:
            msg.hasChangedAddress = true;
            if ( stunParseAtrAddress(  body,  attrLen,  msg.changedAddress )== false )
            {
               clog << "problem parsing ChangedAddress" << endl;
               return false;
            }
   
            break;  
				
         case Username: 
            msg.hasUsername = true;
            if (stunParseAtrString( body, attrLen, msg.username) == false)
            {
               clog << "problem parsing Username" << endl;
               return false;
            }

					
            break;
				
         case Password: 
            msg.hasPassword = true;
            if (stunParseAtrString( body, attrLen, msg.password) == false)
            {
               clog << "problem parsing Password" << endl;
               return false;
            }
   
            break;
				
         case MessageIntegrity:
            msg.hasMessageIntegrity = true;
            if (stunParseAtrIntegrity( body, attrLen, msg.messageIntegrity) == false)
            {
               clog << "problem parsing MessageIntegrity" << endl;
               return false;
            }

					
            // read the current HMAC
            // look up the password given the user of given the transaction id 
            // compute the HMAC on the buffer
            // decide if they match or not
            break;
				
         case ErrorCode:
            msg.hasErrorCode = true;
            if (stunParseAtrError(body, attrLen, msg.errorCode) == false)
            {
               clog << "problem parsing ErrorCode" << endl;
               return false;
            }
 
					
            break;
				
         case UnknownAttribute:
            msg.hasUnknownAttributes = true;
            if (stunParseAtrUnknown(body, attrLen, msg.unknownAttributes) == false)
            {
               clog << "problem parsing UnknownAttribute" << endl;
               return false;
            }
            break;
				
         case ReflectedFrom:
            msg.hasReflectedFrom = true;
            if ( stunParseAtrAddress(  body,  attrLen,  msg.reflectedFrom ) == false )
            {
               clog << "problem parsing ReflectedFrom" << endl;
               return false;
            }
            break;  
				
         case XorMappedAddress:
            msg.hasXorMappedAddress = true;
            if ( stunParseAtrAddress(  body,  attrLen,  msg.xorMappedAddress ) == false )
            {
               clog << "problem parsing XorMappedAddress" << endl;
               return false;
            }

            break;  

         case XorOnly:
            msg.xorOnly = true;
            if (verbose) 
            {
               clog << "xorOnly = true" << endl;
            }
            break;  
				
         case ServerName: 
            msg.hasServerName = true;
            if (stunParseAtrString( body, attrLen, msg.serverName) == false)
            {
               clog << "problem parsing ServerName" << endl;
               return false;
            }

            break;
				
         case SecondaryAddress:
            msg.hasSecondaryAddress = true;
            if ( stunParseAtrAddress(  body,  attrLen,  msg.secondaryAddress ) == false )
            {
               clog << "problem parsing secondaryAddress" << endl;
               return false;
            }

            break;  
					
         default:
            if (verbose) clog << "Unknown attribute: " << atrType << endl;
            if ( atrType <= 0x7FFF ) 
            {
               return false;
            }
      }
		
      body += attrLen;
      size -= attrLen;
   }
    
   return true;
}


static char* 
encode16(char* buf, UInt16 data)
{
   UInt16 ndata = htons(data);
   memcpy(buf, reinterpret_cast<void*>(&ndata), sizeof(UInt16));
   return buf + sizeof(UInt16);
}

static char* 
encode32(char* buf, UInt32 data)
{
   UInt32 ndata = htonl(data);
   memcpy(buf, reinterpret_cast<void*>(&ndata), sizeof(UInt32));
   return buf + sizeof(UInt32);
}


static char* 
encode(char* buf, const char* data, unsigned int length)
{
   memcpy(buf, data, length);
   return buf + length;
}


static char* 
encodeAtrAddress4(char* ptr, UInt16 type, const StunAtrAddress4& atr)
{
   ptr = encode16(ptr, type);
   ptr = encode16(ptr, 8);
   *ptr++ = atr.pad;
   *ptr++ = IPv4Family;
   ptr = encode16(ptr, atr.ipv4.port);
   ptr = encode32(ptr, atr.ipv4.addr);
	
   return ptr;
}

static char* 
encodeAtrChangeRequest(char* ptr, const StunAtrChangeRequest& atr)
{
   ptr = encode16(ptr, ChangeRequest);
   ptr = encode16(ptr, 4);
   ptr = encode32(ptr, atr.value);
   return ptr;
}

static char* 
encodeAtrError(char* ptr, const StunAtrError& atr)
{
   ptr = encode16(ptr, ErrorCode);
   ptr = encode16(ptr, 6 + atr.sizeReason);
   ptr = encode16(ptr, atr.pad);
   *ptr++ = atr.errorClass;
   *ptr++ = atr.number;
   ptr = encode(ptr, atr.reason, atr.sizeReason);
   return ptr;
}


static char* 
encodeAtrUnknown(char* ptr, const StunAtrUnknown& atr)
{
   ptr = encode16(ptr, UnknownAttribute);
   ptr = encode16(ptr, 2+2*atr.numAttributes);
   for (int i=0; i<atr.numAttributes; i++)
   {
      ptr = encode16(ptr, atr.attrType[i]);
   }
   return ptr;
}


static char* 
encodeXorOnly(char* ptr)
{
   ptr = encode16(ptr, XorOnly );
   return ptr;
}


static char* 
encodeAtrString(char* ptr, UInt16 type, const StunAtrString& atr)
{
   assert(atr.sizeValue % 4 == 0);
	
   ptr = encode16(ptr, type);
   ptr = encode16(ptr, atr.sizeValue);
   ptr = encode(ptr, atr.value, atr.sizeValue);
   return ptr;
}


static char* 
encodeAtrIntegrity(char* ptr, const StunAtrIntegrity& atr)
{
   ptr = encode16(ptr, MessageIntegrity);
   ptr = encode16(ptr, 20);
   ptr = encode(ptr, atr.hash, sizeof(atr.hash));
   return ptr;
}


unsigned int
stunEncodeMessage( const StunMessage& msg, 
                   char* buf, 
                   unsigned int bufLen, 
                   const StunAtrString& password, 
                   bool verbose)
{
   assert(bufLen >= sizeof(StunMsgHdr));
   char* ptr = buf;
	
   ptr = encode16(ptr, msg.msgHdr.msgType);
   char* lengthp = ptr;
   ptr = encode16(ptr, 0);
   ptr = encode(ptr, reinterpret_cast<const char*>(msg.msgHdr.id.octet), sizeof(msg.msgHdr.id));
	
   if (verbose) clog << "Encoding stun message: " << endl;
   if (msg.hasMappedAddress)
   {

      ptr = encodeAtrAddress4 (ptr, MappedAddress, msg.mappedAddress);
   }
   if (msg.hasResponseAddress)
   {

      ptr = encodeAtrAddress4(ptr, ResponseAddress, msg.responseAddress);
   }
   if (msg.hasChangeRequest)
   {

      ptr = encodeAtrChangeRequest(ptr, msg.changeRequest);
   }
   if (msg.hasSourceAddress)
   {

      ptr = encodeAtrAddress4(ptr, SourceAddress, msg.sourceAddress);
   }
   if (msg.hasChangedAddress)
   {

      ptr = encodeAtrAddress4(ptr, ChangedAddress, msg.changedAddress);
   }
   if (msg.hasUsername)
   {

      ptr = encodeAtrString(ptr, Username, msg.username);
   }
   if (msg.hasPassword)
   {

      ptr = encodeAtrString(ptr, Password, msg.password);
   }
   if (msg.hasErrorCode)
   {
      if (verbose) clog << "Encoding ErrorCode: class=" 
			<< int(msg.errorCode.errorClass)  
			<< " number=" << int(msg.errorCode.number) 
			<< " reason=" 
			<< msg.errorCode.reason 
			<< endl;
		
      ptr = encodeAtrError(ptr, msg.errorCode);
   }
   if (msg.hasUnknownAttributes)
   {
      if (verbose) clog << "Encoding UnknownAttribute: ???" << endl;
      ptr = encodeAtrUnknown(ptr, msg.unknownAttributes);
   }
   if (msg.hasReflectedFrom)
   {

      ptr = encodeAtrAddress4(ptr, ReflectedFrom, msg.reflectedFrom);
   }
   if (msg.hasXorMappedAddress)
   {

      ptr = encodeAtrAddress4 (ptr, XorMappedAddress, msg.xorMappedAddress);
   }
   if (msg.xorOnly)
   {
      if (verbose) clog << "Encoding xorOnly: " << endl;
      ptr = encodeXorOnly( ptr );
   }
   if (msg.hasServerName)
   {

      ptr = encodeAtrString(ptr, ServerName, msg.serverName);
   }
   if (msg.hasSecondaryAddress)
   {

      ptr = encodeAtrAddress4 (ptr, SecondaryAddress, msg.secondaryAddress);
   }

   if (password.sizeValue > 0)
   {
      if (verbose) clog << "HMAC with password: " << password.value << endl;
		
      StunAtrIntegrity integrity;
      computeHmac(integrity.hash, buf, int(ptr-buf) , password.value, password.sizeValue);
      ptr = encodeAtrIntegrity(ptr, integrity);
   }
   if (verbose) clog << endl;
	
   encode16(lengthp, UInt16(ptr - buf - sizeof(StunMsgHdr)));
   return int(ptr - buf);
}

int 
stunRand()
{
   // return 32 bits of random stuff
   assert( sizeof(int) == 4 );
   static bool init=false;
   if ( !init )
   { 
      init = true;
		
      UInt64 tick;
		
#if defined(WIN32) 
      volatile unsigned int lowtick=0,hightick=0;
      __asm
         {
            rdtsc 
               mov lowtick, eax
               mov hightick, edx
               }
      tick = hightick;
      tick <<= 32;
      tick |= lowtick;
#elif defined(__GNUC__) && ( defined(__i686__) || defined(__i386__) )
      asm("rdtsc" : "=A" (tick));
#elif defined (__SUNPRO_CC) || defined( __sparc__ )	
      tick = gethrtime();
#elif defined(__MACH__)  || defined(__linux)
      int fd=open("/dev/random",O_RDONLY);
      read(fd,&tick,sizeof(tick));
      closesocket(fd);
#else
#     error Need some way to seed the random number generator 
#endif 
      int seed = int(tick);
#ifdef WIN32
      srand(seed);
#else
      srandom(seed);
#endif
   }
	
#ifdef WIN32
   assert( RAND_MAX == 0x7fff );
   int r1 = rand();
   int r2 = rand();
	
   int ret = (r1<<16) + r2;
	
   return ret;
#else
   return random(); 
#endif
}


/// return a random number to use as a port 
int
stunRandomPort()
{
   int min=0x4000;
   int max=0x7FFF;
	
   int ret = stunRand();
   ret = ret|min;
   ret = ret&max;
	
   return ret;
}


#ifdef NOSSL
static void
computeHmac(char* hmac, const char* input, int length, const char* key, int sizeKey)
{
   strncpy(hmac,"hmac-not-implemented",20);
}
#else
#include <openssl/hmac.h>

static void
computeHmac(char* hmac, const char* input, int length, const char* key, int sizeKey)
{
   unsigned int resultSize=0;
   HMAC(EVP_sha1(), 
        key, sizeKey, 
        reinterpret_cast<const unsigned char*>(input), length, 
        reinterpret_cast<unsigned char*>(hmac), &resultSize);
   assert(resultSize == 20);
}
#endif


static void
toHex(const char* buffer, int bufferSize, char* output) 
{
   static char hexmap[] = "0123456789abcdef";
	
   const char* p = buffer;
   char* r = output;
   for (int i=0; i < bufferSize; i++)
   {
      unsigned char temp = *p++;
		
      int hi = (temp & 0xf0)>>4;
      int low = (temp & 0xf);
		
      *r++ = hexmap[hi];
      *r++ = hexmap[low];
   }
   *r = 0;
}

void
stunCreateUserName(const StunAddress4& source, StunAtrString* username)
{
   UInt64 time = stunGetSystemTimeSecs();
   time -= (time % 20*60);
   //UInt64 hitime = time >> 32;
   UInt64 lotime = time & 0xFFFFFFFF;
	
   char buffer[1024];
   sprintf(buffer,
           "%08x:%08x:%08x:", 
           UInt32(source.addr),
           UInt32(stunRand()),
           UInt32(lotime));
   assert( strlen(buffer) < 1024 );
	
   assert(strlen(buffer) + 41 < STUN_MAX_STRING);
	
   char hmac[20];
   char key[] = "Jason";
   computeHmac(hmac, buffer, strlen(buffer), key, strlen(key) );
   char hmacHex[41];
   toHex(hmac, 20, hmacHex );
   hmacHex[40] =0;
	
   strcat(buffer,hmacHex);
	
   int l = strlen(buffer);
   assert( l+1 < STUN_MAX_STRING );
   assert( l%4 == 0 );
   
   username->sizeValue = l;
   memcpy(username->value,buffer,l);
   username->value[l]=0;
	
   //if (verbose) clog << "computed username=" << username.value << endl;
}

void
stunCreatePassword(const StunAtrString& username, StunAtrString* password)
{
   char hmac[20];
   char key[] = "Fluffy";
   //char buffer[STUN_MAX_STRING];
   computeHmac(hmac, username.value, strlen(username.value), key, strlen(key));
   toHex(hmac, 20, password->value);
   password->sizeValue = 40;
   password->value[40]=0;
	
   //clog << "password=" << password->value << endl;
}


UInt64
stunGetSystemTimeSecs()
{
   UInt64 time=0;
#if defined(WIN32)  
   SYSTEMTIME t;
   // CJ TODO - this probably has bug on wrap around every 24 hours
   GetSystemTime( &t );
   time = (t.wHour*60+t.wMinute)*60+t.wSecond; 
#else
   struct timeval now;
   gettimeofday( &now , NULL );
   //assert( now );
   time = now.tv_sec;
#endif
   return time;
}


ostream& operator<< ( ostream& strm, const UInt128& r )
{
   strm << int(r.octet[0]);
   for ( int i=1; i<16; i++ )
   {
      strm << ':' << int(r.octet[i]);
   }
    
   return strm;
}

ostream& 
operator<<( ostream& strm, const StunAddress4& addr)
{
   UInt32 ip = addr.addr;
   strm << ((int)(ip>>24)&0xFF) << ".";
   strm << ((int)(ip>>16)&0xFF) << ".";
   strm << ((int)(ip>> 8)&0xFF) << ".";
   strm << ((int)(ip>> 0)&0xFF) ;
	
   strm << ":" << addr.port;
	
   return strm;
}


// returns true if it scucceeded
bool 
stunParseHostName( char* peerName,
               UInt32& ip,
               UInt16& portVal,
               UInt16 defaultPort )
{
   in_addr sin_addr;
    
   char host[512];
   strncpy(host,peerName,512);
   host[512-1]='\0';
   char* port = NULL;
	
   int portNum = defaultPort;
	
   // pull out the port part if present.
   char* sep = strchr(host,':');
	
   if ( sep == NULL )
   {
      portNum = defaultPort;
   }
   else
   {
      *sep = '\0';
      port = sep + 1;
      // set port part
		
      char* endPtr=NULL;
		
      portNum = strtol(port,&endPtr,10);
		
      if ( endPtr != NULL )
      {
         if ( *endPtr != '\0' )
         {
            portNum = defaultPort;
         }
      }
   }
    
   if ( portNum < 1024 ) return false;
   if ( portNum >= 0xFFFF ) return false;
	
   // figure out the host part 
   struct hostent* h;
	
#ifdef WIN32
   assert( strlen(host) >= 1 );
   if ( isdigit( host[0] ) )
   {
      // assume it is a ip address 
      unsigned long a = inet_addr(host);
      //cerr << "a=0x" << hex << a << dec << endl;
		
      ip = ntohl( a );
   }
   else
   {
      // assume it is a host name 
      h = gethostbyname( host );
		
      if ( h == NULL )
      {
         int err = getErrno();
         std::cerr << "error was " << err << std::endl;
         assert( err != WSANOTINITIALISED );
			
         ip = ntohl( 0x7F000001L );
			
         return false;
      }
      else
      {
         sin_addr = *(struct in_addr*)h->h_addr;
         ip = ntohl( sin_addr.s_addr );
      }
   }
	
#else
   h = gethostbyname( host );
   if ( h == NULL )
   {
      int err = getErrno();
      std::cerr << "error was " << err << std::endl;
      ip = ntohl( 0x7F000001L );
      return false;
   }
   else
   {
      sin_addr = *(struct in_addr*)h->h_addr;
      ip = ntohl( sin_addr.s_addr );
   }
#endif
	
   portVal = portNum;
	
   return true;
}


bool
stunParseServerName( char* name, StunAddress4& addr)
{
   assert(name);
	
   // TODO - put in DNS SRV stuff.
	
   bool ret = stunParseHostName( name, addr.addr, addr.port, 3478); 
   if ( ret != true ) 
   {
       addr.port=0xFFFF;
   }	
   return ret;
}


static void
stunCreateErrorResponse(StunMessage& response, int cl, int number, const char* msg)
{
   response.msgHdr.msgType = BindErrorResponseMsg;
   response.hasErrorCode = true;
   response.errorCode.errorClass = cl;
   response.errorCode.number = number;
   strcpy(response.errorCode.reason, msg);
}

#if 0
static void
stunCreateSharedSecretErrorResponse(StunMessage& response, int cl, int number, const char* msg)
{
   response.msgHdr.msgType = SharedSecretErrorResponseMsg;
   response.hasErrorCode = true;
   response.errorCode.errorClass = cl;
   response.errorCode.number = number;
   strcpy(response.errorCode.reason, msg);
}
#endif

static void
stunCreateSharedSecretResponse(const StunMessage& request, const StunAddress4& source, StunMessage& response)
{
   response.msgHdr.msgType = SharedSecretResponseMsg;
   response.msgHdr.id = request.msgHdr.id;
	
   response.hasUsername = true;
   stunCreateUserName( source, &response.username);
	
   response.hasPassword = true;
   stunCreatePassword( response.username, &response.password);
}


int 
stunFindLocalInterfaces(UInt32* addresses,int maxRet)
{
#if defined(WIN32) || defined(__sparc__)
   return 0;
#else
   struct ifconf ifc;
	
   int s = socket( AF_INET, SOCK_DGRAM, 0 );
   int len = 100 * sizeof(struct ifreq);
	
   char buf[ len ];
	
   ifc.ifc_len = len;
   ifc.ifc_buf = buf;
	
   int e = ioctl(s,SIOCGIFCONF,&ifc);
   char *ptr = buf;
   int tl = ifc.ifc_len;
   int count=0;
	
   while ( (tl > 0) && ( count < maxRet) )
   {
      struct ifreq* ifr = (struct ifreq *)ptr;
		
      int si = sizeof(ifr->ifr_name) + sizeof(struct sockaddr);
      tl -= si;
      ptr += si;
      //char* name = ifr->ifr_ifrn.ifrn_name;
      //cerr << "name = " << name << endl;
		
      struct ifreq ifr2;
      ifr2 = *ifr;
		
      e = ioctl(s,SIOCGIFADDR,&ifr2);
      if ( e == -1 )
      {
         break;
      }
		
      //cerr << "ioctl addr e = " << e << endl;
		
      struct sockaddr a = ifr2.ifr_addr;
      struct sockaddr_in* addr = (struct sockaddr_in*) &a;
		
      UInt32 ai = ntohl( addr->sin_addr.s_addr );
      if (int((ai>>24)&0xFF) != 127)
      {
         addresses[count++] = ai;
      }
		
#if 0
      cerr << "Detected interface "
           << int((ai>>24)&0xFF) << "." 
           << int((ai>>16)&0xFF) << "." 
           << int((ai>> 8)&0xFF) << "." 
           << int((ai    )&0xFF) << endl;
#endif
   }
	
   closesocket(s);
	
   return count;
#endif
}


void
stunBuildReqSimple( StunMessage* msg,
                    const StunAtrString& username,
                    bool changePort, bool changeIp, unsigned int id )
{
   assert( msg );
   memset( msg , 0 , sizeof(*msg) );
	
   msg->msgHdr.msgType = BindRequestMsg;
	
   for ( int i=0; i<16; i=i+4 )
   {
      assert(i+3<16);
      int r = stunRand();
      msg->msgHdr.id.octet[i+0]= r>>0;
      msg->msgHdr.id.octet[i+1]= r>>8;
      msg->msgHdr.id.octet[i+2]= r>>16;
      msg->msgHdr.id.octet[i+3]= r>>24;
   }
	
   if ( id != 0 )
   {
      msg->msgHdr.id.octet[0] = id; 
   }
	
   msg->hasChangeRequest = true;
   msg->changeRequest.value =(changeIp?ChangeIpFlag:0) | 
      (changePort?ChangePortFlag:0);
	
   if ( username.sizeValue > 0 )
   {
      msg->hasUsername = true;
      msg->username = username;
   }
}


static void 
stunSendTest( Socket myFd, StunAddress4& dest, 
              const StunAtrString& username, const StunAtrString& password, 
              int testNum, bool verbose )
{ 
   assert( dest.addr != 0 );
   assert( dest.port != 0 );
	
   bool changePort=false;
   bool changeIP=false;
   bool discard=false;
	
   switch (testNum)
   {
      case 1:
      case 10:
      case 11:
         break;
      case 2:
         //changePort=true;
         changeIP=true;
         break;
      case 3:
         changePort=true;
         break;
      case 4:
         changeIP=true;
         break;
      case 5:
         discard=true;
         break;
      default:
         cerr << "Test " << testNum <<" is unkown\n";
         assert(0);
   }
	
   StunMessage req;
   memset(&req, 0, sizeof(StunMessage));
	
   stunBuildReqSimple( &req, username, 
                       changePort , changeIP , 
                       testNum );
	
   char buf[STUN_MAX_MESSAGE_SIZE];
   int len = STUN_MAX_MESSAGE_SIZE;
	
   len = stunEncodeMessage( req, buf, len, password,verbose );
	
   if ( verbose )
   {
      clog << "About to send msg of len " << len << " to " << dest << endl;
   }
	
   sendMessage( myFd, buf, len, dest.addr, dest.port, verbose );
	
   // add some delay so the packets don't get sent too quickly 
#ifdef WIN32 // !cj! TODO - should fix this up in windows
		 clock_t now = clock();
		 assert( CLOCKS_PER_SEC == 1000 );
		 while ( clock() <= now+10) { };
#else
		 usleep(10*1000);
#endif

}


void 
stunGetUserNameAndPassword(  const StunAddress4& dest, 
                             StunAtrString* username,
                             StunAtrString* password)
{ 
   // !cj! This is totally bogus - need to make TLS connection to dest and get a
   // username and password to use 
   stunCreateUserName(dest, username);
   stunCreatePassword(*username, password);
}


bool 
stunTest( StunAddress4& dest, int testNum, bool verbose, StunAddress4* sAddr )
{ 
   assert( dest.addr != 0 );
   assert( dest.port != 0 );
	
   int port = stunRandomPort();
   UInt32 interfaceIp=0;
   if (sAddr)
   {
      interfaceIp = sAddr->addr;
      if ( sAddr->port != 0 )
      {
        port = sAddr->port;
      }
   }
   Socket myFd = openPort(port,interfaceIp,verbose);
	
   StunAtrString username;
   StunAtrString password;
	
   username.sizeValue = 0;
   password.sizeValue = 0;
	
#ifdef USE_TLS
   stunGetUserNameAndPassword( dest, username, password );
#endif
	
   stunSendTest( myFd, dest, username, password, testNum, verbose );
    
   char msg[STUN_MAX_MESSAGE_SIZE];
   int msgLen = STUN_MAX_MESSAGE_SIZE;
   
   StunAddress4 from;
   bool ret = getMessage( myFd,
               msg,
               &msgLen,
               &from.addr,
               &from.port,verbose );

   if (!ret)
	   return false;
	
   StunMessage resp;
   memset(&resp, 0, sizeof(StunMessage));
	
   if ( verbose ) clog << "Got a response" << endl;
   bool ok = stunParseMessage( msg,msgLen, resp,verbose );

   if(!ok)
	   return false;
	
   if ( verbose )
   {
      clog << "\t ok=" << ok << endl;
      clog << "\t id=" << resp.msgHdr.id << endl;
      clog << "\t mappedAddr=" << resp.mappedAddress.ipv4 << endl;
      clog << "\t changedAddr=" << resp.changedAddress.ipv4 << endl;
      clog << endl;
   }
	
   if (sAddr)
   {
      sAddr->port = resp.mappedAddress.ipv4.port;
      sAddr->addr = resp.mappedAddress.ipv4.addr;
   }

   return true;
}


