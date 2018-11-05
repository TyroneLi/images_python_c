
def notOpenLocalPort(port):
    sock = socket.socket()
    result = sock.connect_ex(("127.0.0.1", port))
    # print(result)
    # result结果不是0表示端口没有被监听，可以使用
    # 否则就要关掉端口占用程序重新运行
    return result

def mainListen():
    port = 8880
    # judge whether the local port is used.
    if notOpenLocalPort(port):
        # open up listen port
        ip_port = ("127.0.0.1", 8880)
        sk = socket.socket()
        sk.bind(ip_port)
        sk.listen(1)
    
        while True:
        	# recrive the data from client
            conn, address = sk.accept()
            data = conn.recv(4096)
            data = data.decode()
            # process the image
            big_tasks = predict_image(model, data)

            results_data = pickle.dumps(big_tasks)
            conn.send(results_data)
            conn.close()
    else:
        print("The %s port is using, please shutdown %s port and run the programming again!" % (port, port))
    
if __name__ == '__main__':
    mainListen()