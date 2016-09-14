#ifndef PACKET_H_
# define PACKET_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstring>

#include "global.h"
#include "PacketID.hh"

namespace SCPPS
{
class Packet
{
public:
  Packet();
  ~Packet();

public:
  virtual size_t getPacketLength();
  virtual PacketID getPacketID();
  virtual void serialize(char *dest);
  virtual void unserialize(char *src);
};
}

#endif
