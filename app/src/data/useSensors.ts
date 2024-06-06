import { SensorData } from "@/types/sensor.types";
import { useQuery } from "@tanstack/react-query";
import axios from "axios";

export function useSensors() {
  return useQuery<SensorData>({
    queryKey: ["sensor_data"],
    queryFn: async () => {
      const { data } = await axios.get("http://127.0.0.1:5000/sensor_data");
      console.log("Refetched");
      return data;
    },
  });
}
