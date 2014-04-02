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

#include <flickr_request.h>
#include <flickr_oauth.h>

#include <QCoreApplication>

using namespace app;

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
    QCoreApplication app(argc, argv);

    FlickrOAuthAuthentication oauth;

    oauth.authenticate();

    app.exec();
}
