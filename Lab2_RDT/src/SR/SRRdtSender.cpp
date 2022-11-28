#include "Global.h"
#include "SRRdtSender.h"

SRRdtSender::SRRdtSender() : seqNum(8), windowLen(4), base(0), expectSeqNum(0)
{
}

SRRdtSender::~SRRdtSender()
{
}

bool SRRdtSender::getWaitingState()
{
  // if the window is full
  return (window.size() == windowLen);
}

bool SRRdtSender::send(const Message &message)
{
  if (this->getWaitingState())
  {
    return false;
  }

  // init the packet
  this->lastSendPkt.acknum = -1;
  this->lastSendPkt.seqnum = this->expectSeqNum;
  this->lastSendPkt.checksum = 0;
  memcpy(this->lastSendPkt.payload, message.data, sizeof(message.data));
  this->lastSendPkt.checksum = pUtils->calculateCheckSum(this->lastSendPkt);

  // add the packet to window
  window.push_back(make_pair(this->lastSendPkt, false));

  pUtils->printPacket("The sender send: ", this->lastSendPkt);

  // start the timer
  pns->startTimer(SENDER, Configuration::TIME_OUT, this->lastSendPkt.seqnum);

  // send to network layer
  pns->sendToNetworkLayer(RECEIVER, this->lastSendPkt);

  // update expectSeqNum
  this->expectSeqNum = (this->expectSeqNum + 1) % this->seqNum;

  // log
  this->printWindow();

  return true;
}

void SRRdtSender::receive(const Packet &ackPkt)
{
  // check the ackPkt
  int checksum = pUtils->calculateCheckSum(ackPkt);

  // calculate the position of the ackPkt
  int offset = (ackPkt.acknum - this->base + this->seqNum) % this->seqNum;

  if (checksum == ackPkt.checksum && offset < window.size() && !window.at(offset).second)
  {
    // The packet has never been confirmed
    // mark the packet in the window
    window.at(offset).second = true;
    // stop the timer for the packet
    pns->stopTimer(SENDER, ackPkt.acknum);
    pUtils->printPacket("The sender receives: ", ackPkt);

    while (window.size() != 0 && window.front().second == true)
    {
      window.pop_front();
      this->base = (this->base + 1) % this->seqNum;
    }
  }

  // log
  this->printWindow();
}

void SRRdtSender::timeoutHandler(int seqNum)
{
  // stop the timer
  pns->stopTimer(SENDER, seqNum);
  // calculate the position of the packet
  int offset = (seqNum - this->base + this->seqNum) % this->seqNum;

  if (offset < window.size())
  {
    pUtils->printPacket("Timeout, send packet again: ", window.at(offset).first);
    // restart the timer
    pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
    pns->sendToNetworkLayer(RECEIVER, window.at(offset).first);
  }
  else
  {
    pUtils->printPacket("Timeout, the packet has been confirmed: ", window.at(offset).first);
  }
}

void SRRdtSender::printWindow()
{
  printf("* Sender: The sliding window: base: %d, next sequence: %d\n", this->base, this->expectSeqNum);
  for (auto it = window.begin(); it != window.end(); it++)
  {
    printf("> status: ");
    if (it->second)
    {
      printf("true");
    }
    else
    {
      printf("false");
    }
    printf(", sequence: %d\n", it->first.seqnum);
  }
}