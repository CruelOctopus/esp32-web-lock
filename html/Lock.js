

function StateChange(){
var request = new XMLHttpRequest();
request.open('GET', '/getvalue');
request.responseType = 'text';

request.onload = function() {
var bytearray;
	alert(request.response);
	bytearray = toByteArray(request.response);
	key = toByteArray(request.response);
	Crypto.HMAC(Crypto.SHA256 , bytearray, key);
};

request.send();
}

function readFile(input) {
  let file = input.files[0];

  let reader = new FileReader();

  reader.readAsText(file);

  reader.onload = function() {
    console.log(reader.result);
  };

  reader.onerror = function() {
    console.log(reader.error);
  };

}

function toByteArray(hexString) {
  var result = [];
  for (var i = 0; i < hexString.length; i += 2) {
    result.push(parseInt(hexString.substr(i, 2), 16));
  }
  return result;
}
function ReadKeyFile() {
readFile(document.getElementById('file-input'));
document.getElementById('keybutton').style.background = "Green";
}