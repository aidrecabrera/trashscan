```
graph TD
    subgraph Sensors
        A1(HC-SR04) --> Arduino
        A2(HC-SR04) --> Arduino
        A3(HC-SR04) --> Arduino
        A4(HC-SR04) --> Arduino
    end

    Arduino --> Protobuffer
    Arduino --> GSMModule(GSM Module)
    Protobuffer --> RaspberryPi(Raspberry Pi 4)
    GSMModule --> RaspberryPi
    RaspberryPi --> LCDDisplay(LCD Display)
    RaspberryPi --> WebApp(Web Application)
```

```
graph TD
    subgraph Sensors
        A1(HC-SR04) --> Arduino
        A2(HC-SR04) --> Arduino
        A3(HC-SR04) --> Arduino
        A4(HC-SR04) --> Arduino
    end

    subgraph Communication
        Arduino --> Protobuffer
        Arduino --> GSMModule(GSM Module)
    end

    subgraph Processing
        Protobuffer --> RaspberryPi(Raspberry Pi 4)
        GSMModule --> RaspberryPi
    end

    subgraph Output
        RaspberryPi --> LCDDisplay(LCD Display)
        RaspberryPi --> WebApp(Web Application)
    end
```

[View on Eraser![](https://app.eraser.io/workspace/gwwEilISeXia4nUDfhKC/preview?elements=weabPtvIZjlIGo3tSykqPw&type=embed)](https://app.eraser.io/workspace/gwwEilISeXia4nUDfhKC?elements=weabPtvIZjlIGo3tSykqPw)

[View on Eraser![](https://app.eraser.io/workspace/gwwEilISeXia4nUDfhKC/preview?elements=hdcngoy1XiZrmW9ORuSIkg&type=embed)](https://app.eraser.io/workspace/gwwEilISeXia4nUDfhKC?elements=hdcngoy1XiZrmW9ORuSIkg)

[Raspi Imager](https://www.raspberrypi.com/software/)

[Git](https://www.git-scm.com/download/win)

```bash
winget install --id Git.Git -e --source winget
```