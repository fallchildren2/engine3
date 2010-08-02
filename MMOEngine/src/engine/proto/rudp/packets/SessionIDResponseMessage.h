/*
Copyright (C) 2007 <SWGEmu>. All rights reserved.
Distribution of this file for usage outside of Core3 is prohibited.
*/

#ifndef SESSIONIDRESPONSEMESSAGE_H_
#define SESSIONIDRESPONSEMESSAGE_H_

#include "../BaseProtocol.h"

namespace engine {
  namespace proto {
    namespace rudp {

	class SessionIDResponseMessage : public Packet {
	public:
		SessionIDResponseMessage(BaseProtocol* prot) : Packet(17) {
			insertShort(0x0200);
			insertInt(prot->getConnectionID());
			insertInt(htonl(prot->getSeed()));
			insertByte(0x02);
			insertByte(0x01);
			insertByte(0x04);
			insertByte(0x00);
			insertByte(0x00);
			insertByte(0x01);
			insertByte(0xF0);
		}
	
		static unsigned int parse(Packet* pack) {
			return ntohl(pack->parseInt(6));  // return recieved crcSeed
		}
	};

    } // namespace rudp
  } // namespace proto
} // namespace engine

using namespace engine::proto::rudp;

#endif /*SESSIONIDRESPONSEMESSAGE_H_*/
