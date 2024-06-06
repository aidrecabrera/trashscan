import { BinComponent } from "@/components/component/bin";
import { SensorData } from "@/types/sensor.types";
import { useEffect, useState } from "react";
import { io } from "socket.io-client";
import { BannerComponent } from "./components/component/banner";
import { Card, CardContent } from "./components/ui/card";
import { useSensors } from "./data/useSensors";
import { getPercentage } from "./lib/utils";

const socket = io("http://127.0.0.1:5000");

const bins = [
  { id: 1, title: "Biodegradable" },
  { id: 2, title: "Non-biodegradable" },
  { id: 3, title: "Recyclable" },
  { id: 4, title: "Hazardous" },
];

function App() {
  const [sensorData, setSensorData] = useState<SensorData | null>(null);
  const { data: binStatus } = useSensors();

  useEffect(() => {
    if (binStatus) {
      setSensorData({
        SENSOR_1: getPercentage(binStatus.SENSOR_1),
        SENSOR_2: getPercentage(binStatus.SENSOR_2),
        SENSOR_3: getPercentage(binStatus.SENSOR_3),
        SENSOR_4: getPercentage(binStatus.SENSOR_4),
      });
    }
  }, [binStatus]);

  useEffect(() => {
    const handleSensorUpdate = (data: SensorData) => {
      setSensorData({
        SENSOR_1: getPercentage(data.SENSOR_1),
        SENSOR_2: getPercentage(data.SENSOR_2),
        SENSOR_3: getPercentage(data.SENSOR_3),
        SENSOR_4: getPercentage(data.SENSOR_4),
      });
    };

    socket.on("sensor_update", handleSensorUpdate);

    return () => {
      socket.off("sensor_update", handleSensorUpdate);
    };
  }, []);

  return (
    <div className="flex flex-col items-center justify-center h-screen space-y-10">
      <BannerComponent />
      <div className="flex flex-col items-center justify-center gap-4 mx-24 md:flex-row">
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
    </div>
  );
}

export default App;
