var HexKey ="";
function StateChange(){
if(HexKey ===""){alert("Enter the key first!");}else{
var request = new XMLHttpRequest();
request.open('GET', '/getvalue');
request.responseType = 'text';

request.onload = function() {
var bytearray;
var ByteKey;
	alert(request.response);
	bytearray = toByteArray(request.response);
	ByteKey = toByteArray(HexKey);
	Crypto.HMAC(Crypto.SHA256 , bytearray, ByteKey);
};

request.send();
}
}

function readFile(input) {
  let file = input.files[0];

  let reader = new FileReader();

  reader.readAsText(file);

  reader.onload = function() {
    console.log(reader.result);
	HexKey = reader.result;
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