from time import sleep
import erpc
from  controller import *
import sys

if __name__ == "__main__":

    if len(sys.argv) != 2:
        print("Usage: python led_test_client.py <serial port to use>")
        exit()

    xport = erpc.transport.SerialTransport(sys.argv[1], 9600)
    client_mgr = erpc.client.ClientManager(xport, erpc.basic_codec.BasicCodec)
    client = client.SwitchServiceClient(client_mgr)

    while True:
        start = input()
        if start == "1":
            client.start()
        elif start == "0":
            client.stop()
        sleep(0.5)