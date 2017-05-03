const webpack = require('webpack');
const WebpackNotifierPlugin = require('webpack-notifier');
const fs = require('fs');
const path = require('path');
const autoprefixer = require('autoprefixer');

const dir = path.resolve(__dirname) + '/';

module.exports = {
    devtool: 'cheap-module-eval-source-map',
    entry: [
        // 'webpack-dev-server/client?http://localhost:3000',
        // 'babel-polyfill',
        // 'whatwg-fetch',
        // 'react-hot-loader/patch',
        'webpack/hot/only-dev-server',
        dir + 'main.js'
    ],
    output: {
        path: path.join(__dirname, 'dist'),
        publicPath: '/static/',
        filename: 'bundle.js',
    },
    plugins: [
        // new webpack.ProvidePlugin({
        //     Promise: 'es6-promise',
        //     fetch: 'imports?this=>global!exports?global.fetch!whatwg-fetch'
        // }),
        new webpack.DefinePlugin({
            'process.env': {
                'NODE_ENV': JSON.stringify(process.env.NODE_ENV || 'development')
            }
        }),
        new WebpackNotifierPlugin(),
        new webpack.HotModuleReplacementPlugin(),
        // // new webpack.NoErrorsPlugin(),
        // new WebpackNotifierPlugin()
    ],
    resolve: {
        extensions: ['.js', '.jsx'],
        // fallback: dir + '../src',
        // alias
    },
    devServer: {
        host: '0.0.0.0',
        port: 3001,
        hot: true,
        inline: true,
        filename: 'index.js',
        contentBase: dir,
        stats: {
            chunks: false,
            assets: true,
            hash: false,
            cached: false,
            cachedAssets: false,
            colors: true
        }
    },
    module: {
        loaders: [{
            test: /\.(jsx|js)$/,
            loader: 'babel-loader',  // keep compact=false for debugging
            exclude: /node_modules/,
            query: {
                presets: ['es2015', 'react']
            }
        }, {
            test: /\.(json)$/,
            exclude: /node_modules/,
            loader: 'json-loader'
        }, {
            test: /\.(scss|css)$/,
            loader: "style?singleton!css!postcss!sass?includePaths[]=" + dir
        }, {
            test: /\.(woff|woff2)$/,
            loader: 'url-loader?limit=99999999&mimetype=application/font-woff'
        }, {
            test: /\.(png|svg|gif)$/,
            loader: "url-loader"
        }, {
            test: /\.jpg$/,
            loader: "file-loader"
        }]
    }
};
