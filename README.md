# RF24 Library Installation + Example

This guide provides steps for installing and using the RF24 Library on Raspberry Pi for NRF24L01+ radio module communication.

## Prerequisites
- Raspberry Pi (any recent model)
- NRF24L01+ radio module
- Basic understanding of Raspberry Pi and terminal commands

## Steps

1. **Enable SPI Interface**
```bash
sudo raspi-config
```
Navigate to:
- `Interfacing Options` -> `SPI` -> `Yes` to enable

2. **Install RF24 Library**
```bash
./install_rf24.sh
```
Note: Ensure the script is executable with `chmod +x install_rf24.sh`

3. **Adjust Pin Configuration**
Edit `src/rf24_config.h` to match your specific hardware connections:
- `RF24_CE_PIN`: Chip Enable pin (default: 22)
- `RF24_CSN_PIN`: Chip Select Not pin (default: 0)

4. **Compile Files**
```bash
make
```

## Typical NRF24L01+ Raspberry Pi Wiring

```
NRF24L01+ | Raspberry Pi
-----------------------
VCC       | 3.3V
GND       | GND
CE        | GPIO22 (default in config)
CSN       | CE0/GPIO0 (default in config)
MOSI      | GPIO10 (MOSI)
MISO      | GPIO9 (MISO)
SCK       | GPIO11 (SCLK)
IRQ       | Not connected (optional)
```

## Troubleshooting

### Common Issues
- **Library Not Found**: Ensure library is correctly installed
- **SPI Not Enabled**: Double-check SPI interface in `raspi-config`
- **Pin Mismatch**: Verify physical connections match `rf24_config.h`

### Checking SPI
Verify SPI is working:
```bash
ls /dev/spi*
```
Should show SPI devices

### Permissions
If you encounter permission issues:
```bash
sudo usermod -a -G spi $USER
```
Then reboot your Raspberry Pi

## Additional Resources
- [RF24 Library GitHub](https://github.com/nRF24/RF24)
- [Raspberry Pi SPI Documentation](https://www.raspberrypi.com/documentation/computers/configuration.html#spi)

## Example Programs
- `build/rf24_test.exe`: Transmitter example
- `build/rf24_receiver.exe`: Receiver example

### Running Examples
```bash
# Transmitter
sudo ./build/rf24_test.exe

# Receiver
sudo ./build/rf24_receiver.exe
```
Note: Use `sudo` due to hardware access requirements
