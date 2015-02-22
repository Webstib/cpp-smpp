#ifndef SMPP_SUBMIT_MULTI_RESP_H
#define SMPP_SUBMIT_MULTI_RESP_H

#include "smpp/pdu/smpp_unsuccess_sme.h"
#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT SubmitMultiResp : public Header {
  std::string message_id;
  uint8 no_unsuccess;
  std::list<UnsuccessSme> unsuccess_smes;

public:

  SubmitMultiResp(
    uint32 command_status = 0,
    uint32 sequence_number = 0,
    const std::string &message_id = std::string(),
    uint8 no_unsuccess = 0,
    const std::list<UnsuccessSme> &unsuccess_smes = std::list<UnsuccessSme>());

  SubmitMultiResp(const Header &p);

  SubmitMultiResp(const SubmitMultiResp &p);

  virtual ~SubmitMultiResp() {

  }

  inline void setMessageId(const std::string &v) {
    message_id = v;
  }

  inline void setNoUnsuccess(uint8 v) {
    no_unsuccess = v;
  }

  inline void add(const UnsuccessSme &p) {
    unsuccess_smes.push_back(p);
  }

  inline const std::string &getMessageId() const {
    return message_id;
  }

  inline uint8 getNoUnsuccess() const {
    return no_unsuccess;
  }

  inline const std::list<UnsuccessSme> &getUnsuccessSmes() const {
    return unsuccess_smes;
  }

  inline void remove(const UnsuccessSme &v) {
    unsuccess_smes.remove(v);
  }

  void calcCommandLength();

  SubmitMultiResp &operator =(const SubmitMultiResp &p);

  bool operator ==(const SubmitMultiResp &p) const;

  bool operator !=(const SubmitMultiResp &p) const;

  const static uint32 MIN_LENGTH = 18;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_SUBMIT_MULTI_RESP_H
