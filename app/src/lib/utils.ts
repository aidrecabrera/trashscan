import { type ClassValue, clsx } from "clsx";
import { twMerge } from "tailwind-merge";

export function cn(...inputs: ClassValue[]) {
  return twMerge(clsx(inputs));
}

export function getPercentage(value: number): number {
  return (1 - Math.min(Math.max(value, 0), 40) / 40) * 100;
}
