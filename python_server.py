import socket
import threading
from datetime import datetime

HOST = '127.0.0.1'
PORT = 8080


class Time:
    def now():
        current_time = datetime.now().time()
        formatted_time = current_time.strftime("%H:%M:%S")
        return formatted_time


class Client:
    def __init__(self, socket:socket.socket, username:str):
        self.socket = socket
        self.username = username
    
    def send_message(self,sender:str, message:str):
        self.socket.sendall(Time.now().encode()) # Send Time NOW
        self.socket.sendall(sender.encode()) # Send sender name
        self.socket.sendall(message.encode()) # Send message data


class Server:
    clients = []

    def send_to_all(sender:Client,message:str,include_self:bool = True):
        for client in Server.clients:
            if not include_self and client == sender:
                continue
            client.send_message(sender.username,message)

    def handle_client(socket:socket.socket):
        username = socket.recv(1024).decode()

        for client in Server.clients:
            client.send_message("Server",f"{username} connected to chat!\n")

        current_client = Client(socket=socket,username=username)
        Server.clients.append(current_client)

        while True:
            try:
                message = current_client.socket.recv(1024).decode()
                if message:
                    Server.send_to_all(current_client,message=message)

                    with open("chat_log.txt","a") as f:
                            f.write(message+'\n')
            except Exception as ex:
                print(f"{socket} catched exception: {ex}")
                Server.clients.remove(current_client)
                for client in Server.clients:
                    client.send_message("Server",f"{username} disconnected from chat!\n")
                current_client.socket.close()
                break

    def start():
        server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        server_socket.bind((HOST,PORT))
        server_socket.listen(5)

        print(f"Server is up on {(HOST,PORT)}!")

        server_socket.settimeout(10)

        while True:
            try:
                print("Waiting for connection...")
                client_socket, client_address = server_socket.accept()

                print(f"{client_address} connected!")

                client_thread = threading.Thread(target=Server.handle_client,args=(client_socket,))
                client_thread.start()
            except TimeoutError:
                # print("Timed out!")
                if Server.clients.count == 0:
                    # print("No new connections established! Closing server...")
                    break
                pass

if __name__ == "__main__":
    Server.start()