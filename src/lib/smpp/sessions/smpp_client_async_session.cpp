#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/encoders/smpp_binary_encoder.h"
#include "smpp/decoders/smpp_binary_decoder.h"
#include "smpp/sessions/smpp_client_async_session.h"
#include "smpp/sessions/smpp_client_async_session_p.h"

namespace smpp {

namespace sessions {

using namespace pdu;
using namespace decoders;
using namespace encoders;

ClientAsyncSessionPrivate::ClientAsyncSessionPrivate(
  ClientAsyncSession *parent,
  boost::asio::io_service &service,
  int32 max_in_speed, // msec
  int32 max_out_speed, // msec
  int32 session_init_time,
  int32 enquire_link_time,
  int32 inactivity_time,
  int32 transaction_time,
  int32 max_in_trans,
  int32 max_out_trans) :
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
  max_in_speed(max_in_speed),
  max_out_speed(max_out_speed),
  max_in_trans(max_in_trans),
  max_out_trans(max_out_trans),
  now_state(CLOSED),
  io_service(service),
  socket(boost::asio::ip::tcp::socket(service)),
  timer(service, boost::posix_time::seconds(1)),
  parent(parent) {
  if(max_in_speed < 1) {
    this->max_in_speed = DEFAULT_IN_SPEED;
  }
  if(max_out_speed < 1) {
    this->max_out_speed = DEFAULT_OUT_SPEED;
  }
}

ClientAsyncSessionPrivate::~ClientAsyncSessionPrivate() {
  clearAllQueue(NOTVALIDSTATE);
  parent = 0;
}

void ClientAsyncSessionPrivate::sendPacket(const HeaderPtr &packet) {
  switch(packet->getCommandId()) {
    case smpp::parameters::command_id::BIND_TRANSMITTER:
      if(reinterpret_cast<BindTransmitter *>(packet.get())) {
        if(now_state == OPEN || now_state == OUTBOUND) {
          if(startOutTransaction(packet)) {
            encoder.pack(*reinterpret_cast<BindTransmitter *>(packet.get()));
            writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
            encoder.clear();
          } else {
            out_status_sig(packet, TRANSFULL);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::BIND_RECEIVER:
      if(reinterpret_cast<BindReceiver *>(packet.get())) {
        if(now_state == OPEN || now_state == OUTBOUND) {
          if(startOutTransaction(packet)) {
            encoder.pack(*reinterpret_cast<BindReceiver *>(packet.get()));
            writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
            encoder.clear();
          } else {
            out_status_sig(packet, TRANSFULL);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::BIND_TRANSCEIVER:
      if(reinterpret_cast<BindTransceiver *>(packet.get())) {
        if(now_state == OPEN || now_state == OUTBOUND) {
          if(startOutTransaction(packet)) {
            encoder.pack(*reinterpret_cast<BindTransceiver *>(packet.get()));
            writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
            encoder.clear();
          } else {
            out_status_sig(packet, TRANSFULL);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::UNBIND:
      if(reinterpret_cast<Unbind *>(packet.get())) {
        if(now_state == BOUND_TX ||
           now_state == BOUND_RX ||
           now_state == BOUND_TRX) {
          if(startOutTransaction(packet)) {
            encoder.pack(*reinterpret_cast<Unbind *>(packet.get()));
            writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
            encoder.clear();
          } else {
            out_status_sig(packet, TRANSFULL);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::GENERIC_NACK:
      if(reinterpret_cast<GenericNack *>(packet.get())) {
        if(now_state != CLOSED) {
          encoder.pack(*reinterpret_cast<GenericNack *>(packet.get()));
          writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
          encoder.clear();
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::SUBMIT_SM:
      if(reinterpret_cast<SubmitSm *>(packet.get())) {
        if(now_state == BOUND_TX || now_state == BOUND_TRX) {
          if(checkOutSpeed()) {
            if(startOutTransaction(packet)) {
              encoder.pack(*reinterpret_cast<SubmitSm *>(packet.get()));
              writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
              encoder.clear();
            } else {
              out_status_sig(packet, TRANSFULL);
            }
          } else {
            out_status_sig(packet, MAXSPEED);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::SUBMIT_MULTI:
      if(reinterpret_cast<SubmitMulti *>(packet.get())) {
        if(now_state == BOUND_TX || now_state == BOUND_TRX) {
          if(checkOutSpeed()) {
            if(startOutTransaction(packet)) {
              encoder.pack(*reinterpret_cast<SubmitMulti *>(packet.get()));
              writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
              encoder.clear();
            } else {
              out_status_sig(packet, TRANSFULL);
            }
          } else {
            out_status_sig(packet, MAXSPEED);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::DELIVER_SM_RESP:
      if(reinterpret_cast<DeliverSmResp *>(packet.get())) {
        if(now_state == BOUND_RX || now_state == BOUND_TRX) {
          if(stopInTransactions(packet)) {
            encoder.pack(*reinterpret_cast<DeliverSmResp *>(packet.get()));
            writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
            encoder.clear();
          } else {
            out_status_sig(packet, NOTFOUNDTRANS);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::DATA_SM:
      if(reinterpret_cast<DataSm *>(packet.get())) {
        if(now_state == BOUND_TX || now_state == BOUND_TRX) {
          if(checkOutSpeed()) {
            if(startOutTransaction(packet)) {
              encoder.pack(*reinterpret_cast<DataSm *>(packet.get()));
              writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
              encoder.clear();
            } else {
              out_status_sig(packet, TRANSFULL);
            }
          } else {
            out_status_sig(packet, MAXSPEED);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::DATA_SM_RESP:
      if(reinterpret_cast<DataSmResp *>(packet.get())) {
        if(now_state == BOUND_RX || now_state == BOUND_TRX) {
          if(stopInTransactions(packet)) {
            encoder.pack(*reinterpret_cast<DataSmResp *>(packet.get()));
            writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
            encoder.clear();
          } else {
            out_status_sig(packet, NOTFOUNDTRANS);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::QUERY_SM:
      if(reinterpret_cast<QuerySm *>(packet.get())) {
        if(now_state == BOUND_TX || now_state == BOUND_TRX) {
          if(checkOutSpeed()) {
            if(startOutTransaction(packet)) {
              encoder.pack(*reinterpret_cast<QuerySm *>(packet.get()));
              writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
              encoder.clear();
            } else {
              out_status_sig(packet, TRANSFULL);
            }
          } else {
            out_status_sig(packet, MAXSPEED);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::REPLACE_SM:
      if(reinterpret_cast<ReplaceSm *>(packet.get())) {
        if(now_state == BOUND_TX || now_state == BOUND_TRX) {
          if(checkOutSpeed()) {
            if(startOutTransaction(packet)) {
              encoder.pack(*reinterpret_cast<ReplaceSm *>(packet.get()));
              writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
              encoder.clear();
            } else {
              out_status_sig(packet, TRANSFULL);
            }
          } else {
            out_status_sig(packet, MAXSPEED);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::CANCEL_SM:
      if(reinterpret_cast<CancelSm *>(packet.get())) {
        if(now_state == BOUND_TX || now_state == BOUND_TRX) {
          if(checkOutSpeed()) {
            if(startOutTransaction(packet)) {
              encoder.pack(*reinterpret_cast<CancelSm *>(packet.get()));
              writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
              encoder.clear();
            } else {
              out_status_sig(packet, TRANSFULL);
            }
          } else {
            out_status_sig(packet, MAXSPEED);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::ENQUIRE_LINK:
      if(reinterpret_cast<EnquireLink *>(packet.get())) {
        if(now_state != CLOSED) {
          if(startOutTransaction(packet)) {
            encoder.pack(*reinterpret_cast<EnquireLink *>(packet.get()));
            writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
            encoder.clear();
          } else {
            out_status_sig(packet, TRANSFULL);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    case smpp::parameters::command_id::ENQUIRE_LINK_RESP:
      if(reinterpret_cast<EnquireLinkResp *>(packet.get())) {
        if(now_state != CLOSED) {
          if(stopInTransactions(packet)) {
            encoder.pack(*reinterpret_cast<EnquireLinkResp *>(packet.get()));
            writeSocketData(encoder.getBuff(), encoder.getBuffLen(), packet);
            encoder.clear();
          } else {
            out_status_sig(packet, NOTFOUNDTRANS);
          }
        } else {
          out_status_sig(packet, NOTVALIDSTATE);
        }
      } else {
        out_status_sig(packet, ERRORCID);
      }
      break;
    default:
      out_status_sig(packet, ERRORCID);
      break;
  }
}

void ClientAsyncSessionPrivate::openSession(
  const boost::asio::ip::tcp::endpoint &ep) {
  if(now_state == CLOSED) {
    socket.async_connect(
      ep,
      [this](boost::system::error_code ec) {
        if(!ec) {
          setState(OPEN);
          readHeader();
        } else {
          procError(ec);
        }
      });
  }
}

void ClientAsyncSessionPrivate::closeSession() {
  socket.close();
  setState(CLOSED);
}

void ClientAsyncSessionPrivate::readHeader() {
  decoder.addData(Header::HEADER_LENGTH);
  boost::asio::async_read(
    socket,
    boost::asio::buffer(decoder.header(), Header::HEADER_LENGTH),
    boost::bind(
      &ClientAsyncSessionPrivate::readHeaderHandler,
      this,
      boost::asio::placeholders::error)
  );
}

void ClientAsyncSessionPrivate::readBody(const Header &header) {
  decoder.addData(header.getCommandLength() - Header::HEADER_LENGTH);
  boost::asio::async_read(
    socket,
    boost::asio::buffer(decoder.body(), decoder.bodyLength()),
    boost::bind(
      &ClientAsyncSessionPrivate::readBodyHandler,
      this,
      boost::asio::placeholders::error,
      header)
  );
}

void ClientAsyncSessionPrivate::readHeaderHandler(
  const boost::system::error_code &ec) {
  if(ec) {
    decoder.clear();
    procError(ec);
  } else {
    Header header;
    decoder.unpack(header);
    if(header.getCommandLength() >= Header::HEADER_LENGTH) {
      if(header.getCommandLength() == Header::HEADER_LENGTH) {
        decoder.clear();
        switch(header.getCommandId()) {
          case parameters::command_id::OUTBIND: {
            HeaderPtr ptr(new Outbind(header));
            if(now_state == OPEN) {
              if(header.getCommandStatus() ==
                 parameters::command_status::ESME_ROK) {
                setState(OUTBOUND);
              }
              receive_packet_sig(ptr);
            } else {
              in_status_sig(ptr, NOTVALIDSTATE);
            }
            break;
          }
          case parameters::command_id::UNBIND_RESP: {
            HeaderPtr ptr(new UnbindResp(header));
            if(now_state == UNBOUND) {
              if(stopOutTransactions(ptr)) {
                receive_packet_sig(ptr);
                closeSession();
              } else {
                in_status_sig(ptr, NOTFOUNDTRANS);
              }
            } else {
              in_status_sig(ptr, NOTVALIDSTATE);
            }
            break;
          }
          case parameters::command_id::GENERIC_NACK: {
            HeaderPtr ptr(new GenericNack(header));
            if(now_state != CLOSED) {
              receive_packet_sig(ptr);
            } else {
              in_status_sig(ptr, NOTVALIDSTATE);
            }
            break;
          }
          case parameters::command_id::REPLACE_SM_RESP: {
            HeaderPtr ptr(new ReplaceSmResp(header));
            if(now_state == BOUND_TX || now_state == BOUND_TRX) {
              if(stopOutTransactions(ptr)) {
                receive_packet_sig(ptr);
              } else {
                in_status_sig(ptr, NOTFOUNDTRANS);
              }
            } else {
              in_status_sig(ptr, NOTVALIDSTATE);
            }
            break;
          }
          case parameters::command_id::CANCEL_SM_RESP: {
            HeaderPtr ptr(new CancelSmResp(header));
            if(now_state == BOUND_TX || now_state == BOUND_TRX) {
              if(stopOutTransactions(ptr)) {
                receive_packet_sig(ptr);
              } else {
                in_status_sig(ptr, NOTFOUNDTRANS);
              }
            } else {
              in_status_sig(ptr, NOTVALIDSTATE);
            }
            break;
          }
          case parameters::command_id::ENQUIRE_LINK: {
            HeaderPtr ptr(new EnquireLink(header));
            if(now_state != CLOSED) {
              if(startInTransaction(ptr)) {
                receive_packet_sig(ptr);
              } else {
                in_status_sig(ptr, TRANSFULL);
                EnquireLinkResp *p_resp(new EnquireLinkResp(
                  parameters::command_status::ESME_RMSGQFUL,
                  header.getSequenceNumber()));
                encoder.pack(*p_resp);
                writeSocketData(encoder.getBuff(),
                                encoder.getBuffLen(),
                                HeaderPtr(p_resp));
                encoder.clear();
              }
            } else {
              in_status_sig(ptr, NOTVALIDSTATE);
            }
            break;
          }
          case parameters::command_id::ENQUIRE_LINK_RESP: {
            HeaderPtr ptr(new EnquireLinkResp(header));
            if(now_state != CLOSED) {
              if(stopOutTransactions(ptr)) {
                receive_packet_sig(ptr);
              } else {
                in_status_sig(ptr, NOTFOUNDTRANS);
              }
            } else {
              in_status_sig(ptr, NOTVALIDSTATE);
            }
            break;
          }
          case parameters::command_id::ALERT_NOTIFICATION: {
            HeaderPtr ptr(new AlertNotification(header));
            if(now_state == BOUND_RX || now_state == BOUND_TRX) {
              receive_packet_sig(ptr);
            } else {
              in_status_sig(ptr, NOTVALIDSTATE);
            }
            break;
          }
          default: {
            sendGenericNackErrorDecode(
              parameters::command_status::ESME_RINVCMDID,
              header.getSequenceNumber(),
              ERRORCID);
            break;
          }
        }
        readHeader();
      } else {
        readBody(header);
      }
    } else {
      decoder.clear();
      sendGenericNackErrorDecode(
        parameters::command_status::ESME_RINVCMDLEN,
        header.getSequenceNumber(),
        ERRORDECODE);
      readHeader();
    }
  }
}

void ClientAsyncSessionPrivate::readBodyHandler(
  const boost::system::error_code &ec, const Header &header) {
  if(ec) {
    decoder.clear();
    procError(ec);
  } else {
    HeaderPtr ptr;
    switch(header.getCommandId()) {
      case parameters::command_id::BIND_TRANSMITTER_RESP: {
        BindTransmitterResp *p(new BindTransmitterResp(header));
        if(decoder.unpack(*p)) {
          ptr = HeaderPtr(p);
          if(now_state == OPEN || now_state == OUTBOUND) {
            if(stopOutTransactions(ptr)) {
              if(p->getCommandStatus() == parameters::command_status::ESME_ROK) {
                setState(BOUND_TX);
              }
              receive_packet_sig(ptr);
            } else {
              in_status_sig(ptr, NOTFOUNDTRANS);
            }
          } else {
            in_status_sig(ptr, NOTVALIDSTATE);
          }
        } else {
          sendGenericNackErrorDecode(
            parameters::command_status::ESME_RINVCMDLEN,
            header.getSequenceNumber(),
            ERRORDECODE);
        }
        break;
      }
      case parameters::command_id::BIND_RECEIVER_RESP: {
        BindReceiverResp *p(new BindReceiverResp(header));
        if(decoder.unpack(*p)) {
          ptr = HeaderPtr(p);
          if(now_state == OPEN || now_state == OUTBOUND) {
            if(stopOutTransactions(ptr)) {
              if(p->getCommandStatus() == parameters::command_status::ESME_ROK) {
                setState(BOUND_RX);
              }
              receive_packet_sig(ptr);
            } else {
              in_status_sig(ptr, NOTFOUNDTRANS);
            }
          } else {
            in_status_sig(ptr, NOTVALIDSTATE);
          }
        } else {
          sendGenericNackErrorDecode(
            parameters::command_status::ESME_RINVCMDLEN,
            header.getSequenceNumber(),
            ERRORDECODE);
        }
        break;
      }
      case parameters::command_id::BIND_TRANSCEIVER_RESP: {
        BindTransceiverResp *p(new BindTransceiverResp(header));
        if(decoder.unpack(*p)) {
          ptr = HeaderPtr(p);
          if(now_state == OPEN || now_state == OUTBOUND) {
            if(stopOutTransactions(ptr)) {
              if(p->getCommandStatus() == parameters::command_status::ESME_ROK) {
                setState(BOUND_TRX);
              }
              receive_packet_sig(ptr);
            } else {
              in_status_sig(ptr, NOTFOUNDTRANS);
            }
          } else {
            in_status_sig(ptr, NOTVALIDSTATE);
          }
        } else {
          sendGenericNackErrorDecode(
            parameters::command_status::ESME_RINVCMDLEN,
            header.getSequenceNumber(),
            ERRORDECODE);
        }
        break;
      }
      case parameters::command_id::SUBMIT_SM_RESP: {
        SubmitSmResp *p(new SubmitSmResp(header));
        if(decoder.unpack(*p)) {
          ptr = HeaderPtr(p);
          if(now_state == BOUND_TX || now_state == BOUND_TRX) {
            if(stopOutTransactions(ptr)) {
              receive_packet_sig(ptr);
            } else {
              in_status_sig(ptr, NOTFOUNDTRANS);
            }
          } else {
            in_status_sig(ptr, NOTVALIDSTATE);
          }
        } else {
          sendGenericNackErrorDecode(
            parameters::command_status::ESME_RINVCMDLEN,
            header.getSequenceNumber(),
            ERRORDECODE);
        }
        break;
      }
      case parameters::command_id::SUBMIT_MULTI_RESP: {
        SubmitMultiResp *p(new SubmitMultiResp(header));
        if(decoder.unpack(*p)) {
          ptr = HeaderPtr(p);
          if(now_state == BOUND_TX || now_state == BOUND_TRX) {
            if(stopOutTransactions(ptr)) {
              receive_packet_sig(ptr);
            } else {
              in_status_sig(ptr, NOTFOUNDTRANS);
            }
          } else {
            in_status_sig(ptr, NOTVALIDSTATE);
          }
        } else {
          sendGenericNackErrorDecode(
            parameters::command_status::ESME_RINVCMDLEN,
            header.getSequenceNumber(),
            ERRORDECODE);
        }
        break;
      }
      case parameters::command_id::DELIVER_SM: {
        DeliverSm *p(new DeliverSm(header));
        if(decoder.unpack(*p)) {
          ptr = HeaderPtr(p);
          if(now_state == BOUND_RX || now_state == BOUND_TRX) {
            if(checkInSpeed()) {
              if(startInTransaction(ptr)) {
                receive_packet_sig(ptr);
              } else {
                in_status_sig(ptr, TRANSFULL);
                sendPacket(HeaderPtr(new DeliverSmResp(
                  parameters::command_status::ESME_RMSGQFUL,
                  header.getSequenceNumber())));
              }
            } else {
              in_status_sig(ptr, MAXSPEED);
              sendPacket(HeaderPtr(new DeliverSmResp(
                parameters::command_status::ESME_RTHROTTLED,
                header.getSequenceNumber())));
            }
          } else {
            in_status_sig(ptr, NOTVALIDSTATE);
            sendPacket(HeaderPtr(new DeliverSmResp(
              parameters::command_status::ESME_RINVBNDSTS,
              header.getSequenceNumber())));
          }
        } else {
          sendGenericNackErrorDecode(
            parameters::command_status::ESME_RINVCMDLEN,
            header.getSequenceNumber(),
            ERRORDECODE);
        }
        break;
      }
      case parameters::command_id::DATA_SM: {
        DataSm *p(new DataSm(header));
        if(decoder.unpack(*p)) {
          ptr = HeaderPtr(p);
          if(now_state == BOUND_RX || now_state == BOUND_TRX) {
            if(checkInSpeed()) {
              if(startInTransaction(ptr)) {
                receive_packet_sig(ptr);
              } else {
                in_status_sig(ptr, TRANSFULL);
                sendPacket(HeaderPtr(new DataSmResp(
                  parameters::command_status::ESME_RMSGQFUL,
                  header.getSequenceNumber())));
              }
            } else {
              in_status_sig(ptr, MAXSPEED);
              sendPacket(HeaderPtr(new DataSmResp(
                parameters::command_status::ESME_RTHROTTLED,
                header.getSequenceNumber())));
            }
          } else {
            in_status_sig(ptr, NOTVALIDSTATE);
            sendPacket(HeaderPtr(new DataSmResp(
              parameters::command_status::ESME_RINVBNDSTS,
              header.getSequenceNumber())));
          }
        } else {
          sendGenericNackErrorDecode(
            parameters::command_status::ESME_RINVCMDLEN,
            header.getSequenceNumber(),
            ERRORDECODE);
        }
        break;
      }
      case parameters::command_id::DATA_SM_RESP: {
        DataSmResp *p(new DataSmResp(header));
        if(decoder.unpack(*p)) {
          ptr = HeaderPtr(p);
          if(now_state == BOUND_TX || now_state == BOUND_TRX) {
            if(stopOutTransactions(ptr)) {
              receive_packet_sig(ptr);
            } else {
              in_status_sig(ptr, NOTFOUNDTRANS);
            }
          } else {
            in_status_sig(ptr, NOTVALIDSTATE);
          }
        } else {
          sendGenericNackErrorDecode(
            parameters::command_status::ESME_RINVCMDLEN,
            header.getSequenceNumber(),
            ERRORDECODE);
        }
        break;
      }
      case parameters::command_id::QUERY_SM_RESP: {
        QuerySmResp *p(new QuerySmResp(header));
        if(decoder.unpack(*p)) {
          ptr = HeaderPtr(p);
          if(now_state == BOUND_TX || now_state == BOUND_TRX) {
            if(stopOutTransactions(ptr)) {
              receive_packet_sig(ptr);
            } else {
              in_status_sig(ptr, NOTFOUNDTRANS);
            }
          } else {
            in_status_sig(ptr, NOTVALIDSTATE);
          }
        } else {
          sendGenericNackErrorDecode(
            parameters::command_status::ESME_RINVCMDLEN,
            header.getSequenceNumber(),
            ERRORDECODE);
        }
        break;
      }
      default: {
        sendGenericNackErrorDecode(
          parameters::command_status::ESME_RINVCMDID,
          header.getSequenceNumber(),
          ERRORCID);
        break;
      }
    }
    decoder.clear();
    readHeader();
  }
}

void ClientAsyncSessionPrivate::sendGenericNackErrorDecode(
  uint32 cs, uint32 sn, StatusMessage status) {
  HeaderPtr ptr(new GenericNack(cs, sn));
  in_status_sig(ptr, status);
  sendPacket(ptr);
}

void ClientAsyncSessionPrivate::setState(SessionStates state) {
  if(state == now_state) {
    return;
  }

  switch(state) {
    case CLOSED: {
      stopTimer();
      session_init_timer = 0;
      inactivity_timer = 0;
      enquire_link_timer = 0;
      in_speed_it = 0;
      now_in_speed = 0;
      out_speed_it = 0;
      now_out_speed = 0;
      clearAllQueue(NOTVALIDSTATE);
      break;
    }
    case OPEN: {
      if(now_state == CLOSED ||
         now_state == UNBOUND ||
         now_state == OUTBOUND) {
        session_init_timer = 0;
        inactivity_timer = 0;
        enquire_link_timer = 0;
        in_speed_it = 0;
        now_in_speed = 0;
        out_speed_it = 0;
        now_out_speed = 0;
        startTimer();
      }
      break;
    }
    case OUTBOUND: {
      if(now_state == OPEN) {
        session_init_timer = 0;
      }
      break;
    }
    default:
      break;
  }

  now_state = state;

  state_changed_sig(now_state);

  if(now_state == CLOSED) {
    closed_sess_sig();
  } else if(now_state == OPEN) {
    opened_sess_sig();
  }
}

void ClientAsyncSessionPrivate::sessionActivity() {
  if(inactivity_time > 0) {
    inactivity_timer = 0;
  }
}

void ClientAsyncSessionPrivate::removeAllInTransactions(StatusMessage status) {
  while(!in_transactions.empty()) {
    Message *msg(in_transactions.front());
    in_status_sig(msg->packet, status);
    delete msg;
    in_transactions.remove(msg);
  }
  in_transactions.clear();
}

void ClientAsyncSessionPrivate::removeAllOutTransactions(StatusMessage status) {
  while(!out_transactions.empty()) {
    Message *msg(out_transactions.front());
    out_status_sig(msg->packet, status);
    delete msg;
    out_transactions.remove(msg);
  }
  out_transactions.clear();
}

void ClientAsyncSessionPrivate::clearAllQueue(StatusMessage status) {
  removeAllInTransactions(status);
  removeAllOutTransactions(status);
}

void ClientAsyncSessionPrivate::procError(const boost::system::error_code &ec) {
  error_code = ec;

  clearAllQueue(SESSERROR);

  error_sig(error_code);

  closeSession();
}

void ClientAsyncSessionPrivate::procSessionInitTimeout() {
  if((now_state == OPEN || now_state == OUTBOUND) && session_init_time > 0) {
    session_init_timer++;
    if(session_init_timer >= session_init_time) {
      session_init_timer = 0;
      sess_init_timeout_sig();
      closeSession();
    }
  }
}

void ClientAsyncSessionPrivate::procEnquireLinkTimeout() {
  if(now_state != CLOSED && enquire_link_time > 0) {
    enquire_link_timer++;
    if(enquire_link_timer >= enquire_link_time) {
      enquire_link_timer = 0;
      el_timeout_sig();
    }
  }
}

void ClientAsyncSessionPrivate::procInactivityTimeout() {
  if(now_state != CLOSED && inactivity_time > 0) {
    inactivity_timer++;
    if(inactivity_timer >= inactivity_time) {
      inactivity_timer = 0;
      inact_timeout_sig();
    }
  }
}

void ClientAsyncSessionPrivate::processInTransactions() {
  if(transaction_time == 0)
    return;

  for(auto it = in_transactions.begin(); it != in_transactions.end();) {
    Message *msg(*it);
    msg->ttl++;
    if(msg->ttl >= transaction_time) {
      in_status_sig(msg->packet, TRANSTIMEOUT);
      delete msg;
      it = in_transactions.erase(it);
      continue;
    }
    it++;
  }
}

void ClientAsyncSessionPrivate::processOutTransactions() {
  if(transaction_time == 0)
    return;

  for(auto it = out_transactions.begin(); it != out_transactions.end();) {
    Message *msg(*it);
    msg->ttl++;
    if(msg->ttl >= transaction_time) {
      out_status_sig(msg->packet, TRANSTIMEOUT);
      delete msg;
      it = out_transactions.erase(it);
      continue;
    }
    it++;
  }
}

bool ClientAsyncSessionPrivate::checkInSpeed() {
  if(in_speed_it >= max_in_speed) {
    return false;
  } else {
    in_speed_it++;
    return true;
  }
}

bool ClientAsyncSessionPrivate::checkOutSpeed() {
  if(out_speed_it >= max_out_speed) {
    return false;
  } else {
    out_speed_it++;
    return true;
  }
}

bool ClientAsyncSessionPrivate::startOutTransaction(const HeaderPtr &packet) {
  sessionActivity();

  if(out_transactions.size() < max_out_trans) {
    out_transactions.push_back(new Message(packet));
    out_status_sig(packet, TRANSSTART);
    return true;
  } else {
    out_status_sig(packet, TRANSFULL);
    return false;
  }
}

bool ClientAsyncSessionPrivate::stopOutTransactions(const HeaderPtr &packet) {
  bool res(false);

  uint32 command_id(packet->getCommandId()),
         sequence_number(packet->getSequenceNumber());

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

  for(auto it = out_transactions.begin(); it != out_transactions.end();) {
    Message *msg(*it);
    if(msg->packet->getSequenceNumber() == sequence_number) {
      bool exists(false);
      switch(msg->packet->getCommandId()) {
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
        out_status_sig(msg->packet, RESPONSED);
        res = exists;
        delete msg;
        it = out_transactions.erase(it);
        continue;
      }
    }
    it++;
  }

  return res;
}

bool ClientAsyncSessionPrivate::startInTransaction(const HeaderPtr &packet) {
  sessionActivity();

  if(in_transactions.size() < max_in_trans) {
    in_transactions.push_back(new Message(packet));
    in_status_sig(packet, TRANSSTART);
    return true;
  } else {
    in_status_sig(packet, TRANSFULL);
    return false;
  }
}

bool ClientAsyncSessionPrivate::stopInTransactions(const HeaderPtr &packet) {
  bool res(false);

  uint32 command_id(packet->getCommandId()),
         sequence_number(packet->getSequenceNumber());

  if(command_id != parameters::command_id::DELIVER_SM_RESP &&
     command_id != parameters::command_id::DATA_SM_RESP &&
     command_id != parameters::command_id::ENQUIRE_LINK_RESP) {
    return res;
  }

  for(auto it = in_transactions.begin(); it != in_transactions.end();) {
    Message *msg(*it);
    if(msg->packet->getSequenceNumber() == sequence_number) {
      bool exists(false);
      switch(msg->packet->getCommandId()) {
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
        in_status_sig(msg->packet, RESPONSED);
        res = exists;
        delete msg;
        it = in_transactions.erase(it);
        continue;
      }
    }
    it++;
  }

  return res;
}

void ClientAsyncSessionPrivate::writeSocketData(
  const int8 *bytes,
  uint32 len,
  const HeaderPtr &packet) {
  bool write_in_progress(!write_socket_messages.empty());
  int8 *tmp = new int8[len];
  memcpy(tmp, bytes, len);
  write_socket_messages.push_back(new SocketMessage(tmp, len, packet));
  if(!write_in_progress) {
    boost::asio::async_write(
      socket,
      boost::asio::buffer(write_socket_messages.front()->data,
                          write_socket_messages.front()->len),
      boost::bind(
        &ClientAsyncSessionPrivate::writeSocketDataHandle,
        this,
        boost::asio::placeholders::error)
    );
  }
}

void ClientAsyncSessionPrivate::writeSocketDataHandle(
  const boost::system::error_code &ec) {
  if(ec) {
    procError(ec);
  } else {
    SocketMessage *msg(write_socket_messages.front());
    write_socket_messages.pop_front();
    out_status_sig(msg->packet, SENDED);
    delete [] msg->data;
    delete msg;
    if(!write_socket_messages.empty()) {
      boost::asio::async_write(
        socket,
        boost::asio::buffer(write_socket_messages.front()->data,
                            write_socket_messages.front()->len),
        boost::bind(
          &ClientAsyncSessionPrivate::writeSocketDataHandle,
          this,
          boost::asio::placeholders::error)
      );
    }
  }
}

void ClientAsyncSessionPrivate::startTimer() {
  now_in_speed = in_speed_it;
  in_speed_it = 0;
  now_out_speed = out_speed_it;
  out_speed_it = 0;
  procSessionInitTimeout();
  procEnquireLinkTimeout();
  procInactivityTimeout();
  processInTransactions();
  processOutTransactions();
  timer.expires_at(timer.expires_at() + boost::posix_time::seconds(1));
  timer.async_wait(boost::bind(&ClientAsyncSessionPrivate::startTimer, this));
}

void ClientAsyncSessionPrivate::stopTimer() {
  timer.cancel();
}

ClientAsyncSession::ClientAsyncSession(
  boost::asio::io_service &service,
  int32 max_in_speed,
  int32 max_out_speed,
  int32 session_init_time,
  int32 enquire_link_time,
  int32 inactivity_time,
  int32 transaction_time,
  int32 max_in_trans,
  int32 max_out_trans) :
  p(new ClientAsyncSessionPrivate(this,
                                  service,
                                  max_in_speed,
                                  max_out_speed,
                                  session_init_time,
                                  enquire_link_time,
                                  inactivity_time,
                                  transaction_time,
                                  max_in_trans,
                                  max_out_trans)) {

}

ClientAsyncSession::~ClientAsyncSession() {
  if(p) {
    delete p;
    p = 0;
  }
}

void ClientAsyncSession::setMaxInSpeed(uint32 max_in_speed) {
  p->max_in_speed = max_in_speed;
}

void ClientAsyncSession::setMaxOutSpeed(uint32 max_out_speed) {
  p->max_out_speed = max_out_speed;
}

void ClientAsyncSession::setSessionInitTime(uint32 session_init_time) {
  p->session_init_time = session_init_time;
}

void ClientAsyncSession::setEnquireLinkTime(uint32 enquire_link_time) {
  p->enquire_link_time = enquire_link_time;
}

void ClientAsyncSession::setInactivityTime(uint32 inactivity_time) {
  p->inactivity_time = inactivity_time;
}

void ClientAsyncSession::setTransactionTime(uint32 transaction_time) {
  p->transaction_time = transaction_time;
}

void ClientAsyncSession::setMaxInTrans(uint32 max_in_trans) {
  p->max_in_trans = max_in_trans;
}

void ClientAsyncSession::setMaxOutTrans(uint32 max_out_trans) {
  p->max_out_trans = max_out_trans;
}

smpp::sessions::SessionStates ClientAsyncSession::state() const {
  return p->now_state;
}

boost::asio::ip::tcp::endpoint ClientAsyncSession::remoteEndpoint() const {
  return p->socket.remote_endpoint();
}

boost::asio::ip::tcp::endpoint ClientAsyncSession::localEndpoint() const {
  return p->socket.local_endpoint();
}

uint32 ClientAsyncSession::getMaxInSpeed() const {
  return p->max_in_speed;
}

uint32 ClientAsyncSession::getMaxOutSpeed() const {
  return p->max_out_speed;
}

uint32 ClientAsyncSession::getNowInSpeed() const {
  return p->now_in_speed;
}

uint32 ClientAsyncSession::getNowOutSpeed() const {
  return p->now_out_speed;
}

uint32 ClientAsyncSession::getSessionInitTime() const {
  return p->session_init_time;
}

uint32 ClientAsyncSession::getEnquireLinkTime() const {
  return p->enquire_link_time;
}

uint32 ClientAsyncSession::getInactivityTime() const {
  return p->inactivity_time;
}

uint32 ClientAsyncSession::getTransactionTime() const {
  return p->transaction_time;
}

uint32 ClientAsyncSession::getMaxInTrans() const {
  return p->max_in_trans;
}

uint32 ClientAsyncSession::getMaxOutTrans() const {
  return p->max_out_trans;
}

uint32 ClientAsyncSession::getOpenedInTrans() const {
  return p->in_transactions.size();
}

uint32 ClientAsyncSession::getOpenedOutTrans() const {
  return p->out_transactions.size();
}

const boost::system::error_code &ClientAsyncSession::getLastError() const {
  return p->error_code;
}

void ClientAsyncSession::openSession(const boost::asio::ip::tcp::endpoint &ep) {
  p->io_service.post(
    boost::bind(&ClientAsyncSessionPrivate::openSession, p, ep));
}

void ClientAsyncSession::closeSession() {
  p->io_service.post(boost::bind(&ClientAsyncSessionPrivate::closeSession, p));
}

void ClientAsyncSession::sendPacket(const HeaderPtr &packet) {
  p->io_service.post(
    boost::bind(&ClientAsyncSessionPrivate::sendPacket, p, packet));
}

boost::signals2::connection ClientAsyncSession::openedSession(
  const voidSig::slot_type &slot) {
  return p->opened_sess_sig.connect(slot);
}

boost::signals2::connection ClientAsyncSession::closedSession(
  const voidSig::slot_type &slot) {
  return p->closed_sess_sig.connect(slot);
}

boost::signals2::connection ClientAsyncSession::receivePacket(
  const headerSig::slot_type &slot) {
  return p->receive_packet_sig.connect(slot);
}

boost::signals2::connection ClientAsyncSession::outStatus(
  const statSig::slot_type &slot) {
  return p->out_status_sig.connect(slot);
}

boost::signals2::connection ClientAsyncSession::inStatus(
  const statSig::slot_type &slot) {
  return p->in_status_sig.connect(slot);
}

boost::signals2::connection ClientAsyncSession::stateChanged(
  const stateSig::slot_type &slot) {
  return p->state_changed_sig.connect(slot);
}

boost::signals2::connection ClientAsyncSession::sessionInitTimeout(
  const voidSig::slot_type &slot) {
  return p->sess_init_timeout_sig.connect(slot);
}

boost::signals2::connection ClientAsyncSession::enquireLinkTimeout(
  const voidSig::slot_type &slot) {
  return p->el_timeout_sig.connect(slot);
}

boost::signals2::connection ClientAsyncSession::inactivityTimeout(
  const voidSig::slot_type &slot) {
  return p->inact_timeout_sig.connect(slot);
}

boost::signals2::connection ClientAsyncSession::error(
  const errorSig::slot_type &slot) {
  return p->error_sig.connect(slot);
}

} // namespace sessions

} // namespace smpp
