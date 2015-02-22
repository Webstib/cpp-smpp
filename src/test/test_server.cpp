#include "test_server.h"

TestServer::TestServer(QObject *parent) :
  QTcpServer(parent),
  timer_id(-1),
  session(0),
  test_client(0) {

}

void TestServer::start() {
  if(this->listen(QHostAddress(host), port)) {
    test_client = new TestClient();
    test_client->start();
    timer_id = startTimer(console_update);
  }
}

void TestServer::timerEvent(QTimerEvent *) {
#ifdef unix
  std::system("clear");
#else
  std::system("CLS");
#endif
  QTextStream out(stdout);
  out <<
    /*"Server " << session->localAddress().toString() << ":"
              << session->localPort() << "\n" <<
    "Server Now In Speed " << session->getNowInSpeed() << "\n" <<
    "Server Now Out Speed " << session->getNowOutSpeed() << "\n" <<
    "Server Opened In Transaction " << session->getOpenedInTrans() << "\n" <<
    "Server Opened Out Transaction " << session->getOpenedOutTrans() << "\n" <<*/
    "Client " << test_client->session->peerAddress().toString() << ":"
              << test_client->session->peerPort() << "\n" <<
    "Client Now In Speed " << test_client->session->getNowInSpeed() << "\n" <<
    "Client Now Out Speed " << test_client->session->getNowOutSpeed() << "\n" <<
    "Client Opened In Transaction " << test_client->session->getOpenedInTrans() << "\n" <<
    "Client Opened Out Transaction " << test_client->session->getOpenedOutTrans() << "\n" <<
    "ok sms " << test_client->ok << "\n" <<
    "timeout sms " << test_client->timeout << "\n"
    "transaction queue full sms " << test_client->transfull << "\n"
    "ower max speed sms " << test_client->ower_max_speed << "\n";
}

void TestServer::incomingConnection(int sock) {
  session = new smpp::sessions::ServerAsyncSession(
    sock,
    in_speed,
    out_speed,
    session_init_time,
    enquire_link_time,
    inactivity_time,
    transaction_time,
    max_in_trans,
    max_out_trans,
    threaded);
  connect(session, SIGNAL(openedSession()), SLOT(openSession()));
  connect(session, SIGNAL(closedSession()), SLOT(closeSession()));
  connect(session,
          SIGNAL(error(smpp::sessions::ErrorCode)),
          SLOT(errorSession(smpp::sessions::ErrorCode)));
  connect(session,
          SIGNAL(receive(const smpp::pdu::BindTransceiver &)),
          SLOT(bind(const smpp::pdu::BindTransceiver &)));
  connect(session,
          SIGNAL(receive(const smpp::pdu::Unbind &)),
          SLOT(unbind(const smpp::pdu::Unbind &)));
  connect(session,
          SIGNAL(receive(const smpp::pdu::SubmitSm &)),
          SLOT(submitSm(const smpp::pdu::SubmitSm &)));
  connect(session,
          SIGNAL(receive(const smpp::pdu::DeliverSmResp &)),
          SLOT(deliverSmResp(const smpp::pdu::DeliverSmResp &)));
}

void TestServer::openSession() {
  qDebug() << "open server session";
}

void TestServer::closeSession() {
  qDebug() << "close server session";
  if(timer_id != -1)
    killTimer(timer_id);
  timerEvent(0);
  session->deleteLater();
  session = 0;
  test_client->deleteLater();
  test_client = 0;
}

void TestServer::errorSession(smpp::sessions::ErrorCode) {
  if(timer_id != -1)
    killTimer(timer_id);
  qDebug() << "error server session " << session->errorString();
}

void TestServer::bind(const smpp::pdu::BindTransceiver &p) {
  smpp::pdu::BindTransceiverResp bt(0, p.getSequenceNumber());
  qDebug() << "send BindTransceiverResp" << bt.getSequenceNumber();
  bt.setCommandLength(bt.size());
  session->send(bt);
  timer_id = startTimer(console_update);
}

void TestServer::submitSm(const smpp::pdu::SubmitSm &p) {
  smpp::pdu::SubmitSmResp sm_resp(0, p.getSequenceNumber());
  sm_resp.setCommandLength(sm_resp.size());
  session->send(sm_resp);
  session->send(smpp::pdu::DeliverSm(p.getSequenceNumber()));
}

void TestServer::deliverSmResp(const smpp::pdu::DeliverSmResp &) {

}

void TestServer::unbind(const smpp::pdu::Unbind &p) {
  smpp::pdu::UnbindResp ub(p.getSequenceNumber());
  qDebug() << "send UnbindResp" << ub.getSequenceNumber();
  session->send(ub);
  if(timer_id != -1)
    killTimer(timer_id);
  timerEvent(0);
}
