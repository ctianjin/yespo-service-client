// $ package.hpp $
/*
 * Copyright (C) 2015 RAR Corporation
 * All rights reserved.
 *
 * Created: Nov 6, 2015
 * Contact: Development Department <kunkka.lee@yespo.com>
 * Author: Kunkka Lee <kunkka.lee@yespo.com>
 */


#ifndef _PACKAGE_H
#define _PACKAGE_H

#include <BaseTsd.h>
#include <intsafe.h>

#include "yespo_protocol.pb.h"

#define YP_MAX_BUFFER              2048
#define YP_PACK_HEADER_LEN    8

namespace ns {

class Package
{
public:

	Package();
	~Package();

	bool ParsePack(const void* pBuffer, INT16 lBufferLen);
	INT16 BuildPack(void* pBuffer);

	INT16 GetPayloadOffset() const;
	INT32 GetPayloadLength() const;
	INT32 GetDataLength() const;
	const BYTE* GetOuterHeader() const;	
	const BYTE* GetPayLoad() const;		
	const yespo::Header& OuterHeader()const;
	void SetOuterHeader(const BYTE* header, INT32 len);
	void SetPayLoad(const BYTE* payload, INT32 len);	

public:
	INT64 hton64(const INT64 & h64);
	INT64 ntoh64(const INT64 & n64);
	INT32 hton32(const INT32 & nSrc);
	INT32 ntoh32(const INT32 & nSrc);
	INT16 hton16(const INT16 & sSrc);
	INT16 ntoh16(const INT16 & sSrc);

	INT16 m_nPayloadOffset;
	INT32 m_nDataLength;
	INT32 m_nOuterHeaderLen;
	INT32 m_nPayloadLen;
	BYTE  m_cOuterHeader[1024];
	BYTE  m_cPayload[YP_MAX_BUFFER];

	yespo::Header m_outer_header;

};

}

#endif
