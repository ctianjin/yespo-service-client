// $ package.cpp $
/*
 * Copyright (C) 2015 RAR Corporation
 * All rights reserved.
 *
 * Created: Nov 6, 2015
 * Contact: Development Department <kunkka.lee@yespo.com>
 * Author: Kunkka Lee <kunkka.lee@yespo.com>
 */

#include <WinSock.h>
#include "package.h"

namespace ns {

Package::Package()
{
	m_nPayloadOffset = 0;
	m_nDataLength = 0;
	m_nOuterHeaderLen = 0;
	m_nPayloadLen = 0;
	memset(m_cOuterHeader, '\0', 1024);
	memset(m_cPayload, '\0', YP_MAX_BUFFER);
}

Package::~Package()
{

}

INT64 Package::hton64(const INT64 & h64)
{
    INT64 n64;
    int *nh, *nw;
    int *hh, *hw;
    
    hh = (int*)&h64;
    hw = hh + 1;
    nh = (int*)&n64;
    nw = nh + 1;
    *nh = htonl( *hw );
    *nw = htonl( *hh );
    
    return n64;
}

INT64 Package::ntoh64(const INT64 & n64)
{
    INT64 h64;
    int *nh, *nw;
    int *hh, *hw;
    
    hh = (int*)&h64;
    hw = hh + 1;
    nh = (int*)&n64;
    nw = nh + 1;
    *hh = ntohl( *nw );
    *hw = ntohl( *nh );
    
    return h64;
}

INT32 Package::hton32(const INT32 & nSrc)
{
    return htonl( nSrc );
}

INT32 Package::ntoh32(const INT32 & nSrc)
{
    return ntohl( nSrc );
}

INT16 Package::hton16(const INT16 & sSrc)
{
    return htons( sSrc );
}

INT16 Package::ntoh16(const INT16 & sSrc)
{
    return ntohs( sSrc );
}

bool Package::ParsePack(const void* pBuffer, INT16 lBufferLen)
{
    const BYTE* pcBegin = (const BYTE*)pBuffer;
    const BYTE* p = pcBegin;

	if(p == NULL) {
	    return false;
	}

	INT16 payloadOffset;
	INT32 dataLength;

	if(lBufferLen < YP_PACK_HEADER_LEN) {
	    return false;
	}

	p += 2;  //Ignore 'Y' 'P'
	memcpy((void*)&payloadOffset, (void*)p, sizeof( INT16 ));
    p += sizeof( INT16 );
	memcpy((void*)&dataLength, (void*)p, sizeof( INT32 ));
    p += sizeof( INT32 );

	m_nPayloadOffset = Package::ntoh16(payloadOffset);
	m_nDataLength = Package::ntoh32(dataLength);
    m_nOuterHeaderLen = m_nPayloadOffset - YP_PACK_HEADER_LEN;
	m_nPayloadLen = m_nDataLength - m_nPayloadOffset;

	if(lBufferLen != m_nDataLength) {
	    return false;
	}

	if(m_nOuterHeaderLen > 0) {
	    memcpy(m_cOuterHeader, (void*)p, m_nOuterHeaderLen);
	    m_outer_header.ParseFromArray(m_cOuterHeader, m_nOuterHeaderLen);
	    p += m_nOuterHeaderLen;
	}

	if(m_nPayloadLen > 0) {
	    memcpy(m_cPayload, (void*)p, m_nPayloadLen);
	}
	
	return true;
}

INT16 Package::BuildPack(void* pBuffer)
{
    m_nDataLength = m_nPayloadOffset + m_nPayloadLen;
    if(m_nDataLength == 0) {
    	return m_nDataLength;
    }

    BYTE* pcBegin = (BYTE*)pBuffer;
    BYTE* p = pcBegin;

    INT16 payloadOffset = Package::hton16(m_nPayloadOffset);
    INT32 datalength = Package::hton32(m_nDataLength);

	*p = 'Y'; ++p;
	*p = 'P'; ++p;

	memcpy((void*)p, (const void*)&payloadOffset, sizeof( INT16 ));
    p += sizeof( INT16 );
    memcpy((void*)p, (const void*)&datalength, sizeof( INT32 ));
    p += sizeof( INT32 );

	if(m_nOuterHeaderLen > 0) {
	    memcpy(p, (void*)m_cOuterHeader, m_nOuterHeaderLen);
		p += m_nOuterHeaderLen;
	}

    if(m_nPayloadLen > 0) {
	    memcpy(p, (void*)m_cPayload, m_nPayloadLen);
		p += m_nPayloadLen;
	}

	return m_nDataLength;
}

INT16 Package::GetPayloadOffset() const
{
    return m_nPayloadOffset;
}

INT32 Package::GetPayloadLength() const
{
	return m_nPayloadLen;
}

INT32 Package::GetDataLength() const
{
    return m_nDataLength;
}

const BYTE* Package::GetOuterHeader() const
{
    return m_cOuterHeader;
}

const BYTE* Package::GetPayLoad() const
{
    return m_cPayload;
}

void Package::SetOuterHeader(const BYTE* header, INT32 len)
{
	m_nOuterHeaderLen= len;
    memcpy(m_cOuterHeader, header, len);
    m_outer_header.ParseFromArray(m_cOuterHeader, m_nOuterHeaderLen);

    m_nPayloadOffset = m_nOuterHeaderLen + YP_PACK_HEADER_LEN;
}

void Package::SetPayLoad(const BYTE* payload, INT32 len)
{    
    m_nPayloadLen = len;
    memcpy(m_cPayload, payload, len);
}

const yespo::Header& Package::OuterHeader()const
{
	return m_outer_header;
}

}