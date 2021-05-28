const express = require('express');
const http = require('http');
const socketIO = require('socket.io');
const path = require('path');
const engine = require('ejs-mate');
var bodyParser = require('body-parser')

const app = express();
const server = http.Server(app);
const io = socketIO(server);
app.use(bodyParser.json());


//cors
app.use(function(req, res, next) {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE');
    res.setHeader('Access-Control-Allow-Headers', 'Content-Type');
    res.setHeader('Access-Control-Allow-Credentials', true);
    next();
});

// settings
app.set('views', path.join(__dirname, 'views'));
app.engine('ejs', engine);
app.set('view engine', 'ejs');

// middlewares

// routes
app.use(require('./routes'));

// sockets
require('./sockets')(io);

// Static files
app.use(express.static(path.join(__dirname, 'public')));

// starting the server
server.listen(3000, () => {
  console.log('Server on port', 3000);
});