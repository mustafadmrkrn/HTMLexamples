const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');

const app = express();
const port = 3000;

app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static('public'));

app.post('/urunEkle', (req, res) => {
    const { ad, miktar, fiyat } = req.body;
    const data = `${ad}, ${miktar}, ${fiyat}\n`;

    fs.appendFile('veritabani.csv', data, (err) => {
        if (err) throw err;
        console.log('Ürün başarıyla eklendi.');
        res.send('Ürün başarıyla eklendi.');
    });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
