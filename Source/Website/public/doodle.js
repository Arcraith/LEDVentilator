var NAMESPACE = '/ledrotor';


function initialize(){

};


document.addEventListener("DOMContentLoaded", function() {
   var socket = io(NAMESPACE);

   var mouse = { 
      click: false,
      move: false,
      pos: {x:0, y:0},
      pos_prev: false
   };
   // get canvas element and create context
  
   var canvas = document.getElementById("drawing");
   var context = canvas.getContext('2d');
   var width   = window.innerWidth;
   var height  = window.innerHeight;
   //var socket = io.connect();

   // set canvas to full browser width/height
   canvas.width = 480;
   canvas.height = 480;

   context.fillStyle="white";
   context.fillRect(0,0,canvas.width, canvas.height);

   // register mouse event handlers
   canvas.onmousedown = function(e){ mouse.click = true; };
   canvas.touchstart = function(e) { mouse.clock = true; };
   canvas.onmouseup = function(e){ mouse.click = false; };
   canvas.touchend = function(e) { mouse.click = false; };

   canvas.onmousemove = function(e) {
      var rect = canvas.getBoundingClientRect();
      // normalize mouse position to range 0.0 - 1.0
      mouse.pos.x = (e.clientX - rect.left) / width;
      mouse.pos.y = (e.clientY - rect.top) / height;
      mouse.move = true;
   };

   // draw line received from server
	socket.on('draw_line', function (data) {
      var line = data.line;
      context.lineWidth = 15;
      context.beginPath();
      context.moveTo(line[0].x * width, line[0].y * height);
      context.lineTo(line[1].x * width, line[1].y * height);
      context.stroke();
   });

   socket.on('resetCanvas', function(data){
      context.fillStyle = "white";
      context.clearRect(0, 0, canvas.width, canvas.height);
   });
   
   // main loop, running every 25ms
   function mainLoop() {
      // check if the user is drawing
      if (mouse.click && mouse.move && mouse.pos_prev) {
         // send line to to the server
         socket.emit('draw_line', { line: [ mouse.pos, mouse.pos_prev ] });
         mouse.move = false;
      }
      mouse.pos_prev = {x: mouse.pos.x, y: mouse.pos.y};
      setTimeout(mainLoop, 25);
   }
   mainLoop();
});

var saveCanvas = function(){
   var canvas = document.getElementById("drawing");
   //console.log(canvas.toDataURL());
   document.getElementById("data").setAttribute('value', canvas.toDataURL());
};