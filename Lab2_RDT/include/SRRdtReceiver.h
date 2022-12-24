#if !defined(SR_RDT_RECEIVER_H)
#define SR_RDT_RECEIVER_H

#include "RdtReceiver.h"
#include <deque>

class SRRdtReceiver : public RdtReceiver
{
private:
  int seqNum;
  int expectSequenceNumberRcvd; // 期待收到的下一个报文序号
  int windowLen;
  int base;
  Packet lastAckPkt; // 上次发送的确认报文
  deque<pair<Packet, bool>> window;
  ofstream fout;

public:
  SRRdtReceiver();
  virtual ~SRRdtReceiver();

public:
  void receive(const Packet &packet);
  void printWindow();
};

#endif // SR_RDT_RECEIVER_H
