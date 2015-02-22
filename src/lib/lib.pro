include(../../smpp.pri)

TEMPLATE = lib

DESTDIR = $$LIBRARYES_PATH

TARGET = $$qtLibraryName(smpp)

win32: DLLDESTDIR = $$LIBRARYES_PATH

DEFINES += SMPP_EXPORTS

unix: {
  INSTALLS += /usr/lib
}

win32-g++ {
  QMAKE_CXXFLAGS += -std=c++11
}

INCLUDEPATH += $$PWD \
               G:/src/c++/lib/boost_1_56_0

DEPENDPATH += G:/src/c++/lib/boost_1_56_0

LIBS += -L"G:/src/c++/lib/boost_1_56_0/stage/lib/" \
        -llibboost_system-mgw48-d-1_56 \
        -lws2_32

SOURCES += \
  smpp/pdu/smpp_header.cpp \
  smpp/pdu/smpp_tlv.cpp \
  smpp/pdu/smpp_tlv_fields.cpp \
  smpp/pdu/alert_notification.cpp \
  smpp/pdu/smpp_bind_transmitter_resp.cpp \
  smpp/pdu/smpp_bind_transmitter.cpp \
  smpp/pdu/smpp_bind_transceiver_resp.cpp \
  smpp/pdu/smpp_bind_transceiver.cpp \
  smpp/pdu/smpp_bind_receiver_resp.cpp \
  smpp/pdu/smpp_bind_receiver.cpp \
  smpp/pdu/smpp_outbind.cpp \
  smpp/pdu/smpp_unbind_resp.cpp \
  smpp/pdu/smpp_unbind.cpp \
  smpp/pdu/smpp_generic_nack.cpp \
  smpp/pdu/smpp_enquire_link_resp.cpp \
  smpp/pdu/smpp_enquire_link.cpp \
  smpp/pdu/smpp_submit_sm_resp.cpp \
  smpp/pdu/smpp_submit_sm.cpp \
  smpp/pdu/smpp_deliver_sm_resp.cpp \
  smpp/pdu/smpp_deliver_sm.cpp \
  smpp/pdu/smpp_data_sm_resp.cpp \
  smpp/pdu/smpp_data_sm.cpp \
  smpp/pdu/smpp_replace_sm_resp.cpp \
  smpp/pdu/smpp_replace_sm.cpp \
  smpp/pdu/smpp_query_sm_resp.cpp \
  smpp/pdu/smpp_query_sm.cpp \
  smpp/pdu/smpp_cancel_sm_resp.cpp \
  smpp/pdu/smpp_cancel_sm.cpp \
  smpp/pdu/smpp_submit_multi_resp.cpp \
  smpp/pdu/smpp_submit_multi.cpp \
  smpp/pdu/smpp_unsuccess_sme.cpp \
  smpp/pdu/smpp_multi_destination_address.cpp \
  smpp/pdu/smpp_distribution_list_address.cpp \
  smpp/pdu/smpp_sme_multi_address.cpp \
  smpp/encoders/smpp_hex_encoder.cpp \
  smpp/encoders/smpp_binary_encoder.cpp \
  smpp/decoders/smpp_binary_decoder.cpp \
  smpp/sessions/smpp_client_async_session.cpp #\
  #smpp/sessions/smpp_server_async_session.cpp

HEADERS +=\
  smpp/pdu/smpp_tlv.h \
  smpp/pdu/smpp_tlv_fields.h \
  smpp/smpp_global.h \
  smpp/pdu/smpp_alert_notification.h \
  smpp/pdu/smpp_header.h \
  smpp/parameters/smpp_prameters_header.h \
  smpp/parameters/smpp_prameters_optional.h \
  smpp/parameters/smpp_prameters_mandatory.h \
  smpp/pdu/smpp_bind_transmitter_resp.h \
  smpp/pdu/smpp_bind_transmitter.h \
  smpp/pdu/smpp_bind_transceiver_resp.h \
  smpp/pdu/smpp_bind_transceiver.h \
  smpp/pdu/smpp_bind_receiver_resp.h \
  smpp/pdu/smpp_bind_receiver.h \
  smpp/pdu/smpp_outbind.h \
  smpp/pdu/smpp_unbind_resp.h \
  smpp/pdu/smpp_unbind.h \
  smpp/pdu/smpp_generic_nack.h \
  smpp/pdu/smpp_enquire_link_resp.h \
  smpp/pdu/smpp_enquire_link.h \
  smpp/pdu/smpp_submit_sm_resp.h \
  smpp/pdu/smpp_submit_sm.h \
  smpp/pdu/smpp_deliver_sm_resp.h \
  smpp/pdu/smpp_deliver_sm.h \
  smpp/pdu/smpp_data_sm_resp.h \
  smpp/pdu/smpp_data_sm.h \
  smpp/pdu/smpp_replace_sm_resp.h \
  smpp/pdu/smpp_replace_sm.h \
  smpp/pdu/smpp_query_sm_resp.h \
  smpp/pdu/smpp_query_sm.h \
  smpp/pdu/smpp_cancel_sm_resp.h \
  smpp/pdu/smpp_cancel_sm.h \
  smpp/pdu/smpp_submit_multi_resp.h \
  smpp/pdu/smpp_submit_multi.h \
  smpp/pdu/smpp_unsuccess_sme.h \
  smpp/pdu/smpp_multi_destination_address_base.h \
  smpp/pdu/smpp_multi_destination_address.h \
  smpp/pdu/smpp_distribution_list_address.h \
  smpp/pdu/smpp_sme_multi_address.h \
  smpp/encoders/smpp_hex_encoder.h \
  smpp/encoders/smpp_binary_encoder.h \
  smpp/decoders/smpp_binary_decoder.h \
  smpp/sessions/smpp_session_global.h \
  smpp/sessions/smpp_client_async_session.h \
  smpp/sessions/smpp_client_async_session_p.h   #\
  #smpp/sessions/smpp_server_async_session_p.h \
  #smpp/sessions/smpp_server_async_session.h
