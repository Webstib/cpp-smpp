#ifndef BINARY_DECODER_H
#define BINARY_DECODER_H

#include "smpp/pdu/smpp_bind_transmitter.h"
#include "smpp/pdu/smpp_bind_transmitter_resp.h"
#include "smpp/pdu/smpp_bind_receiver.h"
#include "smpp/pdu/smpp_bind_receiver_resp.h"
#include "smpp/pdu/smpp_bind_transceiver.h"
#include "smpp/pdu/smpp_bind_transceiver_resp.h"
#include "smpp/pdu/smpp_outbind.h"
#include "smpp/pdu/smpp_unbind.h"
#include "smpp/pdu/smpp_unbind_resp.h"
#include "smpp/pdu/smpp_generic_nack.h"
#include "smpp/pdu/smpp_submit_sm.h"
#include "smpp/pdu/smpp_submit_sm_resp.h"
#include "smpp/pdu/smpp_submit_multi.h"
#include "smpp/pdu/smpp_submit_multi_resp.h"
#include "smpp/pdu/smpp_deliver_sm.h"
#include "smpp/pdu/smpp_deliver_sm_resp.h"
#include "smpp/pdu/smpp_data_sm.h"
#include "smpp/pdu/smpp_data_sm_resp.h"
#include "smpp/pdu/smpp_query_sm.h"
#include "smpp/pdu/smpp_query_sm_resp.h"
#include "smpp/pdu/smpp_cancel_sm.h"
#include "smpp/pdu/smpp_cancel_sm_resp.h"
#include "smpp/pdu/smpp_replace_sm.h"
#include "smpp/pdu/smpp_replace_sm_resp.h"
#include "smpp/pdu/smpp_enquire_link.h"
#include "smpp/pdu/smpp_enquire_link_resp.h"
#include "smpp/pdu/smpp_alert_notification.h"

namespace smpp {

namespace decoders {

class SMPP_EXPORT BinaryDecoder {
  int8 *data;
  uint32 data_len;
  uint32 pos;

  void renewData(uint8 len);

public:

  BinaryDecoder(uint32 data_len = 0);

  ~BinaryDecoder();

  void clear();

  void addData(uint32 data_len);

  int8 *header() const;

  int8 *body() const;

  uint32 bodyLength() const;

  bool unpack(uint8 &p);

  bool unpack(uint16 &p);

  bool unpack(uint32 &p);

  bool unpack(int8 *&p, uint32 p_len);

  bool unpack(std::string &p);

  bool unpack(smpp::pdu::SmeMultiAddress &p);

  bool unpack(smpp::pdu::DistributionListAddress &p);

  bool unpack(smpp::pdu::MultiDestinationAddresses &p, uint8 length);

  bool unpack(smpp::pdu::UnsuccessSme &p);

  bool unpack(smpp::pdu::Tlv &p);

  bool unpack(smpp::pdu::TlvFields &p);

  bool unpack(smpp::pdu::Header &p);

  bool unpack(smpp::pdu::BindTransmitter &p, bool unpack_header = false);

  bool unpack(smpp::pdu::BindTransmitterResp &p, bool unpack_header = false);

  bool unpack(smpp::pdu::BindReceiver &p, bool unpack_header = false);

  bool unpack(smpp::pdu::BindReceiverResp &p, bool unpack_header = false);

  bool unpack(smpp::pdu::BindTransceiver &p, bool unpack_header = false);

  bool unpack(smpp::pdu::BindTransceiverResp &p, bool unpack_header = false);

  bool unpack(smpp::pdu::SubmitSm &p, bool unpack_header = false);

  bool unpack(smpp::pdu::SubmitSmResp &p, bool unpack_header = false);

  bool unpack(smpp::pdu::SubmitMulti &p, bool unpack_header = false);

  bool unpack(smpp::pdu::SubmitMultiResp &p, bool unpack_header = false);

  bool unpack(smpp::pdu::DeliverSm &p, bool unpack_header = false);

  bool unpack(smpp::pdu::DeliverSmResp &p, bool unpack_header = false);

  bool unpack(smpp::pdu::DataSm &p, bool unpack_header = false);

  bool unpack(smpp::pdu::DataSmResp &p, bool unpack_header = false);

  bool unpack(smpp::pdu::QuerySm &p, bool unpack_header = false);

  bool unpack(smpp::pdu::QuerySmResp &p, bool unpack_header = false);

  bool unpack(smpp::pdu::CancelSm &p, bool unpack_header = false);

  bool unpack(smpp::pdu::ReplaceSm &p, bool unpack_header = false);

  bool unpack(smpp::pdu::AlertNotification &p, bool unpack_header = false);
};

} // namespace decoders

} // namespace smpp


#endif // BINARY_DECODER_H
