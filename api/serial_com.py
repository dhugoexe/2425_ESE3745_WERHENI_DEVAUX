import serial
import time


class STM32Communication:
    def __init__(self, port: str = '/dev/ttyAMA0', baudrate: int = 115200):
        self.ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=1
        )
        time.sleep(100)

    def __del__(self):
        if hasattr(self, 'ser') and self.ser.is_open:
            self.ser.close()

    def send_command(self, command: str) -> None:
        self.ser.write(f"{command}\n".encode('utf-8'))

    def read_response(self) -> str:
        if self.ser.in_waiting:
            return self.ser.readline().decode('utf-8').strip()
        return ""


def main():

    stm = STM32Communication()
    print(stm.send_command('GET_T'))
    print(stm.read_response())




if __name__ == "__main__":
    main()