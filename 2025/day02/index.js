const fs = require('node:fs')

const expandN = (val, n, partLen) =>{
    let newVal = val;
    for(let i = partLen; i < n*partLen; i+=partLen){
        newVal += val*(10**(i));
    }
    return newVal;
}

const nextId = (a, n) => {
    const aStr = a.toString();
    if(aStr.length % n != 0){
        const partLen = Math.ceil(aStr.length / n);
        const firstPart = 10 ** (partLen-1);
        return expandN(firstPart, n, partLen);
    }
    const parts = [];
    const partLen = aStr.length/n;
    for(let i = 0; i < n; i++){
        parts.push(aStr.slice(i*partLen, (i+1)*partLen));
    }
    const firstVal = Number(parts[0]);
    if(parts.every((val) => Number(val) === firstVal)){
        if(parts[0] === '9'.repeat(partLen)){
            const newPartLen = Math.ceil((aStr.length+1) / n);
            const newfirstVal = firstVal+1;
            return expandN(newfirstVal, n, newPartLen);
        }
        return expandN(firstVal+1, n, partLen);
    }
    const firstMiss = parts.find((val) => Number(val) !== firstVal);
    if(firstMiss < parts[0]){
        return expandN(firstVal, n, partLen);
    }
    return expandN(firstVal+1, n, partLen);
}

const values = (aStr, bStr, n) => {
    let sum = [];
    if(aStr.length > bStr.length){
        return;
    }
    const aVal = Number(aStr);
    const bVal = Number(bStr);
    if(aStr.length % n == 0){
        const parts = [];
        const partLen = aStr.length/n;
        for(let i = 0; i < n; i++){
            parts.push(aStr.slice(i*partLen, (i+1)*partLen));
        }
        if(parts.every((val) => val === parts[0])){
            sum.push(aVal);
        }
    }
    let next = nextId(aVal, n);
    while(next <= bVal){
        sum.push(next);
        next = nextId(next, n);
    }
    return sum;
}

/* silver only
const bruteForce = (aStr, bStr) => {
    if(aStr.length > bStr.length){
        return;
    }
    let curVal = new Number(aStr);
    const max = new Number(bStr);
    while(curVal <= max){
        const curStr = curVal.toString();
        if(curStr.length % 2 == 0){
            const first = curStr.slice(0, curStr.length/2);
            const second = curStr.slice(first.length);
            if(first === second){
                silver += curVal;
            }
        }
        curVal += 1;
    }
}*/

const filename = process.argv[2];
const stream = fs.createReadStream(filename,{
        highWaterMark: 1
    })

let stringA = "";
let stringB = "";
let onA = true;

let silver = 0;
let gold = 0;

stream.on("data", char => {
    if(char == '-'){
        onA = false;
    }else if( char == ',' || char == '\n'){
        let goldValues = [];
        onA = true;
        for(let n = stringB.length; n > 2; n--){
            goldValues = goldValues.concat(values(stringA, stringB, n));
        }
        const twoVal = values(stringA, stringB, 2);
        silver += twoVal.reduce((a,b) => a + b, 0);;
        gold += twoVal.concat(goldValues).filter((v, i, a) => a.indexOf(v) === i).reduce((a,b) => a + b, 0);
        stringA = "";
        stringB = "";
    }else{
        if(onA){
            stringA += char;
        }else{
            stringB += char;
        }
    }
});

stream.on("close", () => {
    console.log('silver', silver);
    console.log('gold', gold);
});