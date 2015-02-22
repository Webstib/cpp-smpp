#include "smpp/decoders/smpp_binary_decoder.h"
#include "smpp/parameters/smpp_prameters_header.h"

namespace smpp {

namespace decoders {

using namespace pdu;

void BinaryDecoder::renewData(uint8 len) {
  if(data) {
    delete [] data;
  }

  if(len > 0) {
    data = new int8[len];
  } else {
    data = 0;
  }
}

BinaryDecoder::BinaryDecoder(uint32 data_len) :
  data(0), data_len(data_len), pos(0) {
  if(data_len != 0) {
    renewData(data_len);
  }
}

BinaryDecoder::~BinaryDecoder() {
  renewData(0);
}

void BinaryDecoder::clear() {
  renewData(0);
  data_len = 0;
  pos = 0;
}

void BinaryDecoder::addData(uint32 len) {
  if(len != 0) {
    int8 *tmp(0);
    if(this->data) {
      tmp = new int8[this->data_len];
      memcpy(tmp, this->data, this->data_len);
    }
    renewData(this->data_len + len);
    if(tmp) {
      memcpy(this->data, tmp, this->data_len);
      delete [] tmp;
    }
    this->data_len += len;
  }
}

int8 *BinaryDecoder::header() const {
  return data;
}

int8 *BinaryDecoder::body() const {
  if(Header::HEADER_LENGTH < data_len) {
    return data + Header::HEADER_LENGTH;
  } else {
    return 0;
  }
}

uint32 BinaryDecoder::bodyLength() const {
  if(Header::HEADER_LENGTH < data_len) {
    return data_len - Header::HEADER_LENGTH;
  } else {
    return 0;
  }
}

bool BinaryDecoder::unpack(uint8 &p) {
  if(pos + 1 > data_len)
    return false;

  p = data[pos++];

  return true;
}

bool BinaryDecoder::unpack(uint16 &p) {
  if(pos + 2 > data_len)
    return false;

  uint16 v(0);
  memcpy(&v, data + pos, 2);
  pos += 2;
  p = ntohs(v);

  return true;
}

bool BinaryDecoder::unpack(uint32 &p) {
  if(pos + 4 > data_len) return false;

  uint32 v(0);
  memcpy(&v, data + pos, 4);
  pos += 4;
  p = ntohl(v);

  return true;
}

bool BinaryDecoder::unpack(int8 *&p, uint32 length) {
  if((pos + length) > data_len)
    return false;

  if(length > 0) {
    p = new int8[length];
    memcpy(p, data + pos, length);
    pos += length;
  } else {
    p = 0;
  }

  return true;
}

bool BinaryDecoder::unpack(std::string &p) {
  uint32 temp_pos(pos);
  while(1) {
    if(temp_pos >= data_len)
      return false;
    if(data[temp_pos++] == '\0')
      break;
  }
  p.clear();
  temp_pos -= pos + 1;
  p.resize(temp_pos);
  for(uint32 i = 0; i < temp_pos; i++)
    p[i] = data[pos++];
  pos++;
  return true;
}

bool BinaryDecoder::unpack(smpp::pdu::SmeMultiAddress &p) {
  uint8 ton;
  bool ok(unpack(ton));
  if(!ok) return ok;

  uint8 npi;
  ok = unpack(npi);
  if(!ok) return ok;

  std::string addr;
  ok = unpack(addr);
  if(!ok) return ok;

  p.setDestAddrTon(ton);
  p.setDestAddrNpi(npi);
  p.setDestinationAddr(addr);

  return ok;
}

bool BinaryDecoder::unpack(smpp::pdu::DistributionListAddress &p) {
  std::string dl_name;
  if(!unpack(dl_name))
    return false;

  p.setDlName(dl_name);

  return true;
}

bool BinaryDecoder::unpack(MultiDestinationAddresses &p, uint8 length) {
  bool ok(true);

  for(uint8 i = 0; i < length; i++) {
    uint8 dest_flag;
    bool ok(unpack(dest_flag));

    if(!ok) return ok;

    switch(dest_flag) {
      case 0x02: {
        SmeMultiAddress sme_multi_addr;

        ok = unpack(sme_multi_addr);

        if(!ok) return ok;

        p.add(sme_multi_addr);
        break;
      }
      case 0x01: {
        DistributionListAddress distrib_addr;

        ok = unpack(distrib_addr);

        if(!ok) return ok;

        p.add(distrib_addr);
        break;
      }
      default:
        return false;
    }
  }

  return ok;
}

bool BinaryDecoder::unpack(UnsuccessSme &p) {
  uint8 ton;
  bool ok(unpack(ton));
  if(!ok) return ok;

  uint8 npi;
  ok = unpack(npi);
  if(!ok) return ok;

  std::string addr;
  ok = unpack(addr);
  if(!ok) return ok;

  uint32 error_status_code;
  ok = unpack(error_status_code);
  if(!ok) return ok;

  p.setDestAddrTon(ton);
  p.setDestAddrNpi(npi);
  p.setDestinationAddr(addr);
  p.setErrorStatusCode(error_status_code);

  return ok;
}

bool BinaryDecoder::unpack(Tlv &p) {
  uint16 tag;
  bool ok(unpack(tag));

  if(!ok) return ok;

  uint16 length;

  ok = unpack(length);

  if(!ok) return ok;

  int8 *value(0);

  ok = unpack(value, length);

  if(!ok) return ok;

  p.setTag(tag);
  p.setValue(value, length);

  delete [] value;

  return ok;
}

bool BinaryDecoder::unpack(TlvFields &p) {
  bool ok(true);

  p.clear();

  while(pos < data_len) {
    Tlv tlv;
    ok = unpack(tlv);
    if(!ok) return ok;
    p.add(tlv);
  }

  return ok;
}

bool BinaryDecoder::unpack(Header &p) {
  uint32 command_length;
  bool ok(unpack(command_length));

  if(!ok) return ok;

  uint32 command_id;

  ok = unpack(command_id);

  if(!ok) return ok;

  uint32 command_status;

  ok = unpack(command_status);

  if(!ok) return ok;

  uint32 sequence_number;

  ok = unpack(sequence_number);

  if(!ok) return ok;

  p.setCommandLength(command_length);
  p.setCommandId(command_id);
  p.setCommandStatus(command_status);
  p.setSequenceNumber(sequence_number);

  return ok;
}

bool BinaryDecoder::unpack(BindTransmitter &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string system_id;
  ok = unpack(system_id);
  if(!ok) return ok;

  std::string password;
  ok = unpack(password);
  if(!ok) return ok;

  std::string system_type;
  ok = unpack(system_type);
  if(!ok) return ok;

  uint8 interface_version;
  ok = unpack(interface_version);
  if(!ok) return ok;

  uint8 addr_ton;
  ok = unpack(addr_ton);
  if(!ok) return ok;

  uint8 addr_npi;
  ok = unpack(addr_npi);
  if(!ok) return ok;

  std::string address_range;
  ok = unpack(address_range);
  if(!ok) return ok;

  p.setSystemId(system_id);
  p.setPassword(password);
  p.setSystemType(system_type);
  p.setInterfaceVersion(interface_version);
  p.setAddrTon(addr_ton);
  p.setAddrNpi(addr_npi);
  p.setAddressRange(address_range);

  return ok;
}

bool BinaryDecoder::unpack(BindTransmitterResp &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string system_id;
  ok = unpack(system_id);
  if(!ok) return ok;

  p.setSystemId(system_id);

  ok = unpack((TlvFields &)p);

  return ok;
}

bool BinaryDecoder::unpack(BindReceiver &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string system_id;
  ok = unpack(system_id);
  if(!ok) return ok;

  std::string password;
  ok = unpack(password);
  if(!ok) return ok;

  std::string system_type;
  ok = unpack(system_type);
  if(!ok) return ok;

  uint8 interface_version;
  ok = unpack(interface_version);
  if(!ok) return ok;

  uint8 addr_ton;
  ok = unpack(addr_ton);
  if(!ok) return ok;

  uint8 addr_npi;
  ok = unpack(addr_npi);
  if(!ok) return ok;

  std::string address_range;
  ok = unpack(address_range);
  if(!ok) return ok;

  p.setSystemId(system_id);
  p.setPassword(password);
  p.setSystemType(system_type);
  p.setInterfaceVersion(interface_version);
  p.setAddrTon(addr_ton);
  p.setAddrNpi(addr_npi);
  p.setAddressRange(address_range);

  return ok;
}

bool BinaryDecoder::unpack(BindReceiverResp &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string system_id;
  ok = unpack(system_id);
  if(!ok) return ok;

  p.setSystemId(system_id);

  ok = unpack((TlvFields &)p);

  return ok;
}

bool BinaryDecoder::unpack(BindTransceiver &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string system_id;
  ok = unpack(system_id);
  if(!ok) return ok;

  std::string password;
  ok = unpack(password);
  if(!ok) return ok;

  std::string system_type;
  ok = unpack(system_type);
  if(!ok) return ok;

  uint8 interface_version;
  ok = unpack(interface_version);
  if(!ok) return ok;

  uint8 addr_ton;
  ok = unpack(addr_ton);
  if(!ok) return ok;

  uint8 addr_npi;
  ok = unpack(addr_npi);
  if(!ok) return ok;

  std::string address_range;
  ok = unpack(address_range);
  if(!ok) return ok;

  p.setSystemId(system_id);
  p.setPassword(password);
  p.setSystemType(system_type);
  p.setInterfaceVersion(interface_version);
  p.setAddrTon(addr_ton);
  p.setAddrNpi(addr_npi);
  p.setAddressRange(address_range);

  return ok;
}

bool BinaryDecoder::unpack(BindTransceiverResp &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string system_id;
  ok = unpack(system_id);
  if(!ok) return ok;

  p.setSystemId(system_id);

  ok = unpack((TlvFields &)p);

  return ok;
}

bool BinaryDecoder::unpack(SubmitSm &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string service_type;
  ok = unpack(service_type);
  if(!ok) return ok;

  uint8 source_addr_ton;
  ok = unpack(source_addr_ton);
  if(!ok) return ok;

  uint8 source_addr_npi;
  ok = unpack(source_addr_npi);
  if(!ok) return ok;

  std::string source_addr;
  ok = unpack(source_addr);
  if(!ok) return ok;

  uint8 dest_addr_ton;
  ok = unpack(dest_addr_ton);
  if(!ok) return ok;

  uint8 dest_addr_npi;
  ok = unpack(dest_addr_npi);
  if(!ok) return ok;

  std::string dest_addr;
  ok = unpack(dest_addr);
  if(!ok) return ok;

  uint8 esm_class;
  ok = unpack(esm_class);
  if(!ok) return ok;

  uint8 protocol_id;
  ok = unpack(protocol_id);
  if(!ok) return ok;

  uint8 priority_flag;
  ok = unpack(priority_flag);
  if(!ok) return ok;

  std::string schedule_delivery_time;
  ok = unpack(schedule_delivery_time);
  if(!ok) return ok;

  std::string validity_period;
  ok = unpack(validity_period);
  if(!ok) return ok;

  uint8 registered_delivery;
  ok = unpack(registered_delivery);
  if(!ok) return ok;

  uint8 replace_if_present_flag;
  ok = unpack(replace_if_present_flag);
  if(!ok) return ok;

  uint8 data_coding;
  ok = unpack(data_coding);
  if(!ok) return ok;

  uint8 sm_default_msg_id;
  ok = unpack(sm_default_msg_id);
  if(!ok) return ok;

  uint8 sm_length;
  ok = unpack(sm_length);
  if(!ok) return ok;

  int8 *short_message(0);
  ok = unpack(short_message, sm_length);
  if(!ok) return ok;

  p.setServiceType(service_type);
  p.setSourceAddrTon(source_addr_ton);
  p.setSourceAddrNpi(source_addr_npi);
  p.setSourceAddr(source_addr);
  p.setDestAddrTon(dest_addr_ton);
  p.setDestAddrNpi(dest_addr_npi);
  p.setDestinationAddr(dest_addr);
  p.setEsmClass(esm_class);
  p.setProtocolId(protocol_id);
  p.setPriorityFlag(priority_flag);
  p.setScheduleDeliveryTime(schedule_delivery_time);
  p.setValidityPeriod(validity_period);
  p.setRegisteredDelivery(registered_delivery);
  p.setReplaceIfPresentFlag(replace_if_present_flag);
  p.setDataCoding(data_coding);
  p.setSmDefaultMsgId(sm_default_msg_id);
  p.setSmLength(sm_length);
  p.setShortMessage(short_message, sm_length);

  delete [] short_message;

  ok = unpack((TlvFields &)p);

  return ok;
}

bool BinaryDecoder::unpack(SubmitSmResp &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  if(p.getCommandStatus() == smpp::parameters::command_status::ESME_ROK) {
    std::string message_id;
    ok = unpack(message_id);
    if(!ok) return ok;

    p.setMessageId(message_id);
  }

  return ok;
}

bool BinaryDecoder::unpack(SubmitMulti &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string service_type;
  ok = unpack(service_type);
  if(!ok) return ok;

  uint8 source_addr_ton;
  ok = unpack(source_addr_ton);
  if(!ok) return ok;

  uint8 source_addr_npi;
  ok = unpack(source_addr_npi);
  if(!ok) return ok;

  std::string source_addr;
  ok = unpack(source_addr);
  if(!ok) return ok;

  uint8 num_of_dests;
  ok = unpack(num_of_dests);
  if(!ok) return ok;

  MultiDestinationAddresses destination_addrs;
  ok = unpack(destination_addrs, num_of_dests);
  if(!ok) return ok;

  uint8 esm_class;
  ok = unpack(esm_class);
  if(!ok) return ok;

  uint8 protocol_id;
  ok = unpack(protocol_id);
  if(!ok) return ok;

  uint8 priority_flag;
  ok = unpack(priority_flag);
  if(!ok) return ok;

  std::string schedule_delivery_time;
  ok = unpack(schedule_delivery_time);
  if(!ok) return ok;

  std::string validity_period;
  ok = unpack(validity_period);
  if(!ok) return ok;

  uint8 registered_delivery;
  ok = unpack(registered_delivery);
  if(!ok) return ok;

  uint8 replace_if_present_flag;
  ok = unpack(replace_if_present_flag);
  if(!ok) return ok;

  uint8 data_coding;
  ok = unpack(data_coding);
  if(!ok) return ok;

  uint8 sm_default_msg_id;
  ok = unpack(sm_default_msg_id);
  if(!ok) return ok;

  uint8 sm_length;
  ok = unpack(sm_length);
  if(!ok) return ok;

  int8 *short_message(0);
  ok = unpack(short_message, sm_length);
  if(!ok) return ok;

  p.setServiceType(service_type);
  p.setSourceAddrTon(source_addr_ton);
  p.setSourceAddrNpi(source_addr_npi);
  p.setSourceAddr(source_addr);
  p.setNumberOfDests(num_of_dests);
  p.setDestAddresses(destination_addrs);
  p.setEsmClass(esm_class);
  p.setProtocolId(protocol_id);
  p.setPriorityFlag(priority_flag);
  p.setScheduleDeliveryTime(schedule_delivery_time);
  p.setValidityPeriod(validity_period);
  p.setRegisteredDelivery(registered_delivery);
  p.setReplaceIfPresentFlag(replace_if_present_flag);
  p.setDataCoding(data_coding);
  p.setSmDefaultMsgId(sm_default_msg_id);
  p.setSmLength(sm_length);
  p.setShortMessage(short_message, sm_length);

  delete [] short_message;

  ok = unpack((TlvFields &)p);

  return ok;
}

bool BinaryDecoder::unpack(SubmitMultiResp &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string message_id;
  ok = unpack(message_id);
  if(!ok) return ok;

  uint8 no_unsuccess;
  ok = unpack(no_unsuccess);
  if(!ok) return ok;

  for(int i = 0; i < no_unsuccess; i++) {
    UnsuccessSme unsuccess_sme;
    ok = unpack(unsuccess_sme);
    if(!ok) return ok;
    p.add(unsuccess_sme);
  }

  return ok;
}

bool BinaryDecoder::unpack(DeliverSm &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string service_type;
  ok = unpack(service_type);
  if(!ok) return ok;

  uint8 source_addr_ton;
  ok = unpack(source_addr_ton);
  if(!ok) return ok;

  uint8 source_addr_npi;
  ok = unpack(source_addr_npi);
  if(!ok) return ok;

  std::string source_addr;
  ok = unpack(source_addr);
  if(!ok) return ok;

  uint8 dest_addr_ton;
  ok = unpack(dest_addr_ton);
  if(!ok) return ok;

  uint8 dest_addr_npi;
  ok = unpack(dest_addr_npi);
  if(!ok) return ok;

  std::string dest_addr;
  ok = unpack(dest_addr);
  if(!ok) return ok;

  uint8 esm_class;
  ok = unpack(esm_class);
  if(!ok) return ok;

  uint8 protocol_id;
  ok = unpack(protocol_id);
  if(!ok) return ok;

  uint8 priority_flag;
  ok = unpack(priority_flag);
  if(!ok) return ok;

  std::string schedule_delivery_time;
  ok = unpack(schedule_delivery_time);
  if(!ok) return ok;

  std::string validity_period;
  ok = unpack(validity_period);
  if(!ok) return ok;

  uint8 registered_delivery;
  ok = unpack(registered_delivery);
  if(!ok) return ok;

  uint8 replace_if_present_flag;
  ok = unpack(replace_if_present_flag);
  if(!ok) return ok;

  uint8 data_coding;
  ok = unpack(data_coding);
  if(!ok) return ok;

  uint8 sm_default_msg_id;
  ok = unpack(sm_default_msg_id);
  if(!ok) return ok;

  uint8 sm_length;
  ok = unpack(sm_length);
  if(!ok) return ok;

  int8 *short_message(0);
  ok = unpack(short_message, sm_length);
  if(!ok) return ok;

  p.setServiceType(service_type);
  p.setSourceAddrTon(source_addr_ton);
  p.setSourceAddrNpi(source_addr_npi);
  p.setSourceAddr(source_addr);
  p.setDestAddrTon(dest_addr_ton);
  p.setDestAddrNpi(dest_addr_npi);
  p.setDestinationAddr(dest_addr);
  p.setEsmClass(esm_class);
  p.setProtocolId(protocol_id);
  p.setPriorityFlag(priority_flag);
  p.setScheduleDeliveryTime(schedule_delivery_time);
  p.setValidityPeriod(validity_period);
  p.setRegisteredDelivery(registered_delivery);
  p.setReplaceIfPresentFlag(replace_if_present_flag);
  p.setDataCoding(data_coding);
  p.setSmDefaultMsgId(sm_default_msg_id);
  p.setSmLength(sm_length);
  p.setShortMessage(short_message, sm_length);

  delete [] short_message;

  ok = unpack((TlvFields &)p);

  return ok;
}

bool BinaryDecoder::unpack(DeliverSmResp &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string message_id;
  ok = unpack(message_id);
  if(!ok) return ok;

  p.setMessageId(message_id);

  return ok;
}

bool BinaryDecoder::unpack(DataSm &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string service_type;
  ok = unpack(service_type);
  if(!ok) return ok;

  uint8 source_addr_ton;
  ok = unpack(source_addr_ton);
  if(!ok) return ok;

  uint8 source_addr_npi;
  ok = unpack(source_addr_npi);
  if(!ok) return ok;

  std::string source_addr;
  ok = unpack(source_addr);
  if(!ok) return ok;

  uint8 dest_addr_ton;
  ok = unpack(dest_addr_ton);
  if(!ok) return ok;

  uint8 dest_addr_npi;
  ok = unpack(dest_addr_npi);
  if(!ok) return ok;

  std::string dest_addr;
  ok = unpack(dest_addr);
  if(!ok) return ok;

  uint8 esm_class;
  ok = unpack(esm_class);
  if(!ok) return ok;

  uint8 registered_delivery;
  ok = unpack(registered_delivery);
  if(!ok) return ok;

  uint8 data_coding;
  ok = unpack(data_coding);
  if(!ok) return ok;

  p.setServiceType(service_type);
  p.setSourceAddrTon(source_addr_ton);
  p.setSourceAddrNpi(source_addr_npi);
  p.setSourceAddr(source_addr);
  p.setDestAddrTon(dest_addr_ton);
  p.setDestAddrNpi(dest_addr_npi);
  p.setDestinationAddr(dest_addr);
  p.setEsmClass(esm_class);
  p.setRegisteredDelivery(registered_delivery);
  p.setDataCoding(data_coding);

  ok = unpack((TlvFields &)p);

  return ok;
}

bool BinaryDecoder::unpack(DataSmResp &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string message_id;
  ok = unpack(message_id);
  if(!ok) return ok;

  p.setMessageId(message_id);

  ok = unpack((TlvFields &)p);

  return ok;
}

bool BinaryDecoder::unpack(QuerySm &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string message_id;
  ok = unpack(message_id);
  if(!ok) return ok;

  uint8 source_addr_ton;
  ok = unpack(source_addr_ton);
  if(!ok) return ok;

  uint8 source_addr_npi;
  ok = unpack(source_addr_npi);
  if(!ok) return ok;

  std::string source_addr;
  ok = unpack(source_addr);
  if(!ok) return ok;

  p.setMessageId(message_id);
  p.setSourceAddrTon(source_addr_ton);
  p.setSourceAddrNpi(source_addr_npi);
  p.setSourceAddr(source_addr);

  return ok;
}

bool BinaryDecoder::unpack(QuerySmResp &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string message_id;
  ok = unpack(message_id);
  if(!ok) return ok;

  std::string final_date;
  ok = unpack(final_date);
  if(!ok) return ok;

  uint8 message_state;
  ok = unpack(message_state);
  if(!ok) return ok;

  uint8 error_code;
  ok = unpack(error_code);
  if(!ok) return ok;

  p.setMessageId(message_id);
  p.setFinalDate(final_date);
  p.setMessageState(message_state);
  p.setErrorCode(error_code);

  return ok;
}

bool BinaryDecoder::unpack(CancelSm &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string service_type;
  ok = unpack(service_type);
  if(!ok) return ok;

  std::string message_id;
  ok = unpack(message_id);
  if(!ok) return ok;

  uint8 source_addr_ton;
  ok = unpack(source_addr_ton);
  if(!ok) return ok;

  uint8 source_addr_npi;
  ok = unpack(source_addr_npi);
  if(!ok) return ok;

  std::string source_addr;
  ok = unpack(source_addr);
  if(!ok) return ok;

  uint8 dest_addr_ton;
  ok = unpack(dest_addr_ton);
  if(!ok) return ok;

  uint8 dest_addr_npi;
  ok = unpack(dest_addr_npi);
  if(!ok) return ok;

  std::string dest_addr;
  ok = unpack(dest_addr);
  if(!ok) return ok;

  p.setServiceType(service_type);
  p.setMessageId(message_id);
  p.setSourceAddrTon(source_addr_ton);
  p.setSourceAddrNpi(source_addr_npi);
  p.setSourceAddr(source_addr);
  p.setDestAddrTon(dest_addr_ton);
  p.setDestAddrNpi(dest_addr_npi);
  p.setDestinationAddr(dest_addr);

  return ok;
}

bool BinaryDecoder::unpack(ReplaceSm &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  std::string message_id;
  ok = unpack(message_id);
  if(!ok) return ok;

  uint8 source_addr_ton;
  ok = unpack(source_addr_ton);
  if(!ok) return ok;

  uint8 source_addr_npi;
  ok = unpack(source_addr_npi);
  if(!ok) return ok;

  std::string source_addr;
  ok = unpack(source_addr);
  if(!ok) return ok;

  std::string schedule_delivery_time;
  ok = unpack(schedule_delivery_time);
  if(!ok) return ok;

  std::string validity_period;
  ok = unpack(validity_period);
  if(!ok) return ok;

  uint8 registered_delivery;
  ok = unpack(registered_delivery);
  if(!ok) return ok;

  uint8 sm_default_msg_id;
  ok = unpack(sm_default_msg_id);
  if(!ok) return ok;

  uint8 sm_length;
  ok = unpack(sm_length);
  if(!ok) return ok;

  int8 *short_message(0);
  ok = unpack(short_message, sm_length);
  if(!ok) return ok;

  p.setMessageId(message_id);
  p.setSourceAddrTon(source_addr_ton);
  p.setSourceAddrNpi(source_addr_npi);
  p.setSourceAddr(source_addr);
  p.setScheduleDeliveryTime(schedule_delivery_time);
  p.setValidityPeriod(validity_period);
  p.setRegisteredDelivery(registered_delivery);
  p.setSmDefaultMsgId(sm_default_msg_id);
  p.setSmLength(sm_length);
  p.setShortMessage(short_message, sm_length);

  delete [] short_message;

  return ok;
}

bool BinaryDecoder::unpack(AlertNotification &p, bool unpack_header) {
  bool ok(true);
  if(unpack_header) {
    ok = unpack((Header &)p);
    if(!ok) return ok;
  }

  uint8 source_addr_ton;
  ok = unpack(source_addr_ton);
  if(!ok) return ok;

  uint8 source_addr_npi;
  ok = unpack(source_addr_npi);
  if(!ok) return ok;

  std::string source_addr;
  ok = unpack(source_addr);
  if(!ok) return ok;

  uint8 esme_addr_ton;
  ok = unpack(esme_addr_ton);
  if(!ok) return ok;

  uint8 esme_addr_npi;
  ok = unpack(esme_addr_npi);
  if(!ok) return ok;

  std::string esme_addr;
  ok = unpack(esme_addr);
  if(!ok) return ok;

  p.setSourceAddrTon(source_addr_ton);
  p.setSourceAddrNpi(source_addr_npi);
  p.setSourceAddr(source_addr);
  p.setEsmeAddrTon(esme_addr_ton);
  p.setEsmeAddrNpi(esme_addr_npi);
  p.setEsmeAddr(esme_addr);

  ok = unpack((TlvFields &)p);

  return ok;
}

} // namespace decodes

} // namespace smpp

