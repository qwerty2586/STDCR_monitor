# stimulator_control
Raspberry pi utility for controling and monitoring Stimulation device for cognitive research over bluetooth or serial port

## Installation

To compile in clean raspbian stretch you need to install dependencies.

```
sudo apt-get install qtbase5-dev libqt5serialport5-dev cmake
```

Then you are ready to download source.

```
git clone https://github.com/qwerty2586/stimulator_control.git
```

You can compile binaries like this.

```
cd stimulator_control
cmake .
make -j4
```

Now you can run your binary.

```
./stimulator_control
```

## Troubleshooting

If you have trouble to make your bluetooth working, follow [these steps](https://www.raspberrypi.org/learning/robo-butler/bluetooth-setup/).
