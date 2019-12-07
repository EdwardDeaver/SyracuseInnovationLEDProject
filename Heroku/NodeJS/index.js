'use strict';
const express = require('express');
const path = require('path')
const bodyParser = require('body-parser');
const socketIO = require('socket.io');

const PORT = process.env.PORT || 5000

// SETUP FILES
//////////////
// Post: Description :  Creates a Post request end point for data. The data sent in is then verified. If successfully verified it is sent to the socket io client. 
//     Pre-conditions: NodeJS has the key stored as an environment variable. 
//     Post-conditions: Connection is closed.  
const server = express()
  .use(express.static(path.join(__dirname, 'public')))
  .use(bodyParser.json())
  .set('views', path.join(__dirname, 'views'))
  .set('view engine', 'ejs')
  .get('/', (req, res) => res.render('pages/index'))
  .post("/sendsensorsata", function(request, response) {
    let SENSOR1 = 0;
  	let SENSOR2 = 0;
  	let sensor1Status = false;
  	let sensor2Status = false;
  	let secretStatus = false;
  	console.log("AT CHECKS");
  	//console.log(response);
  	secretStatus = checkSecret(request.body.secret);
  	if(secretStatus){
  		console.log("secret passed");
  	}
  	else{
  			response.status(400);
  			response.end();
  	}
  	sensor1Status = checkSensor(request.body.sensor1);
  	if(sensor1Status){
  		console.log("sensor1Status passed");
  	}
    else{
        response.status(400);
        response.end();
    }
  	sensor2Status = checkSensor(request.body.sensor2);
  	if(sensor2Status){
  		console.log("sensor2Status passed");
  	}
    else{
        response.status(400);
        response.end();
    }
  	if(secretStatus && sensor1Status && sensor2Status){
  		console.log("ALL TRUE");
  		let CollectionData = JSON.parse('{ "sensor1":'+ request.body.sensor1 +', "sensor2":'+ request.body.sensor2 +'}');
  		io.emit('sensors', CollectionData);
  		response.status(100);
  		response.end("PASSED");
  		//console.log(CollectionData);
  	//io.emit('news', CollectionData);
  	}
  	else{
  	response.status(400);
  	response.end();
  	}
  	//Socket io emit on the 'news' event
  	response.end();
})
    .listen(PORT, () => console.log(`Listening on ${ PORT }`))


// Express related
//Description :  Validates the secret key. Checks if enviroment variable is like the one sent by the server
//Pre-conditions: Environment variable set. 
//Post-conditions:Returns true or false. 
function checkSecret(secretSent){
  try{
    if (process.env.SECRET == secretSent){
       return true;   
    }
    else{
      return false;
    }
  }
  catch{
    console.log("SECRET FAILED");
    return false;
  }
}
/// EXPRESS POST RELATED FUNCTIONS
/// INPUT VALIDATION
// FUNC DESC: Validate Sensor Data
//				Checks if number is undefined
//				Checks if it is a number 
//				If not = False
//				If true = True

// Express related
//Description : Validates Sensor Data.
//Pre-conditions: None.
//Post-conditions:Returns true or false. 
function checkSensor(sensorData){
  try{
    //SENSOR IS  Defined
    if(sensorData === undefined){
      return false;
    }
    //SENSOR IS  DEFINED
    else{
      // IS SENSOR a number
      if (typeof sensorData  === "number") {
        return true;
      }
      //SENSOR 1 is NOT  number 
      else{
        return false;
      }
    }
  }
  catch{
    console.log("SENSOR FAILED");
    return false;
  }
}

var io = socketIO(server);

io.on('connection', (socket) => {
  console.log('Client connected');
  socket.on('disconnect', () => console.log('Client disconnected'));
});

setInterval(() => io.emit('time', new Date().toTimeString()), 1000);


