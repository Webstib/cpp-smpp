#ifndef SMPP_HELP_FUNC_H
#define SMPP_HELP_FUNC_H

#include <QtCore/QTextCodec>
#include "smpp/parameters/smpp_prameters_mandatory.h"

namespace smpp {

namespace helpers {

static const quint32 UDH_LEN = 0x00000006;

struct SMSUDH {
  quint8 sequence_number;
  quint8 length;
  quint8 position;
};

static inline bool getUdh(SMSUDH &udh, const QByteArray &b_data) {
  if((quint32)b_data.size() < UDH_LEN)
    return false;

  if(b_data[0] == 0x05 && b_data[1] == 0x00 && b_data[2] == 0x03) {
    udh.sequence_number = b_data[3];
    udh.length = b_data[4];
    udh.position = b_data[5];
    return true;
  } else {
    return false;
  }
}

static inline QByteArray toDataCodes(const QString &data, quint8 codes) {
  QTextCodec *codec(0);
  switch(codes) {
    case smpp::parameters::data_coding::ASCII: {
      QByteArray b_data(data.toUtf8());
      const int b_data_size(b_data.size());
      const int ol(b_data_size * 7 / 8);
      const bool rest(ol != (double)b_data_size * 7 / 8);
      QByteArray output(ol + (rest ? 1 : 0), '\0');

      int bits(0);
      unsigned char octet(0x00);

      for(int i = 0, j = 0; i < b_data_size; i++) {
        if(bits == 7) {
          bits = 0;
          continue;
        }
        octet = (b_data[i] & 0x7f) >> bits;
        if(i < b_data_size - 1)
          octet |= ((b_data[i + 1] & 0x7f) << (7 - bits));
        output[j++] = octet;
        bits++;
      }

      return output;
    }
    case smpp::parameters::data_coding::BIT8_2:
    case smpp::parameters::data_coding::BIT8_4:
      return data.toLocal8Bit();
    case smpp::parameters::data_coding::JIS:
      codec = QTextCodec::codecForName("JIS X 0208");
      break;
    case smpp::parameters::data_coding::CYRLLIC:
      codec = QTextCodec::codecForName("ISO 8859-5");
      break;
    case smpp::parameters::data_coding::HUBREW:
      codec = QTextCodec::codecForName("ISO 8859-8");
      break;
    case smpp::parameters::data_coding::PICTOGRAM:
      codec = QTextCodec::codecForName("Shift-JIS");
      break;
    case smpp::parameters::data_coding::MUSIC:
      codec = QTextCodec::codecForName("ISO 2022-JP");
      break;
    case smpp::parameters::data_coding::UCS2:
      codec = QTextCodec::codecForName("UTF-16BE");
      return codec->fromUnicode(data).mid(2);
    case smpp::parameters::data_coding::LATIN_1:
    case smpp::parameters::data_coding::SMSC_DEFAULT:
    default:
      return data.toLatin1();
  }

  return codec->fromUnicode(data);
}

static inline QString fromDataCodes(const QByteArray &b_data, quint8 codes) {
  QTextCodec *codec(0);
  switch(codes) {
    case smpp::parameters::data_coding::ASCII: {
      const int b_data_size(b_data.size()), ol(b_data_size * 8 / 7);
      int i(0), j(0);
      const bool rest(ol != (double)b_data_size * 8 / 7);
      QByteArray output(ol + (rest ? 1 : 0), '\0');
      quint8 bits(0), octet(0), low_byte(0), high_byte(0);
      while(j < b_data_size) {
        high_byte = b_data[j];
        if(bits == 0) {
          octet = high_byte & 0x7F;
        } else if(bits == 7) {
          octet = high_byte >> 1;
        } else {
          low_byte = b_data[j - 1];
          octet = (high_byte << bits) | (low_byte >> (8 - bits));
          octet &= 0x7F;
        }
        output[i++] = octet;
        if(bits != 6)
          j++;
        if(bits == 7) {
          bits = 0;
        } else {
          bits++;
        }
      }
      return QString::fromUtf8(output);
    }
    case smpp::parameters::data_coding::BIT8_2:
    case smpp::parameters::data_coding::BIT8_4:
      return QString::fromLocal8Bit(b_data);
    case smpp::parameters::data_coding::JIS:
      codec = QTextCodec::codecForName("JIS X 0208");
      break;
    case smpp::parameters::data_coding::CYRLLIC:
      codec = QTextCodec::codecForName("ISO 8859-5");
      break;
    case smpp::parameters::data_coding::HUBREW:
      codec = QTextCodec::codecForName("ISO 8859-8");
      break;
    case smpp::parameters::data_coding::UCS2:
      codec = QTextCodec::codecForName("UTF-16BE");
      break;
    case smpp::parameters::data_coding::PICTOGRAM:
      codec = QTextCodec::codecForName("Shift-JIS");
      break;
    case smpp::parameters::data_coding::MUSIC:
      codec = QTextCodec::codecForName("ISO 2022-JP");
      break;
    case smpp::parameters::data_coding::LATIN_1:
    case smpp::parameters::data_coding::SMSC_DEFAULT:
    default:
      return QString::fromLatin1(b_data);
      break;
  }

  return codec->toUnicode(b_data);
}

static inline QByteArray convertDataBetweenCodes(quint8 from_codes,
                                          quint8 to_codes,
                                          const QByteArray &from_data) {
  return toDataCodes(fromDataCodes(from_data, from_codes), to_codes);
}

} // namespace helpers

} // namespace smpp


#endif // SMPP_HELP_FUNC_H
