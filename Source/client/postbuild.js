var sys = require('sys');
var exec = require('child_process').exec;
var os = require('os');

function puts(error, stdout, stderr) { console.log(stdout) }

if (os.type() === 'Windows_NT') 
   exec("move build ..", puts); 
else if (os.type() === 'Darwin') 
   exec("mv build ..", puts); 
else if (os.type() === 'Linux') 
   exec("mv build ..", puts);
else
   throw new Error("Unsupported OS found: " + os.type());