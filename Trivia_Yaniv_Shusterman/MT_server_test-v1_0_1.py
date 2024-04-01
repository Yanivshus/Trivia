import socket
import json

def create_data_length_as_bytes(num):
    code_size = 4  # Assuming the size of integer is 4 bytes (32 bits)
    bytes_list = []

    # Extract each byte from the integer
    for i in range(code_size):
        byte_value = (num >> (i * 8)) & 0xFF
        bytes_list.insert(0, byte_value)  # Insert the byte at the beginning of the list
    return bytes(bytes_list)

def main():
    # Server's IP address and port
    server_ip = "127.0.0.1"  # Change this to the server's IP address
    server_port = 9999  # Change this to the server's port

    # Create a socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        data = {
            "username" : "yaniv",
            "password" : "goat12"
        }
        dataJs = json.dumps(data)
        buffer = []
        buffer.append(chr(100).encode())
        buffer.append(create_data_length_as_bytes(len(dataJs)))
        print(len(dataJs), dataJs)
        buffer.append(dataJs.encode())
        print(buffer)
        concatenated_bytes = b''.join(buffer)
        print(concatenated_bytes)

        # Connect to the server
        client_socket.connect((server_ip, server_port))
        print("Connected to the server")

        # Send data to the server
        client_socket.sendall(concatenated_bytes)
        
        # Receive data from the server
        received_data = client_socket.recv(1024)
        print("Received from server:", received_data.decode())

 

    except socket.error as e:
        print("Error:", e)

    finally:
        # Close the socket connection
        client_socket.close()

if __name__ == "__main__":
    main()
