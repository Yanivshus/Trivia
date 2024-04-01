import socket
import json
import bson

def create_data_length_as_bytes(num):
    code_size = 4  # Assuming the size of integer is 4 bytes (32 bits)
    bytes_list = []

    # Extract each byte from the integer
    for i in range(code_size):
        byte_value = (num >> (i * 8)) & 0xFF
        bytes_list.insert(0, byte_value)  # Insert the byte at the beginning of the list
    return bytes(bytes_list)

def choose(input_str):
    if input_str == "S":
        return 101
    else:
        return 100

def details(input_str):
    if input_str == "S":
        return {
            "username" : "yaniv",
            "password" : "goat12",
            "email" : "kai@gmail.com"
        }
    else:
        return {
            "username" : "yaniv",
            "password" : "goat12"
        }



def main():
    dataj = {
        "key" : 1
    }

    print(bson.dumps(dataj))

    # Server's IP address and port
    server_ip = "127.0.0.1"  # Change this to the server's IP address
    server_port = 9999  # Change this to the server's port

    # Create a socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
          # Connect to the server
        client_socket.connect((server_ip, server_port))
        print("Connected to the server")

        while True:
            choice = input("Enter either sign-up or login: ")
            code = choose(choice)
            data = details(choice)

            dataJs = bson.dumps(data)
            buffer = []
            buffer.append(chr(code).encode())
            buffer.append(create_data_length_as_bytes(len(dataJs)))
            print(len(dataJs), dataJs)
            buffer.append(dataJs)
            print(buffer)
            concatenated_bytes = b''.join(buffer)
            print(concatenated_bytes)
        

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
