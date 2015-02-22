#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_alert_notification.h"

namespace smpp {

namespace pdu {

AlertNotification::AlertNotification(
  uint32 sequence_number,
  uint8 source_addr_ton,
  uint8 source_addr_npi,
  const std::string &source_addr,
  uint8 esme_addr_ton,
  uint8 esme_addr_npi,
  const std::string &esme_addr) :
  Header(MIN_LENGTH,
         parameters::command_id::ALERT_NOTIFICATION,
         parameters::command_status::ESME_ROK,
         sequence_number),
  source_addr_ton(source_addr_ton),
  source_addr_npi(source_addr_npi),
  source_addr(source_addr),
  esme_addr_ton(esme_addr_ton),
  esme_addr_npi(esme_addr_npi),
  esme_addr(esme_addr) {

}

AlertNotification::AlertNotification(const Header &p) : Header(p) {

}

AlertNotification::AlertNotification(const AlertNotification &p) :
  Header(p),
  TlvFields(p),
  source_addr_ton(p.source_addr_ton),
  source_addr_npi(p.source_addr_npi),
  source_addr(p.source_addr),
  esme_addr_ton(p.esme_addr_ton),
  esme_addr_npi(p.esme_addr_npi),
  esme_addr(p.esme_addr) {

}

void AlertNotification::calcCommandLength() {
  setCommandLength(
    MIN_LENGTH + source_addr.size() + esme_addr.size() + TlvFields::size());
}

AlertNotification &AlertNotification::operator =(const AlertNotification &p) {
  if(this != &p) {
    Header::operator=(p);
    source_addr_ton = p.source_addr_ton;
    source_addr_npi = p.source_addr_npi;
    source_addr = p.source_addr;
    esme_addr_ton = p.esme_addr_ton;
    esme_addr_npi = p.esme_addr_npi;
    esme_addr = p.esme_addr;
    TlvFields::operator=(p);
  }
  return *this;
}

bool AlertNotification::operator ==(const AlertNotification &p) const {
  return(Header::operator==(p) &&
         source_addr_ton == p.source_addr_ton &&
         source_addr_npi == p.source_addr_npi &&
         source_addr == p.source_addr &&
         esme_addr_ton == p.esme_addr_ton &&
         esme_addr_npi == p.esme_addr_npi &&
         esme_addr == p.esme_addr &&
         TlvFields::operator==(p));
}

bool AlertNotification::operator !=(const AlertNotification &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
