#!/usr/bin/python

import socket
import subprocess

while True:
    print "Listen..."
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind(("", 1956))
    data, addr = server_socket.recvfrom(1024)
    output = subprocess.check_output(["/Users/cmarrin/Library/Application Support/etherclock/printinfo", ""])
    print "Sending:", output
    server_socket.sendto(output, ("10.0.1.210", 1956))
    server_socket.close()
