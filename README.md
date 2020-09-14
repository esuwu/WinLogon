# WinLogon

WinLogon service intercepts user's login event into the system and launch "Notepad" in my case. The service is a daemon, so you shoud run it accordingly. 

## How to run:
* First of all you should rewrite the hardcode path for logs in function WriteLog()
* sc.exe create [ServiceName] [binPath= ] 
* Find the service in Task Manager (tab "Services") and make the service to start automatically (after reboot or power off). Maybe you should run it in that Task Manager.
