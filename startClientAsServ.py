# служба
import subprocess as p
import time as t
import select
import sys
import socket
def launch_client(host,port):# будет кортеж из 2 строк - параметров cmd
 execut="./client" # клиент который собирает разово информацию об окне и отправляет
 proc=p.Popen([execut,host,port],shell=False)
 return 0 # успешный код завершения
def serv(host,port,period):
 period_sec=period*60
 try:
   while True:
     launch_client(host,port)
     t.sleep(period_sec)
 except KeyboardInterrupt: # по Ctr+C выходим из службы
     print("exited")
args_from_sys=sys.argv
host=args_from_sys[1]
port=args_from_sys[2]
period=int(args_from_sys[3])
if len(args_from_sys)!=4:
    print("Usage:<client_service> <host> <port> <period_min must'>'=1>")
    sys.exit(1)
serv(host,port,period)     