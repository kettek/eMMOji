var eMMOji = eMMOji || {};

eMMOji.Client = eMMOji.Client || function() {
  var mSocket = null;
  var mUrl    = '';
  var mState  = 0;
  var STATE = {
    CONNECTING: 0,
    LOGGED_IN:  1,
    READY:      2
  };

  // stub handler functions
  this.handlerMessage = function() {};

  this.onOpen = function(event) {
    console.log('connection opened to ' + mUrl);

    var login = {
      name: "Client"
    };
    mSocket.send(JSON.stringify(login));
  }
  this.onClose = function(event) {
    console.log('connection closed to ' + mUrl);
    console.log(event);
  }
  this.onMessage = function(event) {
    switch (mState) {
      case STATE.CONNECTING:
        if (event.data == 'OK') {
          console.log('Logged in!');
          mState = STATE.LOGGED_IN;
        }
        break;
      case STATE.LOGGED_IN:
        console.log('Message: ', event.data);
        // keep receiving updates and once the Map is fully populated, send OK
        //mSocket.send("OK");
        break;
    }
  }

  this.gogogo = function(url) {
    mSocket = new WebSocket(mUrl = url);

    mSocket.addEventListener('open', this.onOpen);
    mSocket.addEventListener('close', this.onClose);
    mSocket.addEventListener('message', this.onMessage);
  }
};
