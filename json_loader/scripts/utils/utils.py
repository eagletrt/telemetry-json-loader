import os

def check_or_create_dir(path):
    if not os.path.isdir(path):
        try:
            os.makedirs(path)
            return True
        except:
            return False
    return True