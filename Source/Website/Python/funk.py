from socketIO_client import SocketIO, BaseNamespace


class MainNamespace(BaseNamespace):
    def on_connect(*args):
        print("connected")
    
    def on_maxVote(*args):
        print("Neuer Max Value erhalten")
        # codesend...
        
        num = 1
        #kompletter Bullshitcode, der nur getestet werden soll
        """        
        while True:
            time.sleep(2)
            sender.sendDecimal(num, 24)
            num += 1
        """
        
socketIO = SocketIO("141.22.50.128", 80, MainNamespace)

while True:
    socketIO.wait()