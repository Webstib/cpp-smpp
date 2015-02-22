#ifndef SMPP_SUBMIT_SM_H
#define SMPP_SUBMIT_SM_H

#include "smpp/pdu/smpp_header.h"
#include "smpp/pdu/smpp_tlv_fields.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT SubmitSm : public Header, public TlvFields {
  std::string service_type;
  uint8 source_addr_ton;
  uint8 source_addr_npi;
  std::string source_addr;
  uint8 dest_addr_ton;
  uint8 dest_addr_npi;
  std::string destination_addr;
  uint8 esm_class;
  uint8 protocol_id;
  uint8 priority_flag;
  std::string schedule_delivery_time;
  std::string validity_period;
  uint8 registered_delivery;
  uint8 replace_if_present_flag;
  uint8 data_coding;
  uint8 sm_default_msg_id;
  uint8 sm_length;
  int8 *short_message;

  void renewShortMessage(uint8 len);

public:

  SubmitSm(uint32 sequence_number = 0,
           const std::string &service_type = std::string(),
           uint8 source_addr_ton = 0,
           uint8 source_addr_npi = 0,
           const std::string &source_addr = std::string(),
           uint8 dest_addr_ton = 0,
           uint8 dest_addr_npi = 0,
           const std::string &destination_addr = std::string(),
           uint8 esm_class = 0,
           uint8 protocol_id = 0,
           uint8 priority_flag = 0,
           const std::string &schedule_delivery_time = std::string(),
           const std::string &validity_period = std::string(),
           uint8 registered_delivery = 0,
           uint8 replace_if_present_flag = 0,
           uint8 data_coding = 0,
           uint8 sm_default_msg_id = 0,
           uint8 sm_length = 0,
           const int8 *short_message = 0);

  SubmitSm(const Header &p);

  SubmitSm(const SubmitSm &p);

  virtual ~SubmitSm() {
    renewShortMessage(0);
  }

  inline void setServiceType(const std::string &v) {
    service_type = v;
  }

  inline void setSourceAddrTon(uint8 v) {
    source_addr_ton = v;
  }

  inline void setSourceAddrNpi(uint8 v) {
    source_addr_npi = v;
  }

  inline void setSourceAddr(const std::string &v) {
    source_addr = v;
  }

  inline void setDestAddrTon(uint8 v) {
    dest_addr_ton = v;
  }

  inline void setDestAddrNpi(uint8 v) {
    dest_addr_npi = v;
  }

  inline void setDestinationAddr(const std::string &v) {
    destination_addr = v;
  }

  inline void setEsmClass(uint8 v) {
    esm_class = v;
  }

  inline void setProtocolId(uint8 v) {
    protocol_id = v;
  }

  inline void setPriorityFlag(uint8 v) {
    priority_flag = v;
  }

  inline void setScheduleDeliveryTime(const std::string &v) {
    schedule_delivery_time = v;
  }

  inline void setValidityPeriod(const std::string &v) {
    validity_period = v;
  }

  inline void setRegisteredDelivery(uint8 v) {
    registered_delivery = v;
  }

  inline void setReplaceIfPresentFlag(uint8 v) {
    replace_if_present_flag = v;
  }

  inline void setDataCoding(uint8 v) {
    data_coding = v;
  }

  inline void setSmDefaultMsgId(uint8 v) {
    sm_default_msg_id = v;
  }

  inline void setSmLength(uint8 v) {
    sm_length = v;
  }

  void setShortMessage(const int8 *v, uint8 v_len);

  inline const std::string &getServiceType() const {
    return service_type;
  }

  inline uint8 getSourceAddrTon() const {
    return source_addr_ton;
  }

  inline uint8 getSourceAddrNpi() const {
    return source_addr_npi;
  }

  inline const std::string &getSourceAddr() const {
    return source_addr;
  }

  inline uint8 getDestAddrTon() const {
    return dest_addr_ton;
  }

  inline uint8 getDestAddrNpi() const {
    return dest_addr_npi;
  }

  inline const std::string &getDestinationAddr() const {
    return destination_addr;
  }

  inline uint8 getEsmClass() const {
    return esm_class;
  }

  inline uint8 getProtocolId() const {
    return protocol_id;
  }

  inline uint8 getPriorityFlag() const {
    return priority_flag;
  }

  inline const std::string &getScheduleDeliveryTime() const {
    return schedule_delivery_time;
  }

  inline const std::string &getValidityPeriod() const {
    return validity_period;
  }

  inline uint8 getRegisteredDelivery() const {
    return registered_delivery;
  }

  inline uint8 getReplaceIfPresentFlag() const {
    return replace_if_present_flag;
  }

  inline uint8 getDataCoding() const {
    return data_coding;
  }

  inline uint8 getSmDefaultMsgId() const {
    return sm_default_msg_id;
  }

  inline uint8 getSmLength() const {
    return sm_length;
  }

  inline const int8 *getShortMessage() const {
    return short_message;
  }

  void calcCommandLength();

  SubmitSm &operator =(const SubmitSm &p);

  bool operator ==(const SubmitSm &p) const;

  bool operator !=(const SubmitSm &p) const;

  static const uint32 MIN_LENGTH = 33;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_SUBMIT_SM_H
