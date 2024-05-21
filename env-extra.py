from os.path import isfile
Import("env")

if isfile(".env"):
    with open(".env", "r") as f:
        lines = f.readlines()
        envs = ["-D{}".format(line.strip()) for line in lines]
        env.Append(BUILD_FLAGS=envs)
else:
    print("File .env not accessible")
