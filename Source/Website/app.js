var PORT = 3000;

// Initialisierung des Express Servers
var express = require("express");
var app = express();

// Der Server braucht eine Warteschlange
var inputQueue = [];
// nutze inputQueue.push(..) und inputQueue.shift() zum Ein-/Auslesen

app.use(express.static(__dirname + "/public"));
var server = app.listen(PORT);
// Initialisierung Websockets
var socketio = require("socket.io");
var io = socketio.listen(server);
var outputInterval;
var OUTPUTTIME = 20000;

// Eventhandler für die Verbindung eines neuen Clients
io.sockets.on("connection", function(socket){
	
	// Wenn der Client connected, muss einmal die ContentQueue auf Anfrage geladen werden
	socket.on("initializeList", function(data){
		socket.emit("initializeContentQueue", JSON.stringify(inputQueue));
	});

	// Behandeln von neuem Input und broadcasten an alle Clients
	socket.on("inputText", function(data){
		console.log("new input: " + data.input);
		if(inputQueue.length > 0){
			// Speichern in der Input-Warteschlange
			inputQueue.push(data.input);
			//Broadcast an alle Clients
			io.sockets.emit("updateList", JSON.stringify(inputQueue));
		} else {
			// Speichern in der Input-Warteschlange
			inputQueue.push(data.input);
			//Broadcast an alle Clients
			io.sockets.emit("updateList", JSON.stringify(inputQueue));
			// Starten eines neuen Timers
			outputInterval = setInterval(sendCurrentText, OUTPUTTIME);
		}
	});

});

// TODO: Laufen eines Timers für das Senden des aktuellen Textes
var sendCurrentText = function(){
	// prüfen, ob es Output gibt, der an den Mikrokontroller übergeben werden kann
	 if(inputQueue.length == 1){
		var output = inputQueue.shift();

		// aktualisiere Listen aller Clients
		io.sockets.emit("updateList", JSON.stringify(inputQueue));
		// kein Element mehr vorhanden, lösche den Timer
		clearInterval(outputInterval);
	} else if(inputQueue.length > 0){
		var output = inputQueue.shift();

		// aktualisiere Listen aller Clients
		io.sockets.emit("updateList", JSON.stringify(inputQueue));
	}

	// TODO: senden des Output an den Mikrokontroller
}

// TODO: Senden des aktuellen Textes an den Controller