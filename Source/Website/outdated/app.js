var PORT = 3000;
var express = require('express');
var app = express();
var server = require('http').Server(app);
//var io = require('socket.io')(server);
server.listen(PORT);

var bodyParser = require("body-parser");
app.use(express.static(__dirname + '/public'));
app.use(bodyParser.urlencoded({extended: false}));
app.use(bodyParser.json());

var io = require('socket.io').listen(server);

// Server-Code für das Projekt "slider" einbinden und initialisieren
require('./ledrotor-server.js').initialize(io);