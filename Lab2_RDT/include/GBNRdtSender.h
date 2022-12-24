#if !defined(GBN_RDT_SENDER_H)
#define GBN_RDT_SENDER_H

#include "RdtSender.h"
#include <deque>

class GBNRdtSender : public RdtSender
{
private:
  int seqNum;
  int windowLen;
  int base;
  int expectSeqNum;
  deque<Packet> window;
  Packet lastSendPkt;
  ofstream fout;

public:
  bool getWaitingState();
  bool send(const Message &message);
  void receive(const Packet &ackPkt);
  void timeoutHandler(int seqNum);
  void printWindow();

public:
  GBNRdtSender();
  virtual ~GBNRdtSender();
};

#endif // GBN_RDT_SENDER_H
