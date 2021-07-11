const serial_port = "COM10";

const Influx = require('influx');

const influx = new Influx.InfluxDB({
    host: 'influx.protomakers.club',
    database: 'scantnik',
    port: 443,
    protocol:"https",
    schema: [
      {
        measurement: 'sat',
        fields: {
          temperature: Influx.FieldType.FLOAT
        },
        tags: [
          'host'
        ]
      }
    ]
});
/*
influx.writePoints([
    {
      measurement: 'scantnik',
      fields: {
        vel: Math.random()
     },
    }
]).catch(err => {
    console.error(`Error saving data to InfluxDB! ${err.stack}`)
});
*/
const SerialPort = require('serialport')

let Readline = SerialPort.parsers.Readline; // make instance of Readline parser
let parser = new Readline(); // make a new parser to read ASCII lines

const path = require('path');
const fs = require('fs').promises;

const log_filename = path.join(__dirname, Date.now() + ".txt");
fs.writeFile(log_filename, "");



const teensy = new SerialPort(serial_port, {
    baudRate: 9600
});

teensy.pipe(parser); // pipe the serial stream to the parser



parser.on('data', (data) => {
    fs.appendFile(log_filename, data);
    console.log("Packet:" + data);
    let line = data.toString();

    //id, millis, co2, cov, Pbme, Pbmp
    let values = line.split(",");
    if(values.length !== 12) {
        console.log("droped packet, incorrect param count");
        return;
    }

    //id, seg, co2, cov, p1, p2, h, vel, hum, t1, t2, uv, alt, lat, lng, satcount, hdop

    let id = Number(values[0]);
    let seg = Number(values[1]);
    let co2 = Number(values[2]);
    let cov = Number(values[3]);
    let p1 = Number(values[4]);
    let p2 = Number(values[5]);
    let h = Number(values[6]);
    let vel = Number(values[7]);
    let hum = Number(values[8]);
    let t1 = Number(values[9]);
    let t2 = Number(values[10]);
    let uv = Number(values[11]);
   /* let alt = Number(values[12]);
    let lat = Number(values[13]);
    let lng = Number(values[14]);
    let satcount = Number(values[15]);
    let hdop = Number(values[16]);*/

  /*  console.log({
        id,seg,co2,cov,p1,p2,h,vel,hum,t1,t2,uv//,alt,lat,lng,satcount,hdop
     });*/
    
     console.log(`id: ${id}\t\tt: ${seg}\t\tco2: ${co2}\t\tcov: ${cov}\t\tp1: ${p1}\t\tp2: ${p2}\t\talt: ${h}\t\tvel: ${vel}\t\thum: ${hum}\t\tt1: ${t1}\t\tt2: ${t2}\t\tuv: ${uv}`);

    influx.writePoints([
        {
        measurement: 'scantnik',
        fields: {
            id,seg,co2,cov,p1,p2,h,vel,hum,t1,t2,uv//,alt,lat,lng,satcount,hdop
        },
        }
    ]).catch(err => {
        console.error(`Error saving data to InfluxDB! ${err.stack}`)
    })
});

teensy.on("open", async (e) => {
    console.log("Connected to usb")
});