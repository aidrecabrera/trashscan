import { BinComponent } from "@/components/component/bin";
import { SensorData } from "@/types/sensor.types";
import { AnimatePresence, motion } from "framer-motion";
import { useEffect, useState } from "react";
import { io } from "socket.io-client";
import { BannerComponent } from "./components/component/banner";
import {
  Card,
  CardContent,
  CardFooter,
  CardHeader,
} from "./components/ui/card";
import { useSensors } from "./data/useSensors";
import { getPercentage } from "./lib/utils";

const socket = io("http://127.0.0.1:5000");

const bins = [
  { id: 1, title: "Biodegradable", color: "fill-blue-500" },
  { id: 2, title: "Non-biodegradable", color: "fill-yellow-500" },
  { id: 3, title: "Recyclable", color: "fill-green-500" },
  { id: 4, title: "Hazardous", color: "fill-red-500" },
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
    <div className="flex flex-col items-center justify-center p-4 space-y-10 md:p-10">
      <BannerComponent />
      <div className="flex flex-col flex-wrap items-center justify-center gap-4 md:flex-row">
        {bins.map((bin) => (
          <Card key={bin.id}>
            <CardHeader>
              <h3 className="text-lg font-semibold">{bin.title}</h3>
            </CardHeader>
            <CardContent className="flex items-center justify-center">
              <BinComponent
                binPathHeightPercent={
                  sensorData
                    ? Math.floor(
                        sensorData[`SENSOR_${bin.id}` as keyof SensorData]
                      )
                    : 0
                }
                color={bin.color}
              />
            </CardContent>
            <CardFooter className="flex items-center justify-center py-4">
              <div className="flex items-center gap-2">
                <AnimatePresence>
                  <motion.p
                    key={
                      sensorData
                        ? sensorData[`SENSOR_${bin.id}` as keyof SensorData]
                        : "0"
                    }
                    initial={{ opacity: 0, y: 10 }}
                    animate={{ opacity: 1, y: 0 }}
                    exit={{ opacity: 0, y: -10 }}
                    className="text-xl font-semibold text-gray-800"
                  >
                    {sensorData
                      ? `${Math.floor(
                          sensorData[`SENSOR_${bin.id}` as keyof SensorData]
                        )}%`
                      : "0%"}{" "}
                    <span>
                      {sensorData
                        ? sensorData[`SENSOR_${bin.id}` as keyof SensorData] >
                          75
                          ? "Full"
                          : sensorData[`SENSOR_${bin.id}` as keyof SensorData] >
                            50
                          ? "Half"
                          : sensorData[`SENSOR_${bin.id}` as keyof SensorData] >
                            25
                          ? "Low"
                          : "Empty"
                        : "Empty"}
                    </span>
                  </motion.p>
                </AnimatePresence>
              </div>
            </CardFooter>
          </Card>
        ))}
      </div>
    </div>
  );
}

export default App;
