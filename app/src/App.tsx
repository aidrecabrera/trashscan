import { BinComponent } from "@/components/component/bin";
import { SensorData } from "@/types/sensor.types";
import { useEffect, useState } from "react";
import { io } from "socket.io-client";
import { Card, CardContent } from "./components/ui/card";
import { useSensors } from "./data/useSensors";

const socket = io("http://127.0.0.1:5000");

function App() {
  const [sensorData, setSensorData] = useState<SensorData | null>(null);
  const bins = [
    { id: 1, title: "Biodegradable" },
    { id: 2, title: "Non-biodegradable" },
    { id: 3, title: "Recyclable" },
    { id: 4, title: "Hazardous" },
  ];

  const { data: binStatus } = useSensors();

  useEffect(() => {
    if (binStatus) {
      setSensorData(binStatus);
    }
  }, [binStatus]);

  useEffect(() => {
    socket.on("sensor_update", (data: SensorData) => {
      setSensorData(data);
    });

    return () => {
      socket.off("sensor_update");
    };
  }, []);

  return (
    <div className="flex flex-col items-center justify-center gap-4 mx-24 md:h-screen md:flex-row">
      {bins.map((bin) => (
        <Card key={bin.id}>
          <div className="relative m-10">
            <h1 className="text-2xl font-bold text-center">{bin.title}</h1>
            <CardContent>
              <BinComponent
                binPathHeightPercent={
                  sensorData
                    ? sensorData[`SENSOR_${bin.id}` as keyof SensorData]
                    : 0
                }
                className="fill-blue-700"
              />
            </CardContent>
          </div>
        </Card>
      ))}
    </div>
  );
}

export default App;
