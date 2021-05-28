
const inputFileName = 'input.csv';

function parseData(rawData) {
    registers = [];
    lines = rawData.split('\n');
    var numLines = lines.length;
    for(var i=0; i < numLines; i++){
        var parseLine = lines[i].split(',');
        var register = []
        for(var j=0; j < parseLine.length; j++){
            register.push(parseLine[j]);
        }
        registers.push(register);
    }
    return registers;
}

async function readFile(dataFileName){
    return new Promise(function (resolve, reject) {
        var url = "./files/"+dataFileName;
        xhr = new XMLHttpRequest();   
        xhr.open('GET', url)
        if (xhr.overrideMimeType)
        xhr.overrideMimeType('text/plain; charset=x-user-defined')
        xhr.onreadystatechange =function(){
            if (this.readyState == 4){
                if (this.status == 200){
                    const data = this.response;
                    resolve(data);
                } else {
                    reject({
                        status: this.status,
                        statusText: xhr.statusText
                    });
                }
            }                   
        }
        xhr.send();
    });
}

async function readFileAndParseData(dataFileName){
    return parseData(await readFile(dataFileName));
} 

function buildHeader(headers) {
    output = "";
    headers.forEach((headerValue) => {
        output += `<th scope="col">${headerValue}</th>`;
    });
    return `<thead><tr>${output}</tr></thead>`;
}

function buildBody(registers) {
    output = "";
    registers.forEach((register) => {
        htmlRegister = "";
        register.forEach((field) => {
            htmlRegister += `<td>${field}</td>`
        });
        output += `<tr>${htmlRegister}</tr>`
    });
    return `<tbody>${output}</tbody>`
}

function fillTable(tableId, data) {
    var table = document.getElementById(tableId);
    table.innerHTML = "";
    table.insertAdjacentHTML('beforeend',buildHeader(data[0]));
    table.insertAdjacentHTML('beforeend', buildBody(data.slice(1, data.length)));
}

async function reloadTableData(){
    let data = await readFileAndParseData(inputFileName);
    fillTable('table', data);
}

function sendQuery(){
    var query = document.getElementById('query-input').value;
    const xhr = new XMLHttpRequest();
    xhr.open('POST', 'http://127.0.0.1:3000/');
    xhr.setRequestHeader('Content-type', 'application/json;charset=UTF-8');
    xhr.onreadystatechange = function() {
        if (this.status === 200) {
            console.log(this.response);
            readDataResponse();
        }
    }
    xhr.send(JSON.stringify({"query":query}));
}

async function readDataResponse(){
    const queryResponse = await readFile(inputFileName);
    const isValid = isAValidQueryResponse(queryResponse);
    if(isValid) reloadTableData();
    else alert(queryResponse.substr(1, queryResponse.length));
}

function isAValidQueryResponse(queryResponse){
    if(queryResponse.charAt(0) == "%") return false;
    return true;
}