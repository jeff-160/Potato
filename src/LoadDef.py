import json, os, sys

dir = os.path.dirname(os.path.dirname(sys.executable) if getattr(sys, 'frozen', False) else os.path.dirname(os.path.abspath(__file__))) 
path = f"{dir}\default\settings.json"

def ParseSettings():
    res = ""
    with open(path, 'r') as file:
        data = json.load(file)
        for key in data:
            res+=f"{key},{data[key]},"
        res = res[:-1]
        print(res)


if __name__=="__main__":
    ParseSettings()