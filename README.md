# WinLogon

WinLogon service intercepts user's login into the system event and launch "Notepad" in my case. The service is a daemon, so you shoud run it accordingly. 

## How to run:
* First of all you should rewrite the hardcode path for logs in function WriteLog()
* sc.exe create [ServiceName] [binPath= ] 
* Find the service in Task Manager (tab "Services") and make the service (It was named "AdvancedNewService") to start automatically (after reboot or power off). Maybe you should run it in that Task Manager.
