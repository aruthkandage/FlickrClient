#include <iostream>
#include <flickr_request.h>

using namespace app;
using namespace std;

void test_signature() {
    FlickrRequestBase request("https://www.flickr.com/services/oauth/request_token");

    request.addRequestParam("oauth_nonce", "37026218");
    
    cout << request.getSignature().data() << endl;
}

int main(int argc, char** argv) {
    test_signature();
}
