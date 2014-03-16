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
