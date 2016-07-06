from socketIO_client import SocketIO, BaseNamespace
#from pi_switch import RCSwitchSender


sender = RCSwitchSender()
sender.enableTransmit(0)

class MainNamespace(BaseNamespace):
    def sendMaxVote(data):
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
        
socketIO = SocketIO("localhost", 3000, MainNamespace)   
socketIO.on('maxVote', sendMaxVote)
socketIO.wait()