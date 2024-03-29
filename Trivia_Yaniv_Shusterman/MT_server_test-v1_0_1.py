import socket

def main():
    # Server's IP address and port
    server_ip = "127.0.0.1"  # Change this to the server's IP address
    server_port = 9999  # Change this to the server's port

    # Create a socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to the server
        client_socket.connect((server_ip, server_port))
        print("Connected to the server")

        # Receive data from the server
        received_data = client_socket.recv(1024)
        print("Received from server:", received_data.decode())

        # Send data to the server
        message = "Hello"
        client_socket.sendall(message.encode())

    except socket.error as e:
        print("Error:", e)

    finally:
        # Close the socket connection
        client_socket.close()

if __name__ == "__main__":
    main()
