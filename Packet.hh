#ifndef PACKET_H_
# define PACKET_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "global.h"

class Packet
{
public:
  Packet();
  ~Packet();

public:
  virtual size_t getPacketLength();
  virtual size_t getPacketID();
  virtual void serialize(char *dest);
  virtual void unserialize(char *src);
};

#endif
