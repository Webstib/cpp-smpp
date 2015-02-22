#include "smpp/parameters/smpp_prameters_header.h"
#include "test_client.h"

TestSmppClient::TestSmppClient() :
  sequence_number(0),
  io(new boost::asio::io_service),
  session(new smpp::sessions::ClientAsyncSession(
    *io,
    in_speed,
    out_speed,
    session_init_time,
    enquire_link_time,
    inactivity_time,
    transaction_time,
    max_in_trans,
    max_out_trans
  )),
  send_timer(new boost::asio::deadline_timer(
    *io, boost::posix_time::milliseconds(send_speed))),
  console_update_timer(new boost::asio::deadline_timer(
    *io, boost::posix_time::milliseconds(console_update))) {

  session->error(boost::bind(&TestSmppClient::error, this, _1));

  session->openedSession(boost::bind(
    &TestSmppClient::openedSession,
    this));

  session->receivePacket(boost::bind(&TestSmppClient::receivePacket, this, _1));

  boost::asio::ip::tcp::endpoint ep(
    boost::asio::ip::address::from_string(host), port);

  session->openSession(ep);

  io->run();
}

void TestSmppClient::initConsoleUpdateTimer() {
  console_update_timer->expires_at(
    console_update_timer->expires_at() +
    boost::posix_time::milliseconds(console_update));
  console_update_timer->async_wait(
    boost::bind(&TestSmppClient::consoleUpdate, this));
}

void TestSmppClient::consoleUpdate() {
#ifdef unix
  std::system("clear");
#else
  std::system("CLS");
#endif
  std::cout <<
  "Client " << session->remoteEndpoint().address() << ":"
            << session->remoteEndpoint().port() << "\n" <<
  "Client Now In Speed " << session->getNowInSpeed() << "\n" <<
  "Client Now Out Speed " << session->getNowOutSpeed() << "\n" <<
  "Client Opened In Transaction " << session->getOpenedInTrans() << "\n" <<
  "Client Opened Out Transaction " << session->getOpenedOutTrans() << "\n";
  initConsoleUpdateTimer();
}

void TestSmppClient::initSendTimer() {
  send_timer->expires_at(
    send_timer->expires_at() + boost::posix_time::milliseconds(send_speed));
  send_timer->async_wait(boost::bind(&TestSmppClient::sendMessage, this));
}

void TestSmppClient::sendMessage() {
  if(session->state() == smpp::sessions::BOUND_TRX && sequence_number < max_iter + 1) {
    //std::cout << "send message " << sequence_number << "\r\n";
    std::string text("test");
    smpp::pdu::SubmitSm *ssm(new smpp::pdu::SubmitSm(
      sequence_number++,
      "",
      1,
      0,
      "79048794747",
      1,
      0,
      "79048794747",
      0,
      0,
      0,
      "",
      "",
      1,
      0,
      0,
      0,
      text.size() + 1,
      text.data()));
    ssm->calcCommandLength();
    session->sendPacket(smpp::sessions::HeaderPtr(ssm));
    initSendTimer();
  } else {
    send_timer->cancel();
  }
}

void TestSmppClient::openedSession() {
  std::cout << "opened session\r\n";
  smpp::pdu::BindTransceiver *bt(new smpp::pdu::BindTransceiver(
    sequence_number++,
    "smppclient1",
    "password"));
  bt->calcCommandLength();
  session->sendPacket(smpp::sessions::HeaderPtr(bt));
}

void TestSmppClient::closedSession() {

}

void TestSmppClient::receivePacket(smpp::sessions::HeaderPtr p) {
  switch(p->getCommandId()) {
    case smpp::parameters::command_id::BIND_TRANSCEIVER_RESP:
      if(p->getCommandStatus() == 0) {
        std::cout << "bind trx ok\r\n";
        sendMessage();
        consoleUpdate();
      } else {
        std::cout << "bind fault ok\r\n";
      }
      break;
    case smpp::parameters::command_id::SUBMIT_SM_RESP:
      //std::cout << "submit_sm_resp " << p->getSequenceNumber() << "\r\n";
      break;
    case smpp::parameters::command_id::DELIVER_SM: {
      //std::cout << "deliver_sm " << p->getSequenceNumber() << "\r\n";
      smpp::sessions::HeaderPtr dsm_resp(new smpp::pdu::DeliverSmResp(
        0, p->getSequenceNumber()));
      session->sendPacket(dsm_resp);
      break;
    }
    case smpp::parameters::command_id::ENQUIRE_LINK: {
      //std::cout << "ENQUIRE_LINK " << p->getSequenceNumber() << "\r\n";
      smpp::sessions::HeaderPtr el_resp(new smpp::pdu::EnquireLinkResp(
        0, p->getSequenceNumber()));
      session->sendPacket(el_resp);
      break;
    }
    default:
      std::cout << "receive unknown packet ok\r\n";
      break;
  }
}

void TestSmppClient::outStatus(smpp::sessions::HeaderPtr,
                               smpp::sessions::StatusMessage) {

}

void TestSmppClient::inStatus(smpp::sessions::HeaderPtr,
                              smpp::sessions::StatusMessage) {

}

void TestSmppClient::stateChanged(smpp::sessions::SessionStates) {

}

void TestSmppClient::sessionInitTimeout() {

}

void TestSmppClient::enquireLinkTimeout() {

}

void TestSmppClient::inactivityTimeout() {

}

void TestSmppClient::error(const boost::system::error_code &e) {
  std::cout << "error session: " << e.message() << "\r\n";
  send_timer->cancel();
}
