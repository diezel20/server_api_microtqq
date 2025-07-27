
// server.js
const express = require('express');
const app = express();
const port = 80; // Port par défaut pour HTTP

// Tableau pour stocker les dernières températures
let temperatures = { temp1: 0, temp2: 0, temp3: 0 };

// Middleware pour parser les requêtes GET
app.use(express.json());

// Endpoint pour recevoir les températures
app.get('/update', (req, res) => {
  temperatures.temp1 = parseFloat(req.query.temp1) || temperatures.temp1;
  temperatures.temp2 = parseFloat(req.query.temp2) || temperatures.temp2;
  temperatures.temp3 = parseFloat(req.query.temp3) || temperatures.temp3;
  console.log('Températures reçues :', temperatures);
  res.send('Données reçues');
});

// Page web pour afficher les températures
app.get('/', (req, res) => {
  res.send(`
    <!DOCTYPE html>
    <html>
    <head>
      <title>Surveillance des Températures</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        body { font-family: Arial, sans-serif; text-align: center; }
        h1 { margin: 50px 0; }
        .temp { font-size: 24px; margin: 20px; }
      </style>
    </head>
    <body>
      <h1>Températures en Temps Réel</h1>
      <div class="temp">Température 1: <span>${temperatures.temp1.toFixed(1)} °C</span></div>
      <div class="temp">Température 2: <span>${temperatures.temp2.toFixed(1)} °C</span></div>
      <div class="temp">Température 3: <span>${temperatures.temp3.toFixed(1)} °C</span></div>
    </body>
    </html>
  `);
});

// Démarrer le serveur
app.listen(port, () => {
  console.log(`Serveur démarré sur http://localhost:${port}`);
});