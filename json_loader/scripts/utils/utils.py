import os


def vector_recursive_index(index: str, max_count: int, count: int):
    if count == 0:
        return ""
    else:
        ret = "["+index*(max_count-count+1)+"]"
        ret += vector_recursive_index(index, max_count, count-1)
    return ret

def check_or_create_dir(path):
    if not os.path.isdir(path):
        try:
            os.makedirs(path)
            return True
        except:
            return False
    return True