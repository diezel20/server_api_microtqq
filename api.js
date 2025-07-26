// Import modules 
import express from 'express'; 
import bodyParser from 'body-parser';
import morgan from 'morgan';
import { config } from 'dotenv';
import mongoose from 'mongoose';

// import tools db 
import connectDB from './db/config.js';
import FieldTemperatureDate from './db/models/temperatureModel.js';

// Config env 
config()
const {PORT,API_KEY} = process.env

// Start api server 
const app = express()
// Connect DB 
await connectDB().then(function(){
        app 
    // middleware
    .use(bodyParser.urlencoded({extended: true}))
    .use(bodyParser.json())
    .use(morgan('dev'))

    // Routes 

    // update tmpture 
    .get('/', function(request , response){
        response.send('API_SERVER_MICROTQQ')
    })
    .get('/update', async function(request , response){
      const { api_key , field1, field2, field3 } = request.query;

  
      if(api_key !== API_KEY || api_key==null) {
          return response.status(401).json({message : 'api key not valid or not found'})
      }

      if (!field1 || !field2 || !field3) {
        return response.status(400).json({ message: 'Missing temperature fields' });
      }

    
      const val1 = parseFloat(field1);
      const val2 = parseFloat(field2);
      const val3 = parseFloat(field3);

      if (isNaN(val1) || isNaN(val2) || isNaN(val3)) {
          return response.status(400).json({ message: 'Temperature fields must be valid numbers.' });
      }

      const session = await mongoose.startSession();
      session.startTransaction();
      try {
          const receivedAt = new Date();

          const entries = [
            {
              sensorId: 'field1',
              value: val1, 
              receivedAt
            },
            {
              sensorId: 'field2',
              value: val2, 
              receivedAt
            },
            {
              sensorId: 'field3',
              value: val3, 
              receivedAt
            },
          ];

          await FieldTemperatureDate.insertMany(entries, { session });

          await session.commitTransaction();
          session.endSession();

    
          response.status(200).json({ message: 'Data saved successfully' });
      } catch (err) {
          await session.abortTransaction();
          session.endSession();
          console.error('Error saving data:', err);
         
          response.status(500).json({ error: 'Failed to save data' });
      }
  })

    //api get tmpture 
    .get('/get-tpture', async function(request , response){
        try {
          const sensorIds = ["field1", "field2", "field3"];
          const results = {};
      
          for (const sensorId of sensorIds) {
            const entries = await FieldTemperatureDate.find({ sensorId })
              .sort({ receivedAt: -1 }) // les plus récents d'abord
              .limit(5)
              .lean();
      
            results[sensorId] = entries.reverse().map(entry => ({
              receivedAt: entry.receivedAt.toTimeString().split(' ')[0],
              value: entry.value,
              descriptionField: entry.descriptionField ?? null,
            }));
          }
      
          response.status(200).json({ data: results });
          } catch (error) {
            console.error("Error fetching data:", error);
            response.status(500).json({ error: "Error server" });
          }
    })
    .get('/all-data',async function(request , response){
        const {api_key} = request.query
        if(api_key!==API_KEY || api_key==null ) {
          return response.status(401).json({message : 'api key not valid or not found'})
        }
        const data = await FieldTemperatureDate.find()
        response.status(200).json({data : data})
    } )
    // server started 
    .listen(PORT ,'0.0.0.0', function(){
        console.log(`Server started on the port ${PORT}`)
    })
    })
    .catch(function(){
        process.exit(1);
})



