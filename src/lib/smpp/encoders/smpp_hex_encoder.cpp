#include "smpp/encoders/smpp_hex_encoder.h"
#include "smpp/parameters/smpp_prameters_header.h"

namespace smpp {

namespace encoders {

using namespace pdu;

const int8 HexEncoder::hex[] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'A', 'B', 'C', 'D', 'E', 'F'
};

HexEncoder::HexEncoder() {

}

void HexEncoder::clear() {
  data.clear();
}

std::string HexEncoder::getBuff() const {
  return data;
}

std::string HexEncoder::byte_2_str(const int8 *bytes, uint32 size) {
  std::string str;
  for(uint32 i = 0; i < size; i++) {
    const int8 ch = bytes[i];
    str.push_back(hex[(ch & 0xF0) >> 4]);
    str.push_back(hex[ch & 0xF]);
  }
  return str;
}

void HexEncoder::pack(const int8 *p, uint32 p_len) {
  if(p_len > 0)
    data.append(byte_2_str(p, p_len) + " ");
}

void HexEncoder::pack(uint8 p) {
  uint8 v(p);
  int8 *c_pt(reinterpret_cast<int8 *>(&v));
  data += byte_2_str(c_pt, 1) + " ";
}

void HexEncoder::pack(uint16 p) {
  uint16 v(htons(p));
  int8 *c_pt(reinterpret_cast<int8 *>(&v));
  data += byte_2_str(c_pt, 2) + " ";
}

void HexEncoder::pack(uint32 p) {
  uint32 v(htonl(p));
  int8 *c_pt(reinterpret_cast<int8 *>(&v));
  data += byte_2_str(c_pt, 4) + " ";
}

void HexEncoder::pack(const std::string &p) {
  pack(p.data(), p.size() + 1);
}

void HexEncoder::pack(const MultiDestinationAddresses &p) {
  std::list<MultiDestinationAddressBase *> list(p.getAll());

  for(std::list<MultiDestinationAddressBase *>::const_iterator it =
        list.begin();
      it != list.end();
      it++) {
    pack(*it);
  }
}

void HexEncoder::pack(const DistributionListAddress &p) {
  pack(p.getDestFlag());
  pack(p.getDlName());
}

void HexEncoder::pack(MultiDestinationAddressBase *p) {
  switch(p->getDestFlag()) {
    case 0x02:
      return pack(*reinterpret_cast<DistributionListAddress *>(p));
    case 0x01:
      return pack(*reinterpret_cast<SmeMultiAddress *>(p));
    default:
      break;
  }
}

void HexEncoder::pack(const SmeMultiAddress &p) {
  pack(p.getDestFlag());
  pack(p.getDestAddrTon());
  pack(p.getDestAddrNpi());
  pack(p.getDestinationAddr());
}

void HexEncoder::pack(const UnsuccessSme &p) {
  pack(p.getDestAddrTon());
  pack(p.getDestAddrNpi());
  pack(p.getDestinationAddr());
  pack(p.getErrorStatusCode());
}

void HexEncoder::pack(const Tlv &p) {
  pack(p.getTag());
  pack(p.getLength());
  pack(p.getValue());
}

void HexEncoder::pack(const TlvFields &p) {
  TlvList list(p.getList());

  for(TlvList::const_iterator it = list.begin();
      it != list.end();
      it++) {
    pack(**it);
  }
}

void HexEncoder::pack(const Header &p) {
  pack(p.getCommandLength());
  pack(p.getCommandId());
  pack(p.getCommandStatus());
  pack(p.getSequenceNumber());
}

void HexEncoder::pack(const BindTransmitter &p) {
  pack((Header)p);
  pack(p.getSystemId());
  pack(p.getPassword());
  pack(p.getSystemType());
  pack(p.getInterfaceVersion());
  pack(p.getAddrTon());
  pack(p.getAddrNpi());
  pack(p.getAddressRange());
}

void HexEncoder::pack(const BindTransmitterResp &p) {
  pack((Header)p);
  pack(p.getSystemId());
  pack((TlvFields)p);
}

void HexEncoder::pack(const BindReceiver &p) {
  pack((Header)p);
  pack(p.getSystemId());
  pack(p.getPassword());
  pack(p.getSystemType());
  pack(p.getInterfaceVersion());
  pack(p.getAddrTon());
  pack(p.getAddrNpi());
  pack(p.getAddressRange());
}

void HexEncoder::pack(const BindReceiverResp &p) {
  pack((Header)p);
  pack(p.getSystemId());
  pack((TlvFields)p);
}

void HexEncoder::pack(const BindTransceiver &p) {
  pack((Header)p);
  pack(p.getSystemId());
  pack(p.getPassword());
  pack(p.getSystemType());
  pack(p.getInterfaceVersion());
  pack(p.getAddrTon());
  pack(p.getAddrNpi());
  pack(p.getAddressRange());
}

void HexEncoder::pack(const BindTransceiverResp &p) {
  pack((Header)p);
  pack(p.getSystemId());
  pack((TlvFields)p);
}

void HexEncoder::pack(const Outbind &p) {
  pack((Header)p);
  pack(p.getSystemId());
  pack(p.getPassword());
}

void HexEncoder::pack(const Unbind &p) {
  pack((Header)p);
}

void HexEncoder::pack(const UnbindResp &p) {
  pack((Header)p);
}

void HexEncoder::pack(const GenericNack &p) {
  pack((Header)p);
}

void HexEncoder::pack(const SubmitSm &p) {
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

void HexEncoder::pack(const SubmitSmResp &p) {
  pack((Header)p);
  if(p.getCommandStatus() == smpp::parameters::command_status::ESME_ROK)
    pack(p.getMessageId());
}

void HexEncoder::pack(const SubmitMulti &p) {
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

void HexEncoder::pack(const SubmitMultiResp &p) {
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

void HexEncoder::pack(const DeliverSm &p) {
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

void HexEncoder::pack(const DeliverSmResp &p) {
  pack((Header)p);
  pack(p.getMessageId());
}

void HexEncoder::pack(const DataSm &p) {
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

void HexEncoder::pack(const DataSmResp &p) {
  pack((Header)p);
  pack(p.getMessageId());
  pack((TlvFields)p);
}

void HexEncoder::pack(const QuerySm &p) {
  pack((Header)p);
  pack(p.getMessageId());
  pack(p.getSourceAddrTon());
  pack(p.getSourceAddrNpi());
  pack(p.getSourceAddr());
}

void HexEncoder::pack(const QuerySmResp &p) {
  pack((Header)p);
  pack(p.getMessageId());
  pack(p.getFinalDate());
  pack(p.getMessageState());
  pack(p.getErrorCode());
}

void HexEncoder::pack(const CancelSm &p) {
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

void HexEncoder::pack(const CancelSmResp &p) {
  pack((Header)p);
}

void HexEncoder::pack(const ReplaceSm &p) {
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

void HexEncoder::pack(const ReplaceSmResp &p) {
  pack((Header)p);
}

void HexEncoder::pack(const EnquireLink &p) {
  pack((Header)p);
}

void HexEncoder::pack(const EnquireLinkResp &p) {
  pack((Header)p);
}

void HexEncoder::pack(const AlertNotification &p) {
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

