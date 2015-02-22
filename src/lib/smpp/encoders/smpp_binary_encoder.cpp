#include "smpp/encoders/smpp_binary_encoder.h"
#include "smpp/parameters/smpp_prameters_header.h"

namespace smpp {

namespace encoders {

using namespace pdu;

BinaryEncoder::BinaryEncoder() : data(0), data_len(0), pos(-1) {

}

BinaryEncoder::~BinaryEncoder() {
  if(data) {
    delete [] data;
    data = 0;
  }
}

void BinaryEncoder::clear() {
  resize(0);
}

void BinaryEncoder::resize(uint32 size) {
  if(data) {
    delete [] data;
  }

  if(size > 0) {
    data = new int8[size];
  } else {
    data = 0;
  }

  data_len = size;
  pos = 0;
}

const int8 *BinaryEncoder::getBuff() const {
  return data;
}

uint32 BinaryEncoder::getBuffLen() const {
  return data_len;
}

void BinaryEncoder::pack(uint8 p) {
  data[pos++] = p;
}

void BinaryEncoder::pack(uint16 p) {
  uint16 v(htons(p));
  memcpy(data + pos, &v, 2);
  pos += 2;
}

void BinaryEncoder::pack(uint32 p) {
  uint32 v(htonl(p));
  memcpy(data + pos, &v, 4);
  pos += 4;
}

void BinaryEncoder::pack(const int8 *p, uint32 p_len) {
  memcpy(data + pos, p, p_len);
  pos += p_len;
}

void BinaryEncoder::pack(const std::string &p) {
  pack(p.data(), p.size() + 1);
}

void BinaryEncoder::pack(const MultiDestinationAddresses &p) {
  std::list<MultiDestinationAddressBase *> list(p.getAll());

  for(std::list<MultiDestinationAddressBase *>::const_iterator it =
        list.begin();
      it != list.end();
      it++) {
    pack(*it);
  }
}

void BinaryEncoder::pack(const DistributionListAddress &p) {
  pack(p.getDestFlag());
  pack(p.getDlName());
}

void BinaryEncoder::pack(MultiDestinationAddressBase *p) {
  switch(p->getDestFlag()) {
    case 0x02:
      return pack(*reinterpret_cast<DistributionListAddress *>(p));
    case 0x01:
      return pack(*reinterpret_cast<SmeMultiAddress *>(p));
    default:
      break;
  }
}

void BinaryEncoder::pack(const SmeMultiAddress &p) {
  pack(p.getDestFlag());
  pack(p.getDestAddrTon());
  pack(p.getDestAddrNpi());
  pack(p.getDestinationAddr());
}

void BinaryEncoder::pack(const UnsuccessSme &p) {
  pack(p.getDestAddrTon());
  pack(p.getDestAddrNpi());
  pack(p.getDestinationAddr());
  pack(p.getErrorStatusCode());
}

void BinaryEncoder::pack(const Tlv &p) {
  pack(p.getTag());
  pack(p.getLength());
  pack(p.getValue());
}

void BinaryEncoder::pack(const TlvFields &p) {
  TlvList list(p.getList());

  for(TlvList::const_iterator it = list.begin();
      it != list.end();
      it++) {
    pack(**it);
  }
}

void BinaryEncoder::pack(const Header &p) {
  pack(p.getCommandLength());
  pack(p.getCommandId());
  pack(p.getCommandStatus());
  pack(p.getSequenceNumber());
}

void BinaryEncoder::pack(const BindTransmitter &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getSystemId());
  pack(p.getPassword());
  pack(p.getSystemType());
  pack(p.getInterfaceVersion());
  pack(p.getAddrTon());
  pack(p.getAddrNpi());
  pack(p.getAddressRange());
}

void BinaryEncoder::pack(const BindTransmitterResp &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getSystemId());
  pack((TlvFields)p);
}

void BinaryEncoder::pack(const BindReceiver &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getSystemId());
  pack(p.getPassword());
  pack(p.getSystemType());
  pack(p.getInterfaceVersion());
  pack(p.getAddrTon());
  pack(p.getAddrNpi());
  pack(p.getAddressRange());
}

void BinaryEncoder::pack(const BindReceiverResp &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getSystemId());
  pack((TlvFields)p);
}

void BinaryEncoder::pack(const BindTransceiver &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getSystemId());
  pack(p.getPassword());
  pack(p.getSystemType());
  pack(p.getInterfaceVersion());
  pack(p.getAddrTon());
  pack(p.getAddrNpi());
  pack(p.getAddressRange());
}

void BinaryEncoder::pack(const BindTransceiverResp &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getSystemId());
  pack((TlvFields)p);
}

void BinaryEncoder::pack(const Outbind &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getSystemId());
  pack(p.getPassword());
}

void BinaryEncoder::pack(const Unbind &p) {
  resize(Header::HEADER_LENGTH);
  pack((Header)p);
}

void BinaryEncoder::pack(const UnbindResp &p) {
  resize(Header::HEADER_LENGTH);
  pack((Header)p);
}

void BinaryEncoder::pack(const GenericNack &p) {
  resize(Header::HEADER_LENGTH);
  pack((Header)p);
}

