#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_bind_transceiver_resp.h"

namespace smpp {

namespace pdu {

BindTransceiverResp::BindTransceiverResp(
  uint32 command_status,
  uint32 sequence_number,
  const std::string &system_id) :
  Header(MIN_LENGTH,
         parameters::command_id::BIND_TRANSCEIVER_RESP,
         command_status,
         sequence_number),
  system_id(system_id) {

}

BindTransceiverResp::BindTransceiverResp(const Header &p) : Header(p) {

}

BindTransceiverResp::BindTransceiverResp(const BindTransceiverResp &p) :
  Header(p),
  TlvFields(p),
  system_id(p.system_id) {

}

void BindTransceiverResp::calcCommandLength() {
  setCommandLength(MIN_LENGTH + system_id.size() + TlvFields::size());
}

BindTransceiverResp &BindTransceiverResp::operator =(const BindTransceiverResp &p) {
  if(this != &p) {
    Header::operator=(p);
    system_id = p.system_id;
    TlvFields::operator=(p);
  }
  return *this;
}

bool BindTransceiverResp::operator ==(const BindTransceiverResp &p) const {
  return(Header::operator==(p) &&
         system_id == p.system_id &&
         TlvFields::operator==(p));
}

bool BindTransceiverResp::operator !=(const BindTransceiverResp &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
