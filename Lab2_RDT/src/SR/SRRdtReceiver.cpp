#include "Global.h"
#include "SRRdtReceiver.h"

SRRdtReceiver::SRRdtReceiver() : expectSequenceNumberRcvd(0), windowLen(4), base(0), seqNum(8), fout("SR_RECEIVER_WINDOW.txt")
{
  lastAckPkt.acknum = 7;
  lastAckPkt.checksum = 0;
  lastAckPkt.seqnum = -1;
  for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++)
  {
    lastAckPkt.payload[i] = '.';
  }
  lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
  // init the window
  for (int i = 0; i < windowLen; i++)
  {
    Packet p = Packet();
    p.seqnum = -1;
    window.push_back(make_pair(p, false));
  }
}

SRRdtReceiver::~SRRdtReceiver()
{
}

void SRRdtReceiver::receive(const Packet &packet)
{
  // check the packet
  int checksum = pUtils->calculateCheckSum(packet);
  // calculate the positio of the packet
  int offset = (packet.seqnum - this->base + this->seqNum) % this->seqNum;

  if (checksum == packet.checksum && offset < window.size() && !window.at(offset).second)
  {
    // save the packet
    window.at(offset).first = packet;
    window.at(offset).second = true;
    pUtils->printPacket("The receiver receives: ", packet);

    // send ACK
    lastAckPkt.acknum = packet.seqnum;
    lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
    pUtils->printPacket("The receiver send ACK", lastAckPkt);
    pns->sendToNetworkLayer(SENDER, lastAckPkt);

    // update the sliding window
    while (window.front().second == true)
    {
      // deliver the msg to application layer
      Message msg;
      memcpy(msg.data, window.front().first.payload, sizeof(window.front().first.payload));
      pns->delivertoAppLayer(RECEIVER, msg);

      // update next ACK number
      this->base = (this->base + 1) % this->seqNum;
      this->expectSequenceNumberRcvd = (this->expectSequenceNumberRcvd + 1) % this->seqNum;

      // update window
      window.pop_front();
      Packet p = Packet();
      p.seqnum = -1;
      window.push_back(make_pair(p, false));
    }
  }
  else
  {
    if (checksum != packet.checksum)
    {
      pUtils->printPacket("Packet corrupted: ", packet);
    }
    else
    {
      pUtils->printPacket("The receiver has already received: ", packet);
      lastAckPkt.acknum = packet.seqnum;
      lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);

      // send the ACK again
      pUtils->printPacket("The receiver send ACK again: ", lastAckPkt);
      pns->sendToNetworkLayer(SENDER, lastAckPkt);
    }
  }

  // log
  this->printWindow();
}

void SRRdtReceiver::printWindow()
{
  printf("* Receiver: The sliding window: base: %d, next sequence: %d\n", this->base, this->expectSequenceNumberRcvd);
  fout << "base: " << base << ", next: " << expectSequenceNumberRcvd << endl;

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
    fout << "(" << it->second << ", " << it->first.seqnum << "), ";
  }
  fout << endl << endl;
}