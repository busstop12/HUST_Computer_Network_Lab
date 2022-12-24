#if !defined(SR_RFT_SENDER_H)
#define SR_RFT_SENDER_H

#include "RdtSender.h"
#include <deque>

class SRRdtSender : public RdtSender
{
private:
  int seqNum;
  int windowLen;
  int base;
  int expectSeqNum;
  deque<pair<Packet, bool>> window;
  Packet lastSendPkt;
  ofstream fout;

public:
  bool getWaitingState();
  bool send(const Message &message);
  void receive(const Packet &ackPkt);
  void timeoutHandler(int seqNum);
  void printWindow();

public:
  SRRdtSender();
  virtual ~SRRdtSender();
};

#endif // SR_RFT_SENDER_H
