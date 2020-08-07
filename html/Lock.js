var HexKey ="";
function StateChange(){
if(HexKey ===""){alert("Enter the key first!");}else{

var HMACResult ="";
var request = new XMLHttpRequest();
request.open('GET', '/getvalue');
request.responseType = 'text';

request.onload = function() {
var bytearray;
var ByteKey;

	console.log(request.response);
	bytearray = Crypto.util.hexToBytes(request.response);
	console.log(bytearray);
	ByteKey = Crypto.util.hexToBytes(HexKey);
	console.log(ByteKey);
	HMACResult = Crypto.HMAC(Crypto.SHA256 , bytearray, ByteKey);
	console.log(HMACResult);
	
	var request1 = new XMLHttpRequest();
	request1.open('GET', '/lock/0n/'+HMACResult);
	request1.responseType = 'json';

	request1.onload = function() {
	console.log(request1.response);
	alert(request1.response);
	};
	request1.send();
	
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