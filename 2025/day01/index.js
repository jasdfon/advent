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
        const rotationValue = new Number(line.slice(1));
        let initialPos = position;
        if(negative){
            position = position - rotationValue;
            while(position < 0){
                position += 100;
                gold += 1;
            }
            if(position == 0){
                gold += 1;
            }
            if(initialPos == 0){
                gold -= 1;
            }
        }else{
            position = position + rotationValue;
            while(position > 99){
                position -= 100;
                gold +=1;
            }
        }
        if(position == 0 && rotationValue != 0){
            silver += 1;
        }
});

rl.on("close", () => {
    console.log('silver', silver);
    console.log('gold', gold);
});