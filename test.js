var SerialPort = require('serialport').SerialPort;
var request = require('request');

var sp= new SerialPort("COM3");

sp.on("open", function()
{
sp.on('data', function(data){
var datajson=JSON.parse(data.toString());
request({
url:" https://api.thingspeak.com/update ",
method:"post",
headers: {
'X-THINGSPEAKAPIKEY' : 'TS6DLQATECACCDWE'
},
form:{
field1:JSON.stringify(dataJson.Tempreature),
field2:JSON.stringify(dataJson.Humidity)
}
},
function(err,response,body){
if (err)
{
console.log(err);
}
else{
console.log(response.statusCode,body);
}
});
});
});