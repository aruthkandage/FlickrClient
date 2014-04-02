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

#include <hmac.h>
#include <QCryptographicHash>

namespace app {

QByteArray HMAC::hmac(const QByteArray& key, const QByteArray& message, QCryptographicHash::Algorithm hashAlgorithm) {
    static const int blockSize = 64; // block-size for MD5 & SHA-1
    QByteArray keyCopy;
    QByteArray messageCopy(message);
    QByteArray result;

    if(key.length() > blockSize) {
        // shorten key to blockSize using hash 
        keyCopy = QCryptographicHash::hash(key, hashAlgorithm); 
    } else if(key.length() < blockSize) {
        // zero pad the key so that it is blockSize
        keyCopy = key;
        keyCopy.append(QByteArray(blockSize - key.length(), (char) 0)); 
    } else {
        keyCopy = key;
    }

    QByteArray innerPad(blockSize, (char) 0x36);
    for(int i=0; i < blockSize; i++) {
        innerPad[i] = innerPad[i] ^ keyCopy[i];
    }

    QByteArray outerPad(blockSize, (char) 0x5C);
    for(int i=0; i < blockSize; i++) {
        outerPad[i] = outerPad[i] ^ keyCopy[i];
    }

    result = QCryptographicHash::hash(messageCopy.prepend(innerPad), hashAlgorithm);
    return QCryptographicHash::hash(result.prepend(outerPad), hashAlgorithm);
}

QByteArray HMAC::sha1(const QByteArray& key, const QByteArray& message) {
    return hmac(key, message, QCryptographicHash::Sha1);
}

QByteArray HMAC::md5(const QByteArray& key, const QByteArray& message) {
    return hmac(key, message, QCryptographicHash::Md5);
}

}
