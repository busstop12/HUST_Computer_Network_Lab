#if !defined(GBN_RDT_RECEIVER_H)
#define GBN_RDT_RECEIVER_H

#include "RdtReceiver.h"

class GBNRdtReceiver : public RdtReceiver
{
private:
  int seqNum;
  int expectSequenceNumberRcvd; // 期待收到的下一个报文序号
  Packet lastAckPkt;            //上次发送的确认报文

public:
  GBNRdtReceiver();
  virtual ~GBNRdtReceiver();

public:
  void receive(const Packet &packet);
};

#endif // GBN_RDT_RECEIVER_H
