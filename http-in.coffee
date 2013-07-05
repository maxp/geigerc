

HTTP_PORT = 8008

http = require 'http'
url  = require 'url'

http.createServer( (req, res) ->
    console.log ""+req.socket.remoteAddress+":", url.parse(req.url, true).query
    res.writeHead 204, ""
    res.end()
).listen HTTP_PORT

#.