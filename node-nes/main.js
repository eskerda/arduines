var sys = require('sys');
var exec = require('child_process').exec;
var serialport = require('serialport');
var SerialPort = serialport.SerialPort;

function puts(error, stdout, stderr) { sys.puts(stdout) }


var serialPort = new SerialPort("/dev/ttyACM1", {
    baudrate: 9600
});

/**
NES Word Mapping
x x x x x x x x
| | | | | | | |_  A
| | | | | | |___  B
| | | | | |_____  SELECT
| | | | |_______  START
| | | |_________  UP
| | |___________  DOWN
| |_____________  LEFT
|_______________  RIGHT

xdotool key <key>
**/

var posEvent = {
    '0': 'd',
    '1': 'a',
    '2': 's',
    '3': 'w',
    '4': 'x',
    '5': 'z',
    '6': 'j',
    '7': 'k'
}

var lastWord = "11111111";

var cleanKeys = function(){
    for(var j = 0; j < posEvent.length; j++){
        exec('xdotool up --delay 0 '+posEvent[j]);
    }
}

serialPort.on("data", function (data) {
    var word = data[0];
    var binWord = data[0].toString(2);
    if (binWord.length < 8){
        // Pad 0 to left
        for(var i = 0; i < 8 - binWord.length; i++ ){
            binWord = '0'+binWord;
        }
    }

    for (var i = 0; i < binWord.length; i++){
        if (binWord[i] != lastWord[i]){
            if (binWord[i] == '0'){
                cleanKeys();
                exec('xdotool keydown --delay 0 '+posEvent[i]);
            }                
            else{
                exec('xdotool keyup --delay 0 '+posEvent[i]);
            }
        }
    }
    lastWord = binWord;
});
