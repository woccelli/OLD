import React from 'react';

const request = require('request-promise');
const apiAddress = 'http://localhost:5000';


const getStations = () => {
  return request({
    "method": "GET",
    "uri": "https://api.jcdecaux.com/vls/v1/stations?contract=lyon&apiKey=359bf5db0df9b3cb8381ff0429d2fdcb28688905",
    "json": true
  })
};

const predict = (date) => {
  return request({
    "method": "GET",
    "uri": apiAddress + "/day_predict/"
    + date.getFullYear() + "-" +
    date.getMonth().toString().padStart(2, '0') + "-" +
    date.getDate().toString().padStart(2, '0'),
    "json": true
  })
};

export {
  getStations,
  predict
};