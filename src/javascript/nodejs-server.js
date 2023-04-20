var fileRoot = '.'
var index = 'index.html'
var cache = {};
var fs = require('fs')
var path = require('path')
var http = require('http');
var mime = require('mime'); // 需要 npm install mime
var server = http.createServer();

function send404(response) {
    response.writeHead(404, {'Content-Type': 'text/plain'});
    response.write('Error 404, Not Found');
    response.end();
}

function sendFile(response, filePath, fileContents) {
    response.writeHead(200, {'Content-Type': mime.getType(path.basename(filePath))});
    response.end(fileContents);
}

function serverStatic(response, cache, absPath) {
    if (cache[absPath]) { // 在内存中
        sendFile(response, absPath, cache[absPath]);
    } else {
        fs.exists(absPath, function(exists) {
            if (exists) {
                fs.readFile(absPath, function(err, data) {
                    if (err) { // 文件读取失败
                        send404(response);
                    } else {
                        cache[absPath] = data; // 写入缓存
                        sendFile(response, absPath, data);
                    }
                });
            } else { // file not exist
                send404(response);
            }
        });
    }
}

server.on('request', function(req, res) {
    var filePath = false;
    console.log('url', req.url);
    var absPath = fileRoot + req.url;
    try {
        if (fs.statSync(absPath).isDirectory()) { // fs.stat是异步的
            absPath += index;
        }
        serverStatic(res, cache, absPath);
    } catch(err) {
        send404(res);
    }
});

server.listen(8080);

console.log('server created at http://localhost:8080');
