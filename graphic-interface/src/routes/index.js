const express = require('express');
const router = express.Router();
const fileManager = require('../helpers/file-manager');

router.get('/', function(req, res) {
    res.render('index');
});

router.post('/', async function(req, res) {
    query = req.body.query;
    console.log("Query:" + query);
    fileManager.writer(query);
    var isDetectChange = await fileManager.fileChangesDetector('input.csv');
    if(isDetectChange) return res.status(200).json("Archivo modificado.");
    return res.status(500).json("Problemas con el DBMS.");
});

module.exports = router;