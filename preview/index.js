/* eslint no-console: 0 */

const path                  = require('path')
const express               = require('express')
const bodyParser            = require('body-parser')
const webpack               = require('webpack')
const webpackDevMiddleware  = require('webpack-dev-middleware')
const webpackHotMiddleware  = require('webpack-hot-middleware')
// const favicon               = require('serve-favicon')

const config                = require('./webpack.config.js')
const compiler              = webpack(config)
const port                  = 8080

const app                   = express()

const apiServer             = require('../server/apiServer.js')

// parse application/json
app.use(bodyParser.json())

// set favicon
// app.use(favicon(path.join(__dirname, '../favicon.ico')))

// set static paths
// app.use('/fonts', express.static(path.join(__dirname, '../fonts')))
// app.use('/styles', express.static(path.join(__dirname, '../styles')))

app.use(webpackDevMiddleware(compiler, {
    publicPath: config.output.publicPath,
    contentBase: 'client',
    stats: {
        colors: true,
        hash: false,
        timings: true,
        chunks: false,
        chunkModules: false,
        modules: false
    }
}))

app.use(webpackHotMiddleware(compiler))

app.get('/', (req, res) => {
    const indexPath = path.join(__dirname, './main.html')
    res.sendFile(indexPath)
})

app.get('/api/doors', (req, res) => {
    apiServer.getAllStatus(req, res)
})

app.post('/api/door/:id', (req, res) => {
    apiServer.pushButtonForDoor(req, res)
})

app.listen(port, function () {
    console.log(`Garage Door Server listening on port ${port}:`)
})
