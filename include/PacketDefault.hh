#pragma once

#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "Packet.hh"

typedef struct			s_packet_data_default
{
  unsigned char		uchar_test;
  unsigned short	ushort_test;
  unsigned int		uint_test;
  short				short_test;
  int				int_test;
}__attribute__((__packed__))	t_packet_data_default;

class PacketDefault: public SCPPS::Packet
{
public:
	PacketDefault();
	~PacketDefault();

public:
	size_t getPacketLength();
	PacketID getPacketID();
	void serialize(char *dest);
	void unserialize(char *src);
	
public:
	t_packet_data_default _data;
};
