import { SensorData } from "@/types/sensor.types";
import { useQuery } from "@tanstack/react-query";
import axios from "axios";

export function useSensors() {
  return useQuery<SensorData>({
    queryKey: ["sensor_data"],
    queryFn: async () => {
      const { data } = await axios.get("http://192.168.1.3:5000/sensor_data");
      return data;
    },
    staleTime: 1000 * 60 * 5,
    gcTime: 1000 * 60 * 30,
    refetchInterval: 2500,
  });
}
