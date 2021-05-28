module.exports = io => {
    io.on('connection', socket => {
  
      console.log('new socket connected');
  
      socket.on('user', (coords) => {
        socket.broadcast.emit('newUser', coords);
      });
    });
  };
  