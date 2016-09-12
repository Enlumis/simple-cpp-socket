#include "Packet.hh"

Packet::Packet() {
	
}
Packet::~Packet() {
	
}
Packet::PacketID Packet::getPacketID() {
	return Packet::DEFAULT;
}
size_t Packet::getPacketLength() {
	return 0;
}
void Packet::serialize(char *dest) {
}
void Packet::unserialize(char *src) {
}