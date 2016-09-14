#include "Packet.hh"

namespace SCPPS
{
Packet::Packet() {
	
}
Packet::~Packet() {
	
}
PacketID Packet::getPacketID() {
	return PACKET_DEFAULT;
}
size_t Packet::getPacketLength() {
	return 0;
}
void Packet::serialize(char *dest) {
	(void)dest;
}
void Packet::unserialize(char *src) {
	(void)src;
}
}
