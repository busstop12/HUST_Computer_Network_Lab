#if !defined(TCP_RDT_SENDER_H)
#define TCP_RDT_SENDER_H

#include "RdtSender.h"
#include <deque>
#include <fstream>

using namespace std;

class TCPRdtSender : public RdtSender
{
private:
  int seqNum;
  int windowLen;
  int base;
  int expectSeqNum;
  deque<Packet> window;
  Packet lastSendPkt;
  int count;
  ofstream fout;

public:
  bool getWaitingState();
  bool send(const Message &message);
  void receive(const Packet &ackPkt);
  void timeoutHandler(int seqNum);
  void printWindow();

public:
  TCPRdtSender();
  virtual ~TCPRdtSender();
};

#endif // TCP_RDT_SENDER_H
