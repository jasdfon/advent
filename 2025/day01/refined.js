const fs = require('node:fs')
const readline = require('node:readline');

let position = 50;
let silver = 0;
let gold = 0;

const filename = process.argv[2];
if (!filename) {
  console.error(`usage: node ${process.argv[1]} <file>`);
  process.exit(1);
}

const rl = readline.createInterface({
  input: fs.createReadStream(filename)
});

rl.on("line", line => {
    const negative = line.startsWith('L');
    if ( line.length < 2 ){
        line = line.charAt(0) + '00';
    } else if ( line.length < 3 ){
        line = line.charAt(0) + '0' +  line.charAt(1);
    }
    const fullRotationCount = new Number(line.slice(1,-2));
    const positionMove = new Number(line.slice(-2));

    let initialPos = position;
    gold += fullRotationCount;

    if(negative){
        position = position - positionMove;
        if(position < 0){
            position += 100;
            gold += 1;
        }
        if(position == 0){
            gold += 1;
        }
        if(initialPos == 0){
            gold -= 1;
        }
    } else {
        position = position + positionMove;
        if(position > 99){
            position -= 100;
            gold +=1;
        }
    }
    if(position == 0 && (fullRotationCount > 0 || positionMove > 0)){
        silver += 1;
    }
});

rl.on("close", () => {
    console.log('silver', silver);
    console.log('gold', gold);
});