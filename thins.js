var serialport = require('serialport')
var request = require('request');

var sp= new serialport.SerialPort("COM4",
{
	baudrate:9600,
	parser:serialport.parsers.readline("\n")

});

sp.on("open", function ()
{
	console.log("Ready to listen....");
	sp.on('data', function(data){
		console.log(data.toString());
		var dataJson= JSON.parse(data.toString());

		console.log("making a temp request");
		console.log(dataJson.Temperature);

		 request({
		 	url:'https://api.thingspeak.com/update',
		 	method:'POST',
		 	headers: {
		 		'X-THINGSPEAKAPIKEY' : 'TS6DLQATECACCDWE'

		 	},

		 	form: {
		 		field1: JSON.stringfy(dataJson.Temperature),
		 		field2: JSON.stringfy(dataJson.Humidity)
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
