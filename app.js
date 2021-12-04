const express = require("express");
const ejs = require("ejs");
const bodyParser = require("body-parser");
const mongoose = require("mongoose");
const _ = require("lodash");
const https = require("https");

const app = express();

app.set('view engine', 'ejs');

app.use(bodyParser.urlencoded({
    extended: true
  }));

app.use(express.static("public"));

mongoose.connect("mongodb+srv:// ### Insert mongoDB access link here ### ");

const plantSchema = {
    name: String,
    waterLevel: mongoose.Decimal128,
    pH: mongoose.Decimal128,
    sunlight: Number
};

const Plant = mongoose.model("Plant", plantSchema);

//For adding a new entry for testing.
const tomato1 = new Plant({
    name: "tomato",
    waterLevel: 1.5,
    pH: 7.0,
    sunlight: 5
})

let port = process.env.PORT;
if (port == null || port == "") {
  port = 3000;
}

app.listen(port, function(){
    console.log("server started")
});

app.get("/", function(req, res){
    res.render("home");
});

app.get("/plants", function(req, res){
    const url = "https://api.thingspeak.com/channels/### Insert thingspeak API link here ###"
    https.get(url , function(response){
        console.log(response.statusCode);
        response.on("data", function(data){
            const bigData = JSON.parse(data);
            const plantData = bigData.feeds[0]; //only one feed is returned

            var newDocument = [];

            // console.log(plantData);
            Plant.findOneAndUpdate( {name: plantData.field1}, {waterLevel: parseFloat(plantData.field2), pH: parseFloat(plantData.field3), sunlight: parseInt(plantData.field4),}, {new: true, upsert: true},function(err){
                if (err) {
                    console.log(err);
                } else {
                    return
                }
            });
            console.log(plantData.field5)
            Plant.findOneAndUpdate( {name: plantData.field5}, {waterLevel: parseFloat(plantData.field6), pH: parseFloat(plantData.field7), sunlight: parseInt(plantData.field8),}, {new: true, upsert: true},function(err){
                if (err) {
                    console.log(err);
                } else {
                    return
                }
            });

            Plant.find({}, function(err, foundPlants){
                if (foundPlants.length === 0) {
                    res.redirect("/");
                } else {
                    res.render("plants", {
                        newPlants: foundPlants
                    })
                }
            });
        })
    });
});

app.get("/contact", function(req, res){
    res.render("contact");
});

app.get("/plantGuide", function(req, res){
    res.render("plantGuide");
});