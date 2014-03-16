#include <iostream>
#include <hmac.h>
#include <QtGlobal>

using namespace std;
using namespace app;

bool byteArrayCompare(const QByteArray& a, const QByteArray& b) {
    return a.length() == b.length() && a.contains(b);
}

int main(int argc, char** argv) {
    Q_ASSERT(byteArrayCompare(HMAC::sha1("", ""), QByteArray::fromHex("fbdb1d1b18aa6c08324b7d64b71fb76370690e1d"))); 
    Q_ASSERT(byteArrayCompare(HMAC::sha1("secret", "The quick brown fox jumps over the lazy dog's tail!"), QByteArray::fromHex("74efb20c613e662df43fdf5ff9fc2c12272971a9"))); 
    Q_ASSERT(byteArrayCompare(HMAC::sha1("YadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYada!!", "The quick brown fox jumps over the lazy dog's tail!"), QByteArray::fromHex("66c37245cedf178a37022259f721e8cfd9c91fb3"))); 

    Q_ASSERT(byteArrayCompare(HMAC::md5("", ""), QByteArray::fromHex("74e6f7298a9c2d168935f58c001bad88"))); 
    Q_ASSERT(byteArrayCompare(HMAC::md5("secret", "The quick brown fox jumps over the lazy dog's tail!"), QByteArray::fromHex("279111ddb5894165e1ff73245eef9b4e"))); 
    Q_ASSERT(byteArrayCompare(HMAC::md5("YadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYadaYada!!", "The quick brown fox jumps over the lazy dog's tail!"), QByteArray::fromHex("9923c5f651877fb2be56071935b5e07c"))); 
}
