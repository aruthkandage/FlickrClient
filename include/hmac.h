/**
 *  Qt Flickr Client
 *  Copyright (C) 2014 Aruth Kandage
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*/ 

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
