# electrometer-lora-iot

Firmware for a Heltec WiFi LoRa 32 V2 (ESP32) electricity-meter node. It samples energy data (EmonLib), encrypts payloads (tiny-AES-c), and sends them over LoRa for the companion [electrometer](https://github.com/Dhi13man/electrometer) Flutter app.

## Install

[PlatformIO](https://platformio.org/) and a Heltec WiFi LoRa 32 V2 board.

```sh
git clone https://github.com/Dhi13man/electrometer-lora-iot.git
cd electrometer-lora-iot
pio run
```

`platformio.ini` targets `heltec_wifi_lora_32_V2` at 115200 baud on the Arduino framework.

## Use

Build and upload with PlatformIO. Source is under `src/`; vendored libraries (EmonLib, Heltec ESP32, Filters, tiny-AES-c) are under `lib/`. Serial monitor:

```sh
pio device monitor
```

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## License

MIT. See [LICENSE](LICENSE).
