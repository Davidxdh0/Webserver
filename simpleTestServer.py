import socket

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = ('localhost', 80)
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)
counter = 0

while True:
    # Wait for a connection
    connection, client_address = sock.accept()
    try:
        print(f"connection from {client_address}")

        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(1500)
            if data:
                if counter == 0:
                    msg = "{\"test\":\"data\"}"
                    counter = 1
                else:
                    msg = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 15\r\n\r\n{\"test\":\"data\"}"
                    counter = 0
                connection.sendall(str.encode(msg))
                print(msg)
                connection.close()
                break

    finally:
        # Clean up the connection
        connection.close()
