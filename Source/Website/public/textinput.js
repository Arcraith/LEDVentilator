var NAMESPACE = '/ledrotor';

function initialize(){
	var socket = io(NAMESPACE);

	var textfield = document.getElementById("textfield");
	var sendButton = document.getElementById("button");
	sendButton.addEventListener("click", function(){
 		socket.emit("inputText", {"input": textfield.value});
 		textfield.value = "";
 	});

	var inputList = document.getElementById("contentQueue");

	// Einmaliger Erstaufbau der Liste nach Anfrage der ContentQueue
	socket.on("initializeContentQueue", function(data){
		var inputQueue = JSON.parse(data);
		
		createList(inputQueue);
		console.log("initialized");
	});

	// Updaten der ContentQueue
	socket.on("updateList", function(data){
		// die Input-Queue anzeigen/aktualisieren
		var inputQueue = JSON.parse(data);
		while(inputList.firstChild){
			inputList.removeChild(inputList.firstChild);
		}

		createList(inputQueue);
	});

	var createList = function(inputArray){
		for(var i = 0; i < inputArray.length; i++){
			var li = document.createElement("li");
			li.appendChild(document.createTextNode(inputArray[i]));
			inputList.appendChild(li);
		}
	}

	socket.on("takePicture", function(data){
		console.log("hallloooooo");
	});
};