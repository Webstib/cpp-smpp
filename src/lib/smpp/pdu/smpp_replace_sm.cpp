#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_replace_sm.h"

namespace smpp {

namespace pdu {

void ReplaceSm::renewShortMessage(uint8 len) {
  if(short_message) {
    delete [] short_message;
  }

  if(len > 0) {
    short_message = new int8[len];
  } else {
    short_message = 0;
  }
}

ReplaceSm::ReplaceSm(uint32 sequence_number,
                     const std::string &message_id,
                     uint8 source_addr_ton,
                     uint8 source_addr_npi,
                     const std::string &source_addr,
                     const std::string &schedule_delivery_time,
                     const std::string &validity_period,
                     uint8 registered_delivery,
                     uint8 sm_default_msg_id,
                     uint8 sm_length,
                     const int8 *short_message) :
  Header(MIN_LENGTH,
         parameters::command_id::REPLACE_SM,
         parameters::command_status::ESME_ROK,
         sequence_number),
  message_id(message_id),
  source_addr_ton(source_addr_ton),
  source_addr_npi(source_addr_npi),
  source_addr(source_addr),
  schedule_delivery_time(schedule_delivery_time),
  validity_period(validity_period),
  registered_delivery(registered_delivery),
  sm_default_msg_id(sm_default_msg_id),
  sm_length(sm_length),
  short_message(0) {
  if(short_message != 0 && sm_length != 0) {
    renewShortMessage(sm_length);
    memcpy(this->short_message, short_message, sm_length);
  }
}

ReplaceSm::ReplaceSm(const Header &p) :
  Header(p),
  source_addr_ton(0),
  source_addr_npi(0),
  registered_delivery(0),
  sm_default_msg_id(0),
  sm_length(0),
  short_message(0) {

}

ReplaceSm::ReplaceSm(const ReplaceSm &p) :
  Header(p),
  message_id(p.message_id),
  source_addr_ton(p.source_addr_ton),
  source_addr_npi(p.source_addr_npi),
  source_addr(p.source_addr),
  schedule_delivery_time(p.schedule_delivery_time),
  validity_period(p.validity_period),
  registered_delivery(p.registered_delivery),
  sm_default_msg_id(p.sm_default_msg_id),
  sm_length(p.sm_length),
  short_message(0) {
  renewShortMessage(sm_length);

  if(p.short_message != 0 && sm_length != 0) {
    memcpy(short_message, p.short_message, sm_length);
  }
}

void ReplaceSm::setShortMessage(const int8 *v, uint8 v_len) {
  renewShortMessage(v_len);

  if(v != 0 && v_len != 0) {
    memcpy(short_message, v, v_len);
  }
}

void ReplaceSm::calcCommandLength() {
  setCommandLength(
    MIN_LENGTH +
    message_id.size() +
    source_addr.size() +
    schedule_delivery_time.size() +
    validity_period.size() +
    sm_length);
}

ReplaceSm &ReplaceSm::operator =(const ReplaceSm &p) {
  if(this != &p) {
    Header::operator=(p);
    message_id = p.message_id;
    source_addr_ton = p.source_addr_ton;
    source_addr_npi = p.source_addr_npi;
    source_addr = p.source_addr;
    schedule_delivery_time = p.schedule_delivery_time;
    validity_period = p.validity_period;
    registered_delivery = p.registered_delivery;
    sm_default_msg_id = p.sm_default_msg_id;
    sm_length = p.sm_length;
    renewShortMessage(sm_length);

    if(p.short_message != 0 && p.sm_length != 0) {
      memcpy(short_message, p.short_message, sm_length);
    }
  }
  return *this;
}

bool ReplaceSm::operator ==(const ReplaceSm &p) const {
  return(Header::operator==(p) &&
         message_id == p.message_id &&
         source_addr_ton == p.source_addr_ton &&
         source_addr_npi == p.source_addr_npi &&
         source_addr == p.source_addr &&
         schedule_delivery_time == p.schedule_delivery_time &&
         validity_period == p.validity_period &&
         registered_delivery == p.registered_delivery &&
         sm_default_msg_id == p.sm_default_msg_id &&
         sm_length == p.sm_length &&
         memcmp(p.short_message, short_message, sm_length) == 0);
}

bool ReplaceSm::operator !=(const ReplaceSm &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
