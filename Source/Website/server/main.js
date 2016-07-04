var NAMESPACE = '/ledrotor';
module.exports = {

	initialize: function(socket, express, app){
		console.log("ledrotor: my module is starting");

		var nsp = socket.of(NAMESPACE);
		
		// Der Server braucht eine Warteschlange --> nutze inputQueue.push(..) und inputQueue.shift() zum Ein-/Auslesen
		var inputQueue = [];

		var outputInterval;
		var OUTPUTTIME = 20000;
		

		var lines_drawn = [];

		// for saving images
		var fs = require("fs");
		var mkdirp = require('mkdirp');
		var bodyParser = require("body-parser");
		app.use("/ledrotor", express.static(__dirname+'/../public'));
		app.use(bodyParser.urlencoded({extended: false}));
		app.use(bodyParser.json());

		//vote-values
		var votesPicture1 = 0;
		var votesPicture2 = 0;
		var votesPicture3 = 0;
		var votesPicture4 = 0;

		var votes = {
			"1": 0,
			"2": 0,
			"3": 0,
			"4": 0
		};


		// Eventhandler für die Verbindung eines neuen Clients
		nsp.on("connection", function(socket){
			
			console.log("ledrotor: device connected");
			// Wenn der Client connected, muss einmal die ContentQueue auf Anfrage geladen werden
			
			socket.emit("checkBoxVotes", { "checkbox1": votes[1], "checkbox2": votes[2], "checkbox3": votes[3], "checkbox4": votes[4]});
			
			// Texteingabe
			socket.emit("initializeContentQueue", JSON.stringify(inputQueue));
		
			// Behandeln von neuem Input und broadcasten an alle Clients
			socket.on("inputText", function(data){
		 		//console.log("ledrotor: new input: " + data.input);
		 		if(inputQueue.length > 0){
		 			// Speichern in der Input-Warteschlange
		 			inputQueue.push(data.input);
		 			//Broadcast an alle Clients
		 			nsp.emit("updateList", JSON.stringify(inputQueue));
		 		} else {
		 			// Speichern in der Input-Warteschlange
		 			inputQueue.push(data.input);
		 			//Broadcast an alle Clients
		 			nsp.emit("updateList", JSON.stringify(inputQueue));
		 			// Starten eines neuen Timers
					outputInterval = setInterval(sendCurrentText, OUTPUTTIME);
		 		}
		 	});
			
		
			// Doodle-IO
			for(var i in lines_drawn){
				socket.emit("draw_line", { line: lines_drawn[i] } );
			}

			socket.on("draw_line", function(data){
				lines_drawn.push(data.line);
				nsp.emit("draw_line", { line: data.line });
			});

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
				}

				nsp.emit("checkBoxVotes", { "checkbox1": votes[1], "checkbox2": votes[2], "checkbox3": votes[3], "checkbox4": votes[4]});
			});

		});

		// Vorerst vertagt
		/*
		app.post('/ledrotor/server/', function(request, response){
			//console.log("username: " + request.body.data);

			var img = request.body.data;

			// strip off the data: url prefix to get just the base64-encoded bytes
			var data = img.replace(/^data:image\/\w+;base64,/, "");
			var buf = new Buffer(data, 'base64');

			mkdirp('/ledrotor/server/images', function(err){
				if(err) console.error("directory could not be created");
				else console.log("pow!");
			});

			fs.writeFile('ledrotor/server/images/image.png', buf);
		});
		*/

		var sendCurrentText = function(){
		 	// prüfen, ob es Output gibt, der an den Mikrokontroller übergeben werden kann
		 	if(inputQueue.length == 1){
		 		var output = inputQueue.shift();
		 
		 		// aktualisiere Listen aller Clients
		 		nsp.emit("updateList", JSON.stringify(inputQueue));
		 		// kein Element mehr vorhanden, lösche den Timer
		 		clearInterval(outputInterval);
		 	} else if(inputQueue.length > 0){
		 		var output = inputQueue.shift();
		 
		 		// aktualisiere Listen aller Clients
		 		nsp.emit("updateList", JSON.stringify(inputQueue));
		 	}
		};

		// Relikt aus der Vergangenheit?
		var updateImage = function(){
			nsp.emit('imageUpdate');
		};

		var resetCanvas = function(){
			nsp.emit("resetCanvas");
			lines_drawn = [];
		};

		var resetVotes = function(){
			// senden an Python
			nsp.emit("maxVote", {"code": getMaxVote()});

			// reset
			for(var i = 1; i <= Object.keys(votes).length; i++){
				votes[i] = 0;
			}
			nsp.emit("checkBoxReset");
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

		setInterval(updateImage, 10000);

		setInterval(resetVotes, 1200000); // alle 20 Minuten

		setInterval(resetCanvas, 600000); // alle 10 Minuten
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