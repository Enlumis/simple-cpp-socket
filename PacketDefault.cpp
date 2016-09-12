#include "PacketDefault.hh"

PacketDefault::PacketDefault() {
	this->_data.uchar_test = 56;
	this->_data.ushort_test = 1324;
	this->_data.uint_test = 2543624;
	this->_data.short_test = 456;
	this->_data.int_test = 423452;
}
PacketDefault::~PacketDefault() {
	
}
Packet::PacketID PacketDefault::getPacketID() {
	return Packet::DEFAULT;
}
size_t PacketDefault::getPacketLength() {
	return sizeof(t_packet_data_default);
}
void PacketDefault::serialize(char *dest) {
	my_strncpy(dest,(char*) &this->_data, sizeof(t_packet_data_default));
}
void PacketDefault::unserialize(char *src) {
	t_packet_data_default *data_rebuild = (t_packet_data_default*)src;
	this->_data = *data_rebuild;
}