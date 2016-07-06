var NAMESPACE = '/ledrotor';
module.exports = {

	initialize: function(socket, express, app){
		console.log("ledrotor: my module is starting");

		var nsp = socket.of(NAMESPACE);

		var fs = require("fs");
		var bodyParser = require("body-parser");
		app.use("/ledrotor", express.static(__dirname+'/../public'));
		app.use(bodyParser.urlencoded({extended: false}));
		app.use(bodyParser.json());

		// picture-select
		var votes = {
			"1": 0,
			"2": 0,
			"3": 0,
			"4": 0,
			"5": 0,
			"6": 0
		};

		// textwarteschlange
		var inputQueue = [];
		var outputInterval;
		var OUTPUTTIME = 20000;

		// doodleIO
		var lines_drawn = [];

		nsp.on("connection", function(socket){
			//console.log("ledrotor: device connected");

			socket.emit("initializeContentQueue", JSON.stringify(inputQueue));
			
			// <------------------ ab hier Code für die Bilderauswahl auf der Startseite ------------------------>

			socket.emit("checkBoxVotes", { "checkbox1": votes[1], "checkbox2": votes[2], "checkbox3": votes[3], "checkbox4": votes[4], "checkbox5": votes[5], "checkbox6": votes[6]});
		
			// Bildauswahl
			socket.on("checkBoxChange", function(data){

				if(data.id == "checkbox1"){
					if(data.bool){
						votes[1]++;
					} else {
						votes[1]--;
					}	
				} else if(data.id == "checkbox2"){
					if(data.bool){
						votes[2]++;
					} else {
						votes[2]--;
					}
				} else if(data.id == "checkbox3"){
					if(data.bool){
						votes[3]++;
					} else {
						votes[3]--;
					}
				} else if(data.id == "checkbox4"){
					if(data.bool){
						votes[4]++;
					} else {
						votes[4]--;
					}
				} else if(data.id == "checkbox5"){
					if(data.bool){
						votes[5]++;
					} else {
						votes[5]--;
					}
				} else if(data.id == "checkbox6"){
					if(data.bool){
						votes[6]++;
					} else {
						votes[6]--;
					}
				}

				socket.emit("checkBoxVotes", { "checkbox1": votes[1], "checkbox2": votes[2], "checkbox3": votes[3], "checkbox4": votes[4], "checkbox5": votes[5], "checkbox6": votes[6]});
				socket.broadcast.emit("checkBoxVotes", { "checkbox1": votes[1], "checkbox2": votes[2], "checkbox3": votes[3], "checkbox4": votes[4], "checkbox5": votes[5], "checkbox6": votes[6]});
			});

			var resetVotes = function(){
				// senden an Python
				socket.emit("maxVote", {"code": getMaxVote()});
				//socket.broadcast.emit("maxVote", {"code": getMaxVote()});

				//console.log("maxvote sent");

				// reset
				for(var i = 1; i <= Object.keys(votes).length; i++){
					votes[i] = 0;
				}
				socket.broadcast.emit("checkBoxReset");
				// TODO: Auswerten der Votes!
			};

			var getMaxVote = function(){
				var max = 0, maxIndex = -1;
				for(var i = 1; i <= Object.keys(votes).length; i++){
					if(parseInt(votes[i]) > max){
						max = votes[i];
						maxIndex = i;
					}
				}
				return maxIndex;
			};

			setInterval(resetVotes, 10000);	// alle X Sekunden

			// <------------------ Bilderauswahl Ende ------------------------>

			// <------------------ ab hier Code für die Textwarteschlange ------------------------>
			
			// Behandeln von neuem Input und broadcasten an alle Clients
			socket.on("inputText", function(data){
		 		//console.log("ledrotor: new input: " + data.input);
		 		if(inputQueue.length > 0){
		 			// Speichern in der Input-Warteschlange
		 			inputQueue.push(data.input);
		 			//Broadcast an alle Clients
		 			socket.emit("updateList", JSON.stringify(inputQueue));
		 			socket.broadcast.emit("updateList", JSON.stringify(inputQueue));
		 		} else {
		 			// Speichern in der Input-Warteschlange
		 			inputQueue.push(data.input);
		 			//Broadcast an alle Clients
		 			socket.emit("updateList", JSON.stringify(inputQueue));
		 			socket.broadcast.emit("updateList", JSON.stringify(inputQueue));
		 			// Starten eines neuen Timers
					outputInterval = setInterval(sendCurrentText, OUTPUTTIME);
		 		}
		 	});

		 	var sendCurrentText = function(){

			 	if(inputQueue.length == 1){
			 		var output = inputQueue.shift();
			 
			 		// aktualisiere Listen aller Clients
			 		socket.emit("updateList", JSON.stringify(inputQueue));
			 		socket.broadcast.emit("updateList", JSON.stringify(inputQueue));
			 		// kein Element mehr vorhanden, lösche den Timer
			 		clearInterval(outputInterval);
			 	} else if(inputQueue.length > 0){
			 		var output = inputQueue.shift();
			 
			 		// aktualisiere Listen aller Clients
			 		socket.emit("updateList", JSON.stringify(inputQueue));
			 		socket.broadcast.emit("updateList", JSON.stringify(inputQueue));
			 	}
			};

		 	// <------------------ Textwarteschlange Ende ------------------------>

		 	// <------------------ ab hier Code für DoodleIO ------------------------>

			// Doodle-IO
			for(var i in lines_drawn){
				socket.emit("draw_line", { line: lines_drawn[i] } );
			}

			socket.on("draw_line", function(data){
				lines_drawn.push(data.line);
				socket.emit("draw_line", { line: data.line });
				socket.broadcast.emit("draw_line", { line: data.line });
			});

			var resetCanvas = function(){
				socket.emit("resetCanvas");
				socket.broadcast.emit("resetCanvas");
				lines_drawn = [];
			};

			setInterval(resetCanvas, 600000); // alle 10 Minuten	

			// <------------------ DoodleIO Ende ------------------------>
	 	
		});

		
	},


	getAuthors(){
		return ["Nora Panhuis", "Friedrich Schimmel", "Ann-Kathrin Schaack", "Tobias Schweisfurth"];
	},
	getInformation(){
		return "Ein POV-Projekt anhand eines LED-Rotors";
	},
	getTitle(){
		return "LED-Rotor";
	}
}