from socketIO_client import SocketIO
from pi_switch import RCSwitchSender

socketIO = SocketIO("141.22.50.128", 80)
sender = RCSwitchSender()
sender.enableTransmit(0)

def sendMaxVote(data):
    print("Neuer Max Value erhalten")
    # codesend...
    
    num = 1
    #kompletter Bullshitcode, der nur getestet werden soll
    while True:
        time.sleep(2)
        sender.sendDecimal(num, 24)
        num += 1
    
socketIO.on('maxVote', sendMaxVote)
socketIO.wait()