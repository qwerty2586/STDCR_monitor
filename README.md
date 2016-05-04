# STDCR_monitor
Raspberry pi utility for controling and monitoring Stimulation device for cognitive research over bluetooth or serial port

## Installation

To compile in clean raspbian you need to install dependencies.

```
sudo apt-get install qt4-default cmake
```

Then you are ready to download source.

```
git clone https://github.com/neurofishteam/STDCR_monitor.git
```

You can compile binaries like this.

```
cd STDCR_monitor
cmake .
make -j4
```

Now you can run your binary.

```
./STDCR_monitor
```

## Troubleshooting

If you have trouble to make your bluetooth working, follow [these steps](https://www.raspberrypi.org/learning/robo-butler/bluetooth-setup/).
