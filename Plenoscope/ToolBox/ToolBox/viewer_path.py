import os.path

class Path():
    def __init__(self, fullpath):
        self.full = fullpath
        self.path, self.name = os.path.split(fullpath)
        self.name_wo_ext = os.path.splitext(self.name)[0]
        self.path_wo_ext, self.ext = os.path.splitext(fullpath)