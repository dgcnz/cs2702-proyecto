
module.exports = {

    writer : function (query){
        fs = require('fs');
        text = query;
        fs.writeFile('./src/public/files/monitor/query.txt', text, function (err) {
        if (err) return console.log(err);
      });
    },

    fileChangesDetector : async function (fileName, callback, timeout){
      return new Promise((resolve) => {
        fs = require('fs');
        path = './src/public/files/'+fileName;
        fs.watchFile(path, (curr, prev) => {
          console.log('file Changed');
          resolve(true);
        });
      });
    }


}