void BinaryEncoder::pack(const SubmitSm &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getServiceType());
  pack(p.getSourceAddrTon());
  pack(p.getSourceAddrNpi());
  pack(p.getSourceAddr());
  pack(p.getDestAddrTon());
  pack(p.getDestAddrNpi());
  pack(p.getDestinationAddr());
  pack(p.getEsmClass());
  pack(p.getProtocolId());
  pack(p.getPriorityFlag());
  pack(p.getScheduleDeliveryTime());
  pack(p.getValidityPeriod());
  pack(p.getRegisteredDelivery());
  pack(p.getReplaceIfPresentFlag());
  pack(p.getDataCoding());
  pack(p.getSmDefaultMsgId());
  pack(p.getSmLength());
  pack(p.getShortMessage(), p.getSmLength());
  pack((TlvFields)p);
}

void BinaryEncoder::pack(const SubmitSmResp &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  if(p.getCommandStatus() == smpp::parameters::command_status::ESME_ROK)
    pack(p.getMessageId());
}

void BinaryEncoder::pack(const SubmitMulti &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getServiceType());
  pack(p.getSourceAddrTon());
  pack(p.getSourceAddrNpi());
  pack(p.getSourceAddr());
  pack(p.getNumberOfDests());
  pack(p.getDestAddresses());
  pack(p.getEsmClass());
  pack(p.getProtocolId());
  pack(p.getPriorityFlag());
  pack(p.getScheduleDeliveryTime());
  pack(p.getValidityPeriod());
  pack(p.getRegisteredDelivery());
  pack(p.getReplaceIfPresentFlag());
  pack(p.getDataCoding());
  pack(p.getSmDefaultMsgId());
  pack(p.getSmLength());
  pack(p.getShortMessage(), p.getSmLength());
  pack((TlvFields)p);
}

void BinaryEncoder::pack(const SubmitMultiResp &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getMessageId());
  pack(p.getNoUnsuccess());
  std::list<UnsuccessSme> list(p.getUnsuccessSmes());
  for(std::list<UnsuccessSme>::const_iterator it = list.begin();
      it != list.end();
      it++) {
    pack(*it);
  }
}

void BinaryEncoder::pack(const DeliverSm &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getServiceType());
  pack(p.getSourceAddrTon());
  pack(p.getSourceAddrNpi());
  pack(p.getSourceAddr());
  pack(p.getDestAddrTon());
  pack(p.getDestAddrNpi());
  pack(p.getDestinationAddr());
  pack(p.getEsmClass());
  pack(p.getProtocolId());
  pack(p.getPriorityFlag());
  pack(p.getScheduleDeliveryTime());
  pack(p.getValidityPeriod());
  pack(p.getRegisteredDelivery());
  pack(p.getReplaceIfPresentFlag());
  pack(p.getDataCoding());
  pack(p.getSmDefaultMsgId());
  pack(p.getSmLength());
  pack(p.getShortMessage(), p.getSmLength());
  pack((TlvFields)p);
}

void BinaryEncoder::pack(const DeliverSmResp &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getMessageId());
}

void BinaryEncoder::pack(const DataSm &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getServiceType());
  pack(p.getSourceAddrTon());
  pack(p.getSourceAddrNpi());
  pack(p.getSourceAddr());
  pack(p.getDestAddrTon());
  pack(p.getDestAddrNpi());
  pack(p.getDestinationAddr());
  pack(p.getEsmClass());
  pack(p.getRegisteredDelivery());
  pack(p.getDataCoding());
  pack((TlvFields)p);
}

void BinaryEncoder::pack(const DataSmResp &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getMessageId());
  pack((TlvFields)p);
}

void BinaryEncoder::pack(const QuerySm &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getMessageId());
  pack(p.getSourceAddrTon());
  pack(p.getSourceAddrNpi());
  pack(p.getSourceAddr());
}

void BinaryEncoder::pack(const QuerySmResp &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getMessageId());
  pack(p.getFinalDate());
  pack(p.getMessageState());
  pack(p.getErrorCode());
}

void BinaryEncoder::pack(const CancelSm &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getServiceType());
  pack(p.getMessageId());
  pack(p.getSourceAddrTon());
  pack(p.getSourceAddrNpi());
  pack(p.getSourceAddr());
  pack(p.getDestAddrTon());
  pack(p.getDestAddrNpi());
  pack(p.getDestinationAddr());
}

void BinaryEncoder::pack(const CancelSmResp &p) {
  resize(Header::HEADER_LENGTH);
  pack((Header)p);
}

void BinaryEncoder::pack(const ReplaceSm &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getMessageId());
  pack(p.getSourceAddrTon());
  pack(p.getSourceAddrNpi());
  pack(p.getSourceAddr());
  pack(p.getScheduleDeliveryTime());
  pack(p.getValidityPeriod());
  pack(p.getRegisteredDelivery());
  pack(p.getSmDefaultMsgId());
  pack(p.getSmLength());
  pack(p.getShortMessage(), p.getSmLength());
}

void BinaryEncoder::pack(const ReplaceSmResp &p) {
  resize(Header::HEADER_LENGTH);
  pack((Header)p);
}

void BinaryEncoder::pack(const EnquireLink &p) {
  resize(Header::HEADER_LENGTH);
  pack((Header)p);
}

void BinaryEncoder::pack(const EnquireLinkResp &p) {
  resize(Header::HEADER_LENGTH);
  pack((Header)p);
}

void BinaryEncoder::pack(const AlertNotification &p) {
  resize(p.getCommandLength());
  pack((Header)p);
  pack(p.getSourceAddrTon());
  pack(p.getSourceAddrNpi());
  pack(p.getSourceAddr());
  pack(p.getEsmeAddrTon());
  pack(p.getEsmeAddrNpi());
  pack(p.getEsmeAddr());
  pack((TlvFields)p);
}

} // namespace encoders

} // namespace smpp
