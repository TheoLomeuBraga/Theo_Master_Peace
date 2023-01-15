#!/bin/python3
import sys, os

argsn = 0
args = []
def get_args():
    global argsn,args
    if __name__ == "__main__":
        argsn = len(sys.argv)
        args = sys.argv

def change_windows():
    print("seted to windows mode")

def change_linux():
    print("seted to linux mode")

def run_windows():
    print("runing windows")

def run_linux():
    print("runing linux")

def get_help():
    print("use 'help' to a list of comands")
    print("use 'change' to change to 'windows' or 'linux' os")
    

def main():
    get_args()
    if argsn == 1:
        get_help()
    elif argsn == 2 and args[1] == "help":
        get_help()
    elif argsn == 3 and args[1] == "change":
        if args[2] == "windows":
            change_windows()
        elif args[2] == "linux":
            change_linux()
    elif argsn == 3 and args[1] == "run":
        if args[2] == "windows":
            run_windows()
        elif args[2] == "linux":
            run_linux()

main()