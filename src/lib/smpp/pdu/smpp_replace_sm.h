#ifndef SMPP_REPLACE_SM_H
#define SMPP_REPLACE_SM_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT ReplaceSm : public Header {
  std::string message_id;
  uint8 source_addr_ton;
  uint8 source_addr_npi;
  std::string source_addr;
  std::string schedule_delivery_time;
  std::string validity_period;
  uint8 registered_delivery;
  uint8 sm_default_msg_id;
  uint8 sm_length;
  int8 *short_message;

  void renewShortMessage(uint8 len);

public:

  ReplaceSm(uint32 sequence_number = 0,
            const std::string &message_id = std::string(),
            uint8 source_addr_ton = 0,
            uint8 source_addr_npi = 0,
            const std::string &source_addr = std::string(),
            const std::string &schedule_delivery_time = std::string(),
            const std::string &validity_period = std::string(),
            uint8 registered_delivery = 0,
            uint8 sm_default_msg_id = 0,
            uint8 sm_length = 0,
            const int8 *short_message = 0);

  ReplaceSm(const Header &p);

  ReplaceSm(const ReplaceSm &p);

  virtual ~ReplaceSm() {
    renewShortMessage(0);
  }

  inline void setMessageId(const std::string &v) {
    message_id = v;
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

  inline void setScheduleDeliveryTime(const std::string &v) {
    schedule_delivery_time = v;
  }

  inline void setValidityPeriod(const std::string &v) {
    validity_period = v;
  }

  inline void setRegisteredDelivery(uint8 v) {
    registered_delivery = v;
  }

  inline void setSmDefaultMsgId(uint8 v) {
    sm_default_msg_id = v;
  }

  inline void setSmLength(uint8 v) {
    sm_length = v;
  }

  void setShortMessage(const int8 *v, uint8 v_len);

  inline const std::string &getMessageId() const {
    return message_id;
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

  inline const std::string &getScheduleDeliveryTime() const {
    return schedule_delivery_time;
  }

  inline const std::string &getValidityPeriod() const {
    return validity_period;
  }

  inline uint8 getRegisteredDelivery() const {
    return registered_delivery;
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

  ReplaceSm &operator =(const ReplaceSm &p);

  bool operator ==(const ReplaceSm &p) const;

  bool operator !=(const ReplaceSm &p) const;

  static const uint32 MIN_LENGTH = 26;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_REPLACE_SM_H
