// USED TO OBTAIN SOCKET IO DATA FROM SERVER
// SOCKET IO INTERFACE 
var socket = io();

// When socket gets message over socket io on sensors channel. 

socket.on('sensors', function(data) {
  // Sets data
  document.getElementById("sensor1").innerHTML = ""+data.sensor1;
  document.getElementById("sensor2").innerHTML = ""+data.sensor2;
  document.getElementById('server-time').innerHTML =  new Date(data.sentTime * 1000);
  //https://stackoverflow.com/questions/4797675/how-do-i-re-trigger-a-webkit-css-animation-via-javascript
  // Removes the animation class from the status div. then resets the animation frame and adds it back. 
  $('#status').removeClass("animation");
  window.requestAnimationFrame(function(time) {
    window.requestAnimationFrame(function(time) {
    $('#status').addClass("animation");
    });
  });    
});

