#ifndef __HMAC_H__
#define __HMAC_H__

/*
 * Hash-based Message Authentication Algorithm (HMAC)
 * using algorithms from QCryptographic hash (MD4/MD5/SHA-1)
 */

#include <QCryptographicHash>

namespace app {
    class HMAC {
        public:
        static QByteArray hmac(const QByteArray&, const QByteArray&, QCryptographicHash::Algorithm);
        
        /* Convenience functions */
        static QByteArray sha1(const QByteArray&, const QByteArray&);
        static QByteArray md5(const QByteArray&, const QByteArray&);
    };
}

#endif
