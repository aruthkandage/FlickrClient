#include <iostream>
#include <flickr_request.h>

using namespace app;
using namespace std;

/*void test_signature() {
    FlickrRequestBase request("https://api.flickr.com/services/rest");

    request.addRequestParam("nojsoncallback", "1");
    request.addRequestParam("format", "json");
    request.addRequestParam("oauth_nonce", "84354935");
    request.addRequestParam("oauth_timestamp", "1305583871");
    request.addRequestParam("oauth_consumer_key", "653e7a6ecc1d528c516cc8f92cf98611");
    request.addRequestParam("oauth_signature_method", "HMAC-SHA1");
    request.addRequestParam("oauth_version", "1.0");
    request.addRequestParam("oauth_token", "72157626318069415-087bfc7b5816092c");
    request.addRequestParam("method", "flickr.test.login");

    request.setKey("72157626318069415-087bfc7b5816092c");
    request.setSecret("a202d1f853ec69de");
    
    cout << request.getSignature().data() << endl;
}*/

int main(int argc, char** argv) {
    /* test_signature(); */
}
