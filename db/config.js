import mongoose from "mongoose";
import { config } from "dotenv";
config()

// URL de connexion MongoDB Atlas 
const MONGO_URI = process.env.MONGO_URI;


// Options recommandées
const options = {
  useNewUrlParser: true,
  useUnifiedTopology: true,
};

// Fonction pour connecter la base
async function connectDB() {
  try {
    await mongoose.connect(MONGO_URI);
    console.log('Connexion reussie a MongoDB');
  } catch (error) {
    console.error('Erreur de connexion à MongoDB:', error);
    process.exit(1); // Quitte le process en cas d’erreur
  }
}

export default connectDB;
