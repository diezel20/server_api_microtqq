import mongoose from "mongoose";

const fieldTemperatureDateSchema = new mongoose.Schema({
  sensorId: {
    type: String, // Ex: "field1", "field2", "field3"
    required: true,
  },
  descriptionField : {
    type : String , 
    required : false 
  },
  value: {
    type: Number,
    required: true,
  },
  receivedAt: {
    type: Date,
    default: Date.now,
  }
});

const FieldTemperatureDate = mongoose.model("FieldTemperatureDate", fieldTemperatureDateSchema);

export default FieldTemperatureDate;
