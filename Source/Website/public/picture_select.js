var NAMESPACE = "/ledrotor";

function initialize(){
	
	var socket = io.connect(NAMESPACE);

	var checkbox1 = document.getElementById("checkbox1");
	var checkbox2 = document.getElementById("checkbox2");
	var checkbox3 = document.getElementById("checkbox3");
	var checkbox4 = document.getElementById("checkbox4");
	var checkbox5 = document.getElementById("checkbox5");

	var votes1 = document.getElementById("vote1");
	var votes2 = document.getElementById("vote2");
	var votes3 = document.getElementById("vote3");
	var votes4 = document.getElementById("vote4");
	var votes5 = document.getElementById("vote5");

	function checkBoxHandler(){
		socket.emit("checkBoxChange", { "id": this.id, "bool": this.checked });	
	}

	checkbox1.addEventListener("click", checkBoxHandler);
	checkbox2.addEventListener("click", checkBoxHandler);
	checkbox3.addEventListener("click", checkBoxHandler);
	checkbox4.addEventListener("click", checkBoxHandler);
	checkbox5.addEventListener("click", checkBoxHandler);

	socket.on("checkBoxVotes", function(data){
		votes1.innerHTML = "Votes: " + data.checkbox1;
		votes2.innerHTML = "Votes: " + data.checkbox2;
		votes3.innerHTML = "Votes: " + data.checkbox3;
		votes4.innerHTML = "Votes: " + data.checkbox4;
		votes5.innerHTML = "Votes: " + data.checkbox5;
	});

	socket.on("checkBoxReset", function(data){
		votes1.innerHTML = "Votes: 0";
		votes2.innerHTML = "Votes: 0";
		votes3.innerHTML = "Votes: 0";
		votes4.innerHTML = "Votes: 0";
		votes5.innerHTML = "Votes: 0";

		checkbox1.checked = false;
		checkbox2.checked = false;
		checkbox3.checked = false;
		checkbox4.checked = false;
		checkbox5.checked = false;
	});

	
	socket.on("maxVote", function(data){
		console.log(data.code);
	});
	
}