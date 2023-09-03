import json, os

path = f"{os.getcwd()}\default\settings.json"

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