#ifndef SMPP_SERVER_ASYNC_SESSION_P_H
#define SMPP_SERVER_ASYNC_SESSION_P_H

#include <QtCore/QThread>
#include <QtCore/QTimerEvent>
#include <QtCore/QSharedDataPointer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include "smpp/sessions/smpp_session_global.h"

namespace smpp {

namespace sessions {

class ServerAsyncSession;

class ServerAsyncSessionData : public QSharedData {
public:
  ServerAsyncSessionData(
    QObject *parent,
    int sock_descriptor,
    int max_in_speed,
    int max_out_speed,
    int session_init_time,
    int enquire_link_time,
    int inactivity_time,
    int transaction_time,
    int max_in_trans,
    int max_out_trans,
    bool threaded);

  ~ServerAsyncSessionData();

  class Message {
  public:
    Message(const smpp::pdu::Header &header) :
      header(header), timer(0) {

    }

    ~Message() {}

    smpp::pdu::Header header;
    int timer;
  };

  static const int sock_oper_timeout = 20000;

  int session_init_time;
  int enquire_link_time;
  int inactivity_time;
  int transaction_time;

  int now_in_speed;
  int now_out_speed;
  int in_speed_it;
  int out_speed_it;

  int session_init_timer;
  int enquire_link_timer;
  int inactivity_timer;

  int main_timer_id;

  std::string error_message;
  ErrorCode error_code;

  std::list<Message *> in_transactions;
  std::list<Message *> out_transactions;

  int max_in_speed;
  int max_out_speed;
  int max_in_trans;
  int max_out_trans;

  SessionStates now_state;

  int sock_descriptor;
  QTcpSocket *socket;

  QHostAddress peer_address;
  QHostAddress local_address;
  uint16 peer_port;
  uint16 local_port;

  QThread *thread;
};

class ServerAsyncSessionPrivate : public QObject {
  Q_OBJECT
public:
  ServerAsyncSessionPrivate(
    ServerAsyncSession *parent,
    int sock_descriptor,
    int max_in_speed, // sms / sec
    int max_out_speed, // sms / sec
    int session_init_time,
    int enquire_link_time,
    int inactivity_time,
    int transaction_time,
    int max_in_trans,
    int max_out_trans,
    bool threaded);

  ~ServerAsyncSessionPrivate();

public slots:

  void send(const smpp::pdu::AlertNotification &p);

  void send(const smpp::pdu::BindTransmitterResp &p);

  void send(const smpp::pdu::BindReceiverResp &p);

  void send(const smpp::pdu::BindTransceiverResp &p);

  void send(const smpp::pdu::UnbindResp &p);

  void send(const smpp::pdu::Outbind &p);

  void send(const smpp::pdu::GenericNack &p);

  void send(const smpp::pdu::SubmitSmResp &p);

  void send(const smpp::pdu::SubmitMultiResp &p);

  void send(const smpp::pdu::DeliverSm &p);

  void send(const smpp::pdu::DataSm &p);

  void send(const smpp::pdu::DataSmResp &p);

  void send(const smpp::pdu::QuerySmResp &p);

  void send(const smpp::pdu::ReplaceSmResp &p);

  void send(const smpp::pdu::CancelSmResp &p);

  void send(const smpp::pdu::EnquireLink &p);

  void send(const smpp::pdu::EnquireLinkResp &p);

  void closeSession();

  void socketConnected();

  void socketError(QAbstractSocket::SocketError error);

  void socketReadyRead();

signals:

  void sigCloseSession();

  void sigSend(const smpp::pdu::AlertNotification &p);

  void sigSend(const smpp::pdu::BindTransmitterResp &p);

  void sigSend(const smpp::pdu::BindReceiverResp &p);

  void sigSend(const smpp::pdu::BindTransceiverResp &p);

  void sigSend(const smpp::pdu::UnbindResp &p);

  void sigSend(const smpp::pdu::Outbind &p);

  void sigSend(const smpp::pdu::GenericNack &p);

  void sigSend(const smpp::pdu::SubmitSmResp &p);

  void sigSend(const smpp::pdu::SubmitMultiResp &p);

  void sigSend(const smpp::pdu::DeliverSm &p);

  void sigSend(const smpp::pdu::DataSm &p);

  void sigSend(const smpp::pdu::DataSmResp &p);

  void sigSend(const smpp::pdu::QuerySmResp &p);

  void sigSend(const smpp::pdu::ReplaceSmResp &p);

  void sigSend(const smpp::pdu::CancelSmResp &p);

  void sigSend(const smpp::pdu::EnquireLink &p);

  void sigSend(const smpp::pdu::EnquireLinkResp &p);

protected:

  void setState(SessionStates state);

  void timerEvent(QTimerEvent *event);

private:

  void sessionActivity();

  void removeAllInTransactions(StatusMessage status);

  void removeAllOutTransactions(StatusMessage status);

  void clearAllQueue(StatusMessage status);

  void procError(ErrorCode code, const std::string &message);

  void procSessionInitTimeout();

  void procEnquireLinkTimeout();

  void procInactivityTimeout();

  void processInTransactions();

  void processOutTransactions();

  bool checkInSpeed();

  bool checkOutSpeed();

  bool startInTransaction(const smpp::pdu::Header &header);

  bool stopInTransactions(const smpp::pdu::Header &header);

  void deleteInTransaction(const smpp::pdu::Header &header);

  bool startOutTransaction(const smpp::pdu::Header &header);

  bool stopOutTransactions(const smpp::pdu::Header &header);

  void deleteOutTransaction(const smpp::pdu::Header &header);

  bool writeSocketData(const QByteArray &bytes);

  QSharedDataPointer<ServerAsyncSessionData> d;

  friend class ServerAsyncSession;

  ServerAsyncSession *parent;
};

} // namespace sessions

} // namespace smpp

#endif // SMPP_SERVER_ASYNC_SESSION_P_H
