#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_deliver_sm_resp.h"

namespace smpp {

namespace pdu {

DeliverSmResp::DeliverSmResp(uint32 command_status,
                             uint32 sequence_number,
                             const std::string &message_id) :
  Header(MIN_LENGTH,
         parameters::command_id::DELIVER_SM_RESP,
         command_status,
         sequence_number),
  message_id(message_id) {

}

DeliverSmResp::DeliverSmResp(const Header &p) : Header(p) {

}

DeliverSmResp::DeliverSmResp(const DeliverSmResp &p) :
  Header(p),
  message_id(p.message_id) {

}

void DeliverSmResp::calcCommandLength() {
  setCommandLength(MIN_LENGTH + message_id.size());
}

DeliverSmResp &DeliverSmResp::operator =(const DeliverSmResp &p) {
  if(this != &p) {
    Header::operator=(p);
    message_id = p.message_id;
  }
  return *this;
}

bool DeliverSmResp::operator ==(const DeliverSmResp &p) const {
  return(Header::operator==(p) && message_id == p.message_id);
}

bool DeliverSmResp::operator !=(const DeliverSmResp &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
