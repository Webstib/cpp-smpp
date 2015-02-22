#ifndef SMPP_CLIENT_ASYNC_SESSION_P_H
#define SMPP_CLIENT_ASYNC_SESSION_P_H

#include "smpp/sessions/smpp_session_global.h"
#include "smpp/encoders/smpp_binary_encoder.h"
#include "smpp/decoders/smpp_binary_decoder.h"

namespace smpp {

namespace sessions {

class ClientAsyncSession;

class Message {
public:
  Message(const HeaderPtr &packet) : packet(packet), ttl(0) {

  }

  HeaderPtr packet;

  uint32 ttl;
};


class SocketMessage {
public:
  SocketMessage(int8 *data, uint32 len, const HeaderPtr &packet) :
    data(data), len(len), packet(packet) {

  }

  int8 *data;

  uint32 len;

  HeaderPtr packet;
};

class ClientAsyncSessionPrivate {
public:
  ClientAsyncSessionPrivate(
    ClientAsyncSession *parent,
    boost::asio::io_service &service,
    int32 max_in_speed,
    int32 max_out_speed,
    int32 session_init_time,
    int32 enquire_link_time,
    int32 inactivity_time,
    int32 transaction_time,
    int32 max_in_trans,
    int32 max_out_trans);

  ~ClientAsyncSessionPrivate();

  void sendPacket(const HeaderPtr &packet);

  void openSession(const boost::asio::ip::tcp::endpoint &ep);

  void closeSession();

  void readHeader();

  void readBody(const smpp::pdu::Header &packet);

  void readHeaderHandler(const boost::system::error_code &ec);

  void readBodyHandler(const boost::system::error_code &ec,
                       const smpp::pdu::Header &packet);

  void sendGenericNackErrorDecode(uint32 cs, uint32 sn, StatusMessage status);

  void setState(SessionStates state);

  void sessionActivity();

  void removeAllInTransactions(StatusMessage status);

  void removeAllOutTransactions(StatusMessage status);

  void clearAllQueue(StatusMessage status);

  void procError(const boost::system::error_code &ec);

  void procSessionInitTimeout();

  void procEnquireLinkTimeout();

  void procInactivityTimeout();

  void processInTransactions();

  void processOutTransactions();

  bool checkInSpeed();

  bool checkOutSpeed();

  bool startInTransaction(const HeaderPtr &packet);

  bool stopInTransactions(const HeaderPtr &packet);

  bool startOutTransaction(const HeaderPtr &packet);

  bool stopOutTransactions(const HeaderPtr &packet);

  void writeSocketData(const int8 *bytes, uint32 len, const HeaderPtr &packet);

  void writeSocketDataHandle(const boost::system::error_code &ec);

  void startTimer();

  void stopTimer();

  uint32 session_init_time;
  uint32 enquire_link_time;
  uint32 inactivity_time;
  uint32 transaction_time;

  uint32 now_in_speed;
  uint32 now_out_speed;
  uint32 in_speed_it;
  uint32 out_speed_it;

  uint32 session_init_timer;
  uint32 enquire_link_timer;
  uint32 inactivity_timer;

  boost::system::error_code error_code;

  std::list<Message *> in_transactions;
  std::list<Message *> out_transactions;

  uint32 max_in_speed;
  uint32 max_out_speed;
  uint32 max_in_trans;
  uint32 max_out_trans;

  SessionStates now_state;

  boost::asio::io_service &io_service;

  boost::asio::ip::tcp::socket socket;

  boost::asio::deadline_timer timer;

  smpp::encoders::BinaryEncoder encoder;

  smpp::decoders::BinaryDecoder decoder;

  std::list<SocketMessage *> write_socket_messages;

  voidSig opened_sess_sig;

  voidSig closed_sess_sig;

  headerSig receive_packet_sig;

  statSig out_status_sig;

  statSig in_status_sig;

  stateSig state_changed_sig;

  voidSig sess_init_timeout_sig;

  voidSig el_timeout_sig;

  voidSig inact_timeout_sig;

  errorSig error_sig;

  ClientAsyncSession *parent;
};

} // namespace sessions

} // namespace smpp

#endif // SMPP_CLIENT_ASYNC_SESSION_P_H
