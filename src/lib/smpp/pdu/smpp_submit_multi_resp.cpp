#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_submit_multi_resp.h"

namespace smpp {

namespace pdu {

SubmitMultiResp::SubmitMultiResp(
  uint32 command_status,
  uint32 sequence_number,
  const std::string &message_id,
  uint8 no_unsuccess,
  const std::list<UnsuccessSme> &unsuccess_smes) :
  Header(MIN_LENGTH,
         parameters::command_id::SUBMIT_MULTI_RESP,
         command_status,
         sequence_number),
  message_id(message_id),
  no_unsuccess(no_unsuccess),
  unsuccess_smes(unsuccess_smes) {

}

SubmitMultiResp::SubmitMultiResp(const Header &p) : Header(p), no_unsuccess(0) {

}

SubmitMultiResp::SubmitMultiResp(const SubmitMultiResp &p) :
  Header(p),
  message_id(p.message_id),
  no_unsuccess(p.no_unsuccess),
  unsuccess_smes(p.unsuccess_smes) {

}

void SubmitMultiResp::calcCommandLength() {
  uint32 v(MIN_LENGTH + message_id.size());
  for(std::list<UnsuccessSme>::const_iterator it = unsuccess_smes.begin();
      it != unsuccess_smes.end();
      it++) {
    v += it->size();
  }
  setCommandLength(v);
}

SubmitMultiResp &SubmitMultiResp::operator =(const SubmitMultiResp &p) {
  if(*this != p) {
    Header::operator=(p);
    message_id = p.message_id;
    no_unsuccess = p.no_unsuccess;
    unsuccess_smes = p.unsuccess_smes;
  }
  return *this;
}

bool SubmitMultiResp::operator ==(const SubmitMultiResp &p) const {
  return(Header::operator==(p) &&
         message_id == p.message_id &&
         no_unsuccess == p.no_unsuccess &&
         unsuccess_smes == p.unsuccess_smes);
}

bool SubmitMultiResp::operator !=(const SubmitMultiResp &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
