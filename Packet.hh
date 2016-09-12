#ifndef PACKET_H_
# define PACKET_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "global.h"

class Packet
{
public:
  typedef enum                  PacketID
  {
    DEFAULT                   = 0
  }                             PacketID;
public:
  Packet();
  ~Packet();

public:
  virtual size_t getPacketLength();
  virtual Packet::PacketID getPacketID();
  virtual void serialize(char *dest);
  virtual void unserialize(char *src);
};

#endif
