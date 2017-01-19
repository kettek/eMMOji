var eMMOji = eMMOji || {};

eMMOji.Client = eMMOji.Client || function() {
  var mSocket = null;
  var mUrl    = '';

  this.onOpen = function(event) {
    console.log('connection opened to ' + mUrl);
    console.log(event);
    mSocket.send('hai!');
  }
  this.onClose = function(event) {
    console.log('connection closed to ' + mUrl);
    console.log(event);
  }
  this.onMessage = function(event) {
    console.log('Message: ', event.data);
  }

  this.gogogo = function(url) {
    mSocket = new WebSocket(mUrl = url);

    mSocket.addEventListener('open', this.onOpen);
    mSocket.addEventListener('close', this.onClose);
    mSocket.addEventListener('message', this.onMessage);
  }
};
