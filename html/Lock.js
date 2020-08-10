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
	var CurrentState = document.getElementById('lock1').value;
	var request1 = new XMLHttpRequest();
	if (CurrentState == "Unlock"){
		request1.open('GET', '/lock/lock/'+HMACResult);
	}
	if (CurrentState == "Lock"){
		request1.open('GET', '/lock/unlock/'+HMACResult);
	}
	request1.responseType = 'json';
	request1.onload = function() 
	{
		console.log(request1.response);
		alert(request1.response);
		if(request1.response.device == "Lock1")
		{
			if(request1.response.state == "Unlock")
			{
				document.getElementById('lock1').style.background = "Red";
				document.getElementById('lock1').value = request1.response.state;
				document.getElementById('lock1').innerHTML = request1.response.state;
			}
			if(request1.response.state == "Lock")
			{
				document.getElementById('lock1').style.background = "Green";
				document.getElementById('lock1').value = request1.response.state;
				document.getElementById('lock1').innerHTML = request1.response.state;
			}
		}
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