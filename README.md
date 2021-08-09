# Linux Driver

The code in this repository acts as a Linux interface for controlling JURA coffee makers over a [serial (UART)](https://www.raspberrypi.org/documentation/configuration/uart.md) connection.
Once running, the following file-interface will be exposed:
```
/
├── tmp
│   ├── coffee_maker
│   │   ├── tx
│   │   ├── rx
│   │   ├── status
│   │   ├── mode
│   │   └── device
│   └── ...
└── ...
```

* `tx` is a [FIFO named pipe](https://man7.org/linux/man-pages/man7/fifo.7.html) where you can write stuff to, to send it to the coffee maker.
* `rx` is a [FIFO named pipe](https://man7.org/linux/man-pages/man7/fifo.7.html) where you can read stuff from, that gets send from the coffee maker.
* `status` is a simple text file containing the status of the connection (`1` == Running).
* `mode` is a simple text file containing the current operation mode. Currently just a placeholder for the future.
* `device` is a simple text file containing the device name of the connected coffee maker (e.g. `EF532M V02.03`).

## Requirements
The following requirements are required to build this project.
* A C++20 compatible compiler like [gcc](https://gcc.gnu.org/) or [clang](https://clang.llvm.org/)
* The build system is written using [CMake](https://cmake.org/)
* For managing dependencies in CMake, we are using [conan](https://conan.io/)

### Fedora
To install those dependencies on Fedora, run the following commands:
```bash
sudo dnf install -y gcc clang cmake python3 python3-pip
pip3 install --user conan
```

### Raspberry Pi
To install those dependencies on a Raspberry Pi, running the [Raspberry Pi OS](https://www.raspberrypi.org/software/), run the following commands:
```bash
sudo apt install -y cmake python3 python3-pip
pip3 install --user conan
```
For all the other requirements, head over here: https://github.com/Jutta-Proto/hardware-pi#raspberry-pi-os

## Building
Run the following commands to build this project:
```bash
# Clone the repository:
git clone https://github.com/Jutta-Proto/linux-diver.git
# Switch into the newly cloned repository:
cd linux-driver
# Build the project:
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage
From inside the `build` directory, you can execute the driver with the following command:
```
./src/uart_driver /dev/tty0
```
Here `/dev/tty0` is the path to the serial port, you are using. In case you are running this on a Raspberry Pi, and you connected like described [here](https://github.com/Jutta-Proto/hardware-pi#connecting-a-coffee-maker), it should be `/dev/tty0`.

## Example
The following example shows how to read from a [FIFO named pipe](https://man7.org/linux/man-pages/man7/fifo.7.html).
```python
import os
import errno

FIFO = "/tmp/coffee_maker/rx"

try:
    os.mkfifo(FIFO)
except OSError as oe:
    if oe.errno != errno.EEXIST:
        raise

while True:
    print("Opening FIFO...")
    with open(FIFO) as fifo:
        print("FIFO opened")
        while True:
            data = fifo.read()
            if len(data) == 0:
                print("Writer closed")
                break
            print('Read: "{0}"'.format(data))
```
Source: https://stackoverflow.com/a/39089792
