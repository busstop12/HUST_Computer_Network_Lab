#include "Global.h"
#include "TCPRdtSender.h"

TCPRdtSender::TCPRdtSender()
    : seqNum(8), windowLen(4), base(0), expectSeqNum(0), count(0), fout("TCP_LOG.txt")
{

}

TCPRdtSender::~TCPRdtSender()
{
  fout.close();
}

bool TCPRdtSender::getWaitingState()
{
  // if the window is full
  return (window.size() == windowLen);
}

bool TCPRdtSender::send(const Message &message)
{
  if (this->getWaitingState())
  {
    return false;
  }

  // init the packet
  this->lastSendPkt.acknum = -1;
  this->lastSendPkt.seqnum = expectSeqNum;
  this->lastSendPkt.checksum = 0;
  memcpy(this->lastSendPkt.payload, message.data, sizeof(message.data));
  this->lastSendPkt.checksum = pUtils->calculateCheckSum(this->lastSendPkt);

  // add the packet to window
  window.push_back(lastSendPkt);

  // log
  pUtils->printPacket("The sender send: ", this->lastSendPkt);
  printf("The packet sequence number: %d\n", this->lastSendPkt.seqnum);

  // start the timer
  if (this->base == this->expectSeqNum)
  {
    pns->startTimer(SENDER, Configuration::TIME_OUT, this->lastSendPkt.seqnum);
  }

  // send to netwokt layer
  pns->sendToNetworkLayer(RECEIVER, this->lastSendPkt);

  // update expectSeqNum
  this->expectSeqNum = (this->expectSeqNum + 1) % this->seqNum;

  // log
  this->printWindow();

  return true;
}

void TCPRdtSender::receive(const Packet &ackPkt)
{
  // check the ackPkt
  int checksum = pUtils->calculateCheckSum(ackPkt);
  if (checksum == ackPkt.checksum)
  {
    if (ackPkt.acknum != (this->base + this->seqNum - 1) % this->seqNum)
    {
      // The packet is not confirmed

      pUtils->printPacket("The sender received ACK packet", ackPkt);

      // update the window and timer
      while (this->base != (ackPkt.acknum + 1) % this->seqNum)
      {
        pns->stopTimer(SENDER, this->base);
        window.pop_front();
        this->base = (this->base + 1) % this->seqNum;
      }

      // restart the timer for the first packet
      if (window.size())
      {
        pns->startTimer(SENDER, Configuration::TIME_OUT, window.front().seqnum);
      }
    }
    else
    {
      // the packet has already confirmed
      pUtils->printPacket("The packet has already confirmed: ", ackPkt);

      this->count++;
      if (this->count == 3 && window.size())
      {
        pns->sendToNetworkLayer(RECEIVER, window.front());
        this->count = 0;
        // printf("快速重传: %d\n", window.front().seqnum);
        this->fout << "快速重传: " << window.front().seqnum << endl;
      }
    }
  }
  else
  {
    pUtils->printPacket("Packet corrupt: ", ackPkt);
  }

  // log
  this->printWindow();
}

void TCPRdtSender::timeoutHandler(int seqNum)
{
  printf("超时重传\n");

  pns->stopTimer(SENDER, window.front().seqnum);
  pns->startTimer(SENDER, Configuration::TIME_OUT, window.front().seqnum);

  pns->sendToNetworkLayer(RECEIVER, window.front());
}

void TCPRdtSender::printWindow()
{
  printf("* Sender: The sliding window: base: %d, next: %d\n", this->base, this->expectSeqNum);

  for (auto it = window.begin(); it != window.end(); it++)
  {
    printf("> seqNum: %d\n", it->seqnum);
  }
}