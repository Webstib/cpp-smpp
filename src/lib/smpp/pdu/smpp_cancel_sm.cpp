#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_cancel_sm.h"

namespace smpp {

namespace pdu {

CancelSm::CancelSm(uint32 sequence_number,
                   const std::string &service_type,
                   const std::string &message_id,
                   uint8 source_addr_ton,
                   uint8 source_addr_npi,
                   const std::string &source_addr,
                   uint8 dest_addr_ton,
                   uint8 dest_addr_npi,
                   const std::string &destination_addr) :
  Header(MIN_LENGTH,
         parameters::command_id::CANCEL_SM,
         parameters::command_status::ESME_ROK,
         sequence_number),
  service_type(service_type),
  message_id(message_id),
  source_addr_ton(source_addr_ton),
  source_addr_npi(source_addr_npi),
  source_addr(source_addr),
  dest_addr_ton(dest_addr_ton),
  dest_addr_npi(dest_addr_npi),
  destination_addr(destination_addr) {

}

CancelSm::CancelSm(const Header &p) :
  Header(p),
  source_addr_ton(0),
  source_addr_npi(0),
  dest_addr_ton(0),
  dest_addr_npi(0) {

}

CancelSm::CancelSm(const CancelSm &p) :
  Header(p),
  service_type(p.service_type),
  message_id(p.message_id),
  source_addr_ton(p.source_addr_ton),
  source_addr_npi(p.source_addr_npi),
  source_addr(p.source_addr),
  dest_addr_ton(p.dest_addr_ton),
  dest_addr_npi(p.dest_addr_npi),
  destination_addr(p.destination_addr) {

}

void CancelSm::calcCommandLength() {
  setCommandLength(
    MIN_LENGTH +
    service_type.size() +
    message_id.size() +
    source_addr.size() +
    destination_addr.size());
}

CancelSm &CancelSm::operator =(const CancelSm &p) {
  if(this != &p) {
    Header::operator=(p);
    service_type = p.service_type;
    message_id = p.message_id;
    source_addr_ton = p.source_addr_ton;
    source_addr_npi = p.source_addr_npi;
    source_addr = p.source_addr;
    dest_addr_ton = p.dest_addr_ton;
    dest_addr_npi = p.dest_addr_npi;
    destination_addr = p.destination_addr;
  }
  return *this;
}

bool CancelSm::operator ==(const CancelSm &p) const {
  return(Header::operator==(p) &&
         service_type == p.service_type &&
         message_id == p.message_id &&
         source_addr_ton == p.source_addr_ton &&
         source_addr_npi == p.source_addr_npi &&
         source_addr == p.source_addr &&
         dest_addr_ton == p.dest_addr_ton &&
         dest_addr_npi == p.dest_addr_npi &&
         destination_addr == p.destination_addr);
}

bool CancelSm::operator !=(const CancelSm &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
