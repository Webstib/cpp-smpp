#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_submit_multi.h"

namespace smpp {

namespace pdu {

void SubmitMulti::renewShortMessage(uint8 len) {
  if(short_message) {
    delete [] short_message;
  }

  if(len > 0) {
    short_message = new int8[len];
  } else {
    short_message = 0;
  }
}

SubmitMulti::SubmitMulti(uint32 sequence_number,
                         const std::string &service_type,
                         uint8 source_addr_ton,
                         uint8 source_addr_npi,
                         const std::string &source_addr,
                         uint8 number_of_dests,
                         const MultiDestinationAddresses &dest_addresses,
                         uint8 esm_class,
                         uint8 protocol_id,
                         uint8 priority_flag,
                         const std::string &schedule_delivery_time,
                         const std::string &validity_period,
                         uint8 registered_delivery,
                         uint8 replace_if_present_flag,
                         uint8 data_coding,
                         uint8 sm_default_msg_id,
                         uint8 sm_length,
                         const int8 *short_message) :
  Header(MIN_LENGTH,
         parameters::command_id::SUBMIT_SM,
         parameters::command_status::ESME_ROK,
         sequence_number),
  service_type(service_type),
  source_addr_ton(source_addr_ton),
  source_addr_npi(source_addr_npi),
  source_addr(source_addr),
  number_of_dests(number_of_dests),
  dest_addresses(dest_addresses),
  esm_class(esm_class),
  protocol_id(protocol_id),
  priority_flag(priority_flag),
  schedule_delivery_time(schedule_delivery_time),
  validity_period(validity_period),
  registered_delivery(registered_delivery),
  replace_if_present_flag(replace_if_present_flag),
  data_coding(data_coding),
  sm_default_msg_id(sm_default_msg_id),
  sm_length(sm_length),
  short_message(0) {
  if(short_message != 0 && sm_length != 0) {
    renewShortMessage(sm_length);
    memcpy(this->short_message, short_message, sm_length);
  }
}

SubmitMulti::SubmitMulti(const Header &p) :
  Header(p),
  source_addr_ton(0),
  source_addr_npi(0),
  number_of_dests(0),
  esm_class(0),
  protocol_id(0),
  priority_flag(0),
  registered_delivery(0),
  replace_if_present_flag(0),
  data_coding(0),
  sm_default_msg_id(0),
  sm_length(0),
  short_message(0) {

}

SubmitMulti::SubmitMulti(const SubmitMulti &p) :
  Header(p),
  TlvFields(p),
  service_type(p.service_type),
  source_addr_ton(p.source_addr_ton),
  source_addr_npi(p.source_addr_npi),
  source_addr(p.source_addr),
  number_of_dests(p.number_of_dests),
  dest_addresses(p.dest_addresses),
  esm_class(p.esm_class),
  protocol_id(p.protocol_id),
  priority_flag(p.priority_flag),
  schedule_delivery_time(p.schedule_delivery_time),
  validity_period(p.validity_period),
  registered_delivery(p.registered_delivery),
  replace_if_present_flag(p.replace_if_present_flag),
  data_coding(p.data_coding),
  sm_default_msg_id(p.sm_default_msg_id),
  sm_length(p.sm_length),
  short_message(0) {
  renewShortMessage(sm_length);

  if(p.short_message != 0 && sm_length != 0) {
    memcpy(short_message, p.short_message, sm_length);
  }
}

void SubmitMulti::setShortMessage(const int8 *v, uint8 v_len) {
  renewShortMessage(v_len);

  if(v != 0 && v_len != 0) {
    memcpy(short_message, v, v_len);
  }
}

void SubmitMulti::calcCommandLength() {
  setCommandLength(
    MIN_LENGTH +
    service_type.size() +
    source_addr.size() +
    dest_addresses.size() +
    schedule_delivery_time.size() +
    validity_period.size() +
    sm_length +
    TlvFields::size());
}

SubmitMulti &SubmitMulti::operator =(const SubmitMulti &p) {
  if(this != &p) {
    Header::operator=(p);
    service_type = p.service_type;
    source_addr_ton = p.source_addr_ton;
    source_addr_npi = p.source_addr_npi;
    source_addr = p.source_addr;
    number_of_dests = p.number_of_dests;
    dest_addresses = p.dest_addresses;
    esm_class = p.esm_class;
    protocol_id = p.protocol_id;
    priority_flag = p.priority_flag;
    schedule_delivery_time = p.schedule_delivery_time;
    validity_period = p.validity_period;
    registered_delivery = p.registered_delivery;
    replace_if_present_flag = p.replace_if_present_flag;
    data_coding = p.data_coding;
    sm_default_msg_id = p.sm_default_msg_id;
    sm_length = p.sm_length;
    renewShortMessage(sm_length);

    if(p.short_message != 0 && p.sm_length != 0) {
      memcpy(short_message, p.short_message, sm_length);
    }
    TlvFields::operator=(p);
  }
  return *this;
}

bool SubmitMulti::operator ==(const SubmitMulti &p) const {
  return(Header::operator==(p) &&
         service_type == p.service_type &&
         source_addr_ton == p.source_addr_ton &&
         source_addr_npi == p.source_addr_npi &&
         source_addr == p.source_addr &&
         number_of_dests == p.number_of_dests &&
         dest_addresses == p.dest_addresses &&
         esm_class == p.esm_class &&
         protocol_id == p.protocol_id &&
         priority_flag == p.priority_flag &&
         schedule_delivery_time == p.schedule_delivery_time &&
         validity_period == p.validity_period &&
         registered_delivery == p.registered_delivery &&
         replace_if_present_flag == p.replace_if_present_flag &&
         data_coding == p.data_coding &&
         sm_default_msg_id == p.sm_default_msg_id &&
         sm_length == p.sm_length &&
         memcmp(p.short_message, short_message, sm_length) == 0 &&
         TlvFields::operator==(p));
}

bool SubmitMulti::operator !=(const SubmitMulti &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
