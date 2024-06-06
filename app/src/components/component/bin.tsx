import { cn } from "@/lib/utils";

export interface BinProps extends React.HTMLAttributes<HTMLDivElement> {
  width?: number;
  height?: number;
  barColor?: string;
  binPathHeightPercent?: number;
}

export const BinComponent = ({
  className,
  binPathHeightPercent = 100,
  ...props
}: BinProps) => {
  const maxPathHeight = 296.166; // Maximum pixel height the path can occupy
  const binPathHeight = (binPathHeightPercent / 100) * maxPathHeight; // Convert percentage to pixel height
  const yPosition = maxPathHeight - binPathHeight; // Calculate y position to start from the bottom

  return (
    <div className={cn(className)} {...props}>
      <svg
        id="bin_component"
        xmlns="http://www.w3.org/2000/svg"
        viewBox="0 0 445 546"
        shapeRendering={"geometricPrecision" as any}
        textRendering={"geometricPrecision" as any}
        height="100%"
        width="100%"
      >
        <path
          id="bin"
          d="M198.255,62.5902h48.47c13.35,0,20.27,16.27,10.69,25.82-2.72,2.75-6.52,4.45-10.69,4.45h-48.47c-13.35,0-20.27-16.27-10.69-25.85c2.73-2.72,6.53-4.42,10.69-4.42Zm48.48,9.98h-48.47c-4.48,0-6.92,5.5-3.63,8.79.94.94,2.21,1.5,3.63,1.5h48.47c4.48,0,6.92-5.47,3.63-8.76-.94-.94-2.22-1.53-3.63-1.53ZM181.645,488.81c0,12.87-22.05,18.23-35.06,10.83-4.51-2.55-7.31-6.38-7.31-10.83v-5.06c0-2.761-2.239-5-5-5h-14.76c-9.89,0-17.94-8.05-17.94-17.94v-341.955c0-2.133-1.403-3.966-3.2052-5.107-4.3335-2.745-7.1648-7.575-7.1648-12.968v-24.3798c0-7.85,6.83-13.86,14.68-13.86h55.08c3.74,0,4.25-2.83,4.76-5.64c1.27-6.95,2.54-13.9,14.56-13.9h84.42c12.02,0,13.29,6.94,14.57,13.89.51,2.81,1.02,5.64,4.76,5.64h55.08c7.85,0,14.68,6.01,14.68,13.86v24.3798c0,5.39-2.829,10.225-7.159,12.968-1.804,1.143-3.211,2.977-3.211,5.112v341.95c0,9.89-8.05,17.94-17.94,17.94h-14.76c-2.761,0-5,2.239-5,5v5.06c0,12.87-22.08,18.23-35.09,10.83-4.51-2.55-7.29-6.38-7.29-10.83v-5.06c0-2.761-2.239-5-5-5h-71.7c-2.761,0-5,2.239-5,5v5.07Zm-27.37-10.06c-2.761,0-5,2.239-5,5v5.06c0,2.58,12.05,6.77,20.15,2.15c1.39-.77,2.24-1.56,2.24-2.15v-5.06c0-2.761-2.239-5-5-5h-12.39Zm124.05,0c-2.761,0-5,2.239-5,5v5.06c0,2.58,12.05,6.77,20.18,2.15c1.36-.77,2.21-1.56,2.21-2.15v-5.06c0-2.761-2.239-5-5-5h-12.39ZM343.78,81.6102c.003,0,.005-.0023.005-.0051v-5.2049c0-2.27-2.3-3.85-4.68-3.85h-55.08c-12.05,0-13.32-6.94-14.57-13.89-.51-2.83-1.02-5.64-4.76-5.64h-84.42c-3.74,0-4.25,2.81-4.76,5.64-1.25,6.94-2.52,13.89-14.57,13.89h-55.08c-2.38,0-4.68,1.59-4.68,3.85v24.3798c0,2.95,2.44,5.36,5.36,5.36h231.87c2.92,0,5.36-2.41,5.36-5.36v-19.1648c0-.0028.002-.005.005-.005v0Z"
          className={cn(className)}
        />
        <path
          id="bin-s-path2"
          d="M294,139.5h-142c-2.384,0-4.5,1.879-4.5,4.417v296.166c0,2.538,2.116,4.417,4.5,4.417h142c2.384,0,4.5-1.879,4.5-4.417v-296.166c0-2.538-2.116-4.417-4.5-4.417Z"
          transform="translate(-.5 0)"
          fill="#fff"
          stroke="#fff"
          strokeWidth={5}
        />
        <path
          id="bin-s-path3"
          d={`M294,${
            139.5 + yPosition
          }h-142c-2.384,0-4.5,1.879-4.5,4.417v${binPathHeight}c0,2.538,2.116,4.417,4.5,4.417h142c2.384,0,4.5-1.879,4.5-4.417v-${binPathHeight}c0-2.538-2.116-4.417-4.5-4.417Z`}
          transform="translate(-.5 0)"
          fill={
            binPathHeightPercent > 50
              ? "#FF0000"
              : binPathHeightPercent > 25
              ? "#FFA500"
              : "#32CD32"
          }
          stroke="#fff"
          strokeWidth="0"
        />
      </svg>
    </div>
  );
};
