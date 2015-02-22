#include <QtCore/QTimer>
#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/encoders/smpp_binary_encoder.h"
#include "smpp/decoders/smpp_binary_decoder.h"
#include "smpp/sessions/smpp_server_async_session.h"
#include "smpp/sessions/smpp_server_async_session_p.h"

namespace smpp {

namespace sessions {

using namespace pdu;
using namespace decoders;
using namespace encoders;

ServerAsyncSessionData::ServerAsyncSessionData(
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
  bool threaded) :
  session_init_time(session_init_time),
  enquire_link_time(enquire_link_time),
  inactivity_time(inactivity_time),
  transaction_time(transaction_time),
  now_in_speed(0),
  now_out_speed(0),
  in_speed_it(0),
  out_speed_it(0),
  session_init_timer(-1),
  enquire_link_timer(-1),
  inactivity_timer(-1),
  main_timer_id(-1),
  error_code(NO_ERROR),
  max_in_speed(max_in_speed),
  max_out_speed(max_out_speed),
  max_in_trans(max_in_trans),
  max_out_trans(max_out_trans),
  now_state(CLOSED),
  sock_descriptor(sock_descriptor),
  socket(new QTcpSocket(parent)),
  peer_port(0),
  local_port(0),
  thread(0) {
  if(QMetaType::type("QHostAddress") == 0)
    qRegisterMetaType<QHostAddress>("QHostAddress");
  if(QMetaType::type("smpp::sessions::StatusMessage") == 0)
    qRegisterMetaType<smpp::sessions::StatusMessage>(
      "smpp::sessions::StatusMessage");
  if(QMetaType::type("smpp::sessions::ErrorCode") == 0)
    qRegisterMetaType<smpp::sessions::ErrorCode>("smpp::sessions::ErrorCode");
  if(QMetaType::type("QAbstractSocket::SocketError") == 0)
    qRegisterMetaType<QAbstractSocket::SocketError>(
      "QAbstractSocket::SocketError");
  if(QMetaType::type("smpp::sessions::SessionStates") == 0)
    qRegisterMetaType<smpp::sessions::SessionStates>(
      "smpp::sessions::SessionStates");
  if(QMetaType::type("smpp::pdu::Header") == 0)
    qRegisterMetaType<smpp::pdu::Header>("smpp::pdu::Header");
  if(QMetaType::type("smpp::pdu::AlertNotification") == 0)
    qRegisterMetaType<smpp::pdu::AlertNotification>(
      "smpp::pdu::AlertNotification");
  if(QMetaType::type("smpp::pdu::BindTransmitter") == 0)
    qRegisterMetaType<smpp::pdu::BindTransmitter>(
      "smpp::pdu::BindTransmitter");
  if(QMetaType::type("smpp::pdu::BindTransmitterResp") == 0)
    qRegisterMetaType<smpp::pdu::BindTransmitterResp>(
      "smpp::pdu::BindTransmitterResp");
  if(QMetaType::type("smpp::pdu::BindReceiver") == 0)
    qRegisterMetaType<smpp::pdu::BindReceiver>("smpp::pdu::BindReceiver");
  if(QMetaType::type("smpp::pdu::BindReceiverResp") == 0)
    qRegisterMetaType<smpp::pdu::BindReceiverResp>(
      "smpp::pdu::BindReceiverResp");
  if(QMetaType::type("smpp::pdu::BindTransceiver") == 0)
    qRegisterMetaType<smpp::pdu::BindTransceiver>(
      "smpp::pdu::BindTransceiver");
  if(QMetaType::type("smpp::pdu::BindTransceiverResp") == 0)
    qRegisterMetaType<smpp::pdu::BindTransceiverResp>(
      "smpp::pdu::BindTransceiverResp");
  if(QMetaType::type("smpp::pdu::Unbind") == 0)
    qRegisterMetaType<smpp::pdu::Unbind>("smpp::pdu::Unbind");
  if(QMetaType::type("smpp::pdu::UnbindResp") == 0)
    qRegisterMetaType<smpp::pdu::UnbindResp>("smpp::pdu::UnbindResp");
  if(QMetaType::type("smpp::pdu::Outbind") == 0)
    qRegisterMetaType<smpp::pdu::Outbind>("smpp::pdu::Outbind");
  if(QMetaType::type("smpp::pdu::GenericNack") == 0)
    qRegisterMetaType<smpp::pdu::GenericNack>("smpp::pdu::GenericNack");
  if(QMetaType::type("smpp::pdu::SubmitSm") == 0)
    qRegisterMetaType<smpp::pdu::SubmitSm>("smpp::pdu::SubmitSm");
  if(QMetaType::type("smpp::pdu::SubmitSmResp") == 0)
    qRegisterMetaType<smpp::pdu::SubmitSmResp>("smpp::pdu::SubmitSmResp");
  if(QMetaType::type("smpp::pdu::SubmitMulti") == 0)
    qRegisterMetaType<smpp::pdu::SubmitMulti>("smpp::pdu::SubmitMulti");
  if(QMetaType::type("smpp::pdu::SubmitMultiResp") == 0)
    qRegisterMetaType<smpp::pdu::SubmitMultiResp>(
      "smpp::pdu::SubmitMultiResp");
  if(QMetaType::type("smpp::pdu::DeliverSm") == 0)
    qRegisterMetaType<smpp::pdu::DeliverSm>("smpp::pdu::DeliverSm");
  if(QMetaType::type("smpp::pdu::DeliverSmResp") == 0)
    qRegisterMetaType<smpp::pdu::DeliverSmResp>("smpp::pdu::DeliverSmResp");
  if(QMetaType::type("smpp::pdu::DataSm") == 0)
    qRegisterMetaType<smpp::pdu::DataSm>("smpp::pdu::DataSm");
  if(QMetaType::type("smpp::pdu::DataSmResp") == 0)
    qRegisterMetaType<smpp::pdu::DataSmResp>("smpp::pdu::DataSmResp");
  if(QMetaType::type("smpp::pdu::QuerySm") == 0)
    qRegisterMetaType<smpp::pdu::QuerySm>("smpp::pdu::QuerySm");
  if(QMetaType::type("smpp::pdu::QuerySmResp") == 0)
    qRegisterMetaType<smpp::pdu::QuerySmResp>("smpp::pdu::QuerySmResp");
  if(QMetaType::type("smpp::pdu::ReplaceSm") == 0)
    qRegisterMetaType<smpp::pdu::ReplaceSm>("smpp::pdu::ReplaceSm");
  if(QMetaType::type("smpp::pdu::ReplaceSmResp") == 0)
    qRegisterMetaType<smpp::pdu::ReplaceSmResp>("smpp::pdu::ReplaceSmResp");
  if(QMetaType::type("smpp::pdu::EnquireLink") == 0)
    qRegisterMetaType<smpp::pdu::EnquireLink>("smpp::pdu::EnquireLink");
  if(QMetaType::type("smpp::pdu::EnquireLinkResp") == 0)
    qRegisterMetaType<smpp::pdu::EnquireLinkResp>(
      "smpp::pdu::EnquireLinkResp");
  if(threaded) {
    thread = new QThread(parent);
    parent->moveToThread(thread);
  }

  if(max_in_speed < 1) {
    this->max_in_speed = DEFAULT_IN_SPEED;
  }
  if(max_out_speed < 1) {
    this->max_out_speed = DEFAULT_OUT_SPEED;
  }
}

ServerAsyncSessionData::~ServerAsyncSessionData() {
  if(socket) {
    socket->deleteLater();
    socket = 0;
  }
  if(thread) {
    delete thread;
    thread = 0;
  }
}

ServerAsyncSessionPrivate::ServerAsyncSessionPrivate(
  ServerAsyncSession *parent,
  int sock_descriptor,
  int max_in_speed, // msec
  int max_out_speed, // msec
  int session_init_time,
  int enquire_link_time,
  int inactivity_time,
  int transaction_time,
  int max_in_trans,
  int max_out_trans,
  bool threaded) :
  QObject(0),
  d(new ServerAsyncSessionData(this,
                               sock_descriptor,
                               max_in_speed,
                               max_out_speed,
                               session_init_time,
                               enquire_link_time,
                               inactivity_time,
                               transaction_time,
                               max_in_trans,
                               max_out_trans,
                               threaded)),
  parent(parent) {

  connect(this, SIGNAL(sigCloseSession()), this, SLOT(closeSession()));
  connect(this, SIGNAL(sigSend(const smpp::pdu::AlertNotification &)),
          this, SLOT(send(const smpp::pdu::AlertNotification &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::BindTransmitterResp &)),
          this, SLOT(send(const smpp::pdu::BindTransmitterResp &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::BindReceiverResp &)),
          this, SLOT(send(const smpp::pdu::BindReceiverResp &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::BindTransceiverResp &)),
          this, SLOT(send(const smpp::pdu::BindTransceiverResp &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::UnbindResp &)),
          this, SLOT(send(const smpp::pdu::UnbindResp &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::Outbind &)),
          this, SLOT(send(const smpp::pdu::Outbind &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::GenericNack &)),
          this, SLOT(send(const smpp::pdu::GenericNack &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::SubmitSmResp &)),
          this, SLOT(send(const smpp::pdu::SubmitSmResp &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::SubmitMultiResp &)),
          this, SLOT(send(const smpp::pdu::SubmitMultiResp &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::DeliverSm &)),
          this, SLOT(send(const smpp::pdu::DeliverSm &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::DataSm &)),
          this, SLOT(send(const smpp::pdu::DataSm &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::DataSmResp &)),
          this, SLOT(send(const smpp::pdu::DataSmResp &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::QuerySmResp &)),
          this, SLOT(send(const smpp::pdu::QuerySmResp &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::ReplaceSmResp &)),
          this, SLOT(send(const smpp::pdu::ReplaceSmResp &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::EnquireLink &)),
          this, SLOT(send(const smpp::pdu::EnquireLink &)));
  connect(this, SIGNAL(sigSend(const smpp::pdu::EnquireLinkResp &)),
          this, SLOT(send(const smpp::pdu::EnquireLinkResp &)));

  connect(d->socket, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(socketError(QAbstractSocket::SocketError)));
  connect(d->socket, SIGNAL(readyRead()),
          this, SLOT(socketReadyRead()));

  QTimer::singleShot(0, this, SLOT(socketConnected()));
}

ServerAsyncSessionPrivate::~ServerAsyncSessionPrivate() {
  clearAllQueue(NOTVALIDSTATE);
  parent = 0;
}

void ServerAsyncSessionPrivate::socketConnected() {
  if(!d->socket->setSocketDescriptor(d->sock_descriptor)) {
    qFatal("Not set socket descriptor to ServerAsyncSessionPrivate");
  }

  d->peer_address = d->socket->peerAddress();
  d->local_address = d->socket->localAddress();
  d->peer_port = d->socket->peerPort();
  d->local_port = d->socket->localPort();

  setState(OPEN);
}

void ServerAsyncSessionPrivate::socketError(
  QAbstractSocket::SocketError error) {
  if(error == QAbstractSocket::RemoteHostClosedError &&
     (d->now_state == OPEN ||
      d->now_state == OUTBOUND ||
      d->now_state == UNBOUND)) {
    setState(CLOSED);
  } else {
    procError(NET_SOCK_ERR, d->socket->errorString());
  }
}

void ServerAsyncSessionPrivate::socketReadyRead() {
  while(d->socket->bytesAvailable() > 0) {
    QByteArray bytes;
    uint32 read_bytes(0);

    while(read_bytes != Header::HEADER_LENGTH) {
      bytes += d->socket->read(Header::HEADER_LENGTH - read_bytes);

      read_bytes = bytes.count();

      if(read_bytes != Header::HEADER_LENGTH) {
        if(!d->socket->waitForReadyRead(
          ServerAsyncSessionData::sock_oper_timeout)) {
          return procError(RD_SOCK_ERR, "Not read bytes for decode header");
        }
      }
    }

    BinaryDecoder decoder(bytes);

    bytes.clear();
    read_bytes = 0;

    Header header;

    decoder.decode(header);

    emit parent->inStatus(header, RECEIVED);

    if(header.getCommandLength() < Header::HEADER_LENGTH) {
      emit parent->inStatus(header, ERRORDECODE);
      send(GenericNack(parameters::command_status::ESME_RINVCMDLEN,
                       header.getSequenceNumber()));
      return;
    }

    uint32 body_length(header.getCommandLength() - Header::HEADER_LENGTH);

    while(read_bytes != body_length) {
      bytes += d->socket->read(body_length - read_bytes);

      read_bytes = bytes.count();

      if(read_bytes != body_length) {
        if(!d->socket->waitForReadyRead(
          ServerAsyncSessionData::sock_oper_timeout)) {
          return procError(RD_SOCK_ERR, "Not read bytes for decode body");
          return;
        }
      }
    }

    if(read_bytes > 0) {
      decoder.addBody(bytes);
      bytes.clear();
    }

    switch(header.getCommandId()) {
      case parameters::command_id::BIND_TRANSMITTER: {
        BindTransmitter p(header);
        if(decoder.decode(p)) {
          if(d->now_state == OPEN || d->now_state == OUTBOUND) {
            if(startInTransaction(p)) {
              emit parent->receive(p);
            } else {
              emit parent->inStatus(header, TRANSFULL);
              BindTransmitterResp p_resp(
                parameters::command_status::ESME_RMSGQFUL,
                header.getSequenceNumber());
              BinaryEncoder encoder;
              encoder << p_resp;
              writeSocketData(encoder);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
            BindTransmitterResp p_resp(
              parameters::command_status::ESME_RINVBNDSTS,
              header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          GenericNack gn(parameters::command_status::ESME_RINVCMDLEN,
                         header.getSequenceNumber());
          BinaryEncoder encoder;
          encoder << gn;
          writeSocketData(encoder);
        }
        break;
      }
      case parameters::command_id::BIND_RECEIVER: {
        BindReceiver p(header);
        if(decoder.decode(p)) {
          if(d->now_state == OPEN || d->now_state == OUTBOUND) {
            if(startInTransaction(p)) {
              emit parent->receive(p);
            } else {
              emit parent->inStatus(header, TRANSFULL);
              BindReceiverResp p_resp(
                parameters::command_status::ESME_RMSGQFUL,
                header.getSequenceNumber());
              BinaryEncoder encoder;
              encoder << p_resp;
              writeSocketData(encoder);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
            BindReceiverResp p_resp(
              parameters::command_status::ESME_RINVBNDSTS,
              header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          GenericNack gn(parameters::command_status::ESME_RINVCMDLEN,
                         header.getSequenceNumber());
          BinaryEncoder encoder;
          encoder << gn;
          writeSocketData(encoder);
        }
        break;
      }
      case parameters::command_id::BIND_TRANSCEIVER: {
        BindTransceiver p(header);
        if(decoder.decode(p)) {
          if(d->now_state == OPEN || d->now_state == OUTBOUND) {
            if(startInTransaction(p)) {
              emit parent->receive(p);
            } else {
              emit parent->inStatus(header, TRANSFULL);
              BindTransceiverResp p_resp(
                parameters::command_status::ESME_RMSGQFUL,
                header.getSequenceNumber());
              BinaryEncoder encoder;
              encoder << p_resp;
              writeSocketData(encoder);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
            BindTransceiverResp p_resp(
              parameters::command_status::ESME_RINVBNDSTS,
              header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          GenericNack gn(parameters::command_status::ESME_RINVCMDLEN,
                         header.getSequenceNumber());
          BinaryEncoder encoder;
          encoder << gn;
          writeSocketData(encoder);
        }
        break;
      }
      case parameters::command_id::UNBIND: {
        if(d->now_state == BOUND_TX ||
           d->now_state == BOUND_RX ||
           d->now_state == BOUND_TRX) {
          if(startInTransaction(header)) {
            emit parent->receive(Unbind(header));
            if(header.getCommandStatus() ==
               parameters::command_status::ESME_ROK) {
              setState(UNBOUND);
            }
          } else {
            emit parent->inStatus(header, TRANSFULL);
            UnbindResp p_resp(parameters::command_status::ESME_RMSGQFUL,
                              header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, NOTVALIDSTATE);
          UnbindResp p_resp(parameters::command_status::ESME_RINVBNDSTS,
                            header.getSequenceNumber());
          BinaryEncoder encoder;
          encoder << p_resp;
          writeSocketData(encoder);
        }
        break;
      }
      case parameters::command_id::GENERIC_NACK: {
        if(d->now_state != CLOSED) {
          emit parent->receive(GenericNack(header));
        } else {
          emit parent->inStatus(header, NOTVALIDSTATE);
        }
        break;
      }
      case parameters::command_id::SUBMIT_SM: {
        SubmitSm p(header);
        if(decoder.decode(p)) {
          if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
            if(checkInSpeed()) {
              if(startInTransaction(p)) {
                emit parent->receive(p);
              } else {
                emit parent->inStatus(header, TRANSFULL);
                SubmitSmResp p_resp(parameters::command_status::ESME_RMSGQFUL,
                                    header.getSequenceNumber());
                BinaryEncoder encoder;
                encoder << p_resp;
                writeSocketData(encoder);
              }
            } else {
              emit parent->inStatus(header, MAXSPEED);
              SubmitSmResp p_resp(parameters::command_status::ESME_RTHROTTLED,
                                  header.getSequenceNumber());
              BinaryEncoder encoder;
              encoder << p_resp;
              writeSocketData(encoder);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
            SubmitSmResp p_resp(parameters::command_status::ESME_RINVBNDSTS,
                                header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          GenericNack gn(parameters::command_status::ESME_RINVCMDLEN,
                         header.getSequenceNumber());
          BinaryEncoder encoder;
          encoder << gn;
          writeSocketData(encoder);
        }
        break;
      }
      case parameters::command_id::SUBMIT_MULTI: {
        SubmitMulti p(header);
        if(decoder.decode(p)) {
          if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
            if(checkInSpeed()) {
              if(startInTransaction(p)) {
                emit parent->receive(p);
              } else {
                emit parent->inStatus(header, TRANSFULL);
                SubmitMultiResp p_resp(
                  parameters::command_status::ESME_RMSGQFUL,
                  header.getSequenceNumber());
                BinaryEncoder encoder;
                encoder << p_resp;
                writeSocketData(encoder);
              }
            } else {
              emit parent->inStatus(header, MAXSPEED);
              SubmitMultiResp p_resp(
                parameters::command_status::ESME_RTHROTTLED,
                header.getSequenceNumber());
              BinaryEncoder encoder;
              encoder << p_resp;
              writeSocketData(encoder);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
            SubmitMultiResp p_resp(parameters::command_status::ESME_RINVBNDSTS,
                                   header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          GenericNack gn(parameters::command_status::ESME_RINVCMDLEN,
                         header.getSequenceNumber());
          BinaryEncoder encoder;
          encoder << gn;
          writeSocketData(encoder);
        }
        break;
      }
      case parameters::command_id::DELIVER_SM_RESP: {
        DeliverSmResp p(header);
        if(decoder.decode(p)) {
          if(d->now_state == BOUND_RX || d->now_state == BOUND_TRX) {
            if(stopOutTransactions(p)) {
              emit parent->receive(p);
            } else {
              emit parent->inStatus(header, NOTFOUNDTRANS);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          send(GenericNack(parameters::command_status::ESME_RINVCMDLEN,
                           header.getSequenceNumber()));
        }
        break;
      }
      case parameters::command_id::DATA_SM: {
        DataSm p(header);
        if(decoder.decode(p)) {
          if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
            if(checkInSpeed()) {
              if(startInTransaction(p)) {
                emit parent->receive(p);
              } else {
                emit parent->inStatus(header, TRANSFULL);
                DataSmResp p_resp(parameters::command_status::ESME_RMSGQFUL,
                                  header.getSequenceNumber());
                BinaryEncoder encoder;
                encoder << p_resp;
                writeSocketData(encoder);
              }
            } else {
              emit parent->inStatus(header, MAXSPEED);
              DataSmResp p_resp(parameters::command_status::ESME_RTHROTTLED,
                                header.getSequenceNumber());
              BinaryEncoder encoder;
              encoder << p_resp;
              writeSocketData(encoder);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
            DataSmResp p_resp(parameters::command_status::ESME_RINVBNDSTS,
                              header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          GenericNack gn(parameters::command_status::ESME_RINVCMDLEN,
                         header.getSequenceNumber());
          BinaryEncoder encoder;
          encoder << gn;
          writeSocketData(encoder);
        }
        break;
      }
      case parameters::command_id::DATA_SM_RESP: {
        DataSmResp p(header);
        if(decoder.decode(p)) {
          if(d->now_state == BOUND_RX || d->now_state == BOUND_TRX) {
            if(stopOutTransactions(p)) {
              emit parent->receive(p);
            } else {
              emit parent->inStatus(header, NOTFOUNDTRANS);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          send(GenericNack(parameters::command_status::ESME_RINVCMDLEN,
                           header.getSequenceNumber()));
        }
        break;
      }
      case parameters::command_id::QUERY_SM: {
        QuerySm p(header);
        if(decoder.decode(p)) {
          if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
            if(startInTransaction(p)) {
              emit parent->receive(p);
            } else {
              emit parent->inStatus(header, TRANSFULL);
              QuerySmResp p_resp(parameters::command_status::ESME_RMSGQFUL,
                                 header.getSequenceNumber());
              BinaryEncoder encoder;
              encoder << p_resp;
              writeSocketData(encoder);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
            QuerySmResp p_resp(parameters::command_status::ESME_RINVBNDSTS,
                               header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          GenericNack gn(parameters::command_status::ESME_RINVCMDLEN,
                         header.getSequenceNumber());
          BinaryEncoder encoder;
          encoder << gn;
          writeSocketData(encoder);
        }
        break;
      }
      case parameters::command_id::REPLACE_SM: {
        ReplaceSm p(header);
        if(decoder.decode(p)) {
          if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
            if(startInTransaction(p)) {
              emit parent->receive(p);
            } else {
              emit parent->inStatus(header, TRANSFULL);
              ReplaceSmResp p_resp(parameters::command_status::ESME_RMSGQFUL,
                                   header.getSequenceNumber());
              BinaryEncoder encoder;
              encoder << p_resp;
              writeSocketData(encoder);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
            ReplaceSmResp p_resp(parameters::command_status::ESME_RINVBNDSTS,
                                 header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          GenericNack gn(parameters::command_status::ESME_RINVCMDLEN,
                         header.getSequenceNumber());
          BinaryEncoder encoder;
          encoder << gn;
          writeSocketData(encoder);
        }
        break;
      }
      case parameters::command_id::CANCEL_SM: {
        CancelSm p(header);
        if(decoder.decode(p)) {
          if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
            if(startInTransaction(p)) {
              emit parent->receive(p);
            } else {
              emit parent->inStatus(header, TRANSFULL);
              CancelSmResp p_resp(parameters::command_status::ESME_RMSGQFUL,
                                  header.getSequenceNumber());
              BinaryEncoder encoder;
              encoder << p_resp;
              writeSocketData(encoder);
            }
          } else {
            emit parent->inStatus(header, NOTVALIDSTATE);
            CancelSmResp p_resp(parameters::command_status::ESME_RINVBNDSTS,
                                header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, ERRORDECODE);
          GenericNack gn(parameters::command_status::ESME_RINVCMDLEN,
                         header.getSequenceNumber());
          BinaryEncoder encoder;
          encoder << gn;
          writeSocketData(encoder);
        }
        break;
      }
      case parameters::command_id::ENQUIRE_LINK: {
        if(d->now_state != CLOSED) {
          if(startInTransaction(header)) {
            emit parent->receive(EnquireLink(header));
          } else {
            emit parent->inStatus(header, TRANSFULL);
            EnquireLinkResp p_resp(parameters::command_status::ESME_RMSGQFUL,
                                   header.getSequenceNumber());
            BinaryEncoder encoder;
            encoder << p_resp;
            writeSocketData(encoder);
          }
        } else {
          emit parent->inStatus(header, NOTVALIDSTATE);
        }
        break;
      }
      case parameters::command_id::ENQUIRE_LINK_RESP: {
        if(d->now_state != CLOSED) {
          if(stopOutTransactions(header)) {
            emit parent->receive(EnquireLinkResp(header));
          } else {
            emit parent->inStatus(header, NOTFOUNDTRANS);
          }
        } else {
          emit parent->inStatus(header, NOTVALIDSTATE);
        }
        break;
      }
      default: {
        emit parent->inStatus(header, ERRORCID);
        send(GenericNack(parameters::command_status::ESME_RINVCMDID,
                         header.getSequenceNumber()));
      }
    }
  }
}

void ServerAsyncSessionPrivate::closeSession() {
  if(d->socket->state() == QAbstractSocket::ConnectedState) {
    d->socket->disconnectFromHost();
    if(d->socket->state() == QAbstractSocket::UnconnectedState ||
       d->socket->waitForDisconnected(
         ServerAsyncSessionData::sock_oper_timeout)) {
      setState(CLOSED);
    } else {
      procError(DISC_SOCK_ERR,
                std::string("Timeout disconnect from %1:%2")
                  .arg(d->peer_address.toString())
                  .arg(d->peer_port));
    }
  } else {
    setState(CLOSED);
  }
}

void ServerAsyncSessionPrivate::send(const AlertNotification &p) {
  if(p.getCommandId() == parameters::command_id::ALERT_NOTIFICATION) {
    if(d->now_state == BOUND_RX || d->now_state == BOUND_TRX) {
      BinaryEncoder encoder;
      encoder << p;
      if(writeSocketData(encoder)) {
        emit parent->outStatus(p, SENDED);
      } else {
        emit parent->outStatus(p, NOTSENDED);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const BindTransmitterResp &p) {
  if(p.getCommandId() == parameters::command_id::BIND_TRANSMITTER_RESP) {
    if(d->now_state == OPEN || d->now_state == OUTBOUND) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
          if(p.getCommandStatus() == parameters::command_status::ESME_ROK) {
            setState(BOUND_TX);
          }
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const BindReceiverResp &p) {
  if(p.getCommandId() == parameters::command_id::BIND_RECEIVER_RESP) {
    if(d->now_state == OPEN || d->now_state == OUTBOUND) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
          if(p.getCommandStatus() == parameters::command_status::ESME_ROK) {
            setState(BOUND_RX);
          }
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const BindTransceiverResp &p) {
  if(p.getCommandId() == parameters::command_id::BIND_TRANSCEIVER_RESP) {
    if(d->now_state == OPEN || d->now_state == OUTBOUND) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
          if(p.getCommandStatus() == parameters::command_status::ESME_ROK) {
            setState(BOUND_TRX);
          }
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const UnbindResp &p) {
  if(p.getCommandId() == parameters::command_id::UNBIND_RESP) {
    if(d->now_state == UNBOUND) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
          closeSession();
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const Outbind &p) {
  if(p.getCommandId() == parameters::command_id::OUTBIND) {
    if(d->now_state == OPEN) {
      BinaryEncoder encoder;
      encoder << p;
      if(writeSocketData(encoder)) {
        emit parent->outStatus(p, SENDED);
        if(p.getCommandStatus() == parameters::command_status::ESME_ROK) {
          setState(OUTBOUND);
        }
      } else {
        emit parent->outStatus(p, NOTSENDED);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const GenericNack &p) {
  if(p.getCommandId() == parameters::command_id::GENERIC_NACK) {
    if(d->now_state != CLOSED) {
      BinaryEncoder encoder;
      encoder << p;
      if(writeSocketData(encoder)) {
        emit parent->outStatus(p, SENDED);
      } else {
        emit parent->outStatus(p, NOTSENDED);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const SubmitSmResp &p) {
  if(p.getCommandId() == parameters::command_id::SUBMIT_SM_RESP) {
    if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const SubmitMultiResp &p) {
  if(p.getCommandId() == parameters::command_id::SUBMIT_MULTI_RESP) {
    if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const DeliverSm &p) {
  if(p.getCommandId() == parameters::command_id::DELIVER_SM) {
    if(d->now_state == BOUND_RX || d->now_state == BOUND_TRX) {
      if(checkOutSpeed()) {
        if(startOutTransaction(p)) {
          BinaryEncoder encoder;
          encoder << p;
          if(writeSocketData(encoder)) {
            emit parent->outStatus(p, SENDED);
          } else {
            deleteOutTransaction(p);
            emit parent->outStatus(p, NOTSENDED);
          }
        } else {
          emit parent->outStatus(p, TRANSFULL);
        }
      } else {
        emit parent->outStatus(p, MAXSPEED);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const DataSm &p) {
  if(p.getCommandId() == parameters::command_id::DATA_SM) {
    if(d->now_state == BOUND_RX || d->now_state == BOUND_TRX) {
      if(checkOutSpeed()) {
        if(startOutTransaction(p)) {
          BinaryEncoder encoder;
          encoder << p;
          if(writeSocketData(encoder)) {
            emit parent->outStatus(p, SENDED);
          } else {
            deleteOutTransaction(p);
            emit parent->outStatus(p, NOTSENDED);
          }
        } else {
          emit parent->outStatus(p, TRANSFULL);
        }
      } else {
        emit parent->outStatus(p, MAXSPEED);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const DataSmResp &p) {
  if(p.getCommandId() == parameters::command_id::DATA_SM_RESP) {
    if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const QuerySmResp &p) {
  if(p.getCommandId() == parameters::command_id::QUERY_SM_RESP) {
    if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const ReplaceSmResp &p) {
  if(p.getCommandId() == parameters::command_id::REPLACE_SM_RESP) {
    if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const CancelSmResp &p) {
  if(p.getCommandId() == parameters::command_id::CANCEL_SM_RESP) {
    if(d->now_state == BOUND_TX || d->now_state == BOUND_TRX) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const EnquireLink &p) {
  if(p.getCommandId() == parameters::command_id::ENQUIRE_LINK) {
    if(d->now_state != CLOSED) {
      if(startOutTransaction(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
        } else {
          deleteOutTransaction(p);
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, TRANSFULL);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::send(const EnquireLinkResp &p) {
  if(p.getCommandId() == parameters::command_id::ENQUIRE_LINK_RESP) {
    if(d->now_state != CLOSED) {
      if(stopInTransactions(p)) {
        BinaryEncoder encoder;
        encoder << p;
        if(writeSocketData(encoder)) {
          emit parent->outStatus(p, SENDED);
        } else {
          emit parent->outStatus(p, NOTSENDED);
        }
      } else {
        emit parent->outStatus(p, NOTFOUNDTRANS);
      }
    } else {
      emit parent->outStatus(p, NOTVALIDSTATE);
    }
  } else {
    emit parent->outStatus(p, ERRORCID);
  }
}

void ServerAsyncSessionPrivate::setState(SessionStates state) {
  if(state == d->now_state) {
    return;
  }

  switch(state) {
    case CLOSED: {
      if(d->main_timer_id != -1) {
        killTimer(d->main_timer_id);
        d->main_timer_id = -1;
      }
      d->session_init_timer = 0;
      d->inactivity_timer = 0;
      d->enquire_link_timer = 0;
      d->in_speed_it = 0;
      d->now_in_speed = 0;
      d->out_speed_it = 0;
      d->now_out_speed = 0;
      clearAllQueue(NOTVALIDSTATE);
      break;
    }
    case OPEN: {
      if(d->now_state == CLOSED ||
         d->now_state == UNBOUND ||
         d->now_state == OUTBOUND) {
        d->session_init_timer = 0;
        d->inactivity_timer = 0;
        d->enquire_link_timer = 0;
        d->in_speed_it = 0;
        d->now_in_speed = 0;
        d->out_speed_it = 0;
        d->now_out_speed = 0;
        if(d->main_timer_id == -1) {
          d->main_timer_id = startTimer(1000);
        }
      }
      break;
    }
    case OUTBOUND: {
      if(d->now_state == OPEN) {
        d->session_init_timer = 0;
      }
      break;
    }
    default:
      break;
  }

  d->now_state = state;

  emit parent->stateChanged(d->now_state);

  if(d->now_state == CLOSED) {
    emit parent->closedSession();
  } else if(d->now_state == OPEN) {
    emit parent->openedSession();
  }
}

void ServerAsyncSessionPrivate::timerEvent(QTimerEvent *event) {
  if(event->timerId() == d->main_timer_id) {
    d->now_in_speed = d->in_speed_it;
    d->in_speed_it = 0;
    d->now_out_speed = d->out_speed_it;
    d->out_speed_it = 0;
    procSessionInitTimeout();
    procEnquireLinkTimeout();
    procInactivityTimeout();
    processInTransactions();
    processOutTransactions();
  }
}

void ServerAsyncSessionPrivate::sessionActivity() {
  if(d->inactivity_time > 0) {
    d->inactivity_timer = 0;
  }
}

void ServerAsyncSessionPrivate::removeAllInTransactions(StatusMessage status) {
  while(!d->in_transactions.isEmpty()) {
    ServerAsyncSessionData::Message *msg(d->in_transactions.takeFirst());
    emit parent->inStatus(Header(msg->header), status);
    delete msg;
  }
  d->in_transactions.clear();
}

void ServerAsyncSessionPrivate::removeAllOutTransactions(StatusMessage status) {
  while(!d->out_transactions.isEmpty()) {
    ServerAsyncSessionData::Message *msg(d->out_transactions.takeFirst());
    emit parent->outStatus(Header(msg->header), status);
    delete msg;
  }
  d->out_transactions.clear();
}

void ServerAsyncSessionPrivate::clearAllQueue(StatusMessage status) {
  removeAllInTransactions(status);
  removeAllOutTransactions(status);
}

void ServerAsyncSessionPrivate::procError(ErrorCode code,
                                          const std::string &message) {
  d->error_code = code;
  d->error_message = message;

  clearAllQueue(SESSERROR);

  emit parent->error(code);

  if(d->socket->state() != QAbstractSocket::UnconnectedState) {
    d->socket->abort();
  }

  setState(CLOSED);
}

void ServerAsyncSessionPrivate::procSessionInitTimeout() {
  if((d->now_state == OPEN || d->now_state == OUTBOUND) &&
     d->session_init_time > 0) {
    d->session_init_timer++;
    if(d->session_init_timer >= d->session_init_time) {
      d->session_init_timer = 0;
      emit parent->sessionInitTimeout();
      closeSession();
    }
  }
}

void ServerAsyncSessionPrivate::procEnquireLinkTimeout() {
  if(d->now_state != CLOSED && d->enquire_link_time > 0) {
    d->enquire_link_timer++;
    if(d->enquire_link_timer >= d->enquire_link_time) {
      d->enquire_link_timer = 0;
      emit parent->enquireLinkTimeout();
    }
  }
}

void ServerAsyncSessionPrivate::procInactivityTimeout() {
  if(d->now_state != CLOSED && d->inactivity_time > 0) {
    d->inactivity_timer++;
    if(d->inactivity_timer >= d->inactivity_time) {
      d->inactivity_timer = 0;
      emit parent->inactivityTimeout();
    }
  }
}

void ServerAsyncSessionPrivate::processInTransactions() {
  if(d->transaction_time < 0)
    return;

  for(int i = 0; i < d->in_transactions.size();) {
    ServerAsyncSessionData::Message *msg(
          d->in_transactions[i]);
    msg->timer++;
    if(msg->timer >= d->transaction_time) {
      emit parent->inStatus(Header(msg->header), TRANSTIMEOUT);
      delete msg;
      d->in_transactions.removeAt(i);
      continue;
    }
    i++;
  }
}

void ServerAsyncSessionPrivate::processOutTransactions() {
  if(d->transaction_time < 0)
    return;

  for(int i = 0; i < d->out_transactions.size();) {
    ServerAsyncSessionData::Message *msg(
          d->out_transactions[i]);
    msg->timer++;
    if(msg->timer >= d->transaction_time) {
      emit parent->outStatus(Header(msg->header), TRANSTIMEOUT);
      delete msg;
      d->out_transactions.removeAt(i);
      continue;
    }
    i++;
  }
}

bool ServerAsyncSessionPrivate::checkInSpeed() {
  if(d->in_speed_it >= d->max_in_speed) {
    return false;
  } else {
    d->in_speed_it++;
    return true;
  }
}

bool ServerAsyncSessionPrivate::checkOutSpeed() {
  if(d->out_speed_it >= d->max_out_speed) {
    return false;
  } else {
    d->out_speed_it++;
    return true;
  }
}

bool ServerAsyncSessionPrivate::startOutTransaction(const Header &header) {
  sessionActivity();

  if(d->out_transactions.size() < d->max_out_trans) {
    ServerAsyncSessionData::Message *msg =
      new ServerAsyncSessionData::Message(header);

    d->out_transactions.push_back(msg);
    emit parent->outStatus(header, TRANSSTART);
    return true;
  } else {
    emit parent->outStatus(header, TRANSFULL);
    return false;
  }
}

bool ServerAsyncSessionPrivate::stopOutTransactions(const Header &header) {
  bool res(false);

  uint32 command_id(header.getCommandId()),
          sequence_number(header.getSequenceNumber());

  if(command_id != parameters::command_id::DELIVER_SM_RESP &&
     command_id != parameters::command_id::DATA_SM_RESP &&
     command_id != parameters::command_id::ENQUIRE_LINK_RESP) {
    return res;
  }

  for(int i = 0; i < d->out_transactions.size();) {

    ServerAsyncSessionData::Message *msg(d->out_transactions[i]);

    if(msg->header.getSequenceNumber() == sequence_number) {
      bool exists(false);
      switch(msg->header.getCommandId()) {
        case parameters::command_id::DELIVER_SM:
          if(command_id == parameters::command_id::DELIVER_SM_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::DATA_SM:
          if(command_id == parameters::command_id::DATA_SM_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::ENQUIRE_LINK:
          if(command_id == parameters::command_id::ENQUIRE_LINK_RESP) {
            exists = true;
          }
          break;
      }

      if(exists) {
        emit parent->outStatus(msg->header, RESPONSED);
        res = exists;
        delete msg;
        d->out_transactions.removeAt(i);
        continue;
      }
    }
    i++;
  }

  return res;
}

void ServerAsyncSessionPrivate::deleteOutTransaction(const Header &header) {
  for(int i = 0; i < d->out_transactions.size(); i++) {
    ServerAsyncSessionData::Message *msg(d->out_transactions[i]);

    if(msg->header == header) {
      delete msg;
      d->out_transactions.removeAt(i);
      break;
    }
  }
}

bool ServerAsyncSessionPrivate::startInTransaction(const Header &header) {
  sessionActivity();

  if(d->in_transactions.size() < d->max_in_trans) {
    ServerAsyncSessionData::Message *msg =
      new ServerAsyncSessionData::Message(header);
    d->in_transactions.push_back(msg);
    emit parent->inStatus(header, TRANSSTART);
    return true;
  } else {
    emit parent->inStatus(header, TRANSFULL);
    return false;
  }
}

bool ServerAsyncSessionPrivate::stopInTransactions(const Header &header) {
  bool res(false);

  uint32 command_id(header.getCommandId()),
          sequence_number(header.getSequenceNumber());

  if(command_id != parameters::command_id::BIND_RECEIVER_RESP &&
     command_id != parameters::command_id::BIND_TRANSMITTER_RESP &&
     command_id != parameters::command_id::BIND_TRANSCEIVER_RESP &&
     command_id != parameters::command_id::UNBIND_RESP &&
     command_id != parameters::command_id::SUBMIT_SM_RESP &&
     command_id != parameters::command_id::SUBMIT_MULTI_RESP &&
     command_id != parameters::command_id::DATA_SM_RESP &&
     command_id != parameters::command_id::QUERY_SM_RESP &&
     command_id != parameters::command_id::REPLACE_SM_RESP &&
     command_id != parameters::command_id::CANCEL_SM_RESP &&
     command_id != parameters::command_id::ENQUIRE_LINK_RESP) {
    return res;
  }

  for(int i = 0; i < d->in_transactions.size();) {

    ServerAsyncSessionData::Message *msg(d->in_transactions[i]);

    if(msg->header.getSequenceNumber() == sequence_number) {
      bool exists(false);
      switch(msg->header.getCommandId()) {
        case parameters::command_id::BIND_RECEIVER:
          if(command_id == parameters::command_id::BIND_RECEIVER_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::BIND_TRANSMITTER:
          if(command_id == parameters::command_id::BIND_TRANSMITTER_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::BIND_TRANSCEIVER:
          if(command_id == parameters::command_id::BIND_TRANSCEIVER_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::UNBIND:
          if(command_id == parameters::command_id::UNBIND_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::SUBMIT_SM:
          if(command_id == parameters::command_id::SUBMIT_SM_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::SUBMIT_MULTI:
          if(command_id == parameters::command_id::SUBMIT_MULTI_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::DATA_SM:
          if(command_id == parameters::command_id::DATA_SM_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::QUERY_SM:
          if(command_id == parameters::command_id::QUERY_SM_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::REPLACE_SM:
          if(command_id == parameters::command_id::REPLACE_SM_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::CANCEL_SM:
          if(command_id == parameters::command_id::CANCEL_SM_RESP) {
            exists = true;
          }
          break;
        case parameters::command_id::ENQUIRE_LINK:
          if(command_id == parameters::command_id::ENQUIRE_LINK_RESP) {
            exists = true;
          }
          break;
      }

      if(exists) {
        emit parent->inStatus(msg->header, RESPONSED);
        res = exists;
        delete msg;
        d->in_transactions.removeAt(i);
        continue;
      }
    }
    i++;
  }

  return res;
}

void ServerAsyncSessionPrivate::deleteInTransaction(const Header &header) {
  for(int i = 0; i < d->in_transactions.size(); i++) {
    ServerAsyncSessionData::Message *msg(d->in_transactions[i]);

    if(msg->header == header) {
      delete msg;
      d->in_transactions.removeAt(i);
      break;
    }
  }
}

bool ServerAsyncSessionPrivate::writeSocketData(const QByteArray &bytes) {
  if(d->socket->state() == QAbstractSocket::ConnectedState &&
     d->socket->isValid()) {

    qint64 write_bytes(d->socket->write(bytes));
    d->socket->flush();

    if(write_bytes < bytes.size()) {
      return d->socket->waitForBytesWritten(
        ServerAsyncSessionData::sock_oper_timeout);
    } else {
      return true;
    }
  } else {
    return false;
  }
}

ServerAsyncSession::ServerAsyncSession(
  int socket,
  int max_in_speed, // m/sec
  int max_out_speed, // m/sec
  int session_init_time,
  int enquire_link_time,
  int inactivity_time,
  int transaction_time,
  int max_in_trans,
  int max_out_trans,
  bool threaded) :
  p(new ServerAsyncSessionPrivate(this,
                                  socket,
                                  max_in_speed, // msec
                                  max_out_speed, // msec
                                  session_init_time,
                                  enquire_link_time,
                                  inactivity_time,
                                  transaction_time,
                                  max_in_trans,
                                  max_out_trans,
                                  threaded)) {
  if(threaded) {
    QTimer::singleShot(0, this, SLOT(startProcess()));
  }
}

ServerAsyncSession::~ServerAsyncSession() {
  if(p) {
    if(p->d->thread) {
      if(p->d->thread->isRunning()) {
        p->d->thread->quit();
        p->d->thread->wait();
      }
    }
    delete p;
    p = 0;
  }
}

void ServerAsyncSession::startProcess() {
  if(p->d->thread && !p->d->thread->isRunning()) {
    p->d->thread->start();
  }
}

void ServerAsyncSession::setMaxInSpeed(int max_in_speed) {
  p->d->max_in_speed = max_in_speed;
}

void ServerAsyncSession::setMaxOutSpeed(int max_out_speed) {
  p->d->max_out_speed = max_out_speed;
}

void ServerAsyncSession::setSessionInitTime(int session_init_time) {
  p->d->session_init_time = session_init_time;
}

void ServerAsyncSession::setEnquireLinkTime(int enquire_link_time) {
  p->d->enquire_link_time = enquire_link_time;
}

void ServerAsyncSession::setInactivityTime(int inactivity_time) {
  p->d->inactivity_time = inactivity_time;
}

void ServerAsyncSession::setTransactionTime(int transaction_time) {
  p->d->transaction_time = transaction_time;
}

void ServerAsyncSession::setMaxInTrans(int max_in_trans) {
  p->d->max_in_trans = max_in_trans;
}

void ServerAsyncSession::setMaxOutTrans(int max_out_trans) {
  p->d->max_out_trans = max_out_trans;
}

smpp::sessions::SessionStates ServerAsyncSession::state() const {
  return p->d->now_state;
}

QHostAddress ServerAsyncSession::peerAddress() const {
  return p->d->peer_address;
}

uint16 ServerAsyncSession::peerPort() const {
  return p->d->peer_port;
}

QHostAddress ServerAsyncSession::localAddress() const {
  return p->d->local_address;
}

uint16 ServerAsyncSession::localPort() const {
  return p->d->local_port;
}

int ServerAsyncSession::getMaxInSpeed() const {
  return p->d->max_in_speed;
}

int ServerAsyncSession::getMaxOutSpeed() const {
  return p->d->max_out_speed;
}

int ServerAsyncSession::getNowInSpeed() const {
  return p->d->now_in_speed;
}

int ServerAsyncSession::getNowOutSpeed() const {
  return p->d->now_out_speed;
}

int ServerAsyncSession::getSessionInitTime() const {
  return p->d->session_init_time;
}

int ServerAsyncSession::getEnquireLinkTime() const {
  return p->d->enquire_link_time;
}

int ServerAsyncSession::getInactivityTime() const {
  return p->d->inactivity_time;
}

int ServerAsyncSession::getTransactionTime() const {
  return p->d->transaction_time;
}

int ServerAsyncSession::getMaxInTrans() const {
  return p->d->max_in_trans;
}

int ServerAsyncSession::getMaxOutTrans() const {
  return p->d->max_out_trans;
}

int ServerAsyncSession::getOpenedInTrans() const {
  return p->d->in_transactions.size();
}

int ServerAsyncSession::getOpenedOutTrans() const {
  return p->d->out_transactions.size();
}

smpp::sessions::ErrorCode ServerAsyncSession::error() const {
  return p->d->error_code;
}

std::string ServerAsyncSession::errorString() const {
  return p->d->error_message;
}

void ServerAsyncSession::closeSession() {
  emit p->sigCloseSession();
}

void ServerAsyncSession::send(const AlertNotification &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const BindTransmitterResp &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const BindReceiverResp &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const BindTransceiverResp &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const UnbindResp &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const Outbind &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const GenericNack &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const SubmitSmResp &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const SubmitMultiResp &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const DeliverSm &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const DataSm &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const DataSmResp &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const QuerySmResp &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const ReplaceSmResp &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const CancelSmResp &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const EnquireLink &packet) {
  emit p->sigSend(packet);
}

void ServerAsyncSession::send(const EnquireLinkResp &packet) {
  emit p->sigSend(packet);
}

} // namespace sessions

} // namespace smpp
