var SerialPort = require('serialport').SerialPort;
var request = require('request');

var sp= new SerialPort("COM3");



sp.on("open", function ()
{
	console.log("Ready to listen....");
	sp.on('data', function(data){
		console.log(data.toString());
		var dataJson = JSON.parse(data.toString());

		console.log("making a temp request");
		console.log(dataJson.Temperature);

		 request({
		 	url:'https://api.thingspeak.com/update',
		 	method:'POST',
		 	headers: {
		 		'X-THINGSPEAKAPIKEY' : 'TS6DLQATECACCDWE'

		 	},

		 	form: {
		 		field1: JSON.stringify(dataJson.Temperature),
		 		field2: JSON.stringify(dataJson.Humidity)
		 	}
		 },
		 function(error, response, body){
		 	if (error){
		 		console.log(error);
	}
		 	else{
		 		console.log(reaponse.statusCode,body);
		 	}
		 
		 });
	});
});
